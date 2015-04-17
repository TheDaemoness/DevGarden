//#include "configloader.h"
//#include <QDir>
//#include <iostream>

//std::ifstream* getConfigFile(const std::string& name) {
//	std::ifstream* retval = new std::ifstream;
//	retval->open(std::string(DG_CONFIG_PREFIX_LOCAL)+DG_NAME+"/"+name);
//	if(!retval->is_open())
//		retval->open(std::string(DG_CONFIG_PREFIX_GLOBAL)+DG_NAME+"/"+name);
//	if(retval->is_open())
//		return retval;
//	delete retval;
//	return nullptr;
//}

//void makeConfigDirs() {
//	QDir d = QDir::home();
//#ifndef DG_CONFIG_PREFIX_LOCAL_NOCD
//	d.cd(DG_CONFIG_PREFIX_LOCAL);
//#endif
//	if(!d.exists())
//		d.mkpath(DG_CONFIG_PREFIX_LOCAL);
//#ifndef DG_CONFIG_PREFIX_LOCAL_NOCD
//	if(!d.exists(DG_NAME)) {
//		d.mkdir(DG_NAME);
//		d.cd(DG_NAME);
//	}
//#else
//	if(!d.exists(QString(DG_CONFIG_PREFIX_LOCAL)+DG_NAME)) {
//		d.mkdir(QString(DG_CONFIG_PREFIX_LOCAL)+DG_NAME);
//		d.cd(QString(DG_CONFIG_PREFIX_LOCAL)+DG_NAME);
//	}
//#endif
//}
