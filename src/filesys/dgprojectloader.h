#ifndef DGPROJECTLOADER_H
#define DGPROJECTLOADER_H

#include "dgprojectinfo.h"

#include <vector>
#include <map>
#include <memory>

#include <QFileInfo>

class QString;
class QStringList;

/**
 * @brief Loads, stores, and manages all projects' metadata.
 */
class DGProjectLoader {
public: using ProjectListType = std::vector<std::unique_ptr<DGProjectInfo>>;
private:
	ProjectListType projs;
	ProjectListType::iterator current;
	const LangRegistry& langreg;
public:
	DGProjectLoader(const LangRegistry& lr) : langreg(lr) {projs.reserve(1);}
	inline bool empty() const {return projs.empty();}
	bool changeCurrent(size_t index);
	bool closeCurrent();
	bool closeOthers();
	bool closeAll();
	DGProjectInfo* getCurrent() const;
	bool addFile(const QString& path);
	bool addFolder(const QString& path);
	inline ProjectListType::const_iterator cbegin() const {return projs.cbegin();}
	inline ProjectListType::const_iterator cend()   const {return projs.cend();}
	QStringList getProjectNames() const;
	inline size_t count() {return projs.size();}
};

#endif // DGPROJECTLOADER_H
