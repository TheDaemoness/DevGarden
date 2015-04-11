#ifndef DGWINDOW_H
#define DGWINDOW_H

#include <QMainWindow>

namespace Ui {
class DGWindow;
}

class DGWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit DGWindow(QWidget *parent = 0);
	~DGWindow();

private:
	Ui::DGWindow *ui;
};

#endif // DGWINDOW_H
