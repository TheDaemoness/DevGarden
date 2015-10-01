#!/usr/bin/ruby

require 'rbconfig'
require 'pathname'
include RbConfig

_path = Pathname.new(ARGV[0])
_buildpath = Pathname.new(ARGV[1])

Dir.chdir _buildpath.to_s
system "qmake", "-r", _path.to_s
system "make"
