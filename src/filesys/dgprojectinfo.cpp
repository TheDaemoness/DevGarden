#include "dgprojectinfo.h"

#include "../langregistry.h"

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

DGProjectInfo::DGProjectInfo(QDir* f, const LangRegistry& lr) {
	dir = f;
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
				const QString& lang = lr.getLang(entry);
				if(!lang.isEmpty() && lr.isBuildSys(lang))
					targets.emplace("default-"+lang, Target(lr, entry));
				//May do other searching of the root directory.
			}
		}
	}
	if(!targets.empty() && !target.second) {
		auto first = targets.begin();
		target.first = &first->first;
		target.second = &first->second;
	}
}
