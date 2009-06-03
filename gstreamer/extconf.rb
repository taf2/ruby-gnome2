=begin
extconf.rb for Ruby/GStreamer extention library
=end

PACKAGE_NAME = "gst"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gstreamer/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package('gstreamer-0.10') or exit 1
PKGConfig.have_package('gstreamer-plugins-base-0.10') or exit 1

setup_win32(PACKAGE_NAME)

have_header("unistd.h")
have_header("io.h")

if have_library("gstinterfaces-0.10") and
    have_header("gst/interfaces/xoverlay.h")
  $CFLAGS += " -DHAVE_GST_OVERLAY"
end

add_depend_package("glib2", "glib/src", TOPDIR)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GST_COMPILATION")
create_pkg_config_file(nil, nil, 'gstreamer-0.10-ruby.pc')
create_top_makefile
