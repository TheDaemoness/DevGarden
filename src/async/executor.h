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
	std::unique_ptr<TaskChain> curr_;
	std::atomic_flag pass_, fail_, started_, stopped_;
	std::thread runner_;
	std::atomic_bool running_;
	void end() {
		running_.store(false,std::memory_order_acquire);
		stopped_.clear();
	}

public:
	void operator()();
	Executor* operator()(TaskChain*);
	bool run();
	bool stop();

	bool isRunning() {return running_.load(std::memory_order_consume);}
};

#endif // EXECUTOR_H
