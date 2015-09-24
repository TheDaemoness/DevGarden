#include "dgfilecache.h"

#include <QString>

DGFileCache::DGFileCache() : slotter(*this) {
	//current.push_back({QFileInfo(""),nullptr,true});
}
DGFileCache::~DGFileCache() {
	for(auto i = data.begin(), e = data.end(); i != e; ++i) {
		if(!i->second.saved && i->second.getLoader() != nullptr)
			i->second.getLoader()->save(*i->second.getDocument());
	}
}
void DGFileCache::onLostFile(const QString& name) {
	const auto i = data.find(name);
	if(i != data.end())
		i->second.lost = true;
}
void SlotMachine::onLostFile(const QString& path) {
	fc.onLostFile(path);
};
