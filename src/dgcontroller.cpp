#include "dgcontroller.h"

#include "filesys/dgprojectloader.h"

#include <QFileDialog>
#include <QWindow>
#include <QFileSystemModel>

DGController::DGController(DGProjectLoader* pl, QObject *parent) :
	QObject(parent) {
	fsm = nullptr;
	l = pl;
}

void DGController::openFolder() {
	QString str = QFileDialog::getExistingDirectory(nullptr, tr("Open Folder"), QDir::home().absolutePath());
	if(!str.isEmpty())
		l->addFolder(str);
	emit sigProjectListChanged();
	emit sigProjectChanged();
}
void DGController::openFiles() {
	QStringList li = QFileDialog::getOpenFileNames(nullptr, tr("Open Files"), QDir::home().absolutePath());
	if(!li.isEmpty()) {
		for(QString str : li)
			l->addFile(str);
	}
	emit sigProjectListChanged();
	emit sigProjectChanged();
}
void DGController::saveFileAs() {
	QString name = QFileDialog::getSaveFileName(0,tr("Save As..."),"~","",0,0);
}

QStringList DGController::getProjects() {
	return l->getProjectNames();
}

QString DGController::changeProject(size_t index) {
	if(!l->changeCurrent(index))
		return "";
	if(l->getCurrent()->isSingleFile()) {
		if(fsm) {
			delete fsm;
			fsm = nullptr;
		}
		return "";
	}
	else {
		if(!fsm)
			fsm = new QFileSystemModel();
		const QString& retval = l->getCurrent()->getDir()->absolutePath();
		fsm->setRootPath(retval);
		emit sigProjectChanged();
		return retval;
	}
}

QFileSystemModel* DGController::getActiveProjectModel() {
	return fsm;
}
