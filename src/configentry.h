#ifndef CONFIGENTRY_H
#define CONFIGENTRY_H

#include <vector>

#include <QString>
#include <QStringList>

class ConfigEntry {
public:
	ConfigEntry() {}
	ConfigEntry(const QString& dat) {data.push_back(dat.trimmed());}
	~ConfigEntry() {
		for(ConfigEntry* val : children)
			delete val;
	}
	size_t split();
	size_t getInherit();

	ConfigEntry* setData(const QString& val);
	const QString* getData(size_t index = 0) const {return index>=data.length()?nullptr:&data.at(index);}
	size_t sizeData() const {return data.length();}
	QString firstWord() const;

	bool operator<(const ConfigEntry& b) {
		if(firstWord() == b.firstWord()) {
			if(data.length() == b.data.length())
				return children.size() < b.children.size();
			return data.length() < b.data.length();
		}
		return firstWord() < b.firstWord();
	}

	bool operator==(const QString& word) {return firstWord() == word;}

	std::vector<ConfigEntry*>& getChildren() {return children;}
	ConfigEntry*& back() {return children.back();}
	ConfigEntry* push_back(ConfigEntry* val) {children.push_back(val); return this;}

private:
	QStringList data;
	QStringList inherit;
	std::vector<ConfigEntry*> children;
};

bool operator==(const ConfigEntry* ptr, const QString& word);

#endif // CONFIGENTRY_H
