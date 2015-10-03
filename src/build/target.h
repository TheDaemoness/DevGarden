#ifndef TARGET_H
#define TARGET_H

#include <QFileInfo>
#include <map>

#include "../consts.h"

class LangRegistry;
class QDir;

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

	bool build(const QDir& bd, const QString& target_override = dg_consts::STRING_EMPTY, const QString& script = "run.rb") const;
};

#endif // TARGET_H
