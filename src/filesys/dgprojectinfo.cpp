#include "dgprojectinfo.h"

#include <QFile>
#include <QFileSystemModel>

DGProjectInfo::DGProjectInfo(QFile* f) : isdir(false) {
	content.file = f;
}

DGProjectInfo::DGProjectInfo(QFileSystemModel* f) : isdir(true) {
	content.dir = f;
}

DGProjectInfo::~DGProjectInfo() {
	if(isdir)
		delete content.dir;
	else
		delete content.file;
}
