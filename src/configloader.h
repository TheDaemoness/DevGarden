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

void makeConfigDirs();
bool runScript(const char* name);
bool readConfig(const char* name, std::vector<ConfigEntry*>& entries);
ConfigEntry* getConfigEntry(QFile* ptr);
QFile* getUtilityFileRead(const char* name);
QFile* getUtilityFileWrite(const char* name);

#endif // CONFIGLOADER_H
