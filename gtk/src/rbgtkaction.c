/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaction.c -

  $Author: mutoh $
  $Date: 2004/05/30 16:41:13 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "global.h"
                                                                                
#if GTK_CHECK_VERSION(2,4,0)

#define _SELF(self) (GTK_ACTION(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
action_initialize(self, name, label, tooltip, stock_id)
    VALUE self, name, label, tooltip, stock_id;
{
    G_INITIALIZE(self, gtk_action_new(RVAL2CSTR(name),
                                      RVAL2CSTR(label),
                                      RVAL2CSTR(tooltip),
                                      RVAL2CSTR(stock_id)));
    return Qnil;
}

static VALUE
action_get_sensitive(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_action_get_sensitive(_SELF(self)));
}

static VALUE
action_get_visible(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_action_get_visible(_SELF(self)));
}

/* Defined as property.
const gchar* gtk_action_get_name            (GtkAction *action);
gboolean    gtk_action_is_sensitive         (GtkAction *action);
gboolean    gtk_action_is_visible           (GtkAction *action);
*/

static VALUE
action_activate(self)
    VALUE self;
{
    gtk_action_activate(_SELF(self));
    return self;
}

static VALUE
action_create_icon(self, icon_size)
    VALUE self, icon_size;
{
    return GOBJ2RVAL(gtk_action_create_icon(_SELF(self), 
                                            RVAL2GENUM(icon_size, GTK_TYPE_ICON_SIZE)));
}

static VALUE
action_create_menu_item(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_action_create_menu_item(_SELF(self)));
}

static VALUE
action_create_tool_item(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_action_create_tool_item(_SELF(self)));
}

static VALUE
action_connect_proxy(self, proxy)
    VALUE self, proxy;
{
    gtk_action_connect_proxy(_SELF(self), RVAL2WIDGET(proxy));
    return self;
}

static VALUE
action_disconnect_proxy(self, proxy)
    VALUE self, proxy;
{
    gtk_action_disconnect_proxy(_SELF(self), RVAL2WIDGET(proxy));
    return self;
}

static VALUE
action_get_proxies(self)
    VALUE self;
{
    return GSLIST2ARY(gtk_action_get_proxies(_SELF(self)));
}

static VALUE
action_connect_accelerator(self)
    VALUE self;
{
    gtk_action_connect_accelerator(_SELF(self));
    return self;
}

static VALUE
action_disconnect_accelerator(self)
    VALUE self;
{
    gtk_action_disconnect_accelerator(_SELF(self));
    return self;
}

static VALUE
action_block_activate_from(self, proxy)
    VALUE self, proxy;
{
    gtk_action_block_activate_from(_SELF(self), RVAL2WIDGET(proxy));
    return self;
}

static VALUE
action_unblock_activate_from(self, proxy)
    VALUE self, proxy;
{
    gtk_action_unblock_activate_from(_SELF(self), RVAL2WIDGET(proxy));
    return self;
}

static VALUE
action_set_accel_path(self, accel_path)
    VALUE self, accel_path;
{
    gtk_action_set_accel_path(_SELF(self), RVAL2CSTR(accel_path));
    return self;
}
static VALUE
action_set_accel_group(self, accel_group)
    VALUE self, accel_group;
{
    gtk_action_set_accel_group(_SELF(self), RVAL2GOBJ(accel_group));
    return self;
}

#endif

void 
Init_gtk_action()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE gAction = G_DEF_CLASS(GTK_TYPE_ACTION, "Action", mGtk);

    rb_define_method(gAction, "initialize", action_initialize, 4);
    /* (NOTICE) Gtk::Action#sensitive, #visible are special.
       Because there are also Gtk::Action#sensitive?, #visible? as property 
       accessors. */
    rb_define_method(gAction, "sensitive", action_get_sensitive, 0);
    rb_define_method(gAction, "visible", action_get_visible, 0);
    rb_define_method(gAction, "activate", action_activate, 0);
    rb_define_method(gAction, "create_icon", action_create_icon, 1);
    rb_define_method(gAction, "create_menu_item", action_create_menu_item, 0);
    rb_define_method(gAction, "create_tool_item", action_create_tool_item, 0);
    rb_define_method(gAction, "connect_proxy", action_connect_proxy, 1);
    rb_define_method(gAction, "disconnect_proxy", action_disconnect_proxy, 1);
    rb_define_method(gAction, "proxies", action_get_proxies, 0);
    rb_define_method(gAction, "connect_accelerator", action_connect_accelerator, 0);
    rb_define_method(gAction, "disconnect_accelerator", action_disconnect_accelerator, 0);
    rb_define_method(gAction, "block_activate_from", action_block_activate_from, 1);
    rb_define_method(gAction, "unblock_activate_from", action_unblock_activate_from, 1);
    rb_define_method(gAction, "set_accel_path", action_set_accel_path, 1);
    rb_define_method(gAction, "set_accel_group", action_set_accel_group, 1);

    G_DEF_SETTERS(gAction);
#endif
}