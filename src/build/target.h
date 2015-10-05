#ifndef TARGET_H
#define TARGET_H

#include <QFileInfo>
#include <map>

#include "../consts.h"

class LangRegistry;
class QDir;

#include "../langregistry.h"

/**
 * @brief This tin has a label that can be read.
 */
class Target {
	QFileInfo file;
	QString target, buildsys; //NOTE: target here is not this target's name necessarily.
	const LangRegistry& lr;
	std::map<QString,QString> vars;
public:
	/**
	 * @brief Target Makes a new target.
	 * @param lr A LangRegistry instance, used to infer language information from the file.
	 * @param fi A reference to the build script or makefile.
	 * @param target The target name to be passed to the makefile or build script.
	 */
	Target(const LangRegistry& langs, const QFileInfo& fi, const QString& target = "");

	bool sanityCheck() const;

	bool changeFile(const QFileInfo& fi);
	inline QString& at(const QString& key) {return vars[key];}
	QString rm(const QString& key);

	//Java-esque, I know, but unfortunately necessary.
	bool build(const QDir& bd, const QString& target_override, const QString& script) const;
	inline bool build(const QDir& bd, const QString& target_override) const  {
		return build(bd, target_override, lr.getBuildSys(buildsys));
	}
	inline bool build(const QDir& bd) const {
		return build(bd, dg_consts::STRING_EMPTY, lr.getBuildSys(buildsys));
	}
};

#endif // TARGET_H
