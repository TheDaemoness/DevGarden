#!/usr/bin/ruby

require 'rbconfig'
require 'fileutils'
include RbConfig

_runcmd = nil
_torun = ARGV[0].gsub(/(\s)/, "\\ ")

if ARGV.length >= 2
    _torun += ' ' + ARGV[1]
end

case CONFIG['host_os']
when /mswin|windows|cygwin/i

when /darwin/i
    _runcmd = ["osascript", "-e", "tell application \"Terminal\"\n\tactivate\n\tdo script \"" <<
        _torun.sub!('\\','\\\\\\\\') << "; exit\"\nend tell"]
when /linux/i
    _runcmd = [`sh -c 'echo $TERM'`, "-e", _torun.sub!('\\','\\\\') << "; exit"]
end

exec *_runcmd;
