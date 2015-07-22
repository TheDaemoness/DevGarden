#!/usr/bin/ruby

require 'rbconfig'
require 'fileutils'
require 'etc'
include RbConfig

_runcmd = nil;

case CONFIG['host_os']
when /mswin|windows|cygwin/i

when /darwin/i
    _runcmd = ["osascript", "-e", "tell application \"Terminal\"\n\tactivate\n\tdo script \"" << ARGV[0] << "; exit\"\nend tell"]
when /linux/i
    _runcmd = [`sh -c 'echo $TERM'`, "-e", ARGV[0] << "; exit"]
end

exec *_runcmd;
