#ifndef DGFILELOADER_H
#define DGFILELOADER_H

#include <map>
#include <vector>

#include <QObject>
#include <QString>
#include <QTextDocument>
#include <QFileSystemWatcher>

#include "dgprojectloader.h"
#include "filedata.h"

class DGFileCache;
class DGController;

/**
 * @brief This is a workaround because <REDACTED> moc.
 */
class SlotMachine : public QObject {
	Q_OBJECT
public:
	SlotMachine(DGFileCache& that) : QObject(nullptr), fc(that) {}
	DGFileCache& fc;
public slots:
	void onLostFile(const QString& path);
};

/**
 * @brief Prototype reference-counting file loader.
 * @deprecated INCOMPLETE, DO NOT USE!
 */
class DGFileCache { //TODO: Implement and connect.
	SlotMachine slotter;
	QFileSystemWatcher fsw; //No relation to noodles.
	std::map<QString,FileData> data;
	std::map<QString,FileData>::iterator current;
	DGController* ctrl;
public:
	DGFileCache();

	inline void bindController(DGController* dgc) {ctrl = ctrl?ctrl:dgc;}

	inline size_t  getCountLoaded() {return data.size();}
	QTextDocument* getCurrDoc()     {return current->second.getDocument();}
	const QString& getCurrLang()    {return current->second.getLang();}
	const QString& getCurrPath()    {return current->first;}

	bool isCurrSaved() {return current->second.isSaved();}

	void markUnsaved() {current->second.markUnsaved();}

	QTextDocument* set(const QString& path);

	//Triggers FileLoader::save()
	void saveCurrent();

	//Triggers FileLoader::load()
	void reloadCurrent();

	//Drops associated file loader, used by save as.
	void delinkCurrent();

	inline void saveAs() {delinkCurrent(); saveCurrent();}

private:
	friend SlotMachine;
	void onLostFile(const QString& name);

};

#endif // DGFILELOADER_H
