#include "dgcontroller.h"

#include "filesys/dgprojectloader.h"
#include "filesys/dgfilecache.h"

#include "ui/dgwindow.h"
#include "ui/dgcentralwidget.hpp"
#include "ui/editor/codeeditorwidget.h"

#include "async/executor.h"

#include "utils.h"
#include "langregistry.h"

#include <QFileDialog>
#include <QFileSystemModel>
#include <QTextDocument>
#include <QPushButton>
#include <QLabel>

DGController::DGController(DGProjectLoader* pl, DGFileCache* fl, LangRegistry* lr, Executor* exe, QObject *parent) :
	QObject(parent) {
	fsm = nullptr;
	this->pl = pl;
	this->fc = fl;
	this->lr = lr;
	this->exe = exe;
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
	if(fc->saveCurrent()) {
		pl->addFile(fc->getCurrPath());
		sigProjectListChanged();
		sigProjectChanged();
	}
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
}
void DGController::saveFileOthers() {
	fc->saveOthers();
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
}
void DGController::saveFileAll() {
	fc->saveOthers();
	saveFile();
}

void DGController::setView(DGWindow* view) {
	dgw = (dgw?dgw:view);
	dgw->centralWidget->getEditor()->setContents(fc->getCurrDoc());
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
}

void DGController::getFile(const QString& path) {
	QFileInfo fi(path);
	if(fi.isDir() || !fi.exists() || !fi.isReadable())
		return;
	dgw->centralWidget->getEditor()->blockSignals(true);
	dgw->centralWidget->getEditor()->setContents(fc->set(fi));
	dgw->centralWidget->getEditor()->blockSignals(false);
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
	dgw->centralWidget->buttonsSide.at(DGCentralWidget::RUNFILE)->
		setHidden(!(fi.isExecutable() || lr->hasInterpreter(fi)));
}

void DGController::runFile() {
	QStringList sl;
	QFileInfo fi(fc->getCurrPath());
	if(lr->hasInterpreter(fi)) {
		const QString& intrp = lr->getInterpreter(fi);
		if(intrp.isEmpty())
			return;
		sl.append(intrp);
	}
	sl.append(fc->getCurrPath());
	dg_utils::runTool("tools/terminal.rb",&sl);
}

void DGController::fileEdited() {
	fc->markUnsaved();
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
	fc->reloadCurrent();
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
}
void DGController::reloadFileOthers() {
	fc->reloadOthers();
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
}
void DGController::reloadFileAll() {
	fc->reloadOthers();
	reloadFile();
}

QString DGController::getFormattedFileInfo() {
	size_t lines = fc->getCurrDoc()->lineCount();
	auto ptr = pl->getCurrent();
	QString filename;
	if(!ptr)
		filename = "";
	else if(!ptr->isSingleFile())
		filename = ptr->getDir()->relativeFilePath(fc->getCurrPath());
	QString filestat = fc->getCurrStatus();
	if(!fc->isCurrSaveable())
		filestat = " - No Loader";
	else if(!filestat.isEmpty())
		filestat = filestat.prepend(" - ");
	else if(!fc->isCurrSaved())
		filestat = " - Unsaved";
    const auto langname = lr->getHumanName(fc->getCurrLang());
	return (filename.isEmpty()?"":(filename+" - ")) + (!langname.isEmpty()?(langname+" - "):"") +
						QString::number(lines) + (lines==1?" line":" lines")
						+ filestat + ' ';
}

void DGController::newTemplateFile() {}
void DGController::newTemplateProject() {}

void DGController::build() {
	if(pl->getCurrent())
		if(pl->getCurrent()->hasBuildSys())
			;
}
void DGController::clean() {
	if(pl->getCurrent())
		if(pl->getCurrent()->hasBuildSys())
			;
}
void DGController::rebuild() {
	if(pl->getCurrent())
		if(pl->getCurrent()->hasBuildSys())
			;
}
void DGController::abort() {
	;
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
void DGController::onFileCacheUpdate() {
	dgw->centralWidget->fileInfo->setText(getFormattedFileInfo());
}
