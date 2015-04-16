#include "dgcontroller.h"
#include <QFileDialog>
#include <QWindow>

DGController::DGController(QObject *parent) :
	QObject(parent) {
}

void DGController::openFolder() {
	QFileDialog dg;
	dg.setWindowTitle(tr("Open Folder"));
	dg.setFileMode(QFileDialog::Directory);
	dg.setViewMode(QFileDialog::List);
	dg.setOption(QFileDialog::ShowDirsOnly, true);
	if(dg.exec())
		QStringList names = dg.selectedFiles();
}
void DGController::openFiles() {
	QFileDialog dg;
	dg.setWindowTitle(tr("Open Files"));
	dg.setFileMode(QFileDialog::ExistingFiles);
	dg.setViewMode(QFileDialog::Detail);
	if(dg.exec())
		QStringList names = dg.selectedFiles();
}
void DGController::saveFileAs() {
	QString name = QFileDialog::getSaveFileName(0,tr("Save As..."),"~","",0,0);
}
