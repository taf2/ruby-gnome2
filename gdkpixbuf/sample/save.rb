#!/usr/local/bin/ruby
=begin
  scale.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: save.rb,v 1.1 2002/11/19 13:07:24 mutoh Exp $
=end

require 'gdk_pixbuf2'

from = ARGV[0]
to = ARGV[1]
if ! from or ! to
  puts "ruby #{$0} from_file to_file"
  exit(1)
end

src =  Gdk::Pixbuf.new(from)

dst = src.scale(300, 300, Gdk::Pixbuf::INTERP_HYPER)
# This doesn't work ....
#dst.save(to, "jpeg", {:quality => 100})
dst.save(to, "png")
