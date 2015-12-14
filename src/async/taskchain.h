#ifndef TASKCHAIN_H
#define TASKCHAIN_H

#include <memory>
#include <functional>

#include <QString>

class TaskChain {
public:
	using Callback = std::function<bool(const std::atomic_bool&)>;
	static Callback wrap(std::function<void()> call);
	static Callback wrap(std::function<bool()> call);
private:
	std::unique_ptr<TaskChain> pass_, fail_;
	QString name_;
protected:
	Callback task_;
public:
	TaskChain(const QString& name, Callback task) :
		task_(task), pass_(nullptr), fail_(nullptr), name_(name) {};
	TaskChain(const QString& name, Callback task, TaskChain* pass, TaskChain* fail) :
		task_(task), pass_(pass), fail_(fail), name_(name) {};
	TaskChain(const QString& name, Callback task, TaskChain* next, bool onPass = true) :
		task_(task), name_(name)
		{(onPass?pass_:fail_).reset(next);}

	bool run(const std::atomic_bool& stop);
	size_t length() const;

	void setNext(TaskChain* next, bool pass = true)
		{(pass?pass_:fail_).reset(next);}
	TaskChain* next(bool flag) const
		{return (flag?pass_:fail_).get();}
	QString getName() const
		{return name_;}
};

#endif // TASKCHAIN_H
