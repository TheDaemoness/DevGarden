#ifndef DGCONTROLLER_H
#define DGCONTROLLER_H

#include <QObject>
#include <QRegExp>

#include "filesys/dgfilecache.h"

class QFileSystemModel;
class QString;
class QTextDocument;
class DGProjectLoader;
class DGWindow;

class LangRegistry;
class Executor;

/**
 * @brief A file loader QObject interface.
 * Provides a QObject to control DevGarden's file, project, and configuration loaders.
 * It additionally emits signals to alert UI components/other QObjects of updates.
 */
class DGController : public QObject {
	Q_OBJECT
public:
	explicit DGController(DGProjectLoader* pl, DGFileCache* fc, LangRegistry* lr, Executor* exe, QObject *parent = 0);

	/**
	 * @brief getActiveProjectModel Returns the active project's directory model for use in the file system browser.
	 * @return null if the current project is a single file.
	 */
	inline QFileSystemModel* getActiveProjectModel() {return fsm;}
	QString changeProject(size_t index);
	QStringList getProjects();
	QString getPath();
	QString getDir();

	void setView(DGWindow* view);

	QString getFileSaveName();
	QString getFormattedFileInfo();

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
	void sigBuildStarted();
	void sigBuildStopped();

public slots:
	void getFile(const QString& path);
	void runFile();

	void openFolder();
	void openFiles();

	void reloadFile();
	void reloadFileOthers();
	void reloadFileAll();

	void saveFile();
	void saveFileOthers();
	void saveFileAll();
	void saveFileCopy();

	void closeFile();
	void closeProjCurrent();
	void closeProjOthers();
	void closeProjAll();

	void fileEdited();

	void newFile();
	void newTemplateFile();
	void newTemplateProject();

	void build();
	void clean();
	void rebuild();
	void abort();

	void onFileCacheUpdate();

private:
	DGWindow* dgw;
	QFileSystemModel* fsm; //No relation.
	DGProjectLoader* pl;
	DGFileCache* fc;
	LangRegistry* lr;
	Executor* exe;

private slots:
	void onBuildStarted();
	void onBuildStopped();
};

#endif // DGCONTROLLER_H
