#include "dgwindow.h"
#include "ui_dgwindow.h"

DGWindow::DGWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::DGWindow)
{
	ui->setupUi(this);
}

DGWindow::~DGWindow()
{
	delete ui;
}
