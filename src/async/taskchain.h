#ifndef TASKCHAIN_H
#define TASKCHAIN_H

#include <memory>
#include <functional>

class TaskChain {
public:
	using Callback = std::function<bool()>;
	static Callback wrap(std::function<void()> call);
private:
	Callback task_;
	std::unique_ptr<TaskChain> pass_, fail_;
public:
	TaskChain(Callback task) :
		task_(task), pass_(nullptr), fail_(nullptr) {};
	TaskChain(Callback task, Callback pass, Callback fail) :
		task_(task), pass_(new TaskChain(pass)), fail_(new TaskChain(fail)) {};
	TaskChain(Callback task, Callback next, bool onPass = true) : task_(task) {
		(onPass?pass_:fail_).reset(new TaskChain(next));
	}

	bool run() const;
};

#endif // TASKCHAIN_H
