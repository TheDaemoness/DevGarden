#ifndef DGPROJECTLOADER_H
#define DGPROJECTLOADER_H

#include "dgprojectinfo.h"

#include <vector>
#include <map>

#include <QFileInfo>

class QString;
class QStringList;

class DGProjectLoader {
public: using ProjectListType = std::vector<DGProjectInfo*>;
private:
	ProjectListType projs;
	ProjectListType::iterator current;
public:
	DGProjectLoader() {projs.reserve(1);}
	inline bool empty() {return projs.empty();}
	bool changeCurrent(size_t index);
	bool closeCurrent();
	bool closeOthers();
	bool closeAll();
	DGProjectInfo* getCurrent();
	bool addFile(QString path);
	bool addFolder(QString path);
	inline ProjectListType::const_iterator cbegin() {return projs.cbegin();}
	inline ProjectListType::const_iterator cend()   {return projs.cend();}
	QStringList getProjectNames();
};

#endif // DGPROJECTLOADER_H
