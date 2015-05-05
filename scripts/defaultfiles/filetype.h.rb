#!/usr/bin/ruby

require 'Digest'

print "header\n";
_filename = ARGV[0].gsub(/\..+?$/,'');
_filename = _filename.split('/')[-1].gsub(/[^A-Za-z0-9]+/,'_').upcase;
_macroname = 'INCLUDEGUARD_C_'<<_filename<<'_'<<(Digest::MD5.hexdigest(ARGV[0]).upcase)
print "#ifndef "<< _macroname <<"\n"
print "#define "<< _macroname <<"\n"
print "#ifdef __cplusplus\nextern \"C\" \{\n#endif\n\n"
print "//TODO: Insert declarations here.\n\n"
print "#ifdef __cplusplus\n\} //extern C\n#endif\n"
print "#endif //"<< _macroname <<"\n"
