#include "configloader.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QFileInfo>
#include <QProcess>
#include <QStringList>
#include <algorithm>

ConfigFile::ConfigFile(QFile* f) {
	for(ConfigEntry* e = getConfigEntry(f); e != nullptr; e = getConfigEntry(f))
		insert(e);
}

ConfigFile::ConfigFile(const QString& name) {
	QFile* ptr = getUtilityFileRead(name);
	if(!ptr)
		return;
	this->name = name;
	for(ConfigEntry* e = getConfigEntry(ptr); e != nullptr; e = getConfigEntry(ptr))
		insert(e);
	ptr->close();
	delete ptr;
}

ConfigEntry* ConfigFile::at(const QString& name, size_t index) {
	auto it = entries.find(name);
	if(it != entries.end())
		return it->second[index>=it->second.size()?it->second.size()-1:index];
	return nullptr;
}

ConfigFile::Values* ConfigFile::get(const QString& name) {
	auto it = entries.find(name);
	if(it != entries.end())
		return &(it->second);
	return nullptr;
}

size_t ConfigFile::count(const QString& name) const {
	auto it = entries.find(name);
	return (it != entries.end())?it->second.size():0;
}

bool ConfigFile::insert(ConfigEntry* ce) {
	auto it = entries.find(ce->firstWord());
	if(it != entries.end()) {
		it->second.push_back(ce);
		return true;
	}
	else {
		entries.insert(std::make_pair(ce->firstWord(),Values(1,ce)));
		return false;
	}
}

void ConfigFile::erase(const QString& name) {
	for(ConfigEntry* e : entries.at(name))
		delete e;
	entries.erase(name);
}

ConfigFile::Values ConfigFile::remove(const QString& name) {
	Values temp = entries.at(name);
	entries.erase(name);
	return temp;
}

QFileInfo* getUtilityFile(const QString& name) {
	QFileInfo* f = new QFileInfo();
	f->setFile(QDir::home().path()+'/'+DG_CONFIG_PREFIX_LOCAL+DG_NAME+'/'+name);
	if(!f->exists()) {
		f->setFile(QString(DG_CONFIG_PREFIX_GLOBAL)+DG_NAME+"/"+name);
		if(!f->exists()) {
			delete f;
			return nullptr;
		}
	}
	return f;
}

QFile* getUtilityFileRead(const QString& name) {
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

QFile* getUtilityFileWrite(const QString& name) {
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

static ConfigEntry* recurseGetConfigEntry(QFile* ptr, ConfigEntry* parent, size_t depth = 0) {
	size_t ws_count = depth;
	do {
		if(!ptr->atEnd()) {
			ConfigEntry* child = nullptr;
			if(ws_count == depth)
				child = new ConfigEntry(ptr->readLine());
			else if(ws_count > depth)
				child = recurseGetConfigEntry(ptr, parent->back(), ws_count);
			if(child)
				parent->push_back(child);
		}
		ws_count = 0;
		char val = ptr->peek(1)[0];
		while(val == '\t' || val == ' ' || val == '\n' || val == '\r') {
			ptr->read(1);
			if(val == '\n' || val == '\r')
				ws_count = 0;
			else
				++ws_count;
			val = ptr->peek(1)[0];
		}
	} while(ws_count >= depth && !ptr->atEnd());
	return parent;
}

ConfigEntry* getConfigEntry(QFile* ptr) {
	if(!ptr->isOpen() || ptr->atEnd())
		return nullptr;
	ConfigEntry* retval = new ConfigEntry;
	size_t ws_count;
	ws_count = 0;
	retval->setData(ptr->readLine());
	char val = ptr->peek(1)[0];
	while(val == '\t' || val == ' ') {
		ptr->read(1);
		++ws_count;
		val = ptr->peek(1)[0];
	}
	if(ws_count)
		return recurseGetConfigEntry(ptr, retval, ws_count);
	return retval;
}

bool runTool(const QString& name, QStringList* args, QByteArray* out, QByteArray* in) {
	QFileInfo* retval = new QFileInfo;
	retval->setFile(QDir::home().path()+'/'+DG_CONFIG_PREFIX_LOCAL+DG_NAME+'/'+name);
	if(!retval->isExecutable())
		retval->setFile(QString(DG_CONFIG_PREFIX_GLOBAL)+DG_NAME+"/"+name);
	if(!retval->isExecutable()) {
		delete retval;
		return false;
	}
	QProcess proc;
	proc.setProgram(retval->absoluteFilePath());
	if(args)
		proc.setArguments(*args);
	proc.start();
	if(!proc.waitForStarted())
		return false;
	if(in)
		proc.write(*in);
	if(!proc.waitForFinished())
		return !out;
	if(out)
		*out = proc.readAllStandardOutput();
	return true;
}

std::set<QString> getConfigDirs(const QString& name) {
	std::set<QString> retval;
	QDir local = QDir(QDir::home().path()+'/'+DG_CONFIG_PREFIX_LOCAL+DG_NAME+'/'+name);
	QDir global = QDir(QString(DG_CONFIG_PREFIX_GLOBAL)+DG_NAME+"/"+name);
	QStringList v;
	v = local.entryList(QStringList(),QDir::Dirs|QDir::NoDotAndDotDot,QDir::Name);
	for(QString& s : v)
		retval.insert(s);
	v = global.entryList(QStringList(),QDir::Dirs|QDir::NoDotAndDotDot,QDir::Name);
	for(QString& s : v)
		retval.insert(s);
	return retval;
}
