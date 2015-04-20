#ifndef DGCONTROLLER_H
#define DGCONTROLLER_H

#include <QObject>

class QFileSystemModel;
class QString;
class DGProjectLoader;

class DGController : public QObject {
	Q_OBJECT
public:
	explicit DGController(DGProjectLoader* pl, QObject *parent = 0);

	/**
	 * @brief getActiveProjectModel Returns the active project's directory model for use in the file system browser.
	 * @return null if the current project is a single file.
	 */
	QFileSystemModel* getActiveProjectModel();
	QString changeProject(size_t index);
	QStringList getProjects();

signals:
	void sigProjectChanged();
	void sigProjectListChanged();

public slots:
	void openFolder();
	void openFiles();
	void saveFileAs();

private:
	QFileSystemModel* fsm; //No relation.
	DGProjectLoader* l;
};

#endif // DGCONTROLLER_H
