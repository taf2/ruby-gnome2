/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_messages.c -

  $Author: mutoh $
  $Date: 2004/10/31 14:44:44 $

  Copyright (C) 2002,2003 Masao Mutoh

  This file is devided from rbgtkmain.c.
  rbgtkmain.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE rbglib_log_handler_procs;
static ID id_call;

static void
rbglib_log_handler(log_domain, log_level, message, user_data)
    const gchar *log_domain;
    GLogLevelFlags log_level;
    const gchar *message;
    gpointer user_data;
{
    if (TYPE(user_data) == T_BLOCK){
        rb_funcall((VALUE)user_data, id_call, 3,
                   CSTR2RVAL(log_domain), INT2NUM(log_level),
                   CSTR2RVAL(message));

    }
}

static VALUE
rbglib_m_log_set_handler(self, domain, levels)
    VALUE self, domain, levels;
{
    VALUE proc = G_BLOCK_PROC();
    guint handler_id = g_log_set_handler(NIL_P(domain) ? NULL : RVAL2CSTR(domain),
                                         NUM2INT(levels),
                                         rbglib_log_handler, (gpointer)proc);
    G_RELATIVE2(self, proc, UINT2NUM(handler_id), rbglib_log_handler_procs);
    return UINT2NUM(handler_id);
}

static VALUE
rbglib_m_log_remove_handler(self, domain, handler_id)
    VALUE self, domain, handler_id;
{
    g_log_remove_handler(NIL_P(domain) ? NULL : RVAL2CSTR(domain),
                         NUM2UINT(handler_id));
    G_REMOVE_RELATIVE(self, handler_id, rbglib_log_handler_procs);
    return Qnil;
}

static VALUE
rbglib_m_log_set_always_fatal(self, fatal_mask)
    VALUE self, fatal_mask;
{
    return INT2NUM(g_log_set_always_fatal(NUM2INT(fatal_mask)));
}

static VALUE
rbglib_m_log_set_fatal_mask(self, domain, fatal_mask)
    VALUE self, domain, fatal_mask;
{
    return INT2NUM(g_log_set_fatal_mask(NIL_P(domain) ? NULL : RVAL2CSTR(domain),
                                        NUM2INT(fatal_mask)));
}

static VALUE
rbglib_m_log(self, domain, level, str)
    VALUE self, domain, level, str;
{
    g_log(NIL_P(domain) ? NULL : RVAL2CSTR(domain), NUM2INT(level), RVAL2CSTR(str), NULL);
    return Qnil;
}

void
Init_glib_messages()
{
    VALUE mGLog = rb_define_module_under(mGLib, "Log");

    id_call = rb_intern("call");

    rb_global_variable(&rbglib_log_handler_procs);
    rbglib_log_handler_procs = rb_hash_new();
    rb_define_module_function(mGLog, "set_handler", rbglib_m_log_set_handler, 2);
    rb_define_module_function(mGLog, "remove_handler", rbglib_m_log_remove_handler, 2);
    rb_define_module_function(mGLog, "set_always_fatal", rbglib_m_log_set_always_fatal, 1);
    rb_define_module_function(mGLog, "set_fatal_mask", rbglib_m_log_set_fatal_mask, 2);
    rb_define_module_function(mGLog, "log", rbglib_m_log, 3);

    rb_define_const(mGLog, "FATAL_MASK", INT2NUM(G_LOG_FATAL_MASK));
    rb_define_const(mGLog, "LEVEL_USER_SHIFT", INT2NUM(G_LOG_LEVEL_USER_SHIFT));

    /* GLogLevelFlags */
    rb_define_const(mGLog, "FLAG_RECURSION", INT2NUM(G_LOG_FLAG_RECURSION));
    rb_define_const(mGLog, "FLAG_FATAL", INT2NUM(G_LOG_FLAG_FATAL));
    rb_define_const(mGLog, "LEVEL_ERROR", INT2NUM(G_LOG_LEVEL_ERROR));
    rb_define_const(mGLog, "LEVEL_CRITICAL", INT2NUM(G_LOG_LEVEL_CRITICAL));
    rb_define_const(mGLog, "LEVEL_WARNING", INT2NUM(G_LOG_LEVEL_WARNING));
    rb_define_const(mGLog, "LEVEL_MESSAGE", INT2NUM(G_LOG_LEVEL_MESSAGE));
    rb_define_const(mGLog, "LEVEL_INFO", INT2NUM(G_LOG_LEVEL_INFO));
    rb_define_const(mGLog, "LEVEL_DEBUG", INT2NUM(G_LOG_LEVEL_DEBUG));
    rb_define_const(mGLog, "LEVEL_MASK", INT2NUM(G_LOG_LEVEL_MASK));
}
