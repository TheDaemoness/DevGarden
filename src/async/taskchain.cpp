#include "taskchain.h"

TaskChain::Callback TaskChain::wrap(std::function<void()> call) {
	return [=](const std::atomic_bool&)->bool{call(); return true;};
}
TaskChain::Callback TaskChain::wrap(std::function<bool()> call) {
	return [=](const std::atomic_bool&)->bool{return call();};
}

bool TaskChain::run(const std::atomic_bool& b) {
	return task_(b);
}

size_t TaskChain::length() const {
	size_t ret = 0;
	for(TaskChain* next = pass_.get(); next != nullptr; next = next->pass_.get())
		++ret;
	return ret;
}

