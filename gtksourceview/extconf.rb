=begin
extconf.rb for Ruby/GtkSourceView extention library
=end

PACKAGE_NAME = "gtksourceview"
PACKAGE_ID   = "gtksourceview-1.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gtksourceview/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR
require 'mkmf-gnome2'

PKGConfig.have_package(PACKAGE_ID) or exit 1
setup_win32(PACKAGE_NAME)
have_func("gtk_source_print_job_get_type")

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

make_version_header("GTKSOURCEVIEW", PACKAGE_ID)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GTKSOURCEVIEW_COMPILATION")
create_pkg_config_file(PACKAGE_NAME, PACKAGE_ID, ruby_gnome2_version)
create_top_makefile

