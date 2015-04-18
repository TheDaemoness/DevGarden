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

class DGController;

class DGCentralWidget : public QWidget
{
	Q_OBJECT

	public:
		explicit DGCentralWidget(DGController* ctrl, QWidget* parent = 0);

	private:
		void createWidgets();
		void createLayout();
		void setupConnections();

	private slots:
		void resizeDirView();
		void changeProject(const QString& str);

	private:
		// Views
		QTreeView* projectDirView;

		// Models
		QFileSystemModel* projectDirModel;

		// Widgets
		QComboBox* auxComboBox;
		QComboBox* projectComboBox;
		QListWidget* auxPane; // Just a placeholder till custom widget is created.
		QPlainTextEdit* textEditor;
		QLineEdit* bottomBar; // Placeholder
		QPlainTextEdit* splitViewPane; // Placeholder
		QPushButton* bottomButton;

		//Controller ptr (NO OWNERSHIP)
		DGController* ctrl;
};

#endif // DGCENTRALWIDGET_HPP

