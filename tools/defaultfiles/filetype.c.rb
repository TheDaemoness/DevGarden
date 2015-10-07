#!/usr/bin/ruby

print "compiled\n";
_filename = ARGV[0].gsub(/\..+?$/,'');
if(File::exist? _filename+'.h')
	print '#include "'<<File::basename(_filename+'.h')<<"\"\n";
end
