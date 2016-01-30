#ifndef DGWINDOW_H
#define DGWINDOW_H

#include <QMainWindow>

#include "dgmenu.h"

#include "../async/executor.h"

#include <mutex>

class DGCentralWidget;
class DGController;

class ConfigFile;
class LangRegistry;

/**
 * @brief The main window. Contains no widgets.
 */
class DGWindow : public QMainWindow {
	Q_OBJECT

	friend class DGController;
public:
	DGWindow(DGController* dgc, const LangRegistry& lr, Executor* exe, QWidget *parent = 0);
	~DGWindow();
	void configure(ConfigFile& f);
	void disableBuildButtons(bool flag);

public slots:
	void setControlsBuildEnabled(bool enabled);

private:
	void createMenuActions(const LangRegistry& lr);

private slots:
	void openProjectPage();
	void openBugReportPage();
	void quit();
	void toggleFullscreen();
	void zoomReset();
	void zoomIn();
	void zoomOut();
	void nullSlot();

private:
	std::mutex ui_lock;
	DGController* ctrl;
	Executor* exe;

	QMenuBar* bar;
	QMenu *menuFile, *menuEdit, *menuVersion, *menuWindow, *menuHelp;
	QMenu *menuFileNew, *menuVersionInit;

	std::unique_ptr<DGMenu> menuBuild, menuRun;
	std::unique_ptr<DGMenu> menuBuildInit, menuRunDebug, menuRunAnalyze;

	DGCentralWidget* centralWidget;
};

#endif // DGWINDOW_H
