=begin
  top-level extconf.rb for Ruby-GNOME2

  $Id: extconf.rb,v 1.17 2007/10/22 12:19:17 ktou Exp $

  Copyright (C) 2003-2005 Ruby-GNOME2 Project Team
=end

require 'English'
require 'mkmf'
require 'fileutils'
require 'pathname'

priorlibs = ["glib", "gdkpixbuf", "pango", "atk", "gtk"]

#
# detect sub-directories
#
$ruby = File.join(Config::CONFIG['bindir'], Config::CONFIG['RUBY_INSTALL_NAME'] + Config::CONFIG['EXEEXT'])
$ruby = arg_config("--ruby", $ruby)

rm = "rm -f "
if /mswin32/ =~ RUBY_PLATFORM
  rm = "del " 
end


$srcdir = File.dirname(__FILE__)
$topsrcdir = $configure_args["--topsrcdir"] ||= $srcdir
$topdir = $configure_args["--topdir"] ||= Dir.pwd
$strict = $configure_args["--strict"] ? "--strict" : ""

$srcdir = File.expand_path($srcdir)
$topsrcdir = File.expand_path($topsrcdir)
$topdir = File.expand_path($topdir)

subdirs = ARGV.select{|v|  /^--/ !~ v}

if subdirs.size == 0
  subdirs = Dir.glob($topsrcdir+"/*/**/extconf.rb")
  subdirs.collect! do |subdir|
    subdir[0..$topsrcdir.size] = ""
    File.dirname(subdir)
  end
  priorlibs &= subdirs
  subdirs -= priorlibs
  subdirs = priorlibs + subdirs #Change the order
end

#
# generate sub-directory Makefiles
#
targets = []
ignore = []

ruby, *ruby_args = Shellwords.shellwords($ruby)
if ARGV.grep(/\A--ruby=/)
  extra_args = ["--ruby=#{$ruby}"] + ARGV.reject {|arg| /\A--ruby=/ =~ arg}
else
  extra_args = ARGV.dup
end

subdirs.each do |subdir|
  STDERR.puts("#{$0}: Entering directory `#{subdir}'")
  FileUtils.mkdir_p(subdir)
  topdir = File.join(*([".."] * subdir.split(/\/+/).size))
  dir = $topsrcdir
  dir = File.join(topdir, dir) unless Pathname.new(dir).absolute?
  srcdir = File.join(dir, subdir)
  args = ruby_args + ["-C", subdir, File.join(srcdir, "extconf.rb"),
                      "--topsrcdir=#{dir}", "--topdir=#{topdir}",
                      "--srcdir=#{srcdir}", *extra_args]
  ret = system(ruby, *args)
  STDERR.puts("#{$0}: Leaving directory '#{subdir}'")
  if ret
    targets << subdir
  else
    ignore << subdir
  end
end
puts "\n-----"
puts "Target libraries: #{targets.join(', ')}" if targets.size > 0
puts "Ignored libraries: #{ignore.join(', ')}" if ignore.size > 0

#
# generate top-level Makefile
#

File.open("Makefile", "w") do |makefile|
  makefile.print("\
TOPSRCDIR = #{$topsrcdir}
SUBDIRS = #{targets.join(' ')}
COMMAND = #{$ruby} #{$topsrcdir}/exec_make.rb #{$strict}
RM = #{rm}

all:
	$(COMMAND) '$(SUBDIRS)' $(MAKE) all

install:
	$(COMMAND) '$(SUBDIRS)' $(MAKE) install

site-install:
	$(COMMAND) '$(SUBDIRS)' $(MAKE) site-install

clean:
	$(COMMAND) '$(SUBDIRS)' $(MAKE) clean

distclean:
	$(COMMAND) '$(SUBDIRS)' $(MAKE) distclean
	$(RM) Makefile mkmf.log
")
end

puts "-----"
puts "Done."

$makefile_created = true
