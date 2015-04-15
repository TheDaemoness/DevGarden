#include "dgwindow.h"
#include "ui_dgwindow.h"

#include <QDesktopServices>
#include <QUrl>

DGWindow::DGWindow(DGController* dgc, QWidget *parent) :
	QMainWindow(parent)
{
	this->setWindowTitle("DevGarden");
	this->resize(1080,640);
	this->setMinimumSize(640,360);
	ctrl = dgc;

	QMenuBar* bar = new QMenuBar(nullptr);
	this->setMenuBar(bar);

	createMenuActions();
}

void DGWindow::createMenuActions() {
	//TODO: Locale system. This is rather critical, actually.
	menuFile = menuBar()->addMenu(tr("&File"));
	menuFile->addAction("New File/Project...", this, SLOT(nullSlot()), QKeySequence::New);
	menuFile->addAction("Open Folder/Project...", ctrl, SLOT(openFolder()), QKeySequence::Open);
	menuFile->addAction("Open Files...", ctrl, SLOT(openFiles()), QKeySequence(tr("Ctrl+Shift+O")));
	menuFile->addSeparator();
	menuFile->addAction("Save", this, SLOT(nullSlot()), QKeySequence::Save);
	menuFile->addAction("Save As...", this, SLOT(nullSlot()), QKeySequence::SaveAs);
	menuFile->addAction("Save All");
	menuFile->addAction("Reload", this, SLOT(nullSlot()), QKeySequence::Refresh);
	menuFile->addSeparator();
	menuFile->addAction("Close", this, SLOT(nullSlot()), QKeySequence::Close);
	menuFile->addAction("Close Project");
	menuFile->addSeparator();
	menuFile->addAction("Import...");
	menuFile->addAction("Export...");
	menuFile->addSeparator();
	menuFile->addAction("Quit", this, SLOT(quit()), QKeySequence::Quit);

	menuEdit = menuBar()->addMenu(tr("&Edit"));
	menuEdit->addAction("Undo", this, SLOT(nullSlot()), QKeySequence::Undo);
	menuEdit->addAction("Redo", this, SLOT(nullSlot()), QKeySequence::Redo);
	menuEdit->addSeparator();
	menuEdit->addAction("Cut", this, SLOT(nullSlot()), QKeySequence::Cut);
	menuEdit->addAction("Copy", this, SLOT(nullSlot()), QKeySequence::Copy);
	menuEdit->addAction("Paste", this, SLOT(nullSlot()), QKeySequence::Paste);
	menuEdit->addAction("Paste from Clipboard History");
	menuEdit->addSeparator();
	menuEdit->addAction("Select All", this, SLOT(nullSlot()), QKeySequence::SelectAll);
	menuEdit->addAction("Find/Replace", this, SLOT(nullSlot()), QKeySequence::Find);
	menuEdit->addAction("Advanced Find/Replace", this, SLOT(nullSlot()), QKeySequence(tr("Ctrl+Shift+F")));
	menuEdit->addSeparator();
	menuEdit->addAction("Format Selection");
	menuEdit->addAction("Comment Selection");

	menuBuild = menuBar()->addMenu(tr("&Build"));
	menuBuild->addAction("Cancel Build");
	menuBuild->addAction("Regen Build Scripts");
	menuBuild->addAction("Build Settings...");
	menuBuild->addSeparator();
	menuBuildBuild = menuBuild->addMenu("Build");
	menuBuildBuild->addAction("Last Target");
	menuBuildBuild->addAction("Debug");
	menuBuildBuild->addAction("Release");
	menuBuildBuild->addAction("Custom...");
	menuBuild->addAction("Rebuild");
	menuBuild->addAction("Clean");
	menuBuild->addAction("Deploy");
	menuBuild->addSeparator();
	menuBuildBuildAll = menuBuild->addMenu("Build All");
	menuBuildBuildAll->addAction("Last Target");
	menuBuildBuildAll->addAction("Debug");
	menuBuildBuildAll->addAction("Release");
	menuBuildBuildAll->addAction("Custom...");
	menuBuild->addAction("Rebuild All");
	menuBuild->addAction("Clean All");
	menuBuild->addAction("Deploy All");

	menuDebug = menuBar()->addMenu(tr("&Run/Debug"));
	menuDebug->addAction("Run");
	menuDebug->addAction("Run Settings...");
	menuDebug->addSeparator();
	menuDebug->addAction("Debug");
	menuDebug->addAction("Debug External Application...");
	menuDebug->addAction("Load Core File...");
	menuDebug->addAction("Show Debug Window");
	menuDebug->addAction("Debugger Settings...");
	menuDebug->addSeparator();
	menuDebug->addAction("Interrupt");
	menuDebug->addAction("Continue");
	menuDebug->addAction("Toggle Breakpoint");
	menuDebug->addAction("Step Over");
	menuDebug->addAction("Step Into");
	menuDebug->addSeparator();
	menuDebugAnalyze = menuDebug->addMenu("Analyze");
	menuDebugAnalyze->addAction("Memory Checker");
	menuDebugAnalyze->addAction("Thread Checker");
	menuDebugAnalyze->addAction("Call Graph");
	menuDebugAnalyze->addAction("Cache Profiler");
	menuDebugAnalyze->addAction("Heap Profiler");
	menuDebug->addAction("Analyze External Application...");
	menuDebug->addAction("Analysis Settings...");
	menuDebug->addSeparator();
	menuDebug->addAction("Test");
	menuDebug->addAction("Test Settings...");

	menuWindow = menuBar()->addMenu(tr("&Window"));
	menuWindow->addAction("Minimize", this, SLOT(showMinimized()));
	menuWindow->addAction("Expand", this, SLOT(showMaximized()));
	menuWindow->addAction("Toggle Full Screen", this, SLOT(toggleFullscreen()), QKeySequence::FullScreen);
	menuWindow->addSeparator();
	menuWindow->addAction("Toggle Lower Bar");
	menuWindow->addAction("Toggle Split View Bar");
	menuWindow->addAction("Toggle Sidebar");
	menuWindow->addSeparator();
	menuWindow->addAction("Increase Font Size", this, SLOT(zoomIn()), QKeySequence::ZoomIn);
	menuWindow->addAction("Decrease Font Size", this, SLOT(zoomOut()), QKeySequence::ZoomOut);
	menuWindow->addAction("Reset Font Size");
	menuWindow->addSeparator();
	menuWindow->addAction("Toggle Light/Dark Theme");
	menuWindow->addAction("Run Command Prompt...", this, SLOT(nullSlot()), QKeySequence(tr("Alt+Space")));
	menuWindow->addAction("Settings...", this, SLOT(nullSlot()), QKeySequence::Preferences);

	menuHelp = menuBar()->addMenu(tr("&Help"));
	menuHelp->addAction("Manual", this, SLOT(nullSlot()), QKeySequence::HelpContents);
	menuHelp->addAction("Command Reference");
	menuHelp->addAction("Hacking DevGarden");
	menuHelp->addSeparator();
	menuHelp->addAction("Project Page", this, SLOT(openProjectPage()));
	menuHelp->addAction("Report Bug...", this, SLOT(openBugReportPage()));
}

void DGWindow::openProjectPage() {
	QDesktopServices::openUrl(QUrl("https://github.com/TheRabbitologist/DevGarden"));
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

void DGWindow::zoomOut() {}
void DGWindow::zoomIn() {}

void DGWindow::nullSlot() {}

DGWindow::~DGWindow() {
}
