#!/usr/bin/ruby

require 'rbconfig'
require 'pathname'
include RbConfig

_path = Pathname.new(ARGV[0])
_buildpath = _path.dirname.to_s+"/../build-"+(_path.dirname.basename.to_s);

if !Dir.exist? _buildpath
	Dir.mkdir _buildpath
end
Dir.chdir _buildpath

system "qmake", _path.dirname.to_s
system "make"
