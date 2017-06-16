#include "dgprojectloader.h"

#include <QFile>
#include <QDir>
#include <QStringList>

bool DGProjectLoader::changeCurrent(size_t index) {
	if(index >= projs.size())
		return false;
	current = projs.begin() + index;
	return true;
}
bool DGProjectLoader::closeCurrent() {
	if(projs.empty())
		return false;
	current->reset();
	current = projs.erase(current);
	return true;
}

bool DGProjectLoader::closeOthers() {
	if(projs.size() <= 1)
		return false;
    std::unique_ptr<DGProjectInfo> temp{std::move(*current)};
	projs.clear();
    projs.emplace_back(std::move(temp));
	current = projs.begin();
	return true;
}

bool DGProjectLoader::closeAll() {
	if(projs.empty())
		return false;
	projs.clear();
	return true;
}

DGProjectInfo* DGProjectLoader::getCurrent() const {
	return (projs.empty()?nullptr:current->get());
}

bool DGProjectLoader::addFile(const QString& path) {
	QFileInfo* f = new QFileInfo(path);
	if(!f->isReadable() || !f->isWritable()) {
		delete f;
		return false;
	}
	projs.emplace_back(new DGProjectInfo(f));
	current = projs.end()-1;
	return true;
}

bool DGProjectLoader::addFolder(const QString& path) {
	QDir* f = new QDir(path);
	if(!f->exists()) {
		delete f;
		return false;
	}
	projs.emplace_back(new DGProjectInfo(f, langreg));
	current = projs.end()-1;
	return true;
}

QStringList DGProjectLoader::getProjectNames() const {
	QStringList l;
	l.reserve(projs.size());
	for(const std::unique_ptr<DGProjectInfo>& proj : projs)
		l.append(proj->getName());
	return l;
}
