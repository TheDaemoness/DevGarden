#ifndef DGCONTROLLER_H
#define DGCONTROLLER_H

#include <QObject>
#include <QRegExp>

#include "filesys/dgfileloader.h"

class QFileSystemModel;
class QString;
class QTextDocument;
class DGProjectLoader;
class DGWindow;
class LangRegistry;

/**
 * @brief A file loader QObject interface.
 * Provides a QObject to control DevGarden's file, project, and configuration loaders.
 * It additionally emits signals to alert UI components/other QObjects of updates.
 */
class DGController : public QObject {
	Q_OBJECT
public:
	explicit DGController(DGProjectLoader* pl, DGFileLoader* fl, LangRegistry* lr, QObject *parent = 0);

	/**
	 * @brief getActiveProjectModel Returns the active project's directory model for use in the file system browser.
	 * @return null if the current project is a single file.
	 */
	inline QFileSystemModel* getActiveProjectModel() {return fsm;}
	QString changeProject(size_t index);
	QStringList getProjects();
	QString getPath();
	QString getDir();

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
	void runFile();

	void openFolder();
	void openFiles();

	void reloadFile();
	void saveFileCopy();
	void saveFile();

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
	void rebuild() {clean(); build();}

private:
	//NOTE: This is temporary until DGFileLoader is implemented.
	DGFileLoader::FileRef curr_file;

	DGWindow* dgw;
	QFileSystemModel* fsm; //No relation.
	DGProjectLoader* pl;
	DGFileLoader* fl;
	LangRegistry* lr;
};

#endif // DGCONTROLLER_H
