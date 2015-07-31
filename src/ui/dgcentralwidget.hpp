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

/**
 * @brief The actual widgets of the main window.
 */
class DGCentralWidget : public QWidget
{
	Q_OBJECT

	friend class DGController;
public:
	explicit DGCentralWidget(DGController* ctrl, QWidget* parent = 0);

	inline CodeEditorWidget* getEditor() {return textEditor;}

	enum ButtonIdLower {
		SETUP = 0,
		REGEN,
		BUILD,
		REBULD,
		RUN,
		RUNFILE,
		TEST,
		DEBUG,
		ANALYZE
	};
	const static size_t BUTTON_LOWER_COUNT = 9;

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
	QVBoxLayout *splitViewPane, *leftSideLayout;
	QPushButton* bottomButton;

	//WARNING: Arrangement of elements in this array is CRITICAL!
	std::vector<QPushButton*> buttonsLower;
	std::map<QString,QPushButton*> buttonsSide;

	//Controller ptr (NO OWNERSHIP)
	DGController* ctrl;

	QPushButton* makeButton(const QString& txt, int width = 32, int height = 32);
};

#endif // DGCENTRALWIDGET_HPP
