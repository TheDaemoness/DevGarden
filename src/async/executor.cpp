#include "executor.h"

void Executor::operator()() {
	if(!running_.load(std::memory_order_consume))
		curr_.reset();
}
Executor& Executor::operator()(TaskChain* tc) {
	if(!running_.load(std::memory_order_consume))
		curr_.reset(tc);
	return *this;
}

void Executor::run() {
	if(!running_.load(std::memory_order_consume)) {
		running_.store(true,std::memory_order_acquire);
		runner_ = std::thread([this](){
			(curr_->run()?pass_:fail_).clear();
			running_.store(false,std::memory_order_acquire);
		});
	}
}

