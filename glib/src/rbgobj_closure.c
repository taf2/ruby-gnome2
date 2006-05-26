/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_closure.c -

  $Author: ssimons $
  $Date: 2006/05/26 19:19:35 $

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include "global.h"
#include <unistd.h>

#ifdef HAVE_YARV_H
#include "yarv.h"
#define ruby_errinfo (GET_THREAD()->errinfo)
#endif

static ID id_call;
static ID id_holder;
static gboolean rclosure_initialized = FALSE;

#define CALLBACK_KEY       rb_str_new2("callback")
#define EXTRA_ARGS_KEY     rb_str_new2("extra_args")

#define HOLDER(closure)       \
  (rb_ivar_get((closure)->rb_holder, id_holder))
#define SETUP_HOLDER(closure) \
  (rb_ivar_set((closure)->rb_holder, id_holder, rb_hash_new()))

typedef struct _GRClosure GRClosure;

typedef struct _GRClosureHolder
{
    GRClosure *closure;
} GRClosureHolder;

struct _GRClosure
{
    GClosure closure;
    VALUE rb_holder;
    gint count;
    GValToRValSignalFunc g2r_func;
};

static VALUE
rclosure_default_g2r_func(num, values)
    guint num;
    const GValue* values;
{
    int i;
    VALUE args = rb_ary_new2(num);
    for (i = 0; i < num; i++)
        rb_ary_store(args, i, GVAL2RVAL(&values[i]));
    return args;
}

struct marshal_arg
{
    GClosure*       closure;
    GValue*         return_value;
    guint           n_param_values;
    const GValue*   param_values;
    gpointer        invocation_hint;
    gpointer        marshal_data;
};

static int
rclosure_alive_p(GRClosure *rclosure)
{
    return rclosure->count > 0;
}

static VALUE
rclosure_marshal_body(VALUE rb_arg)
{
    struct marshal_arg *arg;
    GRClosure*      rclosure;
    GValue*         return_value;
    guint           n_param_values;
    const GValue*   param_values;
    /* gpointer        invocation_hint;*/
    /* gpointer        marshal_data; */

    VALUE ret = Qnil;
    VALUE args;
    GValToRValSignalFunc func;


    Data_Get_Struct(rb_arg, struct marshal_arg, arg);
    rclosure        = (GRClosure *)(arg->closure);
    return_value    = arg->return_value;   
    n_param_values  = arg->n_param_values; 
    param_values    = arg->param_values;
    /* invocation_hint = arg->invocation_hint; */
    /* marshal_data    = arg->marshal_data; */

    if (rclosure->g2r_func){
        func = (GValToRValSignalFunc)rclosure->g2r_func;
    } else { 
        func = (GValToRValSignalFunc)rclosure_default_g2r_func;
    }
    args = (*func)(n_param_values, param_values);

    if (rclosure_alive_p(rclosure)) {
        VALUE callback, extra_args;
        callback = rb_hash_aref(HOLDER(rclosure), CALLBACK_KEY);
        extra_args = rb_hash_aref(HOLDER(rclosure), EXTRA_ARGS_KEY);

        if (!NIL_P(extra_args)) {
            args = rb_ary_concat(args, extra_args);
        }

        ret = rb_apply(callback, id_call, args);
    } else {
        rb_warn("GRClosure invoking callback: already destroyed");
    }

    if (return_value && G_VALUE_TYPE(return_value))
        rbgobj_rvalue_to_gvalue(ret, return_value);

    return Qnil;
}

static void
rclosure_marshal_do(VALUE arg) {
  int state = 0;

  rb_protect(rclosure_marshal_body, arg, &state);
  if (state && !NIL_P(ruby_errinfo)) {
    char buf[BUFSIZ];
    VALUE errmsg;
    int ret;

    if (ruby_sourcefile) {
      ret = snprintf(buf, BUFSIZ, "%s:", ruby_sourcefile);
      rb_write_error2(buf, MIN(BUFSIZ, ret));
    }
    errmsg = rb_funcall(ruby_errinfo, rb_intern("message"), 0, 0);

    ret = snprintf(buf, BUFSIZ, "%d: %s: %s\n",
            ruby_sourceline, rb_obj_classname(ruby_errinfo),
              RVAL2CSTR(errmsg));
    rb_write_error2(buf, MIN(BUFSIZ, ret));
  }
}

#ifdef HAVE_NATIVETHREAD
static GStaticMutex callback_init_mutex = G_STATIC_MUTEX_INIT;
static GMutex *callback_mutex = NULL;
static GCond *callback_done_cond = NULL;
static GCond *callback_finished_cond = NULL;
static VALUE m_arg = Qnil;
static int callback_fd[2];

static VALUE
rclosure_marshal_pop(void) {
 for (;;) {
   char buf[1];

   /* wait untill we're triggered. If this happens we can read from the pipe
    * and it's guaranteed that the needed mutexes are initialized */
   rb_thread_wait_fd(callback_fd[0]);
   read(callback_fd[0], buf, 1);

   g_mutex_lock(callback_mutex);
   if (m_arg != Qnil) {
     rclosure_marshal_do(m_arg);
   }

   m_arg = Qnil;
   g_cond_signal(callback_done_cond);
   g_mutex_unlock(callback_mutex);
 }
}

static void
init_callback_mutex(void) {
  if (callback_mutex == NULL) {
    g_static_mutex_lock(&callback_init_mutex);
    if (callback_mutex == NULL) {
      callback_mutex = g_mutex_new();
      callback_done_cond = g_cond_new();
      callback_finished_cond = g_cond_new();
    } /* else some other thread initialised it */

    g_static_mutex_unlock(&callback_init_mutex);
  }
}

static void
rclosure_marshal_push(VALUE arg) {
  init_callback_mutex();

  g_mutex_lock(callback_mutex);
  while (m_arg != Qnil) {
    /* Wait for another callback to finish up */
    g_cond_wait(callback_finished_cond, callback_mutex);
  }
  m_arg = arg;
  /* trigger ruby callback thread */
  write(callback_fd[1],"c", 1);

  /* Wait untill the ruby callback thread signals is done and then signal
   * waiting callback pushes that we're finished 
   */
  g_cond_wait(callback_done_cond, callback_mutex);
  g_cond_signal(callback_finished_cond);
  g_mutex_unlock(callback_mutex);
}
#endif

static void
rclosure_marshal(GClosure*       closure,
                 GValue*         return_value,
                 guint           n_param_values,
                 const GValue*   param_values,
                 gpointer        invocation_hint,
                 gpointer        marshal_data)
{
    struct marshal_arg *arg;
    VALUE rb_arg;

    if (!rclosure_initialized) {
        g_closure_invalidate(closure);
        return;
    }

    rb_arg = Data_Make_Struct(rb_cData, struct marshal_arg, 0, 0, arg);

    arg->closure         = closure;
    arg->return_value    = return_value;
    arg->n_param_values  = n_param_values;
    arg->param_values    = param_values;
    arg->invocation_hint = invocation_hint;
    arg->marshal_data    = marshal_data;

#ifdef HAVE_NATIVETHREAD
    if (!is_ruby_native_thread()) {
       if (!g_thread_supported()) {
           rb_bug("glib signal in another thread, but gthreads not supported");
        }
        rclosure_marshal_push(rb_arg);
    } else {
        rclosure_marshal_do(rb_arg);
    }
#else
    rclosure_marshal_do(rb_arg);
#endif
}


static void
rclosure_invalidate(gpointer data, GClosure* closure)
{
    GRClosure *rclosure = (GRClosure*)closure;

    if (rclosure_alive_p(rclosure)) {
        if (rclosure->count != 1) {
            rb_warn("GRClosure invalidate: strange reference count: %d\n",
                    rclosure->count);
        }
        rclosure->count = 0;
    }
}

static void
gr_closure_holder_free(GRClosureHolder *holder)
{
    if (holder) {
        if (rclosure_alive_p(holder->closure)) {
            holder->closure->count--;
        }
        free(holder);
    }
}

GClosure*
g_rclosure_new(VALUE callback_proc, VALUE extra_args, GValToRValSignalFunc g2r_func)
{
    GRClosure* closure;
    GRClosureHolder *holder;

    closure = (GRClosure*)g_closure_new_simple(sizeof(GRClosure), NULL);

    closure->count      = 0;
    closure->g2r_func   = g2r_func;
    closure->rb_holder  = Data_Make_Struct(rb_cData, GRClosureHolder, 0,
                                           gr_closure_holder_free, holder);

    holder->closure = closure;
    SETUP_HOLDER(closure);

    rb_hash_aset(HOLDER(closure), CALLBACK_KEY, callback_proc);
    rb_hash_aset(HOLDER(closure), EXTRA_ARGS_KEY, extra_args);

    g_closure_set_marshal((GClosure*)closure, &rclosure_marshal);
    g_closure_add_invalidate_notifier((GClosure*)closure, NULL,
                                      &rclosure_invalidate);

    return (GClosure*)closure;
}

static void
rclosure_weak_notify(gpointer data, GObject* where_the_object_was)
{
    GRClosure *rclosure = data;
    if (rclosure_alive_p(rclosure)) {
        rclosure->count--;
    }
}

void
g_rclosure_attach(GClosure *closure, VALUE object)
{
    static VALUE mGLibObject = (VALUE)NULL;
    GRClosure *rclosure = (GRClosure *)closure;

    G_CHILD_ADD(object, rclosure->rb_holder);

    rclosure->count++;

    if (!mGLibObject) {
        mGLibObject = rb_const_get(mGLib, rb_intern("Object"));
    }
    if (rb_obj_is_kind_of(object, mGLibObject)) {
        g_object_weak_ref(RVAL2GOBJ(object), rclosure_weak_notify, rclosure);
    }
}

static void
rclosure_end_proc(VALUE _)
{
    rclosure_initialized = FALSE;
}

static void
Init_rclosure()
{
    id_call = rb_intern("call");
    id_holder = rb_intern("holder");

    rclosure_initialized = TRUE;
    rb_set_end_proc(rclosure_end_proc, Qnil);
}

/**********************************************************************/

static VALUE
closure_initialize(self)
    VALUE self;
{
    GClosure* closure = g_rclosure_new(G_BLOCK_PROC(), Qnil, NULL);
    G_INITIALIZE(self, closure);
    g_closure_sink(closure);
    return self;
}

static VALUE
closure_in_marshal(self)
    VALUE self;
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    return closure->in_marshal ? Qtrue : Qfalse;
}

static VALUE
closure_is_invalid(self)
    VALUE self;
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    return closure->is_invalid ? Qtrue : Qfalse;
}

static VALUE
closure_invalidate(self)
    VALUE self;
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    g_closure_invalidate(closure);
    return self;
}

static void
Init_closure()
{
    VALUE cClosure = G_DEF_CLASS(G_TYPE_CLOSURE, "Closure", mGLib);
#ifdef HAVE_NATIVETHREAD
    VALUE thread;
#endif
    rb_define_method(cClosure, "initialize", closure_initialize, 0);
    rb_define_method(cClosure, "in_marshal?", closure_in_marshal, 0);
    rb_define_method(cClosure, "invalid?", closure_is_invalid, 0);
    rb_define_method(cClosure, "invalidate", closure_invalidate, 0);

#ifdef HAVE_NATIVETHREAD
    /* startup the ruby thread to pull callbacks from other threads */
    if (pipe(callback_fd) != 0) {
      rb_bug("Unable to create glib callback thread\n");
    }
    thread = rb_thread_create(rclosure_marshal_pop, NULL);
    G_CHILD_ADD(cClosure, thread);
#endif
}

/**********************************************************************/

void Init_gobject_gclosure()
{
    Init_rclosure();
    Init_closure();
}

