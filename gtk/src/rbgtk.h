/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtk.h -

  $Author: sakai $
  $Date: 2007/07/06 10:13:27 $

  Copyright (C) 2003-2006 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifndef _RBGTK_H
#define _RBGTK_H

#include <rbgobject.h>
#include <rbgtkmacros.h>
#include <gtk/gtk.h>
#include <rbgdk.h>
#include <rbgtkconversions.h>

#include <signal.h>

#define RubyGtkContainerHookModule "RubyGtkContainerHook__"

#define RBGTK_MAJOR_VERSION RBGLIB_MAJOR_VERSION
#define RBGTK_MINOR_VERSION RBGLIB_MINOR_VERSION
#define RBGTK_MICRO_VERSION RBGLIB_MICRO_VERSION

#define RBGTK_INITIALIZE(obj,gtkobj)\
 (rbgtk_initialize_gtkobject(obj, GTK_OBJECT(gtkobj)))

#define GTK_TYPE_ALLOCATION (gtk_allocation_get_type())
#define GTK_TYPE_ACCEL_KEY (gtk_accel_key_get_type())
#define GTK_TYPE_ACCEL_GROUP_ENTRY (gtk_accel_group_entry_get_type())
#define GTK_TYPE_BINDING_SET (gtk_bindingset_get_type())
#define GTK_TYPE_TEXT_APPEARANCE (gtk_text_appearance_get_type())
#define GTK_TYPE_RECENT_FILTER_INFO (gtk_recent_filter_info_get_type())
#define GTK_TYPE_RECENT_DATA (gtk_recent_data_get_type())
#ifndef GTK_TYPE_TARGET_LIST
extern GType gtk_target_list_get_type();
#define GTK_TYPE_TARGET_LIST (gtk_target_list_get_type())
#endif

RUBY_GTK2_VAR VALUE mGtk;

/*
 * for gtk2.0/gtk2.2
 */
#ifndef GTK_TYPE_CLIPBOARD
#define GTK_TYPE_CLIPBOARD (rbgtk_clipboard_get_type())
#define RVAL2CLIPBOARD(obj) (rbgtk_get_clipboard(obj))
#define CLIPBOARD2RVAL(val) (rbgtk_make_clipboard(val))
extern GType rbgtk_clipboard_get_type();
extern GtkClipboard* rbgtk_get_clipboard(VALUE obj);
extern VALUE rbgtk_make_clipboard(GtkClipboard* gobj);
#else
#define RVAL2CLIPBOARD(obj) (GTK_CLIPBOARD(RVAL2GOBJ(obj)))
#define CLIPBOARD2RVAL(val) (GOBJ2RVAL(val))
#endif

#ifndef GTK_TYPE_TREE_ROW_REFERENCE
#define GTK_TYPE_TREE_ROW_REFERENCE (rbgtk_tree_row_reference_get_type())
#define RVAL2TREEROWREFERENCE(obj) (rbgtk_get_tree_row_reference(obj))
extern GType rbgtk_tree_row_reference_get_type();
extern GtkTreeRowReference* rbgtk_get_tree_row_reference(VALUE obj);
#else
#define RVAL2TREEROWREFERENCE(obj) ((GtkTreeRowReference*)(RVAL2BOXED(obj, GTK_TYPE_TREE_ROW_REFERENCE)))
#endif
  

/*
 * for gtk
 */
extern void rbgtk_initialize_gtkobject(VALUE obj, GtkObject *gtkobj);
extern void exec_callback(GtkWidget *widget, gpointer proc);
extern GType gtk_allocation_get_type();
extern GType gtk_accel_key_get_type();
extern GType gtk_accel_group_entry_get_type();
extern GType gtk_bindingset_get_type();
extern GType gtk_text_appearance_get_type();
extern GType gtk_recent_filter_info_get_type();
extern GType gtk_recent_data_get_type();

/*
 * Gtk::TreeIter callback
 */
typedef void (*rbgtkiter_set_value_func)(void *model, GtkTreeIter *iter,
                                         gint column, GValue *value);
extern void rbgtk_register_treeiter_set_value_func(GType, rbgtkiter_set_value_func);

/*
 * Gtk::SelectionData
 */
extern void rbgtk_atom2selectiondata(VALUE type, VALUE size, VALUE src, GdkAtom* gtype,
                                     void** data, gint* format, gint* length);
extern void rbgtk_atom2selectiondata_free(GdkAtom gtype, void* data); 

/*
 * Gtk::Drag
 */
extern GtkTargetEntry* rbgtk_get_target_entry(VALUE targets);

/*
 * Gtk::Container
 */
extern void rbgtkcontainer_register_child_property_setter(GType gtype, 
                                                          const char* prop_name, 
                                                          RValueToGValueFunc func);
extern void rbgtkcontainer_register_child_property_getter(GType gtype, 
                                                          const char* prop_name, 
                                                          GValueToRValueFunc func);

/*
 * Dialog
 */
extern VALUE rbgtk_dialog_add_buttons_internal(VALUE self, VALUE button_ary);

#endif /* _RBGTK_H */
