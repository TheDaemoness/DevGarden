#ifndef DGWINDOW_H
#define DGWINDOW_H

#include <QMainWindow>

class DGWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit DGWindow(QWidget *parent = 0);
	~DGWindow();

private:
	void createMenuActions();

private slots:
	void openProjectPage();
	void openBugReportPage();
	void quit();
	void toggleFullscreen();
	void zoomIn();
	void zoomOut();
	void nullSlot();

private:
	QMenuBar* bar;
	QMenu *menuFile, *menuEdit, *menuBuild, *menuDebug, *menuWindow, *menuHelp;
	QMenu *menuDebugAnalyze, *menuBuildBuild, *menuBuildBuildAll;
};

#endif // DGWINDOW_H
