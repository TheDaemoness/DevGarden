#ifndef CONFIGENTRY_H
#define CONFIGENTRY_H

#include <vector>

#include <QString>
#include <QStringList>

struct ConfigEntry {
	ConfigEntry() {}
	ConfigEntry(const QString& dat) {data.push_back(dat.trimmed());}
	~ConfigEntry() {
		for(ConfigEntry* val : children)
			delete val;
	}
	size_t split();
	size_t getInherit();

	ConfigEntry* setData(const QString& val);
	const QString& getData(size_t index = 0) {return data.at(index);}
	QString firstWord();

	std::vector<ConfigEntry*>& getChildren() {return children;}
	ConfigEntry*& back() {return children.back();}
	ConfigEntry* push_back(ConfigEntry* val) {children.push_back(val); return this;}

private:
	QStringList data;
	QStringList inherit;
	std::vector<ConfigEntry*> children;
};
#endif // CONFIGENTRY_H
