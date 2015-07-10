#include "langrefcount.h"
#include "configloader.h"

#include <QFile>

const QString LangRefCount::DIR = "config/lang";

LangRefCount::LangRefCount() {

	QSet<QString> langs = getConfigDirs(LangRefCount::DIR.toStdString().c_str());
	for(const QString& s : langs) {
		QFile* f = getUtilityFileRead((LangRefCount::DIR+'/'+s+"/properties.conf").toStdString().c_str());
		if(f != nullptr) {
			ConfigFile cf(f);
			delete f;
		}
	}
}
