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

#include "rbgp.h"

#define WE_ARE_LIBGNOMEPRINT_INTERNALS
#include <libgnomeprint/private/gpa-root.h>

static VALUE
gp_gpa_get_printers(VALUE self)
{
  VALUE array = rb_ary_new();
  GPANode *printers = GPA_NODE(gpa_get_printers());
  GPANode *printer;

  printer = gpa_node_get_child (printers, NULL);
  while (printer) {
    rb_ary_push(array, GOBJ2RVAL(printer));
    printer = gpa_node_get_child(printers, printer);
  }
  gpa_node_unref(printers);
  
  return array;
}


void
Init_gnome_print_gpa_root(VALUE mGnome)
{
  VALUE mGPA = rb_define_module_under(mGnome, "GPARoot");
  
  gpa_init();

  rb_define_module_function(mGPA, "printers", gp_gpa_get_printers, 0);
}