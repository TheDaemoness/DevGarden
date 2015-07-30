#ifndef DGPROJECTINFO_H
#define DGPROJECTINFO_H

#include <set>
#include <vector>
#include <map>

class QFileInfo;
class QDir;
class QString;

class LangRegistry;

/**
 * @brief Stores metadata about one project.
 */
class DGProjectInfo {
public:
	explicit DGProjectInfo(QFileInfo* f);
	explicit DGProjectInfo(QDir* f, const LangRegistry& lr);
	virtual ~DGProjectInfo();

	bool operator<(const DGProjectInfo& cmp) const;

	QString getName() const;
	inline bool isSingleFile() const {return !dir;}
	inline QFileInfo* getFile() const {return file;}
	inline QDir* getDir() const {return dir;}
	inline bool hasBuildSys() const {return bsys_choice || bsys_custom;}
protected:
	void catalog(const LangRegistry& lr, const QDir& dir, bool recursive = true);
	inline void catalog(const LangRegistry& lr, bool recursive = true);
private:
	std::set<QString> langs;
	std::vector<QFileInfo> bsys_opts;
	QFileInfo* bsys_choice; //Can be null for bsys_custom to take effect.
	void* bsys_custom; //Type to be replaced.
	//std::map<QDir,QFileInfo> subprojects;
	QFileInfo* file;
	QDir* dir;
};

#endif // DGPROJECTINFO_H
