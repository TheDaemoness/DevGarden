#include "configloader.h"

std::ifstream* getConfigFile(const std::string& name) {
	std::ifstream* retval = new std::ifstream;
	retval->open(std::string(DG_CONFIG_PREFIX_LOCAL)+DG_NAME+"/"+name);
	if(!retval->is_open())
		retval->open(std::string(DG_CONFIG_PREFIX_GLOBAL)+DG_NAME+"/"+name);
	if(retval->is_open())
		return retval;
	delete retval;
	return nullptr;
}
