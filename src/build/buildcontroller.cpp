#include "buildcontroller.h"

#include "../filesys/dgprojectloader.h"
#include "../filesys/dgprojectinfo.h"

#include "../consts.h"

BuildController::BuildController(DGProjectLoader& pl, QObject *parent) : QObject(parent) {
	this->pl = &pl;
}

QString BuildController::getBuildDirName(const DGProjectInfo& info) const {
	return info.getName()+"-build"+(info.hasAltTargets()?'-'+info.getTargetName():dg_consts::STRING_EMPTY); //TODO: Targets.
}

void BuildController::build(DGProjectInfo& f) {
	QDir bd = getBuildDir(f,true);
	//TODO: Trigger process.
}

void BuildController::clean(DGProjectInfo& f) {
	QDir bd = getBuildDir(f,false);
	if(bd.exists()) {
		//TODO: Trigger cleaning.
		bd.cdUp();
		bd.rmdir(getBuildDirName(f));
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

