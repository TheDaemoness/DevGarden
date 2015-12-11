#include "target.h"
#include "utils.h"

#include <QDir>
#include <QTextStream>

#include <thread>

Target::Target(const LangRegistry& langs, const QFileInfo& fi, const QString& tar) : target(tar), lr(langs) {
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
	auto runner = [=]{
		const QString script = lr.getBuildSys(buildsys);
		QString vars;
		for(auto it : this->vars)
			vars.append(it.first+'='+it.second+'\n');
		QStringList args;
		args.append(file.absoluteFilePath());
		args.append(bd.absolutePath());
		if(target_override.isEmpty())
			args.append(target);
		else
			args.append(target_override);
		QTextStream istream(vars.toLocal8Bit());
		dg_utils::runTool(script,&args,nullptr,&istream,async);
	};
	if(async) {
		std::thread(runner).detach();
		return !async->isStopped();
	} else {
		runner();
		return true;
	}
}
