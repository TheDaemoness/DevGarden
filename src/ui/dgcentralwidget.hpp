#ifndef DG_CENTRAL_WIDGET_HPP
#define DG_CENTRAL_WIDGET_HPP

#include <QWidget>

class QTreeView;
class QListWidget;
class QFileSystemModel;
class QComboBox;
class QPlainTextEdit;
class QLineEdit;
class QPushButton;
class CodeEditorWidget;

class QHBoxLayout;
class QVBoxLayout;

class DGController;

#include <map>
#include <unordered_map>
#include <array>

/**
 * @brief The actual widgets of the main window.
 * @warning Does not free any of the memory it allocates itself. DO NOT FREE OR ALLOCATE MULTIPLE INSTANCES!
 */
class DGCentralWidget : public QWidget
{
	Q_OBJECT

	friend class DGController;
	friend class DGWindow;
public:
	explicit DGCentralWidget(DGController* ctrl, QWidget* parent = 0);

	inline CodeEditorWidget* getEditor() {return textEditor;}
	const std::array<const char*,10> BUTTON_LOWER_NAMES = {
		"Setup",
		"Update",
		"Build",
		"Rebuild",
		"Run",
		"Run File",
		"Test",
		"Debug",
		"Analyze",
		"Deploy"
	};
	enum ButtonIdLower {
		SETUP = 0,
		UPDATE,
		BUILD,
		REBUILD,
		RUN,
		RUNFILE,
		TEST,
		DEBUG,
		ANALYZE,
		DEPLOY
	};

public slots:
	void updateProjectList();
	void shrinkProjectList();

private:
	void createWidgets();
	void createLayout();
	void setupConnections();

private slots:
	void resizeDirView();
	void changeProject(int index);
	void changeFile(const QModelIndex& val);
	void setHiddenLeft(bool hide);

private:
	// Views
	QTreeView* projectDirView;

	// Models
	QFileSystemModel* projectDirModel;

	// Widgets
	QComboBox* auxComboBox;
	CodeEditorWidget* textEditor;
	QComboBox* projectComboBox;
	QListWidget* auxPane; // Just a placeholder till custom widget is created.
	QHBoxLayout *bottomBar, *mainLayout;
	QVBoxLayout *splitViewPane, *leftSideLayout, *editorLayout;
	QPushButton* bottomButton;

	//WARNING: Arrangement of elements in this array is CRITICAL!
	std::unordered_map<ButtonIdLower,QPushButton*> buttonsLower;
	std::map<QString,QPushButton*> buttonsSide;

	//Controller ptr (NO OWNERSHIP)
	DGController* ctrl;

	QPushButton* makeButton(const QString& txt, int width = 32, int height = 32);
};

#endif // DGCENTRALWIDGET_HPP
