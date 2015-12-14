#!/usr/bin/ruby

require 'rbconfig'
require 'fileutils'
require 'etc'
include RbConfig

_admin = Process.euid == 0

if !_admin
    print "Performing local uninstall. For global uninstall, run with admin/superuser privileges.\n"
else
    print "Performing global and local uninstall."
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
    if Dir.exist? _dir_global
        print "Removing global directory...\n"
        FileUtils.rm_rf _dir_global, :secure => true
    end
end

if Dir.exist? _dir_local
    print "Remove local directory...\n"
    FileUtils.rm_rf _dir_local
end

print "Done!\n"
