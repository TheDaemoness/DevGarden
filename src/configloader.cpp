#include "configloader.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QFileInfo>

QFile* getConfigFileRead(const char* name) {
	QFile* retval = new QFile;
	retval->setFileName(QDir::home().path()+'/'+DG_CONFIG_PREFIX_LOCAL+DG_NAME+'/'+name);
	retval->open(QFile::ReadOnly);
	if(!retval->isOpen()) {
		retval->setFileName(QString(DG_CONFIG_PREFIX_GLOBAL)+DG_NAME+"/"+name);
		retval->open(QFile::ReadOnly);
	}
	if(retval->isOpen())
		return retval;
	delete retval;
	return nullptr;
}

QFile* getConfigFileWrite(const char* name) {
	QFile* retval = new QFile;
	retval->setFileName(QDir::home().path()+'/'+DG_CONFIG_PREFIX_LOCAL+DG_NAME+"/"+name);
	retval->open(QFile::WriteOnly);
	if(!retval->isOpen()) {
		delete retval;
		return nullptr;
	}
	return retval;
}

void makeConfigDirs() {
	QDir d = QDir::home();
#ifndef DG_CONFIG_PREFIX_LOCAL_NOCD
	d.cd(DG_CONFIG_PREFIX_LOCAL);
#endif
	if(!d.exists())
		d.mkpath(DG_CONFIG_PREFIX_LOCAL);
#ifndef DG_CONFIG_PREFIX_LOCAL_NOCD
	if(!d.exists(DG_NAME)) {
		d.mkdir(DG_NAME);
		d.cd(DG_NAME);
	}
#else
	if(!d.exists(QString(DG_CONFIG_PREFIX_LOCAL)+DG_NAME)) {
		d.mkdir(QString(DG_CONFIG_PREFIX_LOCAL)+DG_NAME);
		d.cd(QString(DG_CONFIG_PREFIX_LOCAL)+DG_NAME);
	}
#endif
}

static ConfigEntry* recurseGetConfigEntry(QFile* ptr, ConfigEntry* parent = new ConfigEntry, size_t depth = 0) {
	return parent;
}

ConfigEntry* getConfigEntry(QFile* ptr) {
	return recurseGetConfigEntry(ptr);
}

bool runScript(const char* name) {
	return false;
}
