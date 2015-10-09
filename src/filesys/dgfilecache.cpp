#include "dgfilecache.h"

#include "../dgcontroller.h"

#include <QString>
#include <QFileDialog>

#define SEAR

DGFileCache::DGFileCache() : slotter(*this) {
	data.emplace("",FileData());
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
	} else {
		QFileInfo fi(path);
		if(fi.exists()) {
			current = data.emplace(fi.absoluteFilePath(),FileData()).first;
			current->second.setFileLoader(FileLoader::create(path));
			current->second.load();
		} else
			current = data.emplace("",std::move(FileData())).first;
	}
	if(old->second.shouldAutoClose())
		data.erase(old);
	return current->second.getDocument();
}

bool DGFileCache::saveCurrent() {
	if(current->second.hasLoader())
		current->second.save();
	else {
		QFileInfo f(ctrl->getFileSaveName());
		auto old = current;
		current = data.emplace(f.absoluteFilePath(),FileData(std::move(current->second))).first;
		current->second.setFileLoader(FileLoader::create(f));
		current->second.save();
		data.erase(old);
		return true;
	}
	return !current->second.hasLoader();
}

void DGFileCache::reloadCurrent() {
	if(current->second.hasLoader())
		current->second.load();
}

void DGFileCache::delinkCurrent() {
	current->second.closeLoader();
}
