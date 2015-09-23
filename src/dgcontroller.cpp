#include "dgcontroller.h"

#include "filesys/dgprojectloader.h"
#include "filesys/dgfilecache.h"

#include "ui/dgwindow.h"
#include "ui/dgcentralwidget.hpp"
#include "ui/editor/codeeditorwidget.h"

#include "utils.h"
#include "langregistry.h"

#include <QFileDialog>
#include <QFileSystemModel>
#include <QTextDocument>
#include <QPushButton>
#include <QLabel>

DGController::DGController(DGProjectLoader* pl, DGFileCache* fl, LangRegistry* lr, QObject *parent) :
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
	QFile f(curr_file.info.absoluteFilePath());
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
	curr_file.info = QFileInfo(path);
	CodeEditorWidget* w = dgw->centralWidget->getEditor();
	w->blockSignals(true);
	w->setContents(f.readAll());
	w->blockSignals(false);
	f.close();
	curr_file.doc = w->document();
	curr_file.saved = true;
	const QString fn = curr_file.info.fileName();
	const QString ext = LangRegistry::getFileExt(fn);
	const bool isext = fn != ext;
	curr_file.lang = lr->getLang(ext, isext);
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
	dgw->centralWidget->buttonsLower.at(DGCentralWidget::RUNFILE)->
		setHidden(!(curr_file.info.isExecutable() ||
					(curr_file.lang.isEmpty()?false:lr->hasInterpreter(ext, isext))));
}

void DGController::runFile() {
	QStringList sl;
	if(!curr_file.info.isExecutable()) {
		const QString fn = curr_file.info.fileName();
		const QString ext = LangRegistry::getFileExt(fn);
		const QString& intrp = lr->getInterpreter(ext, fn != ext);
		if(intrp.isEmpty())
			return;
		sl.append(intrp);
	}
	sl.append(curr_file.info.absoluteFilePath());
	runTool("scripts/terminal.rb",&sl);
}

void DGController::fileEdited() {
	if(!curr_file.doc)
		curr_file.doc = dgw->centralWidget->getEditor()->document();
	curr_file.saved = false;
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
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
		dgw->setControlsBuildEnabled(pl->getCurrent()->hasBuildSys());
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
	if(!(ran = runTool("scripts/defaultfiles/"+exact_name,&args,&data)))
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
	this->getFile(this->curr_file.info.absoluteFilePath());
}

QString DGController::getFormattedFileInfo() {
	size_t lines = curr_file.doc?curr_file.doc->lineCount():1;
	auto ptr = pl->getCurrent();
	QString filename;
	if(!ptr)
		filename = "No File";
	else if(ptr->isSingleFile())
		filename = curr_file.info.exists()?curr_file.info.absoluteFilePath():"No File";
	else
		filename = curr_file.info.exists()?ptr->getDir()->relativeFilePath(curr_file.info.filePath()):"No File";
	return filename + (!curr_file.lang.isEmpty()?" - "+curr_file.lang:"") + " - " +
						QString::number(lines) + (lines==1?" line":" lines")
						+ (!curr_file.saved?" - Unsaved":"");
}

void DGController::newTemplateFile() {}
void DGController::newTemplateProject() {}

void DGController::build() {

}

void DGController::clean() {

}
