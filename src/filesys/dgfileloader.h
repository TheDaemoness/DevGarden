#ifndef DGFILELOADER_H
#define DGFILELOADER_H

#include <QHash>
#include <vector>

#include <QString>
#include <QTextDocument>

#include "dgprojectloader.h"

/**
 * @brief Prototype reference-counting file loader.
 * @deprecated INCOMPLETE, DO NOT USE!
 */
class DGFileLoader { //TODO: Implement and connect.
public:
	struct FileRef {
		QString path;
		QTextDocument* doc;
		bool saved;
		QString lang;
	};
private:
	struct FileData {
		QTextDocument* doc;
		size_t refcount;
	};
	QHash<QString, FileData> unsaved;
	QHash<QString, FileData> saved;
	std::vector<FileRef> current;
	bool save(QTextDocument* tosave, const QString& path);
	bool isVisible(const FileData& dat) {return dat.refcount;}
public:

	DGFileLoader();
	void fileEdited(size_t index = 0);
	void saveCurrentAs(const QString& path);

	size_t addView();
	size_t remView(size_t index = 1);

	inline size_t getCountViews()    {return current.size();}
	inline size_t getCountSaved()    {return saved.size();}
	inline size_t getCountUnsaved()  {return unsaved.size();}

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
