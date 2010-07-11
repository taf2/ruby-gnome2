=begin
extconf.rb for Ruby/GdkPixbuf2 extention library
=end

PACKAGE_NAME = "gdk_pixbuf2"
PACKAGE_ID   = "gdk-pixbuf-2.0"

begin
  require 'rubygems'
  require 'mkmf-gnome2'
  USE_GNOME_GEMS=true
  TOPDIR = File.expand_path(File.dirname(__FILE__))
  SRCDIR = TOPDIR
  require 'glib2'
rescue LoadError => e
  TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
  MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
  SRCDIR = TOPDIR + '/gdkpixbuf'

  $LOAD_PATH.unshift MKMF_GNOME2_DIR

  require 'mkmf-gnome2'
end

PKGConfig.have_package(PACKAGE_ID) or exit 1

setup_win32(PACKAGE_NAME)

have_func("gdk_pixbuf_set_option", "gdk-pixbuf/gdk-pixbuf.h") do |src|
  "#define GDK_PIXBUF_ENABLE_BACKEND\n#{src}"
end
have_header("gdk-pixbuf/gdk-pixbuf-io.h")

if PKGConfig.have_package('gdk-2.0')
  check_cairo
end

if USE_GNOME_GEMS
  # locate the latest glib2 install and add the package dependency
  path = File.dirname(Gem.find_files('rbglib.h').first)
  add_depend_package("glib2", path, '/')
else
  add_depend_package("glib2", "glib/src", TOPDIR)
end
create_pkg_config_file("Ruby/GdkPixbuf2", PACKAGE_ID)
create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR)
