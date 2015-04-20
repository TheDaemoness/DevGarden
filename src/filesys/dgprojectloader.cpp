#include "dgprojectloader.h"

#include <QFile>
#include <QDir>
#include <QStringList>

#include "../dgdebug.hpp"

bool DGProjectLoader::changeCurrent(size_t index) {
	if(index >= projs.size())
		return false;
	current = projs.begin() + index;
	return true;
}
void DGProjectLoader::closeCurrent() {

}

void DGProjectLoader::closeOthers() {
	DGProjectInfo* temp = *current;
	projs.erase(current);
	for(auto e : projs)
		delete e;
	projs.resize(1);
	projs.at(0) = temp;
}

void DGProjectLoader::closeAll() {
	for(auto e : projs)
		delete e;
	projs.clear();
}

DGProjectInfo* DGProjectLoader::getCurrent() {
	return *current;
}

bool DGProjectLoader::addFile(QString path) {
	QFile* f = new QFile(path);
	f->open(QFile::ReadWrite);
	if(!f->isOpen()) {
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
