=begin
  makemo.rb

  Copyright (c) 2004-2006 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  Original file is post-setup.rb from ruby-gettext-package-0.6.0.

  post-setup.rb:
  Copyright (C) 2001-2004 MUTOH Masao <mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby.

  $Id: makemo.rb,v 1.4 2006/06/17 14:14:55 mutoh Exp $
=end
require 'gettext/utils'

GetText.create_mofiles(true, "po", "locale")
