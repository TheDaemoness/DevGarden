#ifndef DGCONTROLLER_H
#define DGCONTROLLER_H

#include <QObject>

class QFileSystemModel;
class QString;

class DGController : public QObject {
	Q_OBJECT
public:
	explicit DGController(QObject *parent = 0);

	/**
	 * @brief getActiveProjectModel Returns the active project's directory model for use in the file system browser.
	 * @return null if the current project is a single file.
	 */
	QFileSystemModel* getActiveProjectModel();
	void changeProject(const QString& str);

signals:

public slots:
	void openFolder();
	void openFiles();
	void saveFileAs();
};

#endif // DGCONTROLLER_H
