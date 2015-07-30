#include "dgprojectinfo.h"

#include "../configloader.h"

#include <QFile>
#include <QFileInfo>
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
	bsys_choice = nullptr;
	bsys_custom = nullptr;
	catalog(false);
	QFileInfo* info = new QFileInfo(f->absoluteFilePath(".dgproject"));
	if(!info->isReadable()) {
		delete info;
		return;
	}
	file = info;
	QFile pjf(file->absoluteFilePath());
	pjf.open(QFile::ReadOnly);
	ConfigFile cf(pjf);
	//TODO: Put something in those config files.
}

DGProjectInfo::~DGProjectInfo() {
	if(dir)
		delete dir;
	delete file;
}

QString DGProjectInfo::getName() const {
	return dir?dir->dirName():file->fileName();
}

inline void DGProjectInfo::catalog(bool recursive) {
	catalog(*dir,recursive);
}

void DGProjectInfo::catalog(const QDir& dir, bool recursive) {
	for(const QFileInfo& entry : dir.entryInfoList()) {
		if(recursive && entry.isDir())
			catalog(entry.absoluteDir(),true);
		else {
			//Process file.
		}
	}
}
