#include "buildcontroller.h"

#include "../filesys/dgprojectloader.h"
#include "../filesys/dgprojectinfo.h"

#include "../consts.h"

#include "../dgdebug.hpp"

BuildController::BuildController(DGProjectLoader& pl) {
	this->pl = &pl;
}

QString BuildController::getBuildDirName(const DGProjectInfo& info) const {
	return info.getName()+"-build"+(info.hasAltTargets()?'-'+info.getTargetName():dg_consts::STRING_EMPTY);
}

void BuildController::build(DGProjectInfo& f) const {
	QDir bd = getBuildDir(f,true);
	if(bd.exists() && f.getTarget())
		f.getTarget()->build(bd);
}

void BuildController::clean(DGProjectInfo& f) const {
	QDir bd = getBuildDir(f,false);
	if(bd.exists()) {
		if(f.getTarget())
			f.getTarget()->build(bd,"clean");
		BuildController::rmRF(bd);
	}
}

void BuildController::rebuild(DGProjectInfo& f) const {
	QDir bd = getBuildDir(f,true);
	if(bd.exists()) {
		if(f.getTarget()) {
			f.getTarget()->build(bd,"clean");
			f.getTarget()->build(bd);
		}
	} else {
		QString name = getBuildDirName(f);
		if(bd.mkdir(name) && bd.cd(name) && f.getTarget())
			f.getTarget()->build(bd);
	}
}


QDir BuildController::getBuildDir(const DGProjectInfo& info, bool make) const {
	QDir retval;
	if(info.isSingleFile())
		retval = info.getFile()->dir();
	else {
		retval = *info.getDir();
		retval.cdUp();
	}
	const QString dirname = getBuildDirName(info);
	if(make && !retval.exists(dirname))
		retval.mkdir(dirname);
	retval.setPath(retval.path()+'/'+dirname);
	return retval;
}

bool BuildController::rmRF(const QDir& dir) {
	for(QFileInfo& entry : dir.entryInfoList(QDir::NoDotAndDotDot  | QDir::AllEntries | QDir::Hidden | QDir::System,
			QDir::DirsFirst)) {
		if(entry.isDir())
			rmRF(QDir(entry.absoluteFilePath()));
		else
			QFile::remove(entry.absoluteFilePath());
	}
	return dir.rmdir(".");
}

