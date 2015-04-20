#include "dgprojectinfo.h"

#include <QFile>
#include <QDir>
#include <QString>

bool DGProjectInfo::operator<(const DGProjectInfo& b) const{
	if(this->isdir != b.isdir)
		return this->isdir;
	return this->getName() < b.getName();
}

DGProjectInfo::DGProjectInfo(QFile* f) : isdir(false) {
	content.file = f;
}

DGProjectInfo::DGProjectInfo(QDir* f) : isdir(true) {
	content.dir = f;
}

DGProjectInfo::~DGProjectInfo() {
	if(isdir)
		delete content.dir;
	else {
		if(content.file->isOpen())
			content.file->close();
		delete content.file;
	}
}

QString DGProjectInfo::getName() const {
	return isdir?content.dir->dirName():content.file->fileName();
}
