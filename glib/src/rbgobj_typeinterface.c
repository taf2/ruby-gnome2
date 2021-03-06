/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_typeinterface.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:28 $
  created at: Sat May 27 16:04:13 JST 2006

  Copyright (C) 2002-2006  Ruby-GNOME2 Project Team
  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include "rbgprivate.h"

VALUE rbgobj_mInterface;
VALUE mMetaInterface;

static VALUE
interface_s_append_features(self, klass)
     VALUE self, klass;
{
    if (!rb_obj_is_kind_of(klass, cInstantiatable))
        rb_raise(rb_eTypeError, "Not a subclass of GLib::Instantiatable");
    return rb_call_super(1, &klass);
}

#if GLIB_CHECK_VERSION(2,4,0)

static VALUE
interface_s_install_property(self, pspec_obj)
    VALUE self, pspec_obj;
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(self);
    gpointer ginterface;
    GParamSpec* pspec;

    if (cinfo->klass != self)
        rb_raise(rb_eTypeError, "%s isn't registered class", rb_class2name(self));

    pspec = G_PARAM_SPEC(RVAL2GOBJ(pspec_obj));
    ginterface = g_type_default_interface_ref(cinfo->gtype);
    g_object_interface_install_property(ginterface, pspec);
    g_type_default_interface_unref(ginterface);

    /* FIXME: define accessor methods */
    return Qnil;
}

static VALUE
interface_s_property(self, property_name)
    VALUE self, property_name;
{
    gpointer ginterface;
    const char* name;
    GParamSpec* prop;
    VALUE result;
    GType gtype = CLASS2GTYPE(self);

    if (SYMBOL_P(property_name))
        name = rb_id2name(SYM2ID(property_name));
    else
        name = StringValuePtr(property_name);

    if (!G_TYPE_IS_INTERFACE(gtype))
        rb_raise(rb_eTypeError, "%s isn't interface module", rb_class2name(self));
    /* XXX: g_type_default_interface_ref(G_TYPE_INTERFACE) causes SEGV. */
    if (gtype == G_TYPE_INTERFACE) {
        rb_raise(rb_const_get(mGLib, rb_intern("NoPropertyError")),
                 "No such property: %s", name);
    }

    ginterface = g_type_default_interface_ref(gtype);
    prop = g_object_interface_find_property(ginterface, name);
    if (!prop){
        g_type_default_interface_unref(ginterface);
        rb_raise(rb_const_get(mGLib, rb_intern("NoPropertyError")),
                 "No such property: %s", name);
    }
    result = GOBJ2RVAL(prop);
    g_type_default_interface_unref(ginterface);

    return result;
}

static VALUE
interface_s_properties(int argc, VALUE* argv, VALUE self)
{
    guint n_properties;
    GParamSpec** props;
    VALUE inherited_too;
    VALUE ary = rb_ary_new();
    int i;
    gpointer ginterface;
    GType gtype = CLASS2GTYPE(self);

    if (rb_scan_args(argc, argv, "01", &inherited_too) == 0)
        inherited_too = Qtrue;

    if (!G_TYPE_IS_INTERFACE(gtype))
        rb_raise(rb_eTypeError, "%s isn't interface module", rb_class2name(self));
    /* XXX: g_type_default_interface_ref(G_TYPE_INTERFACE) causes SEGV. */
    if (gtype == G_TYPE_INTERFACE) return ary;

    ginterface = g_type_default_interface_ref(gtype);
    props = g_object_interface_list_properties(ginterface, &n_properties);
    for (i = 0; i < n_properties; i++){
        if (RVAL2CBOOL(inherited_too) || GTYPE2CLASS(props[i]->owner_type) == self)
            rb_ary_push(ary, rb_str_new2(props[i]->name));
    }
    g_free(props);
    g_type_default_interface_unref(ginterface);

    return ary;
}

#endif

void
rbgobj_init_interface(VALUE interf)
{
    static VALUE rb_mGLibInterface = Qnil;

    rb_extend_object(interf, mMetaInterface);
    if (CLASS2GTYPE(interf) == G_TYPE_INTERFACE) {
	rb_mGLibInterface = interf;
    } else {
        rb_extend_object(interf, rb_mGLibInterface);
        rb_include_module(interf, rb_mGLibInterface);
        rbgobj_define_property_accessors(interf);
    }
}

void
Init_gobject_typeinterface()
{
    mMetaInterface = rb_define_module_under(mGLib, "MetaInterface");
    rb_define_method(mMetaInterface, "gtype", generic_s_gtype, 0);
    rb_define_method(mMetaInterface, "append_features", interface_s_append_features, 1);
#if GLIB_CHECK_VERSION(2,4,0)
    rb_define_method(mMetaInterface, "install_property", interface_s_install_property, 1);
    rb_define_method(mMetaInterface, "property", interface_s_property, 1);
    rb_define_method(mMetaInterface, "properties", interface_s_properties, -1);
#endif

    rbgobj_mInterface = G_DEF_INTERFACE(G_TYPE_INTERFACE, "Interface", mGLib);
}
