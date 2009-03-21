/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbbonobo-main.c -

  $Author: mutoh $
  $Date: 2002/12/22 14:27:43 $

  Copyright (C) 2009 The Ruby GNOME2 Project Team
  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbbonobo.h"

static VALUE
rbonobo_is_initialized(self)
    VALUE self;
{
    return CBOOL2RVAL(bonobo_is_initialized());
}

static VALUE
rbonobo_init(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    int i, gargc;
    VALUE argary;
    char** gargv;

    rb_scan_args(argc, argv, "01", &argary);

    if (NIL_P(argary)){
        gargc = RARRAY_LEN(rb_argv);
        argary = rb_argv;
    } else {
        Check_Type(argary, T_ARRAY);
        gargc = RARRAY_LEN(argary);
    }

    gargv = ALLOCA_N(char*,gargc + 1);
    gargv[0] = RVAL2CSTR(rb_argv0);

    for (i = 0; i < gargc; i++) {
        if (TYPE(RARRAY_PTR(argary)[i]) == T_STRING) {
            gargv[i+1] = RVAL2CSTR(RARRAY_PTR(argary)[i]);
        }
        else {
            gargv[i+1] = "";
        }
    }
    gargc++;

    return CBOOL2RVAL(bonobo_init(&gargc, gargv));
}

/*
gboolean    bonobo_init_full                (int *argc,
                                             char **argv,
                                             CORBA_ORB orb,
                                             PortableServer_POA poa,
                                             PortableServer_POAManager manager);
*/

static VALUE
rbonobo_debug_shutdown(self)
    VALUE self;
{
    return INT2NUM(bonobo_debug_shutdown());
}

static VALUE
rbonobo_main(self)
    VALUE self;
{
    bonobo_main();
    return self;
}

static VALUE
rbonobo_main_quit(self)
    VALUE self;
{
    bonobo_main_quit();
    return self;
}

static VALUE
rbonobo_activate(self)
    VALUE self;
{
    return CBOOL2RVAL(bonobo_activate());
}

/*
CORBA_ORB   bonobo_orb                      (void);
PortableServer_POA bonobo_poa               (void);
PortableServer_POAManager bonobo_poa_manager
                                            (void);
*/
void
Init_bonobo_main()
{
    rb_define_module_function(mBonobo, "initialized?", rbonobo_is_initialized, 0);
    rb_define_module_function(mBonobo, "init", rbonobo_init, -1);
    rb_define_module_function(mBonobo, "debug_shutdown", rbonobo_debug_shutdown, 0);
    rb_define_module_function(mBonobo, "main", rbonobo_main, 0);
    rb_define_module_function(mBonobo, "main_quit", rbonobo_main_quit, 0);
    rb_define_module_function(mBonobo, "activate", rbonobo_activate, 0);

}
