#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include "envmacros.h"
#include "configentry.h"

#include <vector>

#if defined(DG_ENV_MACOS)
	#define DG_CONFIG_PREFIX_GLOBAL "/Library/Application Support/"
	#define DG_CONFIG_PREFIX_LOCAL  "Library/Application Support/"
#elif defined(DG_ENV_WINDOZE)
	#define DG_CONFIG_PREFIX_GLOBAL "ProgramData/"
	#define DG_CONFIG_PREFIX_LOCAL  "AppData/Roaming/"
#elif defined(DG_ENV_UNIX)
	#define DG_CONFIG_PREFIX_GLOBAL "usr/share/"
	#define DG_CONFIG_PREFIX_LOCAL  "."
	#define DG_CONFIG_PREFIX_LOCAL_NOCD
#else
	#define DG_CONFIG_PREFIX_GLOBAL "/"
	#define DG_CONFIG_PREFIX_LOCAL  ""
	#define DG_CONFIG_PREFIX_LOCAL_NOCD
#endif

class QFile;
class QDir;

#include <QString>

#include <set>
#include <map>

/**
 * @brief Stores one file worth of configuration entires.
 */
class ConfigFile {
	using EntryList = std::map<QString,ConfigEntry*>;
	QString name;
	EntryList entries;
public:
	ConfigFile() {};
	ConfigFile(const char* name);
	ConfigFile(QFile* f);
	~ConfigFile() {for (auto& val : entries) delete val.second;}
	inline bool isLoaded() const {return !entries.empty();}
	inline ConfigFile& setName(const QString& name) {this->name = name; return *this;}
	inline const QString& getName() const {return name;}

	inline EntryList::iterator begin() {return entries.begin();}
	inline EntryList::iterator end() {return entries.begin();}

	inline EntryList::const_iterator cbegin() const {return entries.cbegin();}
	inline EntryList::const_iterator cend() const {return entries.cend();}

	inline size_t size() const {return entries.size();}
	ConfigEntry* at(const QString& name) const;

	bool insert(ConfigEntry* ce);
	ConfigEntry* remove(const QString& name);
	void erase(const QString& name);
};

void makeConfigDirs();
bool runTool(const QString& name, QStringList* args = nullptr, QByteArray* out = nullptr, QByteArray* in = nullptr);
ConfigEntry* getConfigEntry(QFile* ptr);
QFile* getUtilityFileRead(const char* name);
QFile* getUtilityFileWrite(const char* name);
std::set<QString> getConfigDirs(const char* name); //Gets unique subdirectories of a provided config directory, across both config folders.

#endif // CONFIGLOADER_H
