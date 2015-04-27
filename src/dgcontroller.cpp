#include "dgcontroller.h"

#include "filesys/dgprojectloader.h"
#include "filesys/dgfileloader.h"

#include "ui/dgwindow.h"
#include "ui/dgcentralwidget.hpp"
#include "ui/editor/codeeditorwidget.h"

#include <QFileDialog>
#include <QFileSystemModel>
#include <QTextDocument>

DGController::DGController(DGProjectLoader* pl, DGFileLoader* fl, QObject *parent) :
	QObject(parent) {
	fsm = nullptr;
	this->pl = pl;
	this->fl = fl;
}

void DGController::openFolder() {
	QString str = QFileDialog::getExistingDirectory(nullptr, tr("Open Folder"), QDir::home().absolutePath());
	if(!str.isEmpty()) {
		pl->addFolder(str);
		emit sigProjectListChanged();
		emit sigProjectChanged();
	}
}
void DGController::openFiles() {
	QStringList li = QFileDialog::getOpenFileNames(nullptr, tr("Open Files"), QDir::home().absolutePath());
	if(!li.isEmpty()) {
		for(QString str : li)
			pl->addFile(str);
		emit sigProjectListChanged();
		emit sigProjectChanged();
	}
}
void DGController::saveFileAs() {
	QString name = QFileDialog::getSaveFileName(0,tr("Save As..."),"~","",0,0);
}

void DGController::saveCurrent() {
	if(current.saved)
		return;
	QFile f(current.path);
	if(!f.open(QFile::WriteOnly))
		return;
	f.write(current.doc->toPlainText().toLocal8Bit());
	f.close();
	current.saved = true;
}

void DGController::getFile(const QString& path) {
	QFile f(path);
	if(!f.open(QFile::ReadOnly))
		return;
	if(current.doc)
		closeCurrent();
	current.path = path;
	CodeEditorWidget* w = dgw->centralWidget->getEditor();
	w->blockSignals(true);
	w->setPlainText(f.readAll());
	w->blockSignals(false);
	f.close();
	current.doc = w->document();
	current.saved = true;
}

void DGController::fileEdited() {
	current.saved = false;
}

void DGController::closeCurrent() {
	saveCurrent();
	dgw->centralWidget->getEditor()->clear();
	if(current.doc) {
		//delete current.doc;
		current.doc = nullptr;
	}
}
void DGController::closeOthers() {closeCurrent();}
void DGController::closeAll() {closeCurrent();}

QStringList DGController::getProjects() {
	return pl->getProjectNames();
}

QString DGController::changeProject(size_t index) {
	if(!pl->changeCurrent(index))
		return "";
	if(pl->getCurrent()->isSingleFile()) {
		if(fsm) {
			delete fsm;
			fsm = nullptr;
		}
		return "";
	}
	else {
		if(!fsm)
			fsm = new QFileSystemModel();
		const QString& retval = pl->getCurrent()->getDir()->absolutePath();
		fsm->setRootPath(retval);
		emit sigProjectChanged();
		return retval;
	}
}

QFileSystemModel* DGController::getActiveProjectModel() {
	return fsm;
}
