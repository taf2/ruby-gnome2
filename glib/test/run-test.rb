#!/usr/bin/env ruby

base = File.expand_path(File.join(File.dirname(__FILE__)))
top = File.expand_path(File.join(base, ".."))

$LOAD_PATH.unshift(top)
require 'test/glib-test-init'

if system("which make > /dev/null")
  system("cd #{top.dump} && make > /dev/null") or exit(1)
end

$LOAD_PATH.unshift(File.join(top, "src"))
$LOAD_PATH.unshift(File.join(top, "src", "lib"))

$LOAD_PATH.unshift(base)
require 'glib-test-utils'

require 'glib2'

if !defined?(Test::Unit::AutoRunner)
  require 'test-unit'
end

exit Test::Unit::AutoRunner.run(true, base)
