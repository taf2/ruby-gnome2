/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-druid-page-standard.c,v 1.1 2002/10/20 07:33:51 tkubo Exp $ */

#include "rbgnome.h"

#define _SELF(self) GNOME_DRUID_PAGE_STANDARD(RVAL2GOBJ(self))
#define _PIXBUF(obj) GDK_PIXBUF(RVAL2GOBJ(obj))
#define _COLOR(obj) ((GdkColor*)RVAL2BOXED(obj, GDK_TYPE_COLOR))

static VALUE
dstandard_initialize(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE title, logo, top_watermark;
    GtkWidget *result;

    if (argc != 0 && argc != 3)
        rb_raise(rb_eArgError, "wrong # of arguments(%d for 0 or 3)", argc);
    rb_scan_args(argc, argv, "03", &title, &logo, &top_watermark);
    if (argc == 0) {
        result = gnome_druid_page_standard_new();
    } else if (argc == 3) {
        result = gnome_druid_page_standard_new_with_vals(RVAL2CSTR(title),
                                                         NIL_P(logo) ? NULL : _PIXBUF(logo),
                                                         NIL_P(top_watermark) ? NULL : _PIXBUF(top_watermark));
    } else {
        rb_bug("report to Ruby/GNOME2 maintainer (dstandard_initialize)");
    }
    RBGTK_INITIALIZE(self, result);
    return Qnil;
}

static VALUE
dstandard_set_title(self, title)
    VALUE self, title;
{
    gnome_druid_page_standard_set_title(_SELF(self), RVAL2CSTR(title));
    return self;
}

static VALUE
dstandard_set_logo(self, logo_image)
    VALUE self, logo_image;
{
    gnome_druid_page_standard_set_logo(_SELF(self), _PIXBUF(logo_image));
    return self;
}

static VALUE
dstandard_set_top_watermark(self, top_watermark_image)
    VALUE self, top_watermark_image;
{
    gnome_druid_page_standard_set_top_watermark(_SELF(self), _PIXBUF(top_watermark_image));
    return self;
}

static VALUE
dstandard_set_title_foreground(self, color)
    VALUE self, color;
{
    gnome_druid_page_standard_set_title_foreground(_SELF(self), _COLOR(color));
    return self;
}

static VALUE
dstandard_set_background(self, color)
    VALUE self, color;
{
    gnome_druid_page_standard_set_background(_SELF(self), _COLOR(color));
    return self;
}

static VALUE
dstandard_set_logo_background(self, color)
    VALUE self, color;
{
    gnome_druid_page_standard_set_logo_background(_SELF(self), _COLOR(color));
    return self;
}

static VALUE
dstandard_set_contents_background(self, color)
    VALUE self, color;
{
    gnome_druid_page_standard_set_contents_background(_SELF(self), _COLOR(color));
    return self;
}

static VALUE
dstandard_append_item(self, question, item, additional_info)
    VALUE self, question, item, additional_info;
{
    gnome_druid_page_standard_append_item(_SELF(self),
                                          NIL_P(question) ? NULL : RVAL2CSTR(question),
                                          GTK_WIDGET(RVAL2GOBJ(item)),
                                          NIL_P(additional_info) ? NULL : RVAL2CSTR(additional_info));
    return self;
}

void
Init_gnome_druid_page_standard(mGnome)
    VALUE mGnome;
{
    VALUE gnoDruidPageStandard = G_DEF_CLASS(GNOME_TYPE_DRUID_PAGE_STANDARD, "DruidPageStandard", mGnome);

    rb_define_method(gnoDruidPageStandard, "initialize", dstandard_initialize, -1);
    rb_define_method(gnoDruidPageStandard, "set_title", dstandard_set_title, 1);
    rb_define_method(gnoDruidPageStandard, "set_logo", dstandard_set_logo, 1);
    rb_define_method(gnoDruidPageStandard, "set_top_watermark", dstandard_set_top_watermark, 1);
    rb_define_method(gnoDruidPageStandard, "set_title_foreground", dstandard_set_title_foreground, 1);
    rb_define_method(gnoDruidPageStandard, "set_background", dstandard_set_background, 1);
    rb_define_method(gnoDruidPageStandard, "set_logo_background", dstandard_set_logo_background, 1);
    rb_define_method(gnoDruidPageStandard, "set_contents_background", dstandard_set_contents_background, 1);
    rb_define_method(gnoDruidPageStandard, "append_item", dstandard_append_item, 3);

    G_DEF_SETTERS(gnoDruidPageStandard);
}
