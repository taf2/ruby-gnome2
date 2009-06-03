=begin
extconf.rb for Ruby/Libart extention library
=end

PACKAGE_NAME = "libart2"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/libart/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package('libart-2.0') or exit 1

$libs += " -lz "
PKGConfig.have_package('libpng12') or have_header("png.h") or exit 1
have_header("jpeglib.h") or exit 1

have_library("png", "png_create_write_struct")
have_library("jpeg", "jpeg_set_defaults")
have_library("art_lgpl_2", "art_affine_identity") or exit 1

setup_win32(PACKAGE_NAME)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_LIBART2_COMPILATION")
create_pkg_config_file(nil, nil, 'libart-2.0-ruby.pc')
create_top_makefile

