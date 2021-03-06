
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
 * Class: Gda::DataModelHash
 * An implementation of Gda::DataModel based on a hash table.
 */
 
/*
 * Class method: new(n_column)
 * n_column:  number of columns for rows in this data model.
 * 
 * Returns: a reference to a newly created Gda::DataModelHash object.
 */
static VALUE rb_gda_datamodel_hash_new(self, n_columns)
    VALUE self, n_columns;
{
    GdaDataModel *data = gda_data_model_hash_new(FIX2INT(n_columns));
    if (data != NULL) {
        G_INITIALIZE(self, data);
    }
    return Qnil;
}

/*
 * Method: set_n_columns(n_columns)
 * n_columns: number of columns for rows this data model should use (must be
 * greated than or equal to 0).
 *
 * Sets the number of columns for rows inserted in this model.
 *
 * Returns: self.
 */
static VALUE rb_gda_datamodel_hash_set_n_columns(self, n_columns)
    VALUE self, n_columns;
{
    gda_data_model_hash_set_n_columns(RGDA_DATAMODEL_HASH(self),
                                      FIX2INT(n_columns));
    return self;
}

/*
 * Method: clear
 *
 * Frees all the rows inserted in this model.
 *
 * Returns: self.
 */
static VALUE rb_gda_datamodel_hash_clear(self)
    VALUE self;
{
    gda_data_model_hash_clear(RGDA_DATAMODEL_HASH(self));
    return self;
}

#if !defined(GDA_AT_LEAST_1_3)
/*
 * Method: get_value_at(colnum, rownum)
 * colnum:  column number (starting from 0).
 * rownum:  row number (starting from 0).
 *
 * Retrieves the value at a specified column and row.
 *
 * Returns: a reference to a Gda::Value object if successful, nil
 * otherwise.
 */
static VALUE rb_gda_datamodel_hash_get_value_at(self, colnum, rownum)
    VALUE self, colnum, rownum;
{
    const GdaValue *val = gda_data_model_hash_get_value_at(RGDA_DATAMODEL(self),
                                                           FIX2INT(colnum),
                                                           FIX2INT(rownum));
    return val != NULL  
        ? RGDA_VALUE_NEW(val)
        : Qnil;
}

/*
 * Method: get_row(rownum)
 * rownum: a row number.
 *
 * Retrieves a row from the underlying hash table.
 *
 * Returns: a reference to a Gda::Row or nil if the requested row is not
 * in the hash table.
 */
static VALUE rb_gda_datamodel_hash_get_row(self, rownum)
    VALUE self, rownum;
{
    const GdaRow *row = gda_data_model_hash_get_row(RGDA_DATAMODEL(self),
                                                    FIX2INT(rownum));
    return row != NULL  
        ? RGDA_ROW_NEW(row)
        : Qnil;
}

#endif

/*
 * Method: insert_row(rownum, row)
 * rownum: the number of the row.
 * row: the row to insert, as a Gda::Row object.
 *
 * Inserts a row in the data model.
 *
 * Returns: self.
 */
static VALUE rb_gda_datamodel_hash_insert_row(self, rownum, row)
    VALUE self, rownum, row;
{
    gda_data_model_hash_insert_row(RGDA_DATAMODEL_HASH(self), 
                                   FIX2INT(rownum),
                                   RGDA_ROW(row));
    return self; 
}

void Init_gda_datamodel_hash(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_DATA_MODEL_HASH, "DataModelHash", mGda);
    
    rb_define_method(c, "initialize",    rb_gda_datamodel_hash_new,           1);
    rb_define_method(c, "set_n_columns", rb_gda_datamodel_hash_set_n_columns, 1);
    rb_define_method(c, "clear",         rb_gda_datamodel_hash_clear,         0);
#if !defined(GDA_AT_LEAST_1_3)
    rb_define_method(c, "get_value_at",  rb_gda_datamodel_hash_get_value_at,  2);
    rb_define_method(c, "get_row",       rb_gda_datamodel_hash_get_row,       1);
#endif
    rb_define_method(c, "insert_row",    rb_gda_datamodel_hash_insert_row,    2);
    G_DEF_SETTER(c, "n_columns"); 
}

