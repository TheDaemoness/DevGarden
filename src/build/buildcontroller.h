#ifndef BUILDCONTROLLER_H
#define BUILDCONTROLLER_H

#include <QObject>
#include <QDir>

#include <future>

#include "../utils.h"

class DGProjectLoader;
class DGProjectInfo;

//TODO: Asynchronous building. This is fairly high priority. As in, equal with syntax highlighting.
class BuildController {
public:
	explicit BuildController(DGProjectLoader& pl);
	/**
	 * @brief getBuildDir Gets the default directory for building a project.
	 * @param info The project to build.
	 * @param make Whether a directory should be made if it does not exist.
	 * @return
	 */
	QString getBuildDirName(const DGProjectInfo& info) const;
	QDir getBuildDir(const DGProjectInfo& info, bool make = true) const;

	bool build(DGProjectInfo& info);
	void clean(DGProjectInfo& info);
	void rebuild(DGProjectInfo& info);
	void abort() {
		aflags.stop();
	}

private:
	std::thread builder;
	dg_utils::RunToolAsyncFlags aflags;
	static bool rmRF(const QDir& dir);
	DGProjectLoader* pl;
};

#endif // BUILDCONTROLLER_H
