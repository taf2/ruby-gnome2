
/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 *
 * This file is part of Ruby/Libgda.
 *
 * Ruby/Libgda is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/Libgda is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/Libgda; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgda.h"

/*
 * Class: Gda::ParameterList
 * A list of Gda::Parameter objects.
 */
VALUE cGdaParameterList;

/*
 * Class method: new(*parameters=nil)
 * parameters: a list of Gda::Parameter objects.
 *
 * Creates a new Gda::ParameterList from optional Gda::Parameter objects.
 *
 * Returns: the newly created Gda::ParameterList object.
 */
static VALUE rb_gda_param_list_new(self, params)
    VALUE self, params;
{
    GdaParameterList *list;
    int i;
    
    list = gda_parameter_list_new();
    if (list != NULL) {
        for (i = 0; i < RARRAY_LEN(params); i++) {
            gda_parameter_list_add_parameter(
                list,
                g_boxed_copy(GDA_TYPE_PARAMETER,
                             RGDA_PARAMETER(rb_ary_entry(params, i))));
        }
        G_INITIALIZE(self, list);
    }
    return Qnil;
}

/*
 * Method: add(param)
 * param: the Gda::Parameter to be added to the list.
 *
 * Adds a new parameter to the list.
 *
 * Returns: self.
 */
static VALUE rb_gda_param_list_add(self, param)
    VALUE self, param;
{
    gda_parameter_list_add_parameter(RGDA_PARAMETER_LIST(self),
                                     g_boxed_copy(GDA_TYPE_PARAMETER,
				                  (RGDA_PARAMETER(param))));
    return self;
}

/*
 * Method: names
 *
 * Gets the names of all parameters in the parameter list.
 *
 * Returns: an Array containing the names of all parameters.
 */
static VALUE rb_gda_param_list_get_names(self)
    VALUE self;
{
    GList *node, *names;
    VALUE arr;

    arr = rb_ary_new();
    names = gda_parameter_list_get_names(RGDA_PARAMETER_LIST(self));
    for (node = g_list_first(names);
         node != NULL;
         node = g_list_next(node))
    {
        rb_ary_push(arr, CSTR2RVAL((const gchar *)node->data));
    }
    g_list_free(names);
    return arr;
}

/*
 * Method: each_name { |name| ... }
 *
 * Calls the block for each parameter in the list, passing its name as
 * parameter.
 *
 * Returns: always nil.
 */
static VALUE rb_gda_param_list_each_name(self)
    VALUE self;
{
    rb_ary_each(rb_gda_param_list_get_names(self));
    return Qnil;
}

/*
 * Method: parameters
 *
 * Gets all parameters in the parameter list.
 *
 * Returns: an Array of Gda::Parameter objects.
 */
static VALUE rb_gda_param_list_get_params(self)
    VALUE self;
{
    GdaParameterList *list;
    GList *node, *names;
    VALUE arr;

    arr = rb_ary_new();
    list = RGDA_PARAMETER_LIST(self);
    names = gda_parameter_list_get_names(list);
    for (node = g_list_first(names);
         node != NULL;
         node = g_list_next(node))
    {
        GdaParameter *param = gda_parameter_list_find(list, (const gchar *)node->data);
        if (param != NULL) {    /* normally should always be non-null... */
            rb_ary_push(arr, RGDA_PARAMETER_NEW(param));
        }
    }
    g_list_free(names);
    return arr;
}

/*
 * Method: each_parameter { |param| ... }
 *
 * Calls the block for each parameter in the parameter list, passing a
 * reference to a Gda::Parameter object as argument.
 *
 * Returns: always nil.
 */
static VALUE rb_gda_param_list_each_param(self)
    VALUE self;
{
    rb_ary_each(rb_gda_param_list_get_params(self));
    return Qnil;
}

/*
 * Method: find(name)
 * name: name of the parameter to search for.
 *
 * Gets a parameter from the list given its name.
 *
 * Returns: a Gda::Parameter reference if found, nil if not found.
 */
static VALUE rb_gda_param_list_find(self, name)
    VALUE self, name;
{
    GdaParameter *param = gda_parameter_list_find(RGDA_PARAMETER_LIST(self),
                                                  RVAL2CSTR(name));
    return param != NULL
        ? RGDA_PARAMETER_NEW(param)
        : Qnil;
}

/*
 * Method: length
 *
 * Returns: the number of parameters stored in the list.
 */
static VALUE rb_gda_param_list_get_length(self)
    VALUE self;
{
    return INT2FIX(gda_parameter_list_get_length(RGDA_PARAMETER_LIST(self)));
}

/*
 * Method: empty?
 *
 * Returns: true if the list is empty, false otherwise.
 */
static VALUE rb_gda_param_list_is_empty(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_parameter_list_get_length(RGDA_PARAMETER_LIST(self)) == 0);
}

/*
 * Method: clear
 *
 * Removes all parameters from the parameter list.
 *
 * Returns: self.
 */
static VALUE rb_gda_param_list_clear(self)
    VALUE self;
{
    gda_parameter_list_clear(RGDA_PARAMETER_LIST(self));
    return self;
}

void Init_gda_parameter_list(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_PARAMETER_LIST, "ParameterList", mGda);

    rb_define_method(c, "initialize", rb_gda_param_list_new, -2);

    rb_define_method(c, "add", rb_gda_param_list_add, 1);
    rb_define_alias(c, "<<", "add");

    rb_define_method(c, "names",     rb_gda_param_list_get_names, 0);
    rb_define_method(c, "each_name", rb_gda_param_list_each_name, 0);

    rb_define_method(c, "parameters",     rb_gda_param_list_get_params, 0);
    rb_define_method(c, "each_parameter", rb_gda_param_list_each_param, 0);

    rb_define_method(c, "find", rb_gda_param_list_find, 1);
    rb_define_alias(c, "[]", "find");

    rb_define_method(c, "length", rb_gda_param_list_get_length, 0);
    rb_define_alias(c, "size", "length");
    rb_define_method(c, "empty?", rb_gda_param_list_is_empty, 0);

    rb_define_method(c, "clear", rb_gda_param_list_clear, 0);

    cGdaParameterList = c;
}

