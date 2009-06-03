=begin
extconf.rb for Ruby/GtkGLExt extention library
=end

PACKAGE_NAME = "gtkglext"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gtkglext/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package('gtkglext-1.0') or exit 1
setup_win32(PACKAGE_NAME)

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GTKGLEXT_COMPILATION")
create_pkg_config_file(nil, nil, 'gtkglext-1.0-ruby.pc')
create_top_makefile
