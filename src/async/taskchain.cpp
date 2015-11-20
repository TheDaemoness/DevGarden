#include "taskchain.h"

TaskChain::Callback TaskChain::wrap(std::function<void()> call) {
	return [=]()->bool{call(); return true;};
}

bool TaskChain::run() {
	const bool winrar = task_();
	TaskChain* next = winrar?pass_.get():fail_.get();
	return next?next->run():winrar;
}

