#include "target.h"
#include "../langregistry.h"

Target::Target(const LangRegistry& lr, const QFileInfo& fi, const QString& tar) : file(fi), target(tar) {
	buildsys = lr.getLang(fi.fileName(),false);
}

QString Target::rm(const QString& key) {
	auto it = vars.find(key);
	if(it != vars.end()) {
		QString retval = it->second;
		vars.erase(it);
		return retval;
	}
}

bool Target::build() const {
	lr.
}
