#include "langregistry.h"
#include "configloader.h"

#include <QFile>
#include <iostream>

const QString LangRegistry::DIR = "config/lang";
const QString LangRegistry::EMPTY = "";

LangRegistry::LangRegistry() {
	QSet<QString> langset = getConfigDirs(LangRegistry::DIR.toStdString().c_str());
	for(const QString& lang : langset) {
		QFile* f = getUtilityFileRead((LangRegistry::DIR+'/'+lang+"/properties.conf").toStdString().c_str());
		if(f != nullptr) {
			ConfigFile cf(f);
			langs.insert(lang,Entry());
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

bool LangRegistry::ready(const QString& fileext) const {
	if(knowsExt(fileext))
		return langs.count(fileexts.value(fileext));
	return false;
}

const QString LangRegistry::load(const QString& fileext) {
	if(knowsExt(fileext)) {
		const QString& lang = fileexts.value(fileext);
		add(QStringList(lang));
		return lang;
	}
	return LangRegistry::EMPTY;
}

bool LangRegistry::add(const QStringList& lang) {

}

bool LangRegistry::rem(const QStringList& lang) {

}
