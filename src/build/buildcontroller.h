#ifndef BUILDCONTROLLER_H
#define BUILDCONTROLLER_H

#include <QObject>
#include <QDir>

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

	void build(DGProjectInfo& info) const;
	void clean(DGProjectInfo& info) const;
	void rebuild(DGProjectInfo& info) const;
private:
	static bool rmRF(const QDir& dir);
	DGProjectLoader* pl;
};

#endif // BUILDCONTROLLER_H
