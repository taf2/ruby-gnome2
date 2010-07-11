=begin
extconf.rb for Ruby/GTK extention library
=end

PACKAGE_NAME = "gtk2"
PKG_CONFIG_ID = "gtk+-2.0"

begin
  require 'rubygems'
  require 'mkmf-gnome2'
  TOPDIR = File.expand_path(File.dirname(__FILE__))
  SRCDIR = TOPDIR + '/src'
  require 'glib2'
  require 'pango'
  require 'atk'
  require 'gdk_pixbuf2'
  USE_GNOME_GEMS=true
rescue LoadError => e
  puts "Not using gems"
  USE_GNOME_GEMS=false

  TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
  MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
  SRCDIR = TOPDIR + '/gtk/src'

  $LOAD_PATH.unshift MKMF_GNOME2_DIR

  require 'mkmf-gnome2'
end

#
# detect GTK+ configurations
#

PKGConfig.have_package('gthread-2.0')
PKGConfig.have_package(PKG_CONFIG_ID) or exit 1
setup_win32(PACKAGE_NAME)

have_header("st.h")
have_header("ruby/st.h")

STDOUT.print("checking for target... ")
STDOUT.flush
target = PKGConfig.variable(PKG_CONFIG_ID, "target")
$defs << "-DRUBY_GTK2_TARGET=\\\"#{target}\\\""
STDOUT.print(target, "\n")

#
# detect location of GDK include files
#
gdkincl = nil
tmpincl = $CFLAGS.gsub(/-D\w+/, '').split(/-I/) + ['/usr/include']
tmpincl.each do |i|
  i.strip!
  
  if FileTest.exist?(i + "/gdk/gdkkeysyms.h")
    gdkincl = i + "/gdk"
    break
  end
end
raise "can't find gdkkeysyms.h" if gdkincl.nil?

gtk_header = "gtk/gtk.h"
have_func('gtk_plug_get_type', gtk_header)
have_func('gtk_socket_get_type', gtk_header)
have_func('pango_render_part_get_type', gtk_header)
have_header('gtk/gtkfilesystem.h') do |src|
  "#define GTK_FILE_SYSTEM_ENABLE_UNSUPPORTED\n#{src}"
end

xlib_header = "X11/Xlib.h"
if target == "x11" and have_header('X11/Xlib.h') and have_library("X11")
  have_func("XReadBitmapFileData", xlib_header)
  have_func("XGetErrorText", xlib_header)
end

if PKGConfig.have_package('gtk+-unix-print-2.0')
  $defs.push("-DHAVE_GTK_UNIX_PRINT")
end

have_func("rb_errinfo")

check_cairo
if USE_GNOME_GEMS
  # locate the latest glib2 install and add the package dependency
  add_depend_package("glib2", File.dirname(Gem.find_files('rbglib.h').first), '/')
  add_depend_package("pango", File.dirname(Gem.find_files('rbpango.h').first), '/')
  add_depend_package("atk", File.dirname(Gem.find_files('rbatk.h').first), '/')
  add_depend_package("gdkpixbuf2", File.dirname(Gem.find_files('rbgdk-pixbuf.h').first), '/')
else
  add_depend_package("glib2", "glib/src", TOPDIR)
  add_depend_package("pango", "pango/src", TOPDIR)
  add_depend_package("atk", "atk/src", TOPDIR)
end

#
# create Makefiles
#

add_distcleanfile("rbgdkkeysyms.h")
add_distcleanfile("rbgtkinits.c")

$defs.delete("-DRUBY_GTK2_COMPILATION")

create_pkg_config_file("Ruby/GTK2", PKG_CONFIG_ID, ruby_gnome2_version)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GTK2_COMPILATION") {
  File.delete("rbgtkinits.c") if FileTest.exist?("rbgtkinits.c")
  SRCDIR_QUOTED = SRCDIR.gsub(' ', '\ ')
  system("#{$ruby} #{SRCDIR_QUOTED}/makeinits.rb #{SRCDIR_QUOTED}/*.c > rbgtkinits.c") or raise "failed to make GTK inits"
  system("#{$ruby} #{SRCDIR_QUOTED}/makekeysyms.rb #{gdkincl}/gdkkeysyms.h > rbgdkkeysyms.h") or raise "failed to make GDK Keysyms"
}

create_top_makefile(["src"])
