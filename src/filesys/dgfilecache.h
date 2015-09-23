#ifndef DGFILELOADER_H
#define DGFILELOADER_H

#include <map>
#include <vector>

#include <QString>
#include <QTextDocument>

#include "dgprojectloader.h"
#include "fileloader.h"

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
	struct FileData {
		QString lang;
		QTextDocument doc;
		bool saved;
		size_t refcount;
		std::unique_ptr<FileLoader> fl;
	};
	std::set<FileData> data;
	std::vector<FileData*> current;
	bool save(QTextDocument* tosave, const QString& path);
	bool isVisible(const FileData& dat) {return dat.refcount;}
public:

	DGFileCache();
	void fileEdited(size_t index = 0);
	void saveCurrentAs(const QString& path);

	size_t addView();
	size_t remView(size_t index = 1);

	inline size_t getCountViews()    {return current.size();}
	inline size_t getCountLoaded()    {return data.size();}

	QTextDocument* set(const QString& path, size_t index = 0);
	QTextDocument* get(size_t index = 0);

	void saveCurrent(size_t index = 0);
	void saveVisible();
	void saveOthers();
	void saveAll() {saveVisible(); saveOthers();}

	void reloadCurrent(size_t index = 0);
	void reloadVisible();
	void reloadOthers() {closeOthers();}
	void reloadAll() {closeOthers(); reloadVisible();}

	void closeCurrent(size_t index = 0);
	void closeVisible();
	void closeOthers();
	void closeAll();

};

#endif // DGFILELOADER_H
