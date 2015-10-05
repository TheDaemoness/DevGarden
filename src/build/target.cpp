#include "target.h"
#include "../langregistry.h"
#include "../consts.h"
#include "../utils.h"

#include <QDir>
#include <QTextStream>

Target::Target(const LangRegistry& langs, const QFileInfo& fi, const QString& tar) : lr(langs), target(tar) {
	changeFile(fi);
}

bool Target::changeFile(const QFileInfo& fi) {
	file = fi;
	buildsys = lr.getLang(file);
	return sanityCheck();
}

bool Target::sanityCheck() const {
	return file.exists() && lr.isBuildSys(buildsys);
}

QString Target::rm(const QString& key) {
	auto it = vars.find(key);
	if(it != vars.end()) {
		QString retval = it->second;
		vars.erase(it);
		return retval;
	}
	return "";
}

bool Target::build(const QDir& bd, dg_utils::RunToolAsyncFlags* async, const QString& target_override) const {
	QStringList args;
	args.append(file.absoluteFilePath());
	args.append(bd.absolutePath());
	if(target_override.isEmpty())
		args.append(target);
	else
		args.append(target_override);
	QString vars;
	for(auto it : this->vars)
		vars.append(it.first+'='+it.second+'\n');
	QByteArray arr = vars.toLocal8Bit(); //Okay, this is getting problematic.
	QTextStream istream(arr);
	return dg_utils::runTool(lr.getBuildSys(buildsys),&args,nullptr,&istream,async);
}
