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
public:
	struct FileRef { //TODO: Remove.
		QFileInfo info;
		QTextDocument* doc;
		bool saved;
		QString lang;
	};
private:
	SlotMachine slotter;
	QFileSystemWatcher fsw; //No relation to noodles.
	std::map<QString,FileData> data;
	std::vector<FileData*> current;
public:
	DGFileCache();
	~DGFileCache();
	void fileEdited(size_t index = 0);
	void saveCurrentAs(const QString& path);

	size_t addView(const QString& path);
	inline void clearViews() {current.clear();}

	inline size_t getCountLoaded()    {return data.size();}

	void set(const QString& path, size_t index = 0);
	QTextDocument* get(size_t index = 0);

	//Triggers FileLoader::save()
	void saveCurrent(size_t index = 0);
	void saveVisible();
	void saveOthers();
	void saveAll() {saveVisible(); saveOthers();}

	//Triggers FileLoader::load()
	void reloadCurrent(size_t index = 0);
	void reloadOthers() {closeOthers();}
	void reloadAll();

	//Closes a NON-REFERENCED SAVED FILE.
	void closeSingle(const QString& str);
	void closeOthers();
	void closeAll();

	//Drops associated file loader, used by save as.
	void delinkCurrent(size_t index = 0);
	void delinkOthers();
	void delinkAll();

	//Uses a specified file loader for one save operation, used by save and copy.
	void copyCurrent(FileLoader& saver, size_t index = 0);

private:
	friend SlotMachine;
	void onLostFile(const QString& name);

};

#endif // DGFILELOADER_H
