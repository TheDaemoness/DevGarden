#include "langregistry.h"

#include "configfile.h"
#include "utils.h"
#include "consts.h"

#include <QFile>
#include <QFileInfo>

#include <iostream>
#include <memory>

const QString LangRegistry::DIR = "config/lang";

const QRegExp LangRegistry::FILEEXT_PATTERN = QRegExp("[a-zA-Z0-9_]\\.");

using namespace dg_consts;

QString LangRegistry::getFileExt(const QString& filename) {
	return filename.section(FILEEXT_PATTERN,-1);
}

/**
 * @brief Get the corresponding language for a file.
 */
const QString& LangRegistry::getLang(const QFileInfo& file) const {
	auto it = filenames.find(file.fileName());
	if(it == filenames.end()) {
		it = fileexts.find(getFileExt(file.fileName()));
		if(it == filenames.end())
			return dg_consts::STRING_EMPTY;
	}
	return it->second.lang;
}

LangRegistry::LangRegistry() {
	std::set<QString> langset = dg_utils::getConfigDirs(LangRegistry::DIR.toStdString().c_str());
	if(langset.empty()) {
		std::cerr << "Failed to find language directories!" << std::endl;
		return;
	}
	ConfigEntry* ie;
	for(const QString& lang : langset) {
		std::unique_ptr<QFile> f{dg_utils::getUtilityFileRead(LangRegistry::DIR+'/'+lang+"/properties.conf")};
		if(f != nullptr) {
			ConfigFile cf(*f);
			LangEntry le;
			QString interpreter;
			if((ie = cf.at("name")))
				le.name = ie->getData(0)->mid(5);
			if((ie = cf.at("build-sys"))) {
				if(ie->split() >= 2)
					le.buildsys = (STRING_DIR_BUILD+lang+'/'+*ie->getData(1)).mid(1);
			}
			if((ie = cf.at("interpreter-external"))) {
				if(ie->split() >= 2)
					interpreter = '@'+*ie->getData(1);
			}
			else if((ie = cf.at("interpreter-internal"))) {
				if(ie->split() >= 2)
					interpreter = '%'+*ie->getData(1);
			}
			langs.insert(std::make_pair(lang,le));
			ConfigFile::Values* fe;
			const QString table[] = {"file-names", "file-exts", ""};
			for(size_t n = 0; !table[n].isEmpty(); ++n) {
				if(!(fe = cf.get(table[n])))
					continue;
				for(ConfigEntry* ce : *fe) {
					size_t e = ce->split();
					for(size_t i = 1; i < e; ++i) {
						const QString* ext = ce->getData(i);
						if(getBindMap(n).count(*ext))
							std::cout << "Languages '" << fileexts.at(*ext).lang.toStdString()
									  << "' and '" << lang.toStdString() << "' use " << table[n].toStdString()
									  << " entry '" << ext->toStdString() << "'. Preferring the former." << std::endl;
						else {
							FileEntry ee(lang);
							ee.interpreter = interpreter;
							getBindMap(n).insert(std::make_pair(*ext,ee));
						}
					}
				}
			}
		} else
			std::cerr << "Failed to find properties.conf for language " << lang.toStdString() << '.' << std::endl;
	}
}

std::set<QString> LangRegistry::makeBuildSysSet() const {
	std::set<QString> retval;
	for(auto& pair : langs) {
		if(!pair.second.buildsys.isEmpty())
			retval.insert(pair.first);
	}
	return retval;
}

const QString& LangRegistry::getHumanName(const QString& lang) const {
	auto it = langs.find(lang);
	if(it != langs.end()) {
		if(it->second.name.isEmpty())
			return it->first;
		return it->second.name;
	}
	return STRING_EMPTY;
}

bool LangRegistry::isBuildSys(const QString& lang) const {
	if(knowsLang(lang))
		return !langs.at(lang).buildsys.isEmpty();
	return false;
}

const QString& LangRegistry::getBuildSys(const QString& lang) const {
	if(knowsLang(lang))
		return langs.at(lang).buildsys;
	return STRING_EMPTY;
}

const QString& LangRegistry::getLang(const QString& name, bool isext) const {
	if(knowsFile(name,isext))
		return getBindMapConst(isext).at(name).lang;
	return STRING_EMPTY;
}

bool LangRegistry::ready(const QString& name, bool isext) const {
	if(knowsFile(name,isext))
		return langs.count(getBindMapConst(isext).at(name).lang);
	return false;
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

QString LangRegistry::getInterpreter(const QString& name, bool isext) const {
	if(!hasInterpreter(name,isext))
		return "";
	LangRegistry::FileEntry extinfo = getBindMapConst(isext).at(name);
	if(extinfo.interpreter.at(0) == '@')
		return extinfo.interpreter.mid(1);
	else if(extinfo.interpreter.at(0) == '%') {
		QFileInfo* f = dg_utils::getUtilityFile(extinfo.interpreter.mid(1).toLocal8Bit());
		if(f) {
			if(f->isExecutable())
				return f->absoluteFilePath();
		}
	}
	return "";
}

QString LangRegistry::getInterpreter(const QFileInfo& fi) const {
	QString exact  = getInterpreter(fi.fileName(),false);
	QString suffix = getInterpreter(fi.suffix(),true);
	if(!exact.isEmpty())
		return exact;
	if(!suffix.isEmpty())
		return suffix;
	return "";
}

bool LangRegistry::hasInterpreter(const QString& name, bool isext) const {
	try {
		return !getBindMapConst(isext).at(name).interpreter.isEmpty();
	} catch(...) {
		return false;
	}
}

bool LangRegistry::hasInterpreter(const QFileInfo& fi) const {
	return hasInterpreter(fi.suffix(),true) || hasInterpreter(fi.fileName(),false);
}
