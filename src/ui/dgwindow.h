#ifndef DGWINDOW_H
#define DGWINDOW_H

#include <QMainWindow>

#include "dgmenu.h"

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
	explicit DGWindow(DGController* dgc, const LangRegistry& lr, QWidget *parent = 0);
	~DGWindow();
	void configure(ConfigFile& f);

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
	DGController* ctrl;

	QMenuBar* bar;
	QMenu *menuFile, *menuEdit, *menuDebug, *menuVersion, *menuWindow, *menuHelp;
	QMenu *menuFileNew, *menuDebugAnalyze, *menuBuildBuild, *menuBuildBuildAll, *menuVersionInit;

	DGMenu *menuBuild;
	DGMenu *menuBuildInit;

	DGCentralWidget* centralWidget;
};

#endif // DGWINDOW_H
