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
	/**
	 * @brief sigProjectChanged Emitted when the active project has been switched to another.
	 */
	void sigProjectChanged();
	/**
	 * @brief sigProjectListChanged() Emitted when the project list changes but the current project is not closed.
	 */
	void sigProjectListChanged();
	/**
	 * @brief sigProjectListClosed() Emitted when the current project list has closed.
	 */
	void sigProjectClosed();

public slots:
	void openFolder();
	void openFiles();
	void saveFileAs();
	void closeCurrent();
	void closeOthers();
	void closeAll();

private:
	QFileSystemModel* fsm; //No relation.
	DGProjectLoader* l;
};

#endif // DGCONTROLLER_H
