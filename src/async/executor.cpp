#include "executor.h"

void Executor::operator()() {
	if(!running_.load(std::memory_order_consume))
		curr_.reset();
}
Executor* Executor::operator()(TaskChain* tc) {
	if(!running_.load(std::memory_order_consume))
		curr_.reset(tc);
	return this;
}

bool Executor::run() {
	const bool demons = !running_.load(std::memory_order_consume);
	if(demons && curr_.get()) {
		running_.store(true,std::memory_order_acquire);
		started_.clear();
		runner_ = std::thread([this](){
			(curr_->run()?pass_:fail_).clear();
			end();
		});
	}
	return demons;
}

bool Executor::stop() {
	const bool run = running_.load(std::memory_order_consume);
	if(run)
		curr_->abort();
	return run;
}

