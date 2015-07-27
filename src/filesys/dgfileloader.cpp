#include "dgfileloader.h"

#include <QString>

DGFileLoader::DGFileLoader() {
	current.push_back({QFileInfo(""),nullptr,true});
}
