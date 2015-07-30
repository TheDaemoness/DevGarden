#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include "configentry.h"

#include <QString>

#include <map>
#include <vector>

class QFile;

/**
 * @brief Stores one file worth of configuration entires.
 */
class ConfigFile {
public:
	using Values = std::vector<ConfigEntry*>;
private:
	using EntryList = std::map<QString,Values>;
	QString name;
	EntryList entries;
public:
	ConfigFile() {};
	ConfigFile(const QString& name);
	ConfigFile(QFile& f);
	~ConfigFile() {for (auto& val : entries) {for (auto& ent : val.second) delete ent;}}
	inline bool isLoaded() const {return !entries.empty();}
	inline ConfigFile& setName(const QString& name) {this->name = name; return *this;}
	inline const QString& getName() const {return name;}

	inline EntryList::iterator begin() {return entries.begin();}
	inline EntryList::iterator end() {return entries.begin();}

	inline EntryList::const_iterator cbegin() const {return entries.cbegin();}
	inline EntryList::const_iterator cend() const {return entries.cend();}

	inline size_t size() const {return entries.size();}
	size_t count(const QString& name) const;
	Values* get(const QString& name); //Do not call on DGFamily.
	ConfigEntry* at(const QString& name, size_t index = 0);

	bool insert(ConfigEntry* ce);
	Values remove(const QString& name);
	void erase(const QString& name);
};

#endif // CONFIGFILE_H
