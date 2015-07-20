#!/usr/bin/ruby
print "build-sys\n"

_ver = 0;
begin
	_ver = `cmake --version`.split(' ')[2]
rescue
end

if(_ver != 0)
	print "cmake_minimum_required("<<_ver<<")\n"
end
print "project("+(ARGV[0].split('/')[-2])+")\n"
