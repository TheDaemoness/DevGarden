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

bool BuildController::build(DGProjectInfo& f) {
	QDir bd = getBuildDir(f,true);
	if(bd.exists() && f.getTarget())
		return f.getTarget()->build(bd,&aflags);
	return false;
}

bool BuildController::clean(DGProjectInfo& f) {
	QDir bd = getBuildDir(f,false);
	if(bd.exists()) {
		if(f.getTarget())
			f.getTarget()->build(bd,"clean");
		return BuildController::rmRF(bd);
	}
	return false;
}

bool BuildController::rebuild(DGProjectInfo& f) {
	QDir bd = getBuildDir(f,true);
	if(bd.exists()) {
		if(f.getTarget()) {
			f.getTarget()->build(bd,"clean");
			return f.getTarget()->build(bd,&aflags);
		}
	} else {
		QString name = getBuildDirName(f);
		if(bd.mkdir(name) && bd.cd(name) && f.getTarget())
			return f.getTarget()->build(bd,&aflags);
	}
	return false;
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

