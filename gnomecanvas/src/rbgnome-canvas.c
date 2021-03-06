/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-canvas.c,v 1.20 2007/07/13 16:07:30 ggc Exp $ */

/* Gnome::Canvas widget for Ruby/Gnome
 * Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgnomecanvas.h"
#include "rbart.h"

#define _SELF(self) GNOME_CANVAS(RVAL2GOBJ(self))

static VALUE
canvas_initialize(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE antialiased;
    GtkWidget* canvas = NULL;

    rb_scan_args(argc, argv, "01", &antialiased);
    if (RVAL2CBOOL(antialiased))
        canvas = gnome_canvas_new_aa();
    else
        canvas = gnome_canvas_new();

    RBGTK_INITIALIZE(self, canvas);

    return Qnil;
}

static VALUE
canvas_root(self)
    VALUE self;
{
    VALUE root;

    root = GOBJ2RVAL(gnome_canvas_root(_SELF(self)));
    G_CHILD_ADD(self, root);

    return root;
}

static VALUE
canvas_set_scroll_region(self, x1, y1, x2, y2)
    VALUE self, x1, y1, x2, y2;
{
    gnome_canvas_set_scroll_region(_SELF(self),
                                   NUM2DBL(x1),
                                   NUM2DBL(y1),
                                   NUM2DBL(x2),
                                   NUM2DBL(y2));
    return self;
}

static VALUE
canvas_get_scroll_region(self)
    VALUE self;
{
    double x1, y1, x2, y2;
    gnome_canvas_get_scroll_region(_SELF(self),
                                   &x1,
                                   &y1,
                                   &x2,
                                   &y2);
    return rb_ary_new3(4, rb_float_new(x1), rb_float_new(y1),
                       rb_float_new(x2), rb_float_new(y2));
}

static VALUE
canvas_set_center_scroll_region(self, center_scroll_region)
    VALUE self, center_scroll_region;
{
    gnome_canvas_set_center_scroll_region(_SELF(self),
                                          RVAL2CBOOL(center_scroll_region));
    return self;
}

static VALUE
canvas_get_center_scroll_region(self)
    VALUE self;
{
    gboolean result;
    result = gnome_canvas_get_center_scroll_region(_SELF(self));
    return CBOOL2RVAL(result);
}

static VALUE
canvas_set_pixels_per_unit(self, n)
    VALUE self, n;
{
    gnome_canvas_set_pixels_per_unit(_SELF(self),
                                     NUM2DBL(n));
    return self;
}

static VALUE
canvas_scroll_to(self, cx, cy)
    VALUE self, cx, cy;
{
    gnome_canvas_scroll_to(_SELF(self),
                           NUM2INT(cx),
                           NUM2INT(cy));
    return self;
}

static VALUE
canvas_get_scroll_offsets(self)
    VALUE self;
{
    int cx, cy;
    gnome_canvas_get_scroll_offsets(_SELF(self),
                                    &cx,
                                    &cy);
    return rb_ary_new3(2, INT2NUM(cx), INT2NUM(cy));
}

static VALUE
canvas_update_now(self)
    VALUE self;
{
    gnome_canvas_update_now(_SELF(self));
    return self;
}

static VALUE
canvas_get_item_at(self, x, y)
    VALUE self, x, y;
{
    GnomeCanvasItem* item;
    item = gnome_canvas_get_item_at(_SELF(self),
                                    NUM2DBL(x),
                                    NUM2DBL(y));
    return GOBJ2RVAL(item);
}


static VALUE
canvas_request_redraw(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    if (argc == 1){
        VALUE uta;
        rb_scan_args(argc, argv, "10", &uta);
        gnome_canvas_request_redraw_uta(_SELF(self), get_art_uta(uta));
    } else {
        VALUE x1, y1, x2, y2;
        rb_scan_args(argc, argv, "40", &x1, &y1, &x2, &y2);
        gnome_canvas_request_redraw(_SELF(self), NUM2INT(x1), NUM2INT(y1), NUM2INT(x2), NUM2INT(y2));
    }
    return self;
}

static VALUE
canvas_w2c_affine(self)
    VALUE self;
{
    double affine[6];
    gnome_canvas_w2c_affine(_SELF(self), affine);
    return make_art_affine(affine);
}

static VALUE
canvas_w2c(self, wx, wy)
    VALUE self, wx, wy;
{
    double cx, cy;
    gnome_canvas_w2c_d(_SELF(self),
                       NUM2DBL(wx),
                       NUM2DBL(wy),
                       &cx,
                       &cy);
    return rb_ary_new3(2, rb_float_new(cx), rb_float_new(cy));
}

static VALUE
canvas_c2w(self, cx, cy)
    VALUE self, cx, cy;
{
    double wx, wy;
    gnome_canvas_c2w(_SELF(self),
                     NUM2INT(cx),
                     NUM2INT(cy),
                     &wx,
                     &wy);
    return rb_ary_new3(2, rb_float_new(wx), rb_float_new(wy));
}

static VALUE
canvas_window_to_world(self, winx, winy)
    VALUE self, winx, winy;
{
    double worldx, worldy;
    gnome_canvas_window_to_world(_SELF(self),
                                 NUM2DBL(winx),
                                 NUM2DBL(winy),
                                 &worldx,
                                 &worldy);
    return rb_ary_new3(2, rb_float_new(worldx), rb_float_new(worldy));
}

static VALUE
canvas_world_to_window(self, worldx, worldy)
    VALUE self, worldx, worldy;
{
    double winx, winy;
    gnome_canvas_world_to_window(_SELF(self),
                                 NUM2DBL(worldx),
                                 NUM2DBL(worldy),
                                 &winx,
                                 &winy);
    return rb_ary_new3(2, rb_float_new(winx), rb_float_new(winy));
}

static VALUE
canvas_get_color(self, spec)
    VALUE self, spec;
{
    GdkColor color;
    gnome_canvas_get_color(_SELF(self), RVAL2CSTR(spec), &color);
    return BOXED2RVAL(&color, GDK_TYPE_COLOR);
}

static VALUE
canvas_get_color_pixel(self, rgba)
    VALUE self, rgba;
{
    return ULONG2NUM(gnome_canvas_get_color_pixel(_SELF(self), NUM2UINT(rgba)));
}

static VALUE
canvas_set_stipple_origin(self, gc)
    VALUE self, gc;
{
    gnome_canvas_set_stipple_origin(_SELF(self),
                                    GDK_GC(RVAL2GOBJ(gc)));
    return self;
}

static VALUE
canvas_set_dither(self, dither)
    VALUE self, dither;
{
    gnome_canvas_set_dither(_SELF(self), RVAL2GENUM(dither, GDK_TYPE_RGB_DITHER));
    return self;
}

static VALUE
canvas_get_dither(self)
    VALUE self;
{
    return GENUM2RVAL(gnome_canvas_get_dither(_SELF(self)), GDK_TYPE_RGB_DITHER);
}

static VALUE
canvas_get_aa(self)
    VALUE self;
{
    return CBOOL2RVAL(_SELF(self)->aa);
}

static VALUE
canvas_s_convert_color(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE r, g, b, a;
    gint ret;

    rb_scan_args(argc, argv, "31", &r, &g, &b, &a);

    if (NIL_P(a)){
        ret = GNOME_CANVAS_COLOR(NUM2INT(r), NUM2INT(g), NUM2INT(b));
    } else {
        ret = GNOME_CANVAS_COLOR_A(NUM2INT(r), NUM2INT(g), NUM2INT(b), NUM2INT(a));
    }
    return INT2NUM(ret);
}


static VALUE
buf_signal_render_background(num, values)
    guint num;
    const GValue* values;
{
    GnomeCanvasBuf* buf = (GnomeCanvasBuf*)g_value_get_pointer(&values[1]);

    return rb_ary_new3(2, GVAL2RVAL(&values[0]),
                       BOXED2RVAL(buf, GNOME_TYPE_CANVAS_BUF));
} 

void
Init_gnome_canvas(mGnome)
    VALUE mGnome;
{
    VALUE gnoCanvas = G_DEF_CLASS(GNOME_TYPE_CANVAS, "Canvas", mGnome);

    rb_define_const(gnoCanvas, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GNOMECANVAS_MAJOR_VERSION),
                                INT2FIX(GNOMECANVAS_MINOR_VERSION),
                                INT2FIX(GNOMECANVAS_MICRO_VERSION)));

    G_DEF_SIGNAL_FUNC(gnoCanvas, "render-background", 
                      (GValToRValSignalFunc)buf_signal_render_background);

    rb_define_method(gnoCanvas, "initialize", canvas_initialize, -1);
    rb_define_method(gnoCanvas, "root", canvas_root, 0);
    rb_define_method(gnoCanvas, "set_scroll_region", canvas_set_scroll_region, 4);
    rb_define_method(gnoCanvas, "scroll_region", canvas_get_scroll_region, 0);
    rb_define_method(gnoCanvas, "set_center_scroll_region", canvas_set_center_scroll_region, 1);
    rb_define_method(gnoCanvas, "center_scroll_region?", canvas_get_center_scroll_region, 0);
    rb_define_method(gnoCanvas, "set_pixels_per_unit", canvas_set_pixels_per_unit, 1);
    rb_define_method(gnoCanvas, "scroll_to", canvas_scroll_to, 2);
    rb_define_method(gnoCanvas, "scroll_offsets", canvas_get_scroll_offsets, 0);
    rb_define_method(gnoCanvas, "update_now", canvas_update_now, 0);
    rb_define_method(gnoCanvas, "get_item_at", canvas_get_item_at, 2);
    rb_define_method(gnoCanvas, "request_redraw", canvas_request_redraw, -1);
    rb_define_method(gnoCanvas, "w2c_affine", canvas_w2c_affine, 0);
    rb_define_method(gnoCanvas, "w2c", canvas_w2c, 2);
    rb_define_method(gnoCanvas, "c2w", canvas_c2w, 2);
    rb_define_method(gnoCanvas, "window_to_world", canvas_window_to_world, 2);
    rb_define_method(gnoCanvas, "world_to_window", canvas_world_to_window, 2);
    rb_define_method(gnoCanvas, "get_color", canvas_get_color, 1);
    rb_define_method(gnoCanvas, "get_color_pixel", canvas_get_color_pixel, 1);
    rb_define_method(gnoCanvas, "set_stipple_origin", canvas_set_stipple_origin, 1);
    rb_define_method(gnoCanvas, "set_dither", canvas_set_dither, 1);
    rb_define_method(gnoCanvas, "dither", canvas_get_dither, 0);

    rb_define_singleton_method(gnoCanvas, "convert_color", canvas_s_convert_color, -1);
    rb_define_const(gnoCanvas, "EPSILON", rb_float_new(GNOME_CANVAS_EPSILON));

    /* TODO: wrap structure elements. */
    rb_define_method(gnoCanvas, "aa?", canvas_get_aa, 0);

    rb_define_alias(gnoCanvas, "w2c_d", "w2c");

    G_DEF_SETTER(gnoCanvas, "center_scroll_region");
    G_DEF_SETTER(gnoCanvas, "pixels_per_unit");
    G_DEF_SETTER(gnoCanvas, "stipple_origin");
    G_DEF_SETTER(gnoCanvas, "dither");
}
