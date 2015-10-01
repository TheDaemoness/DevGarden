#include "configfile.h"

#include "utils.h"

#include <QFile>

using namespace dg_utils;

ConfigFile::ConfigFile(QFile& f) {
	for(ConfigEntry* e = getConfigEntry(f); e != nullptr; e = getConfigEntry(f))
		insert(e);
}

ConfigFile::ConfigFile(const QString& name) {
	QFile* ptr = getUtilityFileRead(name);
	if(!ptr)
		return;
	this->name = name;
	for(ConfigEntry* e = getConfigEntry(*ptr); e != nullptr; e = getConfigEntry(*ptr))
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
