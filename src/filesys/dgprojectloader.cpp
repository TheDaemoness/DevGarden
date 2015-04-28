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
	delete *current;
	current = projs.erase(current);
	return true;
}

bool DGProjectLoader::closeOthers() {
	if(projs.size() <= 1)
		return false;
	DGProjectInfo* temp = *current;
	projs.erase(current);
	for(auto e : projs)
		delete e;
	projs.resize(1);
	projs.at(0) = temp;
	current = projs.begin();
	return true;
}

bool DGProjectLoader::closeAll() {
	if(projs.empty())
		return false;
	for(auto e : projs)
		delete e;
	projs.clear();
	return true;
}

DGProjectInfo* DGProjectLoader::getCurrent() {
	return (projs.empty()?nullptr:*current);
}

bool DGProjectLoader::addFile(QString path) {
	QFileInfo* f = new QFileInfo(path);
	if(!f->isReadable() || !f->isWritable()) {
		delete f;
		return false;
	}
	projs.push_back(new DGProjectInfo(f));
	current = projs.end()-1;
	return true;
}

bool DGProjectLoader::addFolder(QString path) {
	QDir* f = new QDir(path);
	if(!f->exists()) {
		delete f;
		return false;
	}
	projs.push_back(new DGProjectInfo(f));
	current = projs.end()-1;
	return true;
}

QStringList DGProjectLoader::getProjectNames() {
	QStringList l;
	l.reserve(projs.size());
	for(DGProjectInfo* proj : projs)
		l.append(proj->getName());
	return l;
}
