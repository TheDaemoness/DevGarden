#include "langregistry.h"
#include "configloader.h"

#include <QFile>
#include <iostream>

const QString LangRegistry::DIR = "config/lang";

LangRegistry::LangRegistry() {

	QSet<QString> langs = getConfigDirs(LangRegistry::DIR.toStdString().c_str());
	for(const QString& lang : langs) {
		QFile* f = getUtilityFileRead((LangRegistry::DIR+'/'+lang+"/properties.conf").toStdString().c_str());
		if(f != nullptr) {
			ConfigFile cf(f);
			unused.insert(lang,Entry());
			ConfigEntry* fe = cf.at("file-exts");
			if(fe) {
				size_t e = fe->split();
				for(size_t i = 1; i < e; ++i) {
					const QString* ext = fe->getData(i);
					if(fileexts.count(*ext))
						std::cout << "Languages '" << fileexts.value(*ext).toStdString()
								  << "' and '" << lang.toStdString() << "' use file extension '"
								  << ext->toStdString() << "'. Preferring the former." << std::endl;
					else
						fileexts.insert(*ext,lang);
				}
			}
			delete f;
		}
	}
}
