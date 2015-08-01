#include "dgwindow.h"
#include "dgcentralwidget.hpp"
#include "editor/codeeditorwidget.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMenuBar>

#include "../configloader.h"
#include "../dgcontroller.h"
#include "../langregistry.h"

DGWindow::DGWindow(DGController* dgc, const LangRegistry& lr, QWidget *parent) :
	QMainWindow(parent)
{
	this->setWindowTitle(DG_NAME);
	this->resize(1080,640);
	this->setMinimumSize(560,240);
	ctrl = dgc;

	QMenuBar* bar = new QMenuBar(nullptr);
	this->setMenuBar(bar);

	createMenuActions(lr);

	centralWidget = new DGCentralWidget(dgc, this);
	setCentralWidget(centralWidget);
}

void DGWindow::configure(ConfigFile& f) {
	this->centralWidget->getEditor()->configure(f);
}

void DGWindow::createMenuActions(const LangRegistry& lr) {
	//TODO: Locale system. This is rather critical, actually.
	menuFile = menuBar()->addMenu(tr("&File"));
	QMenu* newMenu = menuFile->addMenu(tr("New"));
	newMenu->addAction(tr("Quick File..."), ctrl, SLOT(newFile()), QKeySequence::New);
	newMenu->addAction(tr("File..."), this, SLOT(nullSlot()));
	newMenu->addAction(tr("Project..."), this, SLOT(nullSlot()));
	menuFile->addAction(tr("Open Folder/Project..."), ctrl, SLOT(openFolder()), QKeySequence::Open);
	menuFile->addAction(tr("Open Files..."), ctrl, SLOT(openFiles()), QKeySequence(tr("Ctrl+Shift+O")));
	menuFile->addSeparator();
	menuFile->addAction(tr("Save"), ctrl, SLOT(saveFile()), QKeySequence::Save);
	menuFile->addAction(tr("Save Copy..."), ctrl, SLOT(saveFileCopy()), QKeySequence::SaveAs);
	menuFile->addAction(tr("Save All"));
	menuFile->addAction(tr("Reload"), ctrl, SLOT(reloadFile()), QKeySequence::Refresh);
	menuFile->addSeparator();
	menuFile->addAction(tr("Close File"));
	menuFile->addAction(tr("Close Other Files"));
	menuFile->addAction(tr("Close All Files"));
	menuFile->addSeparator();
	menuFile->addAction(tr("Close Project"), ctrl, SLOT(closeProjCurrent()), QKeySequence::Close);
	menuFile->addAction(tr("Close Other Projects"), ctrl, SLOT(closeProjOthers()));
	menuFile->addAction(tr("Close All Projects"), ctrl, SLOT(closeProjAll()));
	menuFile->addSeparator();
	menuFile->addAction(tr("Import..."));
	menuFile->addAction(tr("Export..."));
	menuFile->addSeparator();
	menuFile->addAction(tr("Quit"), this, SLOT(quit()), QKeySequence::Quit);

	menuEdit = menuBar()->addMenu(tr("&Edit"));
	menuEdit->addAction(tr("Undo"), this, SLOT(nullSlot()), QKeySequence::Undo);
	menuEdit->addAction(tr("Redo"), this, SLOT(nullSlot()), QKeySequence::Redo);
	menuEdit->addSeparator();
	menuEdit->addAction(tr("Cut"), this, SLOT(nullSlot()), QKeySequence::Cut);
	menuEdit->addAction(tr("Copy"), this, SLOT(nullSlot()), QKeySequence::Copy);
	menuEdit->addAction(tr("Paste"), this, SLOT(nullSlot()), QKeySequence::Paste);
	menuEdit->addAction(tr("Paste from Clipboard History"));
	menuEdit->addSeparator();
	menuEdit->addAction(tr("Select All"), this, SLOT(nullSlot()), QKeySequence::SelectAll);
	menuEdit->addAction(tr("Find/Replace"), this, SLOT(nullSlot()), QKeySequence::Find);
	menuEdit->addAction(tr("Advanced Find/Replace"), this, SLOT(nullSlot()), QKeySequence(tr("Ctrl+Shift+F")));
	menuEdit->addSeparator();
	menuEdit->addAction(tr("Format Selection"));
	menuEdit->addAction(tr("Comment Selection"));

	menuBuild = menuBar()->addMenu(tr("&Build"));
	menuBuildInit = new DGMenu(menuBuild->addMenu(tr("Create Build System")));
	std::set<QString> bses = lr.getBuildSysSet();
	if(!bses.empty()) {
		for(const QString& bs : bses)
			menuBuildInit->addAction(bs,lr.getHumanName(bs)+"...");
	} else
		menuBuildInit->getMenu().setDisabled(true);

	menuBuild->addAction(tr("Regen Build Scripts"));
	menuBuild->addAction(tr("Cancel Build"));
	menuBuild->addAction(tr("Build Settings..."));
	menuBuild->addSeparator();
	menuBuild->addAction(tr("Build"));
	menuBuild->addAction(tr("Rebuild"));
	menuBuild->addAction(tr("Clean"));
	menuBuild->addAction(tr("Deploy"));
	menuBuild->addSeparator();
	menuBuildBuildAll = menuBuild->addMenu(tr("Build All"));
	menuBuildBuildAll->addAction(tr("Last Target"));
	menuBuildBuildAll->addAction(tr("Debug"));
	menuBuildBuildAll->addAction(tr("Release"));
	menuBuildBuildAll->addAction(tr("Custom..."));
	menuBuild->addAction(tr("Rebuild All"));
	menuBuild->addAction(tr("Clean All"));
	menuBuild->addAction(tr("Deploy All"));

	menuDebug = menuBar()->addMenu(tr("&Run/Debug"));
	menuDebug->addAction(tr("Run"));
	menuDebug->addAction(tr("Set ARGV..."));
	menuDebug->addAction(tr("Run Settings..."));
	menuDebug->addSeparator();
	menuDebug->addAction(tr("Run Test"));
	menuDebug->addAction(tr("Test Settings..."));
	menuDebug->addSeparator();
	menuDebug->addAction(tr("Debug"));
	menuDebug->addAction(tr("Debug External Application..."));
	menuDebug->addAction(tr("Load Core File..."));
	menuDebug->addAction(tr("Show Debug Window"));
	menuDebug->addAction(tr("Debugger Settings..."));
	menuDebug->addSeparator();
	menuDebug->addAction(tr("Interrupt"));
	menuDebug->addAction(tr("Continue"));
	menuDebug->addAction(tr("Toggle Breakpoint"));
	menuDebug->addAction(tr("Step Over"));
	menuDebug->addAction(tr("Step Into"));
	menuDebug->addSeparator();
	menuDebugAnalyze = menuDebug->addMenu(tr("Analyze"));
	menuDebugAnalyze->addAction(tr("Memory Checker"));
	menuDebugAnalyze->addAction(tr("Thread Checker"));
	menuDebugAnalyze->addAction(tr("Call Graph"));
	menuDebugAnalyze->addAction(tr("Cache Profiler"));
	menuDebugAnalyze->addAction(tr("Heap Profiler"));
	menuDebug->addAction(tr("Analyze External Application..."));
	menuDebug->addAction(tr("Analysis Settings..."));
	menuDebug->addSeparator();
	menuDebug->addAction(tr("Test"));
	menuDebug->addAction(tr("Test Settings..."));

	menuVersion = menuBar()->addMenu(tr("&VCS"));
	menuVersionInit = menuVersion->addMenu(tr("Create Repository"));
	menuVersionInit->addAction(tr("Subversion..."));
	menuVersionInit->addAction(tr("Mercurial..."));
	menuVersionInit->addAction(tr("Git..."));
	menuVersion->addAction(tr("Repository Settings..."));

	menuWindow = menuBar()->addMenu(tr("&Window"));
	menuWindow->addAction(tr("Minimize"), this, SLOT(showMinimized()));
	menuWindow->addAction(tr("Expand"), this, SLOT(showMaximized()));
	menuWindow->addAction(tr("Toggle Full Screen"), this, SLOT(toggleFullscreen()), QKeySequence::FullScreen);
	menuWindow->addSeparator();
	menuWindow->addAction(tr("Toggle Lower Bar"));
	menuWindow->addAction(tr("Toggle Split View Bar"));
	menuWindow->addAction(tr("Toggle Sidebar"));
	menuWindow->addSeparator();
	menuWindow->addAction(tr("Increase Font Size"), this, SLOT(zoomIn()), QKeySequence::ZoomIn);
	menuWindow->addAction(tr("Decrease Font Size"), this, SLOT(zoomOut()), QKeySequence::ZoomOut);
	menuWindow->addAction(tr("Reset Font Size"), this, SLOT(zoomReset()));
	menuWindow->addSeparator();
	menuWindow->addAction(tr("Toggle Light/Dark Theme"));
	menuWindow->addAction(tr("Run Command Prompt..."), this, SLOT(nullSlot()), QKeySequence(tr("Alt+Space")));
	menuWindow->addAction(tr("Settings..."), this, SLOT(nullSlot()), QKeySequence::Preferences);

	menuHelp = menuBar()->addMenu(tr("&Help"));
	menuHelp->addAction(tr("Manual"), this, SLOT(nullSlot()), QKeySequence::HelpContents);
	menuHelp->addAction(tr("Command Reference"));
	menuHelp->addAction(tr("Hacking DevGarden"));
	menuHelp->addSeparator();
	menuHelp->addAction(tr("Project Page"), this, SLOT(openProjectPage()));
	menuHelp->addAction(tr("Report Bug..."), this, SLOT(openBugReportPage()));
}

void DGWindow::openProjectPage() {
	QDesktopServices::openUrl(QUrl("https://github.com/TheRabbitologist/DevGarden/tree/devel"));
}

void DGWindow::openBugReportPage() {
	QDesktopServices::openUrl(QUrl("https://github.com/TheRabbitologist/DevGarden/issues?q=is%3Aopen+is%3Aissue"));
}

void DGWindow::quit() {
	this->close();
}

void DGWindow::toggleFullscreen() {
	this->isFullScreen()?this->showNormal():this->showFullScreen();
}

void DGWindow::zoomOut()    {this->centralWidget->getEditor()->fontSizeDec();}
void DGWindow::zoomIn()     {this->centralWidget->getEditor()->fontSizeInc();}
void DGWindow::zoomReset()  {this->centralWidget->getEditor()->fontSizeRes();}

void DGWindow::nullSlot() {}

DGWindow::~DGWindow() {
	delete menuBuildInit;
}
