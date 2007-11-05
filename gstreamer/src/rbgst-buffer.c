/*
 * Copyright (C) 2007 Ruby-GNOME2 Project Team
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

#define SELF(obj) (RVAL2GST_BUF(obj))

static VALUE
initialize(VALUE self)
{
    G_INITIALIZE(self, gst_buffer_new());
    return Qnil;
}

void
Init_gst_buffer(void)
{
    VALUE rb_cGstBuffer;

    rb_cGstBuffer = G_DEF_CLASS(GST_TYPE_BUFFER, "Buffer", mGst);

    rb_define_method(rb_cGstBuffer, "initialize", initialize, 0);
}