#include "dgfilecache.h"

#include "../dgcontroller.h"

#include <QString>
#include <QFileDialog>

DGFileCache::DGFileCache(const LangRegistry& langs) : lr(langs) {
	data.emplace("",FileData());
	current = data.begin();
	ctrl = nullptr;
	trash.reserve(1);
}

bool DGFileCache::isCurrSaveable() {
	return current->second.getLoader() && !current->second.getLoader()->isInvalid();
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

			auto fl = FileLoader::create(fi.absoluteFilePath());
			fl->setStateUpdateCallback(std::bind(&DGFileCache::onLoaderUpdate,this,current));
			current->second.setFileLoader(fl);

			current->second.setLang(&lr.getLang(fi));
			current->second.load();
		} else
			current = data.emplace("",FileData()).first;
	}
	tryClose(old);
	return current->second.getDocument();
}

bool DGFileCache::saveCurrent() {
	if(current->second.getLoader())
		current->second.save();
	else {
		QString fn = ctrl->getFileSaveName();
		if(fn.isEmpty())
			return false;
		QFileInfo f(fn);
		auto old = current;
		current = data.emplace(f.absoluteFilePath(),FileData(std::move(current->second))).first;

		auto fl = FileLoader::create(f);
		fl->setStateUpdateCallback(std::bind(&DGFileCache::onLoaderUpdate,this,current));
		current->second.setFileLoader(fl);

		current->second.setLang(&lr.getLang(f));
		current->second.save();
		tryClose(old);
		return true;
	}
	return !current->second.getLoader();
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
		return true;
	}
	return false;
}

QString DGFileCache::getCurrStatus() {
	if(current->second.getLoader())
		return current->second.getLoader()->getStatusString();
	return "";
}

void DGFileCache::onLoaderUpdate(std::map<QString,FileData>::iterator it) {
	if(it->second.getLoader()) {
		if(it->second.getLoader()->isInvalid())
			it->second.closeLoader();
	}
	ctrl->onFileCacheUpdate();
}
