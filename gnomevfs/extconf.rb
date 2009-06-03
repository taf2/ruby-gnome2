=begin
extconf.rb for Ruby/GnomeVFS extention library
=end

PACKAGE_NAME = "gnomevfs"
PACKAGE_ID   = "gnome-vfs-2.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gnomevfs/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package(PACKAGE_ID) or exit 1
PKGConfig.have_package('gnome-vfs-module-2.0') or exit 1
PKGConfig.have_package("gobject-2.0") or exit 1

have_func("rb_exec_recursive")

setup_win32(PACKAGE_NAME)

add_depend_package("glib2", "glib/src", TOPDIR)

make_version_header("GNOMEVFS", PACKAGE_ID)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GNOMEVFS_COMPILATION")
create_pkg_config_file('GNOMEVFS', 'src/rbgnomevfsversion.h', 'gnome-vfs-2.0-ruby.pc')
create_top_makefile
