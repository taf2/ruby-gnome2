/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkdata.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
 * Data
 */
void Init_gtk_data()
{
    gData = rb_define_class_under(mGtk, "Data", gObject);

    rb_define_const(gData, "SIGNAL_DISCONNECT", rb_str_new2("disconnect"));

    /* child init */
    Init_gtk_adjustment();
    Init_gtk_tooltips();
}

/*
 * Adjustment
 */
static VALUE
adj_initialize(self, value, lower, upper, step_inc, page_inc, page_size)
    VALUE self, value, lower, upper, step_inc, page_inc, page_size;
{
    set_gobject(self, GTK_OBJECT(gtk_adjustment_new(NUM2DBL(value),
						    NUM2DBL(lower),
						    NUM2DBL(upper),
						    NUM2DBL(step_inc),
						    NUM2DBL(page_inc),
						    NUM2DBL(page_size))));
    return Qnil;
}

static VALUE
adj_set_value(self, value)
    VALUE self, value;
{
    gtk_adjustment_set_value(GTK_ADJUSTMENT(get_gobject(self)),
			     NUM2DBL(value));
    return Qnil;
}

static VALUE
adj_get_value(self)
    VALUE self;
{
    return rb_float_new(GTK_ADJUSTMENT(get_gobject(self))->value);
}

static VALUE
adj_get_lower(self)
    VALUE self;
{
    return rb_float_new(GTK_ADJUSTMENT(get_gobject(self))->lower);
}

static VALUE
adj_get_upper(self)
    VALUE self;
{
    return rb_float_new(GTK_ADJUSTMENT(get_gobject(self))->upper);
}

static VALUE
adj_get_step_increment(self)
    VALUE self;
{
    return rb_float_new(GTK_ADJUSTMENT(get_gobject(self))->step_increment);
}

static VALUE
adj_set_step_increment(self, inc)
    VALUE self, inc;
{
    GTK_ADJUSTMENT(get_gobject(self))->step_increment = NUM2DBL(inc);
    return self;
}

static VALUE
adj_get_page_increment(self)
    VALUE self;
{
    return rb_float_new(GTK_ADJUSTMENT(get_gobject(self))->page_increment);
}

static VALUE
adj_get_page_size(self)
    VALUE self;
{
    return rb_float_new(GTK_ADJUSTMENT(get_gobject(self))->page_size);
}

void Init_gtk_adjustment()
{
    static rbgtk_class_info cinfo;

    gAdjustment = rb_define_class_under(mGtk, "Adjustment", gData);
    cinfo.klass = gAdjustment;
    cinfo.gtype = GTK_TYPE_ADJUSTMENT;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gAdjustment, "initialize", adj_initialize, 6);
    rb_define_method(gAdjustment, "set_value", adj_set_value, 1);
    rb_define_method(gAdjustment, "value=", adj_set_value, 1);
    rb_define_method(gAdjustment, "value", adj_get_value, 0);
    rb_define_method(gAdjustment, "lower", adj_get_lower, 0);
    rb_define_method(gAdjustment, "upper", adj_get_upper, 0);
    rb_define_method(gAdjustment, "step_increment", adj_get_step_increment, 0);
    rb_define_method(gAdjustment, "step_increment=",
		     adj_set_step_increment, 1);
    rb_define_method(gAdjustment, "page_increment", adj_get_page_increment, 0);
    rb_define_method(gAdjustment, "page_size", adj_get_page_size, 0);
}

/*
 * Tooptips
 */
static VALUE
ttips_initialize(self)
    VALUE self;
{
    set_gobject(self, GTK_OBJECT(gtk_tooltips_new()));
    return Qnil;
}

static VALUE
ttips_set_tip(self, win, text, priv)
    VALUE self, win, text, priv;
{
    gtk_tooltips_set_tip(GTK_TOOLTIPS(get_gobject(self)),
			 get_widget(win),
			 NIL_P(text)?NULL:STR2CSTR(text),
			 NIL_P(priv)?NULL:STR2CSTR(priv));

    return self;
}

static VALUE
ttips_set_delay(self, delay)
    VALUE self, delay;
{
    gtk_tooltips_set_delay(GTK_TOOLTIPS(get_gobject(self)), NUM2INT(delay));

    return self;
}

static VALUE
ttips_enable(self)
    VALUE self;
{
    gtk_tooltips_enable(GTK_TOOLTIPS(get_gobject(self)));
    return self;
}

static VALUE
ttips_disable(self)
    VALUE self;
{
    gtk_tooltips_disable(GTK_TOOLTIPS(get_gobject(self)));
    return self;
}

void Init_gtk_tooltips()
{
    static rbgtk_class_info cinfo;

    gTooltips = rb_define_class_under(mGtk, "Tooltips", gData);
    cinfo.klass = gTooltips;
    cinfo.gtype = GTK_TYPE_TOOLTIPS;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gTooltips, "initialize", ttips_initialize, 0);
    rb_define_method(gTooltips, "set_tip", ttips_set_tip, 3);
    rb_define_method(gTooltips, "set_delay", ttips_set_delay, 1);
    rb_define_method(gTooltips, "enable", ttips_enable, 0);
    rb_define_method(gTooltips, "disable", ttips_disable, 0);
}
