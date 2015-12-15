#include "dgtaskstatuslabel.h"

DGTaskStatusLabel::DGTaskStatusLabel(Executor* exe) : exe_(exe)
{
	this->setText("Ready");
	this->setEnabled(false);
}
void DGTaskStatusLabel::onStartTasks() {
	this->setEnabled(true);
	std::pair<size_t,size_t> status;
	QString taskname = exe_->getTaskProgress(status);
	this->setText(QString::number(status.first)+'/'+
		QString::number(status.second)+
		" - "+tr(taskname.toLocal8Bit()));
}

void DGTaskStatusLabel::onNextTask() {
	if(!this->isEnabled())
		return;
	std::pair<size_t,size_t> status;
	QString taskname = exe_->getTaskProgress(status);
	this->setText(QString::number(status.first)+'/'+
		QString::number(status.second)+
		" - "+tr(taskname.toLocal8Bit()));
}

void DGTaskStatusLabel::onTasksDone(bool passed) {
	this->setEnabled(false);
	this->setText(passed?tr("Done"):tr("Failed"));
}

void DGTaskStatusLabel::onTasksStop() {
	this->setEnabled(false);
	this->setText("Aborted");
}

