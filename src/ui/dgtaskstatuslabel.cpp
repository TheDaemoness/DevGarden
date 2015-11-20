#include "DGTaskStatusLabel.h"

DGTaskStatusLabel::DGTaskStatusLabel()
{
	this->setText("Ready");
	this->setEnabled(false);
}
void DGTaskStatusLabel::onStartTasks(int count) {
	this->setEnabled(true);
	working_ = 0;
	total_ = count;
	this->setText(QString::number(working_)+'/'+QString::number(total_));
}

void DGTaskStatusLabel::onTaskPlanB(int len, const QString& name) {
	total_ = working_+(len-1);
	this->setText(QString::number(working_)+'/'+QString::number(total_)+" - "+tr(name.toLocal8Bit()));
}

void DGTaskStatusLabel::onNextTask(const QString& name) {
	++working_;
	this->setText(QString::number(working_)+'/'+QString::number(total_)+" - "+tr(name.toLocal8Bit()));
}

void DGTaskStatusLabel::onTasksDone(bool passed) {
	this->setText(passed?tr("Done"):tr("Failed"));
	this->setEnabled(false);
}

void DGTaskStatusLabel::onTasksStop() {
	this->setText("Aborted");
	this->setEnabled(false);
}

