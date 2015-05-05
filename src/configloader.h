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

class ConfigFile {
	using EntryList = std::vector<ConfigEntry*>;
	QString name;
	EntryList entries;
public:
	ConfigFile() {};
	ConfigFile(const char* name);
	inline bool isLoaded() const {return !entries.empty();}
	inline const QString& getName() const {return name;}

	inline EntryList::iterator begin() {return entries.begin();}
	inline EntryList::iterator end() {return entries.begin();}
	inline EntryList::reverse_iterator rbegin() {return entries.rbegin();}
	inline EntryList::reverse_iterator rend() {return entries.rend();}

	inline EntryList::const_iterator cbegin() const {return entries.cbegin();}
	inline EntryList::const_iterator cend() const {return entries.cend();}
	inline EntryList::const_reverse_iterator crbegin() const {return entries.crbegin();}
	inline EntryList::const_reverse_iterator crend() const {return entries.crend();}

	inline size_t size() const {return entries.size();}
	ConfigEntry* at(size_t index) const {return index>=entries.size()?nullptr:entries[index];}
	ConfigEntry* at(const QString& name) const;
};

void makeConfigDirs();
bool runTool(const QString& name, QStringList* args = nullptr, QByteArray* out = nullptr, QByteArray* in = nullptr);
ConfigEntry* getConfigEntry(QFile* ptr);
QFile* getUtilityFileRead(const char* name);
QFile* getUtilityFileWrite(const char* name);

#endif // CONFIGLOADER_H
