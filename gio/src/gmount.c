/* -*- c-file-style: "ruby" -*- */
/*
 * Ruby/GIO: a Ruby binding of gio-2.0.x.
 * Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
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

#include "gio2.h"

#define _SELF(value) RVAL2GMOUNT(value)

static VALUE
mount_name(VALUE self)
{
	return CSTR2RVAL2(g_mount_get_name(_SELF(self)));
}

static VALUE
mount_uuid(VALUE self)
{
	return CSTR2RVAL2(g_mount_get_uuid(_SELF(self)));
}

static VALUE
mount_icon(VALUE self)
{
	return GOBJ2RVAL(g_mount_get_icon(_SELF(self)));
}

static VALUE
mount_drive(VALUE self)
{
	return GOBJ2RVAL(g_mount_get_drive(_SELF(self)));
}

static VALUE
mount_root(VALUE self)
{
	return GOBJ2RVAL(g_mount_get_root(_SELF(self)));
}

static VALUE
mount_volume(VALUE self)
{
	return GOBJ2RVAL(g_mount_get_volume(_SELF(self)));
}

static VALUE
mount_can_unmount(VALUE self)
{
	return CBOOL2RVAL(g_mount_can_unmount(_SELF(self)));
}


static VALUE
mount_unmount(int argc, VALUE *argv, VALUE self)
{
        VALUE rbflags, rbcancellable, block;
        GMountUnmountFlags flags;
        GCancellable *cancellable;

	rb_scan_args(argc, argv, "02&", &rbflags, &rbcancellable, &block);
	flags = RVAL2GMOUNTUNMOUNTFLAGSDEFAULT(rbflags);
	cancellable = RVAL2GCANCELLABLE(rbcancellable);
	SAVE_BLOCK(block);

        g_mount_unmount(_SELF(self),
                        flags,
                        cancellable,
                        rbgio_async_ready_callback,
                        (gpointer)block);

        return self;
}

static VALUE
mount_unmount_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_mount_unmount_finish(_SELF(self),
                                    RVAL2GASYNCRESULT(result),
                                    &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
mount_remount(int argc, VALUE *argv, VALUE self)
{
        VALUE rbflags, rbmount_operation, rbcancellable, block;
        GMountMountFlags flags;
        GMountOperation *mount_operation;
        GCancellable *cancellable;

	rb_scan_args(argc, argv, "03&", &rbflags, &rbmount_operation, &rbcancellable, &block);
	flags = RVAL2GMOUNTMOUNTFLAGSDEFAULT(rbflags);
	mount_operation = RVAL2GMOUNTOPERATION(rbmount_operation);
	cancellable = RVAL2GCANCELLABLE(rbcancellable);
	SAVE_BLOCK(block);

        g_mount_remount(_SELF(self),
                        flags,
                        mount_operation,
                        cancellable,
                        rbgio_async_ready_callback,
                        (gpointer)block);

        return self;
}

static VALUE
mount_remount_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_mount_remount_finish(_SELF(self),
                                    RVAL2GASYNCRESULT(result),
                                    &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
mount_can_eject(VALUE self)
{
	return CBOOL2RVAL(g_mount_can_eject(_SELF(self)));
}


static VALUE
mount_eject(int argc, VALUE *argv, VALUE self)
{
        VALUE rbflags, rbcancellable, block;
        GMountUnmountFlags flags;
        GCancellable *cancellable;

	rb_scan_args(argc, argv, "02&", &rbflags, &rbcancellable, &block);
	flags = RVAL2GMOUNTUNMOUNTFLAGSDEFAULT(rbflags);
	cancellable = RVAL2GCANCELLABLE(rbcancellable);
	SAVE_BLOCK(block);

        g_mount_eject(_SELF(self),
                      flags,
                      cancellable,
                      rbgio_async_ready_callback,
                      (gpointer)block);

        return self;
}

static VALUE
mount_eject_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_mount_eject_finish(_SELF(self),
                                  RVAL2GASYNCRESULT(result),
                                  &error))
                rbgio_raise_io_error(error);

        return self;
}

static VALUE
mount_guess_content_type(int argc, VALUE *argv, VALUE self)
{
        VALUE rbforce_rescan, rbcancellable, block;
        gboolean force_rescan;
        GCancellable *cancellable;

	rb_scan_args(argc, argv, "02&", &rbforce_rescan, &rbcancellable, &block);
	force_rescan = RVAL2CBOOL(rbforce_rescan);
	cancellable = RVAL2GCANCELLABLE(rbcancellable);
	SAVE_BLOCK(block);

        g_mount_guess_content_type(_SELF(self),
                                   force_rescan,
                                   cancellable,
                                   rbgio_async_ready_callback,
                                   (gpointer)block);

        return self;
}

static VALUE
mount_guess_content_type_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        gchar **content_types;

        content_types = g_mount_guess_content_type_finish(_SELF(self),
                                                          RVAL2GASYNCRESULT(result),
                                                          &error);
        if (content_types == NULL)
                rbgio_raise_io_error(error);

        return str_vector_to_ary(content_types);
}

static VALUE
mount_guess_content_type_sync(int argc, VALUE *argv, VALUE self)
{
        VALUE force_rescan, cancellable;
        GError *error = NULL;
        gchar **content_types;

        rb_scan_args(argc, argv, "02", &force_rescan, &cancellable);

        content_types = g_mount_guess_content_type_sync(_SELF(self),
                                                        RVAL2CBOOL(force_rescan),
                                                        RVAL2GCANCELLABLE(cancellable),
                                                        &error);
        if (content_types == NULL)
                rbgio_raise_io_error(error);

        return str_vector_to_ary(content_types);
}

static VALUE
mount_is_shadowed(VALUE self)
{
	return CBOOL2RVAL(g_mount_is_shadowed(_SELF(self)));
}

static VALUE
mount_shadow(VALUE self)
{
        g_mount_shadow(_SELF(self));

        return self;
}

static VALUE
mount_unshadow(VALUE self)
{
        g_mount_unshadow(_SELF(self));

        return self;
}

void
Init_gmount(VALUE glib)
{
        VALUE mount = G_DEF_INTERFACE(G_TYPE_MOUNT, "Mount", glib);

        G_DEF_CLASS(G_TYPE_MOUNT_MOUNT_FLAGS, "MountFlags", mount);
	G_DEF_CONSTANTS(mount, G_TYPE_MOUNT_MOUNT_FLAGS, "G_MOUNT_");
        G_DEF_CLASS(G_TYPE_MOUNT_UNMOUNT_FLAGS, "UnmountFlags", mount);
	G_DEF_CONSTANTS(mount, G_TYPE_MOUNT_UNMOUNT_FLAGS, "G_MOUNT_");

        rb_define_method(mount, "name", mount_name, 0);
        rb_define_method(mount, "uuid", mount_uuid, 0);
        rb_define_method(mount, "icon", mount_icon, 0);
        rb_define_method(mount, "drive", mount_drive, 0);
        rb_define_method(mount, "root", mount_root, 0);
        rb_define_method(mount, "volume", mount_volume, 0);
        rb_define_method(mount, "can_unmount?", mount_can_unmount, 0);
        rb_define_method(mount, "unmount", mount_unmount, -1);
        rb_define_method(mount, "unmount_finish", mount_unmount_finish, 1);
        rb_define_method(mount, "remount", mount_remount, -1);
        rb_define_method(mount, "remount_finish", mount_remount_finish, 1);
        rb_define_method(mount, "can_eject?", mount_can_eject, 0);
        rb_define_method(mount, "eject", mount_eject, -1);
        rb_define_method(mount, "eject_finish", mount_eject_finish, 1);
        rb_define_method(mount, "guess_content_type", mount_guess_content_type, -1);
        rb_define_method(mount, "guess_content_type_finish", mount_guess_content_type_finish, 1);
        rb_define_method(mount, "guess_content_type_sync", mount_guess_content_type_sync, -1);
        rb_define_method(mount, "shadowed?", mount_is_shadowed, 0);
        rb_define_method(mount, "shadow", mount_shadow, 0);
        rb_define_method(mount, "unshadow", mount_unshadow, 0);
}
