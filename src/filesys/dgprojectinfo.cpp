#include "dgprojectinfo.h"

#include <QFile>
#include <QDir>
#include <QString>

bool DGProjectInfo::operator<(const DGProjectInfo& b) const{
	if(this->isdir != b.isdir)
		return this->isdir;
	return this->getName() < b.getName();
}

DGProjectInfo::DGProjectInfo(QFileInfo* f) : isdir(false) {
	file = f;
	dir = nullptr;
}

DGProjectInfo::DGProjectInfo(QDir* f) : isdir(true) {
	dir = f;
	file = nullptr;
}

DGProjectInfo::~DGProjectInfo() {
	if(isdir)
		delete dir;
	else
		delete file;
}

QString DGProjectInfo::getName() const {
	return isdir?dir->dirName():file->fileName();
}
