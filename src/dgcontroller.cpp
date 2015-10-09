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

DGController::DGController(DGProjectLoader* pl, DGFileCache* fl, LangRegistry* lr, BuildController* bc, QObject *parent) :
	QObject(parent) {
	fsm = nullptr;
	this->bc = bc;
	this->pl = pl;
	this->fl = fl;
	this->lr = lr;
	this->dgw = nullptr;
	connect(this,SIGNAL(sigBuildStarted()),this,SLOT(onBuildStarted()));
	connect(this,SIGNAL(sigBuildStopped()),this,SLOT(onBuildStopped()));
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
	//TODO: Implement.
}

void DGController::saveFile() {
	if(fl->saveCurrent())
		pl->addFile(fl->getCurrPath());
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
	sigProjectListChanged();
	sigProjectChanged();
}

void DGController::setView(DGWindow* view) {
	dgw = (dgw?dgw:view);
	dgw->centralWidget->getEditor()->setDocument(fl->getCurrDoc());
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
}

void DGController::getFile(const QString& path) {
	auto ptr = fl->set(path);
	//TODO: Null check.
	dgw->centralWidget->getEditor()->setDocument(ptr);
}

void DGController::runFile() {
	QStringList sl;
	if(lr->hasInterpreter(fl->getCurrLang())) {
		const QString fn = fl->getCurrPath();
		const QString ext = LangRegistry::getFileExt(fn);
		const QString& intrp = lr->getInterpreter(ext, fn != ext);
		if(intrp.isEmpty())
			return;
		sl.append(intrp);
	}
	sl.append(fl->getCurrPath());
	dg_utils::runTool("tools/terminal.rb",&sl);
}

void DGController::fileEdited() {
	fl->markUnsaved();
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
}

void DGController::closeFile() {
	//TODO: Implement.
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
	if(!(ran = dg_utils::runTool("tools/defaultfiles/"+exact_name,&args,&data)))
		ran = dg_utils::runTool("tools/defaultfiles/"+path_name,&args,&data);
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
	fl->reloadCurrent();
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
}

QString DGController::getFormattedFileInfo() {
	size_t lines = fl->getCurrDoc()->lineCount();
	auto ptr = pl->getCurrent();
	QString filename;
	if(!ptr)
		filename = "No File";
	else if(ptr->isSingleFile())
		filename = fl->getCurrPath();
	else
		filename = fl->getCurrPath().isEmpty()?"No File":ptr->getDir()->relativeFilePath(fl->getCurrPath());
	return filename + (!fl->getCurrLang().isEmpty()?" - "+fl->getCurrLang():"") + " - " +
						QString::number(lines) + (lines==1?" line":" lines")
						+ (!fl->isCurrSaved()?" - Unsaved":"") + ' ';
}

void DGController::newTemplateFile() {}
void DGController::newTemplateProject() {}

void DGController::build() {
	if(pl->getCurrent())
		if(pl->getCurrent()->hasBuildSys())
			bc->build(*pl->getCurrent());
}
void DGController::clean() {
	if(pl->getCurrent())
		if(pl->getCurrent()->hasBuildSys())
			bc->clean(*pl->getCurrent());
}
void DGController::rebuild() {
	if(pl->getCurrent())
		if(pl->getCurrent()->hasBuildSys())
			bc->rebuild(*pl->getCurrent());
}
void DGController::abort() {
	if(bc->isRunning())
		bc->abort();
}

void DGController::onBuildStarted() {
	dgw->disableBuildButtons(true);
	dgw->menuBuild->getAction("Build")->setDisabled(true);
	dgw->menuBuild->getAction("Rebuild")->setDisabled(true);
	dgw->menuBuild->getAction("Clean")->setDisabled(true);
}
void DGController::onBuildStopped() {
	dgw->disableBuildButtons(false);
	bool bs = pl->getCurrent()->hasBuildSys();
	dgw->menuBuild->getAction("Build")->setDisabled(!bs);
	dgw->menuBuild->getAction("Rebuild")->setDisabled(!bs);
	dgw->menuBuild->getAction("Clean")->setDisabled(!bs);
}

QString DGController::getFileSaveName() {
	return QFileDialog::getSaveFileName(nullptr,tr("Save As..."),"~");
}
