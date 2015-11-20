#ifndef DGTASKSTATUSLABEL_H
#define DGTASKSTATUSLABEL_H

#include <QStatusBar>
#include <QLabel>
#include <QLineEdit>

class DGTaskStatusLabel : public QLabel {
	Q_OBJECT
	int working_, total_;
public:
	void onStartTasks(int count);
	void onTaskPlanB(int len, const QString& name);
	void onNextTask(const QString& name);
	void onTasksDone(bool passed = true);
	void onTasksStop();
	DGTaskStatusLabel();
};

#endif // DGTASKSTATUSLABEL_H
