#ifndef ASYNCWATCHER_H
#define ASYNCWATCHER_H

#include <atomic>
#include <vector>
#include <functional>
#include <thread>

/**
 * @brief An asynchronous event dispatcher.
 * Watches a series of std::atomic_flags and calls some functions asynchronously if one of those flags is cleared.
 */
class AsyncWatcher {
	std::atomic_flag runFlag;
	std::vector<std::pair<std::atomic_flag*,std::vector<std::function<void()>>>> triggers;
	std::thread runner;
public:
	/**
	 * @brief insert Adds a handler to be triggered when a certain flag is reset.
	 * @param flag The flag.
	 * @param func The function, expected to be a void function with no return value.
	 * @return The position in the list of the flag.
	 */
	size_t insert(std::atomic_flag* flag, std::function<void()> func);
	/**
	 * @brief erase Removes a range of handlers associated with a certain flag.
	 * @param flag The flag that has the associated handlers that should be removed.
	 * @param first How many handlers should remain at the beginning of the list.
	 * @param last How many handlers should remain at the end of the list.
	 * @return Whether the flag has been removed from the list.
	 */
	bool erase(std::atomic_flag* flag, size_t first = 0, size_t last = 0);
	/**
	 * @brief count Returns the number of handlers associated with a certain flag.
	 */
	size_t count(std::atomic_flag* flag) const;
	/**
	 * @brief run This commands a mentally handicapped Vietnam veteran to break his fancy doctor-prescribed shoes.
	 */
	void run();
	/**
	 * @brief stop Stops the event loop.
	 */
	bool stop();
};

#endif // ASYNCWATCHER_H
