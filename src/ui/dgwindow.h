#ifndef DGWINDOW_H
#define DGWINDOW_H

#include <QMainWindow>

#include "../dgcontroller.h"

class DGCentralWidget;

class DGWindow : public QMainWindow {
	Q_OBJECT

	friend class DGController;
public:
	explicit DGWindow(DGController* dgc, QWidget *parent = 0);
	~DGWindow();

private:
	void createMenuActions();

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
	DGController* ctrl;

	QMenuBar* bar;
	QMenu *menuFile, *menuEdit, *menuBuild, *menuDebug, *menuWindow, *menuHelp;
	QMenu *menuDebugAnalyze, *menuBuildBuild, *menuBuildBuildAll;

	DGCentralWidget* centralWidget;
};

#endif // DGWINDOW_H
