#ifndef AUXPANEHANDLER_H
#define AUXPANEHANDLER_H

#include <functional>
#include <vector>

class AuxPaneHandler {
protected:
	std::vector<std::function<void()>> triggers;
public:
	AuxPaneHandler();
};

#endif // AUXPANEHANDLER_H
