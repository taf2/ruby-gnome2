
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
 * Class: Gda::FieldAttributes
 * Management of database fields.
 */
VALUE cGdaFieldAttributes;

#if defined(GDA_AT_LEAST_1_3)
#  define gda_field_attributes_get_defined_size     gda_column_get_defined_size
#  define gda_field_attributes_set_defined_size     gda_column_set_defined_size
#  define gda_field_attributes_get_name             gda_column_get_name
#  define gda_field_attributes_set_name             gda_column_set_name
#  define gda_field_attributes_get_table            gda_column_get_table
#  define gda_field_attributes_set_table            gda_column_set_table
#  define gda_field_attributes_get_caption          gda_column_get_caption
#  define gda_field_attributes_set_caption          gda_column_set_caption
#  define gda_field_attributes_get_scale            gda_column_get_scale
#  define gda_field_attributes_set_scale            gda_column_set_scale
#  define gda_field_attributes_get_allow_null       gda_column_get_allow_null
#  define gda_field_attributes_set_allow_null       gda_column_set_allow_null
#  define gda_field_attributes_get_primary_key      gda_column_get_primary_key
#  define gda_field_attributes_set_primary_key      gda_column_set_primary_key
#  define gda_field_attributes_get_unique_key       gda_column_get_unique_key
#  define gda_field_attributes_set_unique_key       gda_column_set_unique_key
#  define gda_field_attributes_get_references       gda_column_get_references
#  define gda_field_attributes_set_references       gda_column_set_references
#  define gda_field_attributes_get_auto_increment   gda_column_get_auto_increment
#  define gda_field_attributes_set_auto_increment   gda_column_set_auto_increment
#  define gda_field_attributes_get_position         gda_column_get_position
#  define gda_field_attributes_set_position         gda_column_set_position
#endif

/*
 * Class method: new
 *
 * Returns: a newly created Gda::FieldAttributes object.
 */
static VALUE rb_gda_field_new(self)
    VALUE self;
{
#if defined(GDA_AT_LEAST_1_3)
    GdaColumn *attr = gda_column_new();
#else
    GdaFieldAttributes *attr = gda_field_attributes_new();
#endif
    if (attr != NULL) {
        G_INITIALIZE(self, attr);
    }
    return Qnil;
}

/*
 * Method: defined_size
 *
 * Returns: the defined size of the field attributes.
 */
static VALUE rb_gda_field_get_defined_size(self)
    VALUE self;
{
    return LONG2FIX(gda_field_attributes_get_defined_size(RGDA_FIELD_ATTRIBUTES(self)));
}

/*
 * Method: set_defined_size(size)
 * size: the defined size we want to set.
 * 
 * Sets the defined size of the field attributes.
 *
 * Returns: self.
 */
static VALUE rb_gda_field_set_defined_size(self, defined_size)
    VALUE self, defined_size;
{
    gda_field_attributes_set_defined_size(RGDA_FIELD_ATTRIBUTES(self),
                                          FIX2LONG(defined_size));
    return self;
}

/*
 * Method: name
 *
 * Returns: the name of the field attributes.
 */
static VALUE rb_gda_field_get_name(self)
    VALUE self;
{
    return CSTR2RVAL(gda_field_attributes_get_name(RGDA_FIELD_ATTRIBUTES(self)));
}

/*
 * Method: set_name(name)
 * name: the new name of the field attributes.
 *
 * Sets the name of the field attributes.
 *
 * Returns: self.
 */
static VALUE rb_gda_field_set_name(self, name)
    VALUE self, name;
{
    gda_field_attributes_set_name(RGDA_FIELD_ATTRIBUTES(self),
                                  RVAL2CSTR(name));
    return self;
}

/*
 * Method: table
 *
 * Returns: the name of the table to which this field belongs.
 */
static VALUE rb_gda_field_get_table(self)
    VALUE self;
{
    return CSTR2RVAL(gda_field_attributes_get_table(RGDA_FIELD_ATTRIBUTES(self)));
}

/*
 * Method: set_table(name)
 * name: a table name.
 *
 * Sets the name of the table to which this field belongs.
 *
 * Returns: self.
 */
static VALUE rb_gda_field_set_table(self, table)
    VALUE self, table;
{
    gda_field_attributes_set_table(RGDA_FIELD_ATTRIBUTES(self),
                                   RVAL2CSTR(table));
    return self;
}

/*
 * Method: caption
 *
 * Returns: the caption of the field attributes.
 */
static VALUE rb_gda_field_get_caption(self)
    VALUE self;
{
    return CSTR2RVAL(gda_field_attributes_get_caption(RGDA_FIELD_ATTRIBUTES(self)));
}

/*
 * Method: set_caption(caption)
 * caption: the new caption of the field attributes.
 *
 * Sets the caption of the field attributes.
 *
 * Returns: self.
 */
static VALUE rb_gda_field_set_caption(self, caption)
    VALUE self, caption;
{
    gda_field_attributes_set_caption(RGDA_FIELD_ATTRIBUTES(self),
                                     RVAL2CSTR(caption));
    return self;
}

/*
 * Method: scale
 *
 * Returns: the scale of the field attributes.
 */
static VALUE rb_gda_field_get_scale(self)
    VALUE self;
{
    return LONG2FIX(gda_field_attributes_get_scale(RGDA_FIELD_ATTRIBUTES(self)));
}

/*
 * Method: set_scale(scale)
 * scale: the new scale of the field attributes.
 *
 * Sets the scale of the field attributes.
 *
 * Returns: self.
 */
static VALUE rb_gda_field_set_scale(self, scale)
    VALUE self, scale;
{
    gda_field_attributes_set_scale(RGDA_FIELD_ATTRIBUTES(self),
                                   FIX2LONG(scale));
    return self;
}
 
#if 0    /* FIXME re-implement these methods in the-Ruby-way */
/*
 * DEPRECATED Method: gdatype
 *
 * Returns: the gdatype of the field attributes (see Gda::Value::Type).
 */
static VALUE rb_gda_field_get_gdatype(self)
    VALUE self;
{
    return GENUM2RVAL(gda_field_attributes_get_gdatype(RGDA_FIELD_ATTRIBUTES(self)),
                      GDA_TYPE_VALUE_TYPE);
}

/*
 * DEPRECATED Method: set_gdatype(gdatype)
 * gdatype: the new gdatype of the field attributes (see Gda::Value::Type).
 *
 * Sets the gdatype of the field attributes.
 *
 * Returns: self.
 */
static VALUE rb_gda_field_set_gdatype(self, gdatype)
    VALUE self, gdatype;
{
    gda_field_attributes_set_gdatype(RGDA_FIELD_ATTRIBUTES(self),
                                     RVAL2GENUM(gdatype, GDA_TYPE_VALUE_TYPE));
    return self;
}
#endif

/*
 * Method: allow_null?
 *
 * Gets the 'allow null' flag of the field attributes.
 *
 * Returns: whether the field allows null values or not (true or false).
 */
static VALUE rb_gda_field_get_allow_null(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_field_attributes_get_allow_null(RGDA_FIELD_ATTRIBUTES(self)));
}

/*
 * Method: set_allow_null(allow)
 * allow: whether the given field should allows null values or not.
 *
 * Sets the 'allow null' flag of the field attributes.
 *
 * Returns: self.
 */
static VALUE rb_gda_field_set_allow_null(self, allow_null)
    VALUE self, allow_null;
{
    gda_field_attributes_set_allow_null(RGDA_FIELD_ATTRIBUTES(self),
                                        RVAL2CBOOL(allow_null));
    return self;
}

/*
 * Method: primary_key?
 *
 * Returns: whether if the field is a primary key (true or false).
 */
static VALUE rb_gda_field_get_primary_key(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_field_attributes_get_primary_key(RGDA_FIELD_ATTRIBUTES(self)));
}

/*
 * Method: set_primary_key(pk)
 * pk: whether if the field should be a primary key.
 *
 * Sets the 'primary key' flag of the field attributes.
 *
 * Returns: self.
 */
static VALUE rb_gda_field_set_primary_key(self, primary_key)
    VALUE self, primary_key;
{
    gda_field_attributes_set_primary_key(RGDA_FIELD_ATTRIBUTES(self),
                                         RVAL2CBOOL(primary_key));
    return self;
}

/*
 * Method: unique_key 
 *
 * Returns: whether the field is an unique key (true or false).
 */
static VALUE rb_gda_field_get_unique_key(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_field_attributes_get_unique_key(RGDA_FIELD_ATTRIBUTES(self)));
}

/*
 * Method: set_unique_key(uk)
 * uk: whether if the field should be a unique key.
 *
 * Sets the 'unique key' flag of the field attributes.
 *
 * Returns: self.
 */
static VALUE rb_gda_field_set_unique_key(self, unique_key)
    VALUE self, unique_key;
{
    gda_field_attributes_set_unique_key(RGDA_FIELD_ATTRIBUTES(self),
                                        RVAL2CBOOL(unique_key));
    return self;
}

/*
 * Method: references
 *
 * Returns: the references of the field attributes.
 */
static VALUE rb_gda_field_get_references(self)
    VALUE self;
{
    return CSTR2RVAL(gda_field_attributes_get_references(RGDA_FIELD_ATTRIBUTES(self)));
}

/*
 * Method: set_references(references)
 * references: the new references of the field attributes.
 *
 * Sets the references of the field attributes.
 *
 * Returns: self.
 */
static VALUE rb_gda_field_set_references(self, references)
    VALUE self, references;
{
    gda_field_attributes_set_references(RGDA_FIELD_ATTRIBUTES(self),
                                        RVAL2CSTR(references));
    return self;
}

/*
 * Method: auto_increment?
 *
 * Returns: whether the given field is an auto incremented one (true or false).
 */
static VALUE rb_gda_field_get_auto_increment(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_field_attributes_get_auto_increment(RGDA_FIELD_ATTRIBUTES(self)));
}

/*
 * Method: set_auto_increment(is_auto)
 * is_auto: auto increment boolean status.
 *
 * Sets the auto increment flag for the field.
 *
 * Returns: self.
 */
static VALUE rb_gda_field_set_auto_increment(self, auto_increment)
    VALUE self, auto_increment;
{
    gda_field_attributes_set_auto_increment(RGDA_FIELD_ATTRIBUTES(self),
                                        RVAL2CBOOL(auto_increment));
    return self;
}

/*
 * Method: position
 *
 * Returns: the position of the field the attributes refer to in the containing
 * data model.
 */
static VALUE rb_gda_field_get_position(self)
    VALUE self;
{
    return INT2FIX(gda_field_attributes_get_position(RGDA_FIELD_ATTRIBUTES(self)));
}

/*
 * Method: set_position(position)
 * position: the wanted position of the field in the containing data model.
 *
 * Sets the position of the field the attributes refer to in the containing
 * data model.
 *
 * Returns: self.
 */
static VALUE rb_gda_field_set_position(self, position)
    VALUE self, position;
{
    gda_field_attributes_set_position(RGDA_FIELD_ATTRIBUTES(self),
                                      FIX2INT(position));
    return self;
}

#if defined(GDA_AT_LEAST_1_3)
#  undef gda_field_attributes_get_undefd_size
#  undef gda_field_attributes_set_undefd_size
#  undef gda_field_attributes_get_name
#  undef gda_field_attributes_set_name
#  undef gda_field_attributes_get_table
#  undef gda_field_attributes_set_table
#  undef gda_field_attributes_get_caption
#  undef gda_field_attributes_set_caption
#  undef gda_field_attributes_get_scale
#  undef gda_field_attributes_set_scale
#  undef gda_field_attributes_get_allow_null
#  undef gda_field_attributes_set_allow_null
#  undef gda_field_attributes_get_primary_key
#  undef gda_field_attributes_set_primary_key
#  undef gda_field_attributes_get_unique_key
#  undef gda_field_attributes_set_unique_key
#  undef gda_field_attributes_get_references
#  undef gda_field_attributes_set_references
#  undef gda_field_attributes_get_auto_increment
#  undef gda_field_attributes_set_auto_increment
#  undef gda_field_attributes_get_position
#  undef gda_field_attributes_set_position
#endif

void Init_gda_field_attributes(void) {
#if defined(GDA_AT_LEAST_1_3)
    VALUE c = G_DEF_CLASS(GDA_TYPE_COLUMN, "Column", mGda);
#else
    VALUE c = G_DEF_CLASS(GDA_TYPE_FIELD_ATTRIBUTES, "FieldAttributes", mGda);
#endif

    rb_define_method(c, "initialize", rb_gda_field_new, 0);

    rb_define_method(c, "defined_size",     rb_gda_field_get_defined_size, 0);
    rb_define_method(c, "set_defined_size", rb_gda_field_set_defined_size, 1);

    rb_define_method(c, "name",     rb_gda_field_get_name, 0);
    rb_define_method(c, "set_name", rb_gda_field_set_name, 1);

    rb_define_method(c, "table",     rb_gda_field_get_table, 0);
    rb_define_method(c, "set_table", rb_gda_field_set_table, 1);

    rb_define_method(c, "caption",     rb_gda_field_get_caption, 0);
    rb_define_method(c, "set_caption", rb_gda_field_set_caption, 1);

    rb_define_method(c, "scale",     rb_gda_field_get_scale, 0);
    rb_define_method(c, "set_scale", rb_gda_field_set_scale, 1);

/*  Deprecated
    rb_define_method(c, "gdatype",     rb_gda_field_get_gdatype, 0);
    rb_define_method(c, "set_gdatype", rb_gda_field_set_gdatype, 1);
*/
    rb_define_method(c, "allow_null?",    rb_gda_field_get_allow_null,  0);
    rb_define_method(c, "set_allow_null", rb_gda_field_set_allow_null, 1);

    rb_define_method(c, "primary_key?",    rb_gda_field_get_primary_key, 0);
    rb_define_method(c, "set_primary_key", rb_gda_field_set_primary_key, 1);

    rb_define_method(c, "unique_key?",    rb_gda_field_get_unique_key, 0);
    rb_define_method(c, "set_unique_key", rb_gda_field_set_unique_key, 1);

    rb_define_method(c, "references",     rb_gda_field_get_references, 0);
    rb_define_method(c, "set_references", rb_gda_field_set_references, 1);

    rb_define_method(c, "auto_increment?",    rb_gda_field_get_auto_increment, 0);
    rb_define_method(c, "set_auto_increment", rb_gda_field_set_auto_increment, 1);

    rb_define_method(c, "position",     rb_gda_field_get_position, 0);
    rb_define_method(c, "set_position", rb_gda_field_set_position, 1);

    G_DEF_SETTERS(c);

    cGdaFieldAttributes = c;
}

