#ifndef DGPROJECTINFO_H
#define DGPROJECTINFO_H

#include <set>
#include <vector>
#include <map>

#include "../langregistry.h"
#include "../async/target.h"

#include "../consts.h"

class QFileInfo;
class QDir;
class QString;

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
	inline bool hasBuildSys() const {return target.second;}

	inline bool hasAltTargets() const {return !targets.empty();}
	inline const Target* getTarget() const {return target.second;}
	inline const std::map<QString, Target>& getAltTargets() const {return targets;}
	inline const QString& getTargetName() const {return target.first?*target.first:dg_consts::STRING_EMPTY;}
protected:
	void catalog(const LangRegistry& lr, const QDir& dir, bool recursive = true);
	inline void catalog(const LangRegistry& lr, bool recursive = true);
private:
	std::pair<const QString*, Target*> target;
	std::map<QString, Target> targets;

	//std::map<QDir,QFileInfo> subprojects;
	std::set<QString> langs;
	QFileInfo* file;
	QDir* dir;
};

#endif // DGPROJECTINFO_H
