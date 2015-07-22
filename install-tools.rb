#!/usr/bin/ruby

require 'rbconfig'
require 'fileutils'
require 'etc'
include RbConfig

if Process.euid != 0
    print "This script requires elevated privileges to run.\n"
    exit;
end

_dir_global = "/usr/share/DevGarden/"
_dir_local =  ".DevGarden/"

_dir_curr = Dir.pwd;
Dir.chdir

case CONFIG['host_os']
when /mswin|windows|cygwin/i
    _dir_global = "/ProgramData/DevGarden/"
    _dir_local =  "AppData/Roaming/DevGarden/"
when /darwin/i
    _dir_global = "/Library/Application Support/DevGarden/"
    _dir_local =  "Library/Application Support/DevGarden/"
end

if !Dir.exist? _dir_global
    print "Making global directory...\n"
    Dir.mkdir _dir_global
end

if !Dir.exist? _dir_local
    println "Making local directory...\n"
    Dir.mkdir _dir_local
end

Dir.chdir _dir_curr

print "Copying configuration files...\n"
FileUtils.cp_r "config", _dir_global

print "Copying scripts...\n"
FileUtils.cp_r "scripts", _dir_global

print "Done!\n"
