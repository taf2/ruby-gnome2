/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_gstrv.c -

  $Author: mutoh $
  $Date: 2005/01/09 07:21:01 $

  Copyright (C) 2005  Masao Mutoh
**********************************************************************/

#include "global.h"

#if GLIB_CHECK_VERSION(2,6,0)

static VALUE
strv_to_ruby(const GValue* from)
{
    VALUE ary;
    const gchar** boxed = (const gchar**)g_value_get_boxed(from);
    if (!boxed)
        return Qnil;
  
    ary = rb_ary_new();
    while (*boxed){
        rb_ary_push(ary, CSTR2RVAL(*boxed));
        boxed++;
    }
    return ary;
}

static void
strv_from_ruby(VALUE from, GValue* to)
{
    int i;
    gchar** gstrv;

    if (NIL_P(from)) {
        g_value_set_boxed(to, NULL);
        return;
    }

    Check_Type(from, T_ARRAY);
    gstrv = g_new(gchar*, RARRAY(from)->len + 1);

    for (i = 0; i < RARRAY(from)->len; i++) {
        gstrv[i] = RVAL2CSTR(RARRAY(from)->ptr[i]);
    }
    gstrv[RARRAY(from)->len] = NULL;

    g_value_set_boxed(to, gstrv);

    g_strfreev(gstrv);
}

void Init_gobject_gstrv()
{
    /* GStrv is treated as Array */
//    VALUE gBoxed = G_DEF_CLASS(G_TYPE_BOXED, "Strv", mGLib);
    rbgobj_register_g2r_func(G_TYPE_STRV, strv_to_ruby);
    rbgobj_register_r2g_func(G_TYPE_STRV, strv_from_ruby);
}
#endif