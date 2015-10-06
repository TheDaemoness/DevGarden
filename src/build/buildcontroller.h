#ifndef BUILDCONTROLLER_H
#define BUILDCONTROLLER_H

#include <QObject>
#include <QDir>

#include <future>

#include "../utils.h"
#include "../asyncwatcher.h"

class DGProjectLoader;
class DGProjectInfo;
class DGController;

//TODO: Asynchronous building. This is fairly high priority. As in, equal with syntax highlighting.
class BuildController {
public:
	explicit BuildController(DGProjectLoader& pl);
	~BuildController() {awatcher.stop();}
	/**
	 * @brief getBuildDir Gets the default directory for building a project.
	 * @param info The project to build.
	 * @param make Whether a directory should be made if it does not exist.
	 * @return
	 */
	void setStartStopTriggers(std::function<void()> start, std::function<void()> stop);
	QString getBuildDirName(const DGProjectInfo& info) const;
	QDir getBuildDir(const DGProjectInfo& info, bool make = true) const;

	bool build(DGProjectInfo& info);
	bool clean(DGProjectInfo& info);
	bool rebuild(DGProjectInfo& info);
	void abort() {
		aflags.stop();
	}
	bool isRunning() {
		return !aflags.isStopped();
	}

private:
	AsyncWatcher awatcher;
	dg_utils::RunToolAsyncFlags aflags; //TODO: Move to DGProjectInfo or something.
	static bool rmRF(const QDir& dir);
	DGProjectLoader* pl;
};

#endif // BUILDCONTROLLER_H
