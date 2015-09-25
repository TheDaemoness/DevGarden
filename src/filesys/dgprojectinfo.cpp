#include "dgprojectinfo.h"

#include "../langregistry.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QString>

#include "../dgdebug.hpp"

bool DGProjectInfo::operator<(const DGProjectInfo& b) const{
	if(!dir != !b.dir)
		return !dir;
	return this->getName() < b.getName();
}

DGProjectInfo::DGProjectInfo(QFileInfo* f) {
	file = f;
	dir = nullptr;
}

DGProjectInfo::DGProjectInfo(QDir* f, const LangRegistry& lr) {
	dir = f;
	bsys_choice = nullptr;
	bsys_custom = nullptr;
	catalog(lr, false);
	QFileInfo* info = new QFileInfo(f->absoluteFilePath(".dgproject"));
	if(!info->isReadable()) {
		delete info;
		return;
	}
	file = info;
	QFile pjf(file->absoluteFilePath());
	pjf.open(QFile::ReadOnly);
	//ConfigFile cf(pjf);
	//TODO: Put something in those config files.
	pjf.close();
}

DGProjectInfo::~DGProjectInfo() {
	if(dir)
		delete dir;
	if(file)
		delete file;
}

QString DGProjectInfo::getName() const {
	return dir?dir->dirName():file->fileName();
}

inline void DGProjectInfo::catalog(const LangRegistry& lr, bool recursive) {
	catalog(lr,*dir,recursive);
}

void DGProjectInfo::catalog(const LangRegistry& lr, const QDir& dir, bool recursive) {
	for(const QFileInfo& entry : dir.entryInfoList()) {
		if(recursive && entry.isDir())
			catalog(lr,entry.absoluteDir(),true);
		else {
			if(!recursive) {
				bool bs = false;
				if(lr.knowsFile(entry.fileName(),false))
					bs |= lr.isBuildSys(lr.getLang(entry.fileName(),false));
				if(lr.knowsFile(LangRegistry::getFileExt(entry.fileName())))
					bs |= lr.isBuildSys(lr.getLang(LangRegistry::getFileExt(entry.fileName())));
				if(bs) {
					DEBUG_EMIT(entry.absoluteFilePath());
					this->bsys_opts.push_back(entry);
					bsys_choice = &bsys_opts.back();
				}
				//May do other searching of the root directory.
			}
		}
	}
}
