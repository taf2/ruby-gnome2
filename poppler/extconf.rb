=begin
extconf.rb for Ruby/Poppler extention library
=end

PACKAGE_NAME = "poppler"
PACKAGE_ID = "poppler-glib"

begin
  require 'rubygems'
  require 'mkmf-gnome2'
  TOPDIR = File.expand_path(File.dirname(__FILE__) )
  SRCDIR = TOPDIR + '/src'
  require 'glib2'
  require 'gtk2/base'
  USE_GNOME_GEMS = true
rescue LoadError => e
  TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
  MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
  SRCDIR = TOPDIR + '/poppler/src'

  $LOAD_PATH.unshift MKMF_GNOME2_DIR

  require 'mkmf-gnome2'
  USE_GNOME_GEMS = false
end

PKGConfig.have_package(PACKAGE_ID, 0, 8, 0) or exit 1
setup_win32(PACKAGE_NAME)

check_cairo

if USE_GNOME_GEMS
  add_depend_package("glib2", File.dirname(Gem.find_files('rbglib.h').first), '/')
  add_depend_package("pango", File.dirname(Gem.find_files('rbpango.h').first), '/')
  add_depend_package("atk", File.dirname(Gem.find_files('rbatk.h').first), '/')
  add_depend_package("gdkpixbuf2", File.dirname(Gem.find_files('rbgdk-pixbuf.h').first), '/')
  add_depend_package("gtk2", File.dirname(Gem.find_files('rbgtk.h').first), '/')
else
  add_depend_package("glib2", "glib/src", TOPDIR)
  add_depend_package("gtk2", "gtk/src", TOPDIR)
  add_depend_package("gdk_pixbuf2", "gdkpixbuf", TOPDIR)
end

make_version_header("POPPLER", PACKAGE_ID)

create_pkg_config_file("Ruby/Poppler", PACKAGE_ID)
create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_POPPLER_COMPILATION")
create_top_makefile
