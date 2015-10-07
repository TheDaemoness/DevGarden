#include "asyncwatcher.h"

size_t AsyncWatcher::insert(std::atomic_flag* flag, std::function<void()> func) {
	bool stahp = stop();
	auto it = std::find_if(triggers.begin(),triggers.end(),
		[flag](const decltype(*triggers.begin())& v)->bool {return v.first == flag;});
	size_t retval;
	if(it != triggers.end()) {
		it->second.push_back(func);
		retval = std::distance(triggers.begin(),it);
	}
	else {
		retval = triggers.size();
		triggers.emplace_back(flag,std::vector<decltype(func)>({func}));
		flag->test_and_set();
	}
	if(stahp)
		run();
	return retval;
}

bool AsyncWatcher::erase(std::atomic_flag* flag, size_t first, size_t last) {
	bool stahp = stop();
	auto it = std::find_if(triggers.begin(),triggers.end(),
		[flag](const decltype(*triggers.begin())& v)->bool {return v.first == flag;});
	if(it != triggers.end()) {
		first = first>it->second.size()?it->second.size():first;
		last = last>it->second.size()?it->second.size():last;
		it->second.erase(it->second.begin()+first,it->second.end()-last);
		bool cleared = false;
		if(it->second.empty()) {
			triggers.erase(it);
			cleared = true;
		}
		if(stahp)
			run();
		return cleared;
	} else if(stahp)
		run();
	return true;
}

size_t AsyncWatcher::count(std::atomic_flag* flag) const {
	auto it = std::find_if(triggers.begin(),triggers.end(),
		[flag](const decltype(*triggers.begin())& v)->bool {return v.first == flag;});
	if(it != triggers.end())
		return it->second.size();
	return 0;

}
void AsyncWatcher::run() {
	runFlag.test_and_set();
	runner = std::move(std::thread([this]{
		do {
			for(auto& elem : triggers) {
				if(!elem.first->test_and_set()) {
					for(auto& e : elem.second)
						e();
					std::this_thread::yield();
				}
			}
			std::this_thread::yield();
		} while(runFlag.test_and_set());
	}));
}

bool AsyncWatcher::stop() {
	bool retval = runFlag.test_and_set();
	runFlag.clear();
	if(retval && runner.joinable())
		runner.join();
	return retval;
}
