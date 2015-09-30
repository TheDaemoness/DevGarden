#include "buildcontroller.h"

#include "../filesys/dgprojectloader.h"
#include "../filesys/dgprojectinfo.h"

#include "../consts.h"

BuildController::BuildController(DGProjectLoader& pl) {
	this->pl = &pl;
}

QString BuildController::getBuildDirName(const DGProjectInfo& info) const {
	return info.getName()+"-build"+(info.hasAltTargets()?'-'+info.getTargetName():dg_consts::STRING_EMPTY); //TODO: Targets.
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
			f.getTarget()->clean(bd);
		bd.cdUp();
		bd.rmdir(getBuildDirName(f));
	}
}


void BuildController::rebuild(DGProjectInfo& f) const {
	QDir bd = getBuildDir(f,false);
	if(bd.exists()) {
		if(f.getTarget()) {
			f.getTarget()->clean(bd);
			f.getTarget()->build(bd);
		}
	} else {
		if(bd.mkdir(".") && f.getTarget())
			f.getTarget()->build(bd);
	}
}


QDir BuildController::getBuildDir(const DGProjectInfo& info, bool make) const {
	QDir retval;
	if(!info.isSingleFile())
		retval = info.getFile()->dir();
	else {
		retval = *info.getDir();
		retval.cdUp();
	}
	if(make && !retval.exists(getBuildDirName(info)))
		retval.mkdir(getBuildDirName(info));
	return retval;
}

