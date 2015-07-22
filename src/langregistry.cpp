#include "langregistry.h"
#include "configloader.h"

#include <QFile>
#include <iostream>

const QString LangRegistry::DIR = "config/lang";
const QString LangRegistry::EMPTY = "";

LangRegistry::LangRegistry() {
	std::set<QString> langset = getConfigDirs(LangRegistry::DIR.toStdString().c_str());
	for(const QString& lang : langset) {
		QFile* f = getUtilityFileRead((LangRegistry::DIR+'/'+lang+"/properties.conf").toStdString().c_str());
		if(f != nullptr) {
			ConfigFile cf(f);
			langs.insert(std::make_pair(lang,Entry()));
			ConfigEntry* fe = cf.at("file-exts");
			if(fe) {
				size_t e = fe->split();
				for(size_t i = 1; i < e; ++i) {
					const QString* ext = fe->getData(i);
					if(fileexts.count(*ext))
						std::cout << "Languages '" << fileexts.at(*ext).toStdString()
								  << "' and '" << lang.toStdString() << "' use file extension '"
								  << ext->toStdString() << "'. Preferring the former." << std::endl;
					else
						fileexts.insert(std::make_pair(*ext,lang));
				}
			}
			if((fe = cf.at("interpreter"))) {
				if(fe->split() >= 2)
					langs.at(lang).default_interpreter = '@'+*fe->getData(1);
			}
			else if((fe = cf.at("interpreter-integrated"))) {
				if(fe->split() >= 2)
					langs.at(lang).default_interpreter = '%'+*fe->getData(1);
			}
			delete f;
		}
	}
}

const QString& LangRegistry::getLang(const QString& fileext) const {
	if(knowsExt(fileext))
		return fileexts.at(fileext);
	return EMPTY;
}

bool LangRegistry::ready(const QString& fileext) const {
	if(knowsExt(fileext))
		return langs.count(fileexts.at(fileext));
	return false;
}

const QString& LangRegistry::load(const QString& fileext) {
	if(knowsExt(fileext)) {
		const QString& lang = fileexts.at(fileext);
		add(QStringList(lang));
		return lang;
	}
	return LangRegistry::EMPTY;
}

bool LangRegistry::add(const QStringList& langs) {
	bool issues = false;
	for(const QString& lang : langs) {
		if(!this->langs.at(lang).refs) {
			//TODO: Loading code.
		}
		++this->langs.at(lang).refs;
	}
	return issues;
}

bool LangRegistry::rem(const QStringList& langs) {
	bool deloaded = false;
	for(const QString& lang : langs) {
		bool load = this->langs.at(lang).refs;
		if(load) {
			load = --this->langs.at(lang).refs;
			if(!load) {
				deloaded = true;
				//TODO: Unloading code.
			}
		}
	}
	return deloaded;
}