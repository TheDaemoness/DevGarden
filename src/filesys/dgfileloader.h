#ifndef DGFILELOADER_H
#define DGFILELOADER_H

#include <map>
#include <array>

#include <QFileInfo>
#include <QTextDocument>

#include "dgprojectloader.h"

class DGFileLoader {
private:
	std::list<std::pair<QString,QTextDocument*>> unsaved_files; //Unlimited.
	std::list<std::pair<QString,QTextDocument*>> saved_files; //Auto-limits to 16 files.
	QTextDocument* current;
	void unloadDoc();
public:
	DGFileLoader();
	/**
	 * @brief fileEdited Moves the current file to the front of the unsaved files list.
	 */
	void fileEdited();
	void closeCurrent();
	void closeOthers();
	void closeSaved();
	void closeAll();
	void saveCurrent();
	void saveCurrentAs();
	void saveAll();
	QTextDocument* get(const QString& fi, bool enqueue = true);
	QTextDocument* get() {return current;}

};

#endif // DGFILELOADER_H
