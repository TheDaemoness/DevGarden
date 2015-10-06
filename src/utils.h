#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include "envmacros.h"
#include "configentry.h"

#include <vector>
#include <mutex>

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
#include <atomic>

namespace dg_utils {

class RunToolAsyncFlags;

void makeConfigDirs();
bool runTool(const QString& name, QStringList* args = nullptr); //Disambiguation.
bool runTool(const QString& name, QStringList* args, QByteArray* out, QByteArray* in = nullptr);
bool runTool(const QString& name, QStringList* args,
			 QTextStream* out, QTextStream* in = nullptr,
			 RunToolAsyncFlags* async = nullptr);
ConfigEntry* getConfigEntry(QFile& file);
QFileInfo* getUtilityFile(const QString& name);
QFile* getUtilityFileRead(const QString& name);
QFile* getUtilityFileWrite(const QString& name);
std::set<QString> getConfigDirs(const QString& name); //Gets unique subdirectories of a provided config directory, across both config folders.

class RunToolAsyncFlags  {
	std::atomic_bool stopped;
	std::atomic_flag run;
	friend bool runTool(const QString& name, QStringList* args,
						QTextStream* out, QTextStream* in,
						RunToolAsyncFlags* async);
	inline void reset() {
		stopped.store(false);
		m_in.unlock();
		m_out.unlock();
		run.test_and_set();
	}
public:
	RunToolAsyncFlags() {stopped = true;}
	std::mutex m_in, m_out;
	inline void stop() {run.clear();}
	inline bool isStopped() {return stopped.load();}
};

}

#endif // CONFIGLOADER_H
