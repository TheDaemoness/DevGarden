#ifndef TARGET_H
#define TARGET_H

#include <QFileInfo>
#include <map>

class Target {
	QFileInfo file;
	QString target, buildsys;
	std::map<QString,QString> vars;
public:
	Target(const LangRegistry& lr, const QFileInfo& fi, const QString& target = "");

	QString& set(const QString& key) {return vars[key];}
	QString rm(const QString& key);

	bool build() const;
};

#endif // TARGET_H