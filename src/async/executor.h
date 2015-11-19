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
	std::mutex runblock_;
	std::unique_ptr<TaskChain> curr_;
	std::atomic_flag pass_, fail_;
	std::thread runner_;
	std::atomic_bool running_;
public:
	void operator()();
	Executor& operator()(TaskChain*);
	void run();

	bool isRunning() {return running_.load(std::memory_order_consume);}
};

#endif // EXECUTOR_H
