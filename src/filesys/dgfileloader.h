#ifndef DGFILELOADER_H
#define DGFILELOADER_H

#include <map>
#include <array>

#include <QFileInfo>
#include <QTextDocument>

#include "dgprojectloader.h"

class DGFileLoader {
private:
	std::map<QString,std::pair<QTextDocument*,size_t>> files;
	std::map<DGProjectInfo*, std::list<QFileInfo>> proj_files;
public:
	DGFileLoader();
	inline QTextDocument* getDocument(const QFileInfo& fi) {return files.count(fi.absolutePath())?files[fi.absolutePath()].first:nullptr;}
	void loadFile(DGProjectInfo* proj, const QFileInfo& fi);
	void unloadFile(DGProjectInfo* proj, const QFileInfo& fi);
};

#endif // DGFILELOADER_H
