#ifndef DGPROJECTINFO_H
#define DGPROJECTINFO_H

#include <set>
#include <vector>
#include <map>

#include "../langregistry.h"
#include "../build/target.h"

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
	inline bool hasBuildSys() const {return bsys_choice || bsys_custom;}

	inline bool hasAltTargets() const {return !alt_targets.empty();}
	inline const Target& getTarget() const {return *target.second;}
	inline const std::map<QString, Target>& getAltTargets() const {return alt_targets;}
	inline const QString& getTargetName() const {return target.first?*target.first:dg_consts::STRING_EMPTY;}
protected:
	void catalog(const LangRegistry& lr, const QDir& dir, bool recursive = true);
	inline void catalog(const LangRegistry& lr, bool recursive = true);
private:
	//TODO: Make this shit work.
	std::pair<QString*, Target*> target;
	std::map<QString, Target> alt_targets;
	Target* default_target;

	//TODO: Remove. Especially the void*.
	std::vector<QFileInfo> bsys_opts;
	QFileInfo* bsys_choice; //Can be null for bsys_custom to take effect.
	void* bsys_custom; //Type to be replaced.

	//std::map<QDir,QFileInfo> subprojects;
	std::set<QString> langs;
	QFileInfo* file;
	QDir* dir;
};

#endif // DGPROJECTINFO_H
