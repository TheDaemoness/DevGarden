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
	std::list<std::pair<QString,QTextDocument*>> saved_files; //Auto-limited.

	std::pair<QString,QTextDocument*> current;
	size_t save_buffer_len;
	inline bool isCurrentSaved() {return current.second == saved_files.front().second;}
	void unloadSaved();
	void appendSaved(const std::pair<QString,QTextDocument*>& doc);
	void save(QTextDocument* tosave, const QString& path);
public:
	DGFileLoader(size_t save_buffer);
	DGFileLoader() : DGFileLoader(8) {}
	/**
	 * @brief fileEdited Moves the current file to the front of the unsaved files list.
	 */
	void fileEdited();
	void closeCurrent();
	void closeOthers(bool save = true);
	void closeSaved();
	void closeAll();
	void saveCurrent();
	void saveCurrentAs(const QString& path);
	void saveAll();
	QTextDocument* get(const QString& fi);
	QTextDocument* get() {return current.second;}

};

#endif // DGFILELOADER_H
