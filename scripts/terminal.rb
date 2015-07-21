#!/usr/bin/ruby

require 'rbconfig'
require 'FileUtils'
require 'etc'
include RbConfig

_runcmd = nil;

case CONFIG['host_os']
when /mswin|windows|cygwin/i

when /darwin/i
    _runcmd = ["osascript", "-e", "tell application \"Terminal\" to do script \"" << ARGV[0] << "; exit\""]
when /linux/i
end

exec *_runcmd;
