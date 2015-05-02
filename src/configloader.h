#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include "envmacros.h"

#include <vector>
#include <string>

#if defined(DG_ENV_MACOS)
	#define DG_CONFIG_PREFIX_GLOBAL "Library/Application Support/"
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

struct ConfigEntry {
	std::string data;
	std::string* inherit;
	std::vector<ConfigEntry*> children;
};

void makeConfigDirs();
bool runScript(const char* name);
ConfigEntry* getConfigEntry(QFile* ptr);
QFile* getConfigFileRead(const char* name);
QFile* getConfigFileWrite(const char* name);

#endif // CONFIGLOADER_H
