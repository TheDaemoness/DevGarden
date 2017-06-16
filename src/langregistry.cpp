#include "langregistry.h"

#include "configfile.h"
#include "utils.h"
#include "consts.h"

#include <QFile>
#include <QFileInfo>

#include <iostream>
#include <memory>

const QString LangRegistry::DIR = "config/lang";

using namespace dg_consts;

QString LangRegistry::getFileExt(const QString& filename) {
    auto str = dg_utils::qsExtract(filename);
    auto slash = str.find_last_of('/');
    auto dot   = str.find_last_of('.');
    if(dot >= str.size()-1) //includes npos, >= to account for a dot at the end.
        return "";
    if(slash != std::string::npos && slash > dot)
        return "";
    return QString(str.substr(dot+1).c_str());
}

/**
 * @brief Get the corresponding language for a file.
 */
LangRegistry::LangID LangRegistry::getLang(const QFileInfo& file) const {
	auto it = filenames.find(file.fileName());
	if(it == filenames.end()) {
		it = fileexts.find(file.completeSuffix());
		if(it == fileexts.end())
			return LangRegistry::ID_UNKNOWN;
	}
	return (it->second->lang)-cache_langentries.data();
}

LangRegistry::LangRegistry() {
	std::set<QString> langset = dg_utils::getConfigDirs(LangRegistry::DIR.toLocal8Bit().data());
	if(langset.empty()) {
		std::cerr << "Failed to find language directories!" << std::endl;
		return;
	}
	ConfigEntry* ie;
    cache_langentries.reserve(langset.size());
	for(const QString& lang : langset) {
		std::unique_ptr<QFile> f{dg_utils::getUtilityFileRead(LangRegistry::DIR+'/'+lang+"/properties.conf")};
		if(f != nullptr) {
			ConfigFile cf(*f);
            cache_langentries.push_back({});
			LangEntry* le = &cache_langentries.back();
            QString interpreter("");
			if((ie = cf.at("name")))
				le->name = ie->getData(0)->mid(5);
            else
                le->name = lang;
			if((ie = cf.at("build-sys"))) {
				if(ie->split() >= 2)
					le->buildsys = (STRING_DIR_BUILD+lang+'/'+*ie->getData(1)).mid(1);
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
			for(size_t isext = 0; !table[isext].isEmpty(); ++isext) {
				if(!(fe = cf.get(table[isext])))
					continue;
                auto it = cache_fileentries.emplace(le,std::move(interpreter)).first;
				for(ConfigEntry* ce : *fe) {
					size_t e = ce->split();
					for(size_t i = 1; i < e; ++i) {
						const QString* ext = ce->getData(i);
                        auto& bindmap = getBindMap(isext);
						if(bindmap.count(*ext))
							std::cout << "Languages '" << fileexts.at(*ext)->lang->name.toStdString()
									  << "' and '" << lang.toStdString() << "' use " << table[isext].toStdString()
									  << " entry '" << ext->toStdString() << "'. Preferring the former." << std::endl;
						else
                            bindmap.insert(std::make_pair(*ext,&(*it)));
					}
				}
			}
		} else
			std::cerr << "Failed to find properties.conf for language " << lang.toStdString() << '.' << std::endl;
	}
}

std::set<LangRegistry::LangID> LangRegistry::makeBuildSysSet() const {
	std::set<LangID> retval;
	for(auto& pair : langs) {
		if(!pair.second->buildsys.isEmpty())
			retval.insert(pair.second-cache_langentries.data());
	}
	return retval;
}

QString LangRegistry::getHumanName(LangID id) const {
    if(id >= cache_langentries.size())
        return STRING_EMPTY;
    return cache_langentries[id].name;
}

bool LangRegistry::isBuildSys(LangID id) const {
    if(id >= cache_langentries.size())
        return false;
	return !cache_langentries[id].buildsys.isEmpty();
}

QString LangRegistry::getBuildSys(LangID id) const {
    return cache_langentries.at(id).buildsys;
}

LangRegistry::LangID LangRegistry::getLang(const QString& name, bool isext) const {
	if(knowsFile(name,isext))
		return (getBindMapConst(isext).at(name)->lang)-cache_langentries.data();
	return LangRegistry::ID_UNKNOWN;
}

bool LangRegistry::ready(const QString& name, bool isext) const {
	if(knowsFile(name,isext))
		return langs.count(getBindMapConst(isext).at(name)->lang->name);
	return false;
}

bool LangRegistry::add(const QStringList& langs) {
	bool issues = false;
	for(const QString& lang : langs) {
		if(!this->langs.at(lang)->refs) {
			//TODO: Loading code.
		}
		++this->langs.at(lang)->refs;
	}
	return issues;
}

bool LangRegistry::rem(const QStringList& langs) {
	bool deloaded = false;
	for(const QString& lang : langs) {
		bool load = this->langs.at(lang)->refs;
		if(load) {
			load = --this->langs.at(lang)->refs;
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
	LangRegistry::FileEntry extinfo = *(getBindMapConst(isext).at(name));
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
	const auto& map = getBindMapConst(isext);
    auto it = map.find(name);
    if(it != map.cend())
		return it->second->interpreter.isEmpty();
	return false;
}

bool LangRegistry::hasInterpreter(const QFileInfo& fi) const {
	return hasInterpreter(fi.suffix(),true) || hasInterpreter(fi.fileName(),false);
}
