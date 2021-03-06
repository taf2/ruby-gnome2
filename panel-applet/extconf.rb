=begin
extconf.rb for Ruby/PanelApplet extention library
=end

PACKAGE_NAME = "panelapplet2"
PACKAGE_ID   = "libpanelapplet-2.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
BASE_DIR = TOPDIR + '/panel-applet'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package(PACKAGE_ID, 2, 6) or exit 1
setup_win32(PACKAGE_NAME)

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

make_version_header("PANELAPPLET", PACKAGE_ID)

create_pkg_config_file("Ruby/PanelApplet", PACKAGE_ID,
                       nil, "ruby-panelapplet2.pc")

create_makefile_at_srcdir(PACKAGE_NAME, BASE_DIR + "/src",
                          "-DRUBY_PANELAPPLET_COMPILATION")
$INSTALLFILES ||= []
$INSTALLFILES << ["../lib/**/*.rb", "$(RUBYLIBDIR)", "../lib"]
create_makefile_at_srcdir(PACKAGE_NAME + "_main", BASE_DIR + "/main",
                          "-DRUBY_PANELAPPLET_COMPILATION")
create_top_makefile(["src", "main"])
