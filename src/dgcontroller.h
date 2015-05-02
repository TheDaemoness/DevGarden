#ifndef DGCONTROLLER_H
#define DGCONTROLLER_H

#include <QObject>

#include "filesys/dgfileloader.h"

class QFileSystemModel;
class QString;
class QTextDocument;
class DGProjectLoader;
class DGWindow;

class DGController : public QObject {
	Q_OBJECT
public:
	explicit DGController(DGProjectLoader* pl, DGFileLoader* fl, QObject *parent = 0);

	/**
	 * @brief getActiveProjectModel Returns the active project's directory model for use in the file system browser.
	 * @return null if the current project is a single file.
	 */
	QFileSystemModel* getActiveProjectModel();
	QString changeProject(size_t index);
	QStringList getProjects();
	QString getPath();

	inline void setView(DGWindow* view) {dgw = (dgw?dgw:view);}

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
	 * @brief sigProjectListClosed() Emitted when the current project has closed.
	 */
	void sigProjectClosed();

public slots:
	void getFile(const QString& path);
	void openFolder();
	void openFiles();
	void saveFileCopy();
	void saveFile();
	void closeFile();
	void closeCurrent();
	void closeOthers();
	void closeAll();
	void fileEdited();

private:
	//NOTE: This is temporary until DGFileLoader is implemented.
	DGFileLoader::FileRef current;

	DGWindow* dgw;
	QFileSystemModel* fsm; //No relation.
	DGProjectLoader* pl;
	DGFileLoader* fl;
};

#endif // DGCONTROLLER_H
