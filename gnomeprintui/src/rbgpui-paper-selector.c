/*
 * Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>
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

#include "rbgpui.h"
#include "libgnomeprintui-enum-types.h"

#include <gtk/gtkwidget.h>
#include <libgnomeprint/gnome-print-config.h>
#include <libgnomeprintui/gnome-print-paper-selector.h>

#define _SELF(self) (RVAL2GOBJ(self))
#define RVAL2GPSF(obj) (RVAL2GENUM(obj, GNOME_TYPE_PRINTUI_PAPER_SELECTOR_FLAGS))
/* #define RVAL2GPSF(obj) (NUM2INT(obj)) */

static VALUE
gpui_paper_selector_new(int argc, VALUE *argv, VALUE self)
{
  VALUE config, flags;
  rb_scan_args(argc, argv, "11", &config, &flags);

  if (NIL_P(flags)) {
    G_INITIALIZE(self, gnome_paper_selector_new(RVAL2GOBJ(config)));
  } else {
    G_INITIALIZE(self, gnome_paper_selector_new_with_flags(RVAL2GOBJ(config),
                                                           RVAL2GPSF(flags)));
  }
  return Qnil;
}

void
Init_gnome_paper_selector(VALUE mGnome)
{
  VALUE c = G_DEF_CLASS(GNOME_TYPE_PAPER_SELECTOR, "PaperSelector", mGnome);
  G_DEF_CLASS(GNOME_TYPE_PRINTUI_PAPER_SELECTOR_FLAGS,
              "PaperSelectorFlags", mGnome);

  rb_define_method(c, "initialize", gpui_paper_selector_new, -1);
}