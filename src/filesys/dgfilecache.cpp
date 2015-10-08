#include "dgfilecache.h"

#include "../dgcontroller.h"

#include <QString>
#include <QFileDialog>

#define SEAR

DGFileCache::DGFileCache() : slotter(*this) {
	data.emplace("",std::move(FileData()));
	current = data.begin();
	ctrl = nullptr;
}
void DGFileCache::onLostFile(const QString& name) {
	const auto i = data.find(name);
	if(i != data.end())
		i->second.closeLoader();
}
void SlotMachine::onLostFile(const QString& path) {
	fc.onLostFile(path);
};

QTextDocument* DGFileCache::set(const QString& path) {
	auto it = data.find(path);
	auto old = current;
	--(old->second);
	if(it != data.end()) {
		current = it;
		++(current->second);
		if(old->second.shouldAutoClose())
			data.erase(old);
	}
	return current->second.getDocument();
}

void DGFileCache::saveCurrent() {
	if(current->second.hasLoader())
		current->second.save();
	else {
		QFileInfo f(ctrl->getFileSaveName());
		if(f.exists()) {
			FileLoader* load = FileLoader::create(f);
			current->second.setFileLoader(load);
			current->second.save();
			auto old = current;
			current = data.emplace(f.absoluteFilePath(),FileData(std::move(current->second))).first;
			data.erase(old);
		} else {
			/*Report issue*/
		}
	}
}

void DGFileCache::reloadCurrent() {
	if(current->second.hasLoader())
		current->second.load();
}

void DGFileCache::delinkCurrent() {
	current->second.closeLoader();
}
