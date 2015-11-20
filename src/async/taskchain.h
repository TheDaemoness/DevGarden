#ifndef TASKCHAIN_H
#define TASKCHAIN_H

#include <memory>
#include <functional>

class TaskChain {
public:
	using Callback = std::function<bool()>;
	static Callback wrap(std::function<void()> call);
private:
	std::unique_ptr<TaskChain> pass_, fail_;
	std::atomic_flag stop_;
protected:
	Callback task_;
	bool shouldStop() {
		bool stahp = stop_.test_and_set(std::memory_order_consume);
		stop_.clear(std::memory_order_acquire);
		return stahp;
	}

public:
	TaskChain(Callback task) :
		task_(task), pass_(nullptr), fail_(nullptr) {};
	TaskChain(Callback task, Callback pass, Callback fail) :
		task_(task), pass_(new TaskChain(pass)), fail_(new TaskChain(fail)) {};
	TaskChain(Callback task, Callback next, bool onPass = true) : task_(task) {
		(onPass?pass_:fail_).reset(new TaskChain(next));
	}

	bool run();
	void abort() {stop_.test_and_set(std::memory_order_acquire);}
};

#endif // TASKCHAIN_H
