#include "dgprojectinfo.h"

#include <QFile>
#include <QDir>
#include <QString>

bool DGProjectInfo::operator<(const DGProjectInfo& b) const{
	if(!dir != !b.dir)
		return !dir;
	return this->getName() < b.getName();
}

DGProjectInfo::DGProjectInfo(QFileInfo* f) {
	file = f;
	dir = nullptr;
}

DGProjectInfo::DGProjectInfo(QDir* f) {
	dir = f;
	file = new QFileInfo(f->absoluteFilePath(".dgproject"));
}

DGProjectInfo::~DGProjectInfo() {
	if(dir)
		delete dir;
	delete file;
}

QString DGProjectInfo::getName() const {
	return dir?dir->dirName():file->fileName();
}
