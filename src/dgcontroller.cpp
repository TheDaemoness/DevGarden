#include "dgcontroller.h"

#include "filesys/dgprojectloader.h"
#include "filesys/dgfileloader.h"

#include "ui/dgwindow.h"
#include "ui/dgcentralwidget.hpp"
#include "ui/editor/codeeditorwidget.h"

#include "configloader.h"
#include "langregistry.h"

#include <QFileDialog>
#include <QFileSystemModel>
#include <QTextDocument>
#include <QPushButton>

DGController::DGController(DGProjectLoader* pl, DGFileLoader* fl, LangRegistry* lr, QObject *parent) :
	QObject(parent) {
	fsm = nullptr;
	this->pl = pl;
	this->fl = fl;
	this->lr = lr;
}

void DGController::openFolder() {
	QString str = QFileDialog::getExistingDirectory(nullptr, tr("Open Folder"), QDir::home().absolutePath());
	if(!str.isEmpty()) {
		if(pl->addFolder(str)) {
			emit sigProjectListChanged();
			emit sigProjectChanged();
		}
	}
}
void DGController::openFiles() {
	QStringList li = QFileDialog::getOpenFileNames(nullptr, tr("Open Files"), QDir::home().absolutePath());
	if(!li.isEmpty()) {
		bool atleastone = false;
		for(const QString& str : li) {
			atleastone |= pl->addFile(str);
		}
		if(atleastone) {
			emit sigProjectListChanged();
			emit sigProjectChanged();
		}
	}
}
void DGController::saveFileCopy() {
	QString name = QFileDialog::getSaveFileName(0,tr("Save As..."),"~","",0,0);
	QFile f(name);
	if(!f.open(QFile::WriteOnly))
		return;
	if(curr_file.doc)
		f.write(curr_file.doc->toPlainText().toLocal8Bit());
	else
		f.write(dgw->centralWidget->getEditor()->document()->toPlainText().toLocal8Bit());
	f.close();
	curr_file.saved = true;
}

void DGController::saveFile() {
	if(pl->empty()) {
		QString name = QFileDialog::getSaveFileName(0,tr("Save As..."),"~","",0,0);
		QFile f(name);
		if(!f.open(QFile::WriteOnly))
			return;
		f.write(dgw->centralWidget->getEditor()->document()->toPlainText().toLocal8Bit());
		curr_file.saved = true;
		f.close();
		if(pl->addFile(name)) {
			emit sigProjectListChanged();
			emit sigProjectChanged();
		}
		return;
	}
	if(curr_file.saved)
		return;
	QFile f(curr_file.path);
	if(!f.open(QFile::WriteOnly))
		return;
	f.write(curr_file.doc->toPlainText().toLocal8Bit());
	f.close();
	curr_file.saved = true;
}

void DGController::getFile(const QString& path) {
	QFile f(path);
	if(!f.open(QFile::ReadOnly))
		return;
	if(curr_file.doc)
		closeFile();
	curr_file.path = path;
	CodeEditorWidget* w = dgw->centralWidget->getEditor();
	w->blockSignals(true);
	w->setContents(f.readAll());
	w->blockSignals(false);
	curr_file.exe = QFileInfo(path).isExecutable();
	f.close();
	curr_file.doc = w->document();
	curr_file.saved = true;
	QString ext = path.section('.',-1);
	curr_file.lang = lr->getLang(ext);
	dgw->centralWidget->buttonsLower.at(DGCentralWidget::RUNFILE)->
		setHidden(!(curr_file.exe || (curr_file.lang.isEmpty()?false:lr->hasInterpreter(ext))));
}

void DGController::runFile() {
	QStringList sl;
	if(!curr_file.exe)
		sl.append(lr->getInterpreter(curr_file.path.section('.',-1)) + ' ' + curr_file.path);
	else
		sl.append(curr_file.path);
	runTool("scripts/terminal.rb",&sl);
}

void DGController::fileEdited() {
	curr_file.saved = false;
}

void DGController::closeFile() {
	saveFile();
	dgw->centralWidget->getEditor()->clear();
	if(curr_file.doc) {
		//delete current.doc;
		curr_file.doc = nullptr;
	}
}

void DGController::closeProjCurrent() {
	if(pl->empty())
		return;
	if(pl->getCurrent()->isSingleFile())
		closeFile();
	pl->closeCurrent();
	emit sigProjectClosed();
	emit sigProjectListChanged();
}

void DGController::closeProjOthers() {
	pl->closeOthers();
	emit sigProjectListChanged();
}

void DGController::closeProjAll() {
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
	const QString exact_name = QString("file.")+naem.section('/',-1)+".rb";
	const QString path_name = QString("filetype.")+naem.section('.',-1)+".rb";
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

void DGController::reloadFile() {
	this->curr_file.saved = true;
	this->getFile(this->curr_file.path);
}

void DGController::newTemplateFile() {}
void DGController::newTemplateProject() {}
