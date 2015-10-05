#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include "envmacros.h"
#include "configentry.h"

#include <vector>
#include <future>

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
class QFileInfo;
class QDir;
class QTextStream;

#include <set>

namespace dg_utils {

void makeConfigDirs();
bool runTool(const QString& name, QStringList* args = nullptr); //Disambiguation.
bool runTool(const QString& name, QStringList* args, QByteArray* out, QByteArray* in = nullptr);
bool runTool(const QString& name, QStringList* args, QTextStream* out, QTextStream* in = nullptr);
ConfigEntry* getConfigEntry(QFile& file);
QFileInfo* getUtilityFile(const QString& name);
QFile* getUtilityFileRead(const QString& name);
QFile* getUtilityFileWrite(const QString& name);
std::set<QString> getConfigDirs(const QString& name); //Gets unique subdirectories of a provided config directory, across both config folders.

}

#endif // CONFIGLOADER_H
