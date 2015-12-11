#include "executor.h"


TaskChain* Executor::run(TaskChain* tasks) {
	const bool demons = !running_.load(std::memory_order_consume);
	if(demons && !curr_.get()) {
		curr_.reset(tasks);
		running_.store(true,std::memory_order_acquire);
		stop_.store(false,std::memory_order_acquire);
		progress_ = 1;
		total_ = curr_->length()+1;
		started_.clear();
		runner_ = std::thread([this](){
			const bool flag = curr_->run(stop_);
			std::lock_guard<std::mutex> lg(m_);
			if(stop_.load(std::memory_order_consume)) {
				curr_.reset();
				progress_ = 0;
				total_ = 0;
				stopped_.clear();
				running_.store(false,std::memory_order_acquire);
			}
			else if(!curr_->next(flag)) {
				curr_.reset();
				progress_ = 0;
				total_ = 0;
				(flag?pass_:fail_).clear();
				running_.store(false,std::memory_order_acquire);
			} else {
				curr_.reset(curr_->next(flag));
				++progress_;
				if(!flag)
					total_ = progress_+curr_->length();
				next_.clear();
			}
		});
		return nullptr;
	}
	return tasks;
}

void Executor::stop() {
	const bool run = running_.load(std::memory_order_consume);
	if(run)
		stop_.store(true,std::memory_order_acquire);
}
QString Executor::getTaskProgress(std::pair<size_t,size_t>& state) {
	std::lock_guard<std::mutex> lg(m_);
	state = std::make_pair(progress_,total_);
	if(curr_.get())
		return curr_->getName();
	return QString();
}

