#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include "envmacros.h"
#include "configentry.h"

#include <vector>
#include <mutex>

class QFile;
class QFileInfo;
class QDir;
class QTextStream;

#include <set>
#include <atomic>

namespace dg_utils {

std::string qsExtract(const QString& in);

class GlobalFlags {
	static bool self_test;
public:
	static bool isTesting()  {return self_test;}
	static bool setTesting(bool newValue = true) {bool old = newValue; std::swap(old, self_test); return old;}
};

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
	std::atomic_flag sigStarted, sigStopped;
	RunToolAsyncFlags() {stopped = true;
		sigStarted.test_and_set();
		sigStopped.test_and_set();
	}
	std::mutex m_in, m_out;
	inline void stop() {run.clear();}
	inline bool isStopped() {return stopped.load();}
};

}

#define DG_EC_SELFTEST_ECHO(OUTPUT) if(dg_utils::GlobalFlags::isTesting()) {std::cerr << "SELF-TEST: " << OUTPUT << std::endl;}

#endif // CONFIGLOADER_H
