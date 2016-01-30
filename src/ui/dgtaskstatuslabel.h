#ifndef DGTASKSTATUSLABEL_H
#define DGTASKSTATUSLABEL_H

#include <QStatusBar>
#include <QLabel>
#include <QLineEdit>

#include "../async/executor.h"

class DGTaskStatusLabel : public QLabel {
	Q_OBJECT
	Executor* exe_;
	int working_, total_;
public:
	explicit DGTaskStatusLabel(Executor* exe);
public slots:
	void onStartTasks();
	void onNextTask();
	void onTasksDone(bool passed = true);
	void onTasksStop();
};

#endif // DGTASKSTATUSLABEL_H
