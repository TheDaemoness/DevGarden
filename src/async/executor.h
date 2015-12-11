#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <atomic>
#include <mutex>
#include <thread>

#include "taskchain.h"

/**
 * @brief Single-threaded TaskChain executor.
 * Includes atomic flags to indicate general state.
 */
class Executor {
private:
	std::unique_ptr<TaskChain> curr_;
	std::atomic_flag pass_, fail_, started_, stopped_, next_;
	std::thread runner_;
	std::atomic_bool running_, stop_;

	std::mutex m_;
	size_t progress_, total_;

public:
	TaskChain* run(TaskChain*);
	void stop();
	bool isRunning() {return running_.load(std::memory_order_consume);}
	QString getTaskProgress(std::pair<size_t,size_t>& prog);
};

#endif // EXECUTOR_H
