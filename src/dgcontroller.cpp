#include "dgcontroller.h"

#include "filesys/dgprojectloader.h"
#include "filesys/dgfileloader.h"

#include "ui/dgwindow.h"
#include "ui/dgcentralwidget.hpp"
#include "ui/editor/codeeditorwidget.h"

#include "configloader.h"

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
		for(const QString& str : li)
			pl->addFile(str);
		emit sigProjectListChanged();
		emit sigProjectChanged();
	}
}
void DGController::saveFileCopy() {
	QString name = QFileDialog::getSaveFileName(0,tr("Save As..."),"~","",0,0);
	QFile f(name);
	if(!f.open(QFile::WriteOnly))
		return;
	if(current.doc)
		f.write(current.doc->toPlainText().toLocal8Bit());
	else
		f.write(dgw->centralWidget->getEditor()->document()->toPlainText().toLocal8Bit());
	f.close();
	current.saved = true;
}

void DGController::saveFile() {
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
		closeFile();
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

void DGController::closeFile() {
	saveFile();
	dgw->centralWidget->getEditor()->clear();
	if(current.doc) {
		//delete current.doc;
		current.doc = nullptr;
	}
}

void DGController::closeCurrent() {
	if(pl->empty())
		return;
	if(pl->getCurrent()->isSingleFile())
		closeFile();
	pl->closeCurrent();
	emit sigProjectClosed();
	emit sigProjectListChanged();
}

void DGController::closeOthers() {
	pl->closeOthers();
	emit sigProjectListChanged();
}

void DGController::closeAll() {
	pl->closeAll();
	emit sigProjectClosed();
	emit sigProjectListChanged();
}

QStringList DGController::getProjects() {
	return pl->getProjectNames();
}

QString DGController::getPath() {
	DGProjectInfo* p = pl->getCurrent();
	if(!p)
		return "";
	if(p->isSingleFile())
		return p->getFile()->absolutePath()+'/'+p->getFile()->fileName();
	else
		return p->getDir()->absolutePath();
}

QString DGController::getDir() {
	DGProjectInfo* p = pl->getCurrent();
	if(!p)
		return QDir::home().absolutePath();
	if(p->isSingleFile())
		return p->getFile()->absoluteDir().absolutePath();
	else
		return p->getDir()->absolutePath();
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

void DGController::newFile() {
	QString naem = QFileDialog::getSaveFileName(0,tr("Quick New File..."),this->getDir(),"",0,0);
	if(naem.isEmpty())
		return;
	QByteArray data;
	QString exact_name = QString("file.")+naem.section('/',-1)+".rb";
	QString path_name = QString("filetype.")+naem.section('.',-1)+".rb";
	QStringList args;
	args << QFileInfo(naem).absoluteFilePath();
	args << (pl->empty()?QFileInfo(naem).absolutePath():getPath());

	QString filetext = "";
	QString filetype = "";

	bool ran;

	ran = runTool("scripts/defaultfiles/"+exact_name,&args,&data);
	if(!ran)
		ran = runTool("scripts/defaultfiles/"+path_name,&args,&data);
	if(ran) {
		filetext = data;
		filetype = filetext.section('\n',0);
		filetext = filetext.section('\n',1);
	}
	QFile fi(naem);
	if(fi.open(QFile::WriteOnly)) {
		fi.write(filetext.toLocal8Bit());
		fi.close();
		if(filetype.contains("\\bexecutable\\b"))
			fi.setPermissions(fi.permissions() | QFile::ExeOwner);
		if(!pl->empty() && !pl->getCurrent()->isSingleFile())
			getFile(naem);
	}
}

void DGController::newTemplateFile() {}
void DGController::newTemplateProject() {}

QFileSystemModel* DGController::getActiveProjectModel() {
	return fsm;
}
