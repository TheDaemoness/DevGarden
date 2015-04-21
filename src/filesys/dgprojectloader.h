#ifndef DGPROJECTLOADER_H
#define DGPROJECTLOADER_H

#include "dgprojectinfo.h"

#include <vector>
#include <map>

#include <QFileInfo>

class QString;
class QStringList;

constexpr size_t FILES_OPEN_PER_PROJECT = 16;

class DGProjectLoader {
private:
	std::vector<DGProjectInfo*> projs;
	decltype(projs.begin()) current;
public:
	inline bool empty() {return projs.empty();}
	bool changeCurrent(size_t index);
	bool closeCurrent();
	bool closeOthers();
	bool closeAll();
	DGProjectInfo* getCurrent();
	bool addFile(QString path);
	bool addFolder(QString path);
	inline std::vector<DGProjectInfo*>::const_iterator cbegin() {return projs.begin();}
	inline std::vector<DGProjectInfo*>::const_iterator cend()   {return projs.cend();}
	QStringList getProjectNames();
};

#endif // DGPROJECTLOADER_H
