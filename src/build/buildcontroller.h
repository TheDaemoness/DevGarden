#ifndef BUILDCONTROLLER_H
#define BUILDCONTROLLER_H

#include <QObject>
#include <QDir>

class DGProjectLoader;
class DGProjectInfo;

class BuildController : public QObject {
	Q_OBJECT
public:
	explicit BuildController(DGProjectLoader& pl, QObject *parent = 0);
	/**
	 * @brief getBuildDir Gets the default directory for building a project.
	 * @param info The project to build.
	 * @param make Whether a directory should be made if it does not exist.
	 * @return
	 */
	QString getBuildDirName(const DGProjectInfo& info) const;
	QDir getBuildDir(const DGProjectInfo& info, bool make = true) const;
signals:

public slots:
	void build(DGProjectInfo& info);
	void clean(DGProjectInfo& info);
private:
	DGProjectLoader* pl;
};

#endif // BUILDCONTROLLER_H
