#ifndef DGPROJECTLOADER_H
#define DGPROJECTLOADER_H

#include "dgprojectinfo.h"

#include <vector>
#include <map>

#include <QFileInfo>

class QString;
class QStringList;

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
