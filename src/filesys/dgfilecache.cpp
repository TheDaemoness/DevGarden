#include "dgfilecache.h"

#include "../dgcontroller.h"

#include <QString>
#include <QFileDialog>

void SlotMachine::onLostFile(const QString& path) {
	fc.onLostFile(path);
};

DGFileCache::DGFileCache(const LangRegistry& langs) : lr(langs), slotter(*this) {
	data.emplace("",FileData());
	current = data.begin();
	ctrl = nullptr;
	trash.reserve(1);
}
void DGFileCache::onLostFile(const QString& name) {
	QFileInfo fi(name);
	const auto i = data.find(fi.absoluteFilePath());
	if(i != data.end())
		i->second.closeLoader();
}

QTextDocument* DGFileCache::set(const QFileInfo& fi) {
	auto it = data.find(fi.absoluteFilePath());
	auto old = current;
	--(old->second);
	if(it != data.end()) {
		current = it;
		++(current->second);
	} else {
		if(fi.exists() && fi.isReadable()) {
			current = data.emplace(fi.absoluteFilePath(),FileData()).first;
			current->second.setFileLoader(FileLoader::create(fi.absoluteFilePath()));
			current->second.setLang(&lr.getLang(fi));
			current->second.load();
		} else
			current = data.emplace("",FileData()).first;
	}
	tryClose(old);
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
		current->second.setLang(&lr.getLang(f));
		current->second.save();
		tryClose(old);
		return true;
	}
	return !current->second.hasLoader();
}
bool DGFileCache::saveOthers() {
	bool saved_all = true;
	for(auto it = data.begin(), e = data.end(); it!=e; ++it) {
		if(it != current) {
			saved_all &= it->second.save();
			tryClose(it);
		}
	}
	return saved_all;
}

void DGFileCache::reloadCurrent() {
	current->second.load();
}
void DGFileCache::reloadOthers() {
	bool saved_all = true;
	for(auto it = data.begin(), e = data.end(); it!=e; ++it) {
		if(it != current) {
			saved_all &= it->second.load();
			tryClose(it);
		}
	}
}

void DGFileCache::delinkCurrent() {
	current->second.closeLoader();
}
bool DGFileCache::tryClose(const decltype(current)& it) {
	if(it->second.shouldAutoClose()) {
		trash.emplace_back(it->second.releaseDocument());
		data.erase(it);
	}
}
