#ifndef DGPROJECTLOADER_H
#define DGPROJECTLOADER_H

#include "dgprojectinfo.h"

#include <vector>
#include <memory>

class DGProjectLoader {
public:
	DGProjectLoader();
private:
	std::vector<std::unique_ptr<DGProjectInfo>> projs;
};

#endif // DGPROJECTLOADER_H
