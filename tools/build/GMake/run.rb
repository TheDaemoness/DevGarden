#!/usr/bin/ruby

require 'rbconfig'
require 'pathname'
include RbConfig

Dir.chdir Pathname.new(ARGV[0]).dirname

case CONFIG['host_os']
when /mswin|windows|cygwin/i
	exec "gmake"
else
	exec "make"
end
