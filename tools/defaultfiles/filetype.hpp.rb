#!/usr/bin/ruby

require 'digest'

print "header\n";
_filename = ARGV[0].gsub(/\..+?$/,'');
_filename = _filename.split('/')[-1].gsub(/[^A-Za-z0-9]+/,'_').upcase;
_macroname = 'INCLUDEGUARD_CPP_'<<_filename<<'_'<<(Digest::MD5.hexdigest(ARGV[0]).upcase)
print "#ifndef "<< _macroname <<"\n"
print "#define "<< _macroname <<"\n\n"
print "//TODO: Insert declarations/templates here.\n\n"
print "#endif //"<< _macroname <<"\n"
