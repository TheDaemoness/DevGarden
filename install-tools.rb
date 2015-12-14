#!/usr/bin/ruby

require 'rbconfig'
require 'fileutils'
require 'etc'
include RbConfig

_admin = Process.euid == 0

if !_admin
    print "Performing local install. For global install, run with admin/superuser privileges.\n"
else
    print "Performing global install."
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

if _admin
    if !Dir.exist? _dir_global
        print "Making global directory...\n"
        Dir.mkdir _dir_global
    end
end

if !Dir.exist? _dir_local
    print "Making local directory...\n"
    Dir.mkdir _dir_local
end

Dir.chdir _dir_curr

_dir_install = (if _admin then _dir_global else Dir.home+'/'+_dir_local end)

print "Copying configuration files...\n"
FileUtils.cp_r "config", _dir_install

print "Copying scripts...\n"
FileUtils.cp_r "tools", _dir_install

print "Done!\n"
