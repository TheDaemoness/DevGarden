#ifndef DGFILELOADER_H
#define DGFILELOADER_H

#include <map>
#include <vector>

#include <QObject>
#include <QString>
#include <QTextDocument>

#include "dgprojectloader.h"
#include "filedata.h"

class DGFileCache;
class DGController;

/**
 * @brief Prototype reference-counting file loader.
 * @deprecated INCOMPLETE, DO NOT USE!
 */
class DGFileCache { //TODO: Implement and connect.
	std::map<QString,FileData> data;
	std::map<QString,FileData>::iterator current;
	DGController* ctrl;
	std::vector<std::unique_ptr<QTextDocument>> trash;
	const LangRegistry& lr;
public:
	DGFileCache(const LangRegistry& langs);

	inline void bindController(DGController* dgc) {ctrl = ctrl?ctrl:dgc;}

	inline size_t  getCountLoaded() {return data.size();}
	inline QTextDocument* getCurrDoc()        {return current->second.getDocument();}
	inline LangRegistry::LangID getCurrLang() {return current->second.getLang();}
	inline const QString& getCurrPath()       {return current->first;}
	QString getCurrStatus();

	inline bool isCurrSaved()   {return current->second.isSaved();}
	bool isCurrSaveable();

	inline void markUnsaved()   {current->second.markUnsaved();}
	inline void clean() {trash.clear(); trash.reserve(1);}

	QTextDocument* set(const QFileInfo& path);

	//Triggers FileLoader::save(). Returns true if the file save dialog had to open.
	bool saveCurrent();
	bool saveOthers();

	//Triggers FileLoader::load()
	void reloadCurrent();
	void reloadOthers();

	//Drops associated file loader, used by save as.
	void delinkCurrent();
private:
	bool tryClose(const decltype(current)& it);
	void onLoaderUpdate(std::map<QString,FileData>::iterator it);
	DGFileCache(const DGFileCache&) = delete;

};

#endif // DGFILELOADER_H
