/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgst.h"

/* Class: Gst::Plugin
 * Dynamically loadable Elements 
 */

static GstPlugin *
plugin_copy (const GstPlugin* plugin)
{
	GstPlugin *new_plugin;
	g_return_val_if_fail (plugin != NULL, NULL);
	new_plugin = g_new (GstPlugin, 1);
	*new_plugin = *plugin;
	return new_plugin;
}

GType
gst_plugin_get_type (void) {
	static GType our_type = 0;
	if (our_type == 0)
		our_type = g_boxed_type_register_static ("GstPlugin",
			(GBoxedCopyFunc)plugin_copy,
			(GBoxedFreeFunc)g_free);
	return our_type;
}

/* Method: loaded?
 * Returns: true if the plugin is loaded into memory, false otherwise.
 */
static VALUE
rb_gst_plugin_is_loaded (VALUE self)
{
	GstPlugin *plugin = RGST_PLUGIN (self);
	return CBOOL2RVAL (gst_plugin_is_loaded (plugin));
}

/*
 * Method: features
 *
 * Gets a list of all the features that this plugin provides.
 *
 * Returns: an array of Gst::PluginFeature objects. 
 */
static VALUE
rb_gst_plugin_get_features (VALUE self)
{
	GstPlugin *plugin;
	GList *list;
	VALUE arr;

	arr = rb_ary_new ();
	plugin = RGST_PLUGIN (self);

	for (list = gst_plugin_get_feature_list (plugin);
	     list != NULL;
	     list = g_list_next (list)) {
		GstPluginFeature *feature = GST_PLUGIN_FEATURE(list->data);
		rb_ary_push (arr, instanciate_pluginfeature (feature));
	}
	return arr;
}

/*
 * Method: each_feature { |plugin_feature| ... }
 *
 * Calls the block for each feature this plugin provides, passing
 * a reference to the Gst::PluginFeature object as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_plugin_each_feature (VALUE self)
{
	return rb_ary_yield (rb_gst_plugin_get_features (self));
}

/* Method: filename
 * Returns: the filename where this plugin comes from.
 */
static VALUE
rb_gst_plugin_get_filename (VALUE self)
{
	return CSTR2RVAL (gst_plugin_get_filename (RGST_PLUGIN (self)));
}

/* Method: description 
 * Returns: a long description of the plugin.
 */
static VALUE
rb_gst_plugin_get_description (VALUE self)
{
	return CSTR2RVAL (gst_plugin_get_description (RGST_PLUGIN (self)));
}

/* Method: package 
 * Returns: the name of the package this plugin belongs to.
 */
static VALUE
rb_gst_plugin_get_package (VALUE self)
{
	return CSTR2RVAL (gst_plugin_get_package (RGST_PLUGIN (self)));
}

/* Method: license 
 * Returns: the effective license of the plugin.
 */
static VALUE
rb_gst_plugin_get_license (VALUE self)
{
	return CSTR2RVAL (gst_plugin_get_license (RGST_PLUGIN (self)));
}

/* Method: origin
 * Returns: the URL to the provider of the plugin.
 */
static VALUE
rb_gst_plugin_get_origin (VALUE self)
{
	return CSTR2RVAL (gst_plugin_get_origin ( RGST_PLUGIN (self)));
}

/* Method: name 
 * Returns: the unique name of the plugin.
 */
static VALUE
rb_gst_plugin_get_name (VALUE self)
{
	return CSTR2RVAL (gst_plugin_get_name (RGST_PLUGIN (self)));
}

/* Method: minor_version 
 * Returns: the minor version of the core this plugin was compiled for.
 */
static VALUE
rb_gst_plugin_get_minor_version (VALUE self)
{
	return INT2FIX (RGST_PLUGIN (self)->desc.minor_version);
}

/* Method: major_version 
 * Returns: the major version of the core this plugin was compiled for.
 */
static VALUE
rb_gst_plugin_get_major_version (VALUE self)
{
	return INT2FIX (RGST_PLUGIN (self)->desc.major_version);
}

/* Method: version
 * Returns: the version of the plugin (note: as a string).
 */
static VALUE
rb_gst_plugin_get_version (VALUE self)
{
	return CSTR2RVAL (RGST_PLUGIN (self)->desc.version);
}

/* Method: ==(plugin)
 * Returns: true if two Gst::Plugin objects are refered by the same file,
 * false otherwise.
 */
static VALUE
rb_gst_plugin_is_equal (VALUE self, VALUE other_plugin)
{
	return NIL_P (other_plugin)
		? Qfalse
		: rb_equal (rb_gst_plugin_get_filename (self),
			    rb_gst_plugin_get_filename (other_plugin));
}

void
Init_gst_plugin (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_PLUGIN, "Plugin", mGst);

	rb_define_method (c, "loaded?", rb_gst_plugin_is_loaded, 0);
	rb_define_method (c, "features", rb_gst_plugin_get_features, 0);
	rb_define_method (c, "each_feature", rb_gst_plugin_each_feature, 0);
	rb_define_method (c, "==", rb_gst_plugin_is_equal, 1);

	rb_define_method (c, "filename", rb_gst_plugin_get_filename, 0);
	rb_define_method (c, "major_version", rb_gst_plugin_get_major_version, 
			  0); 
	rb_define_method (c, "minor_version", rb_gst_plugin_get_minor_version, 
			  0); 
	rb_define_method (c, "name", rb_gst_plugin_get_name, 0);
	rb_define_method (c, "description", rb_gst_plugin_get_description, 0);
	rb_define_method (c, "version", rb_gst_plugin_get_version, 0);
	rb_define_method (c, "license", rb_gst_plugin_get_license, 0);
	rb_define_method (c, "package", rb_gst_plugin_get_package, 0);
	rb_define_method (c, "origin", rb_gst_plugin_get_origin, 0);
}
