#ifndef LANGREFCOUNT_H
#define LANGREFCOUNT_H

#include <QMap>
#include <QString>

class LangRegistry {
	static const QString DIR;
	struct Entry {Entry() {refs = 0;} size_t refs;};
	QMap<QString,Entry> used;
	QMap<QString,Entry> unused;
	QMap<QString,QString> fileexts;
public:
	LangRegistry();
	inline size_t countLanguages() const {return used.count() + unused.count();}
	inline size_t countFileexts() const {return fileexts.count();}

};

#endif // LANGREFCOUNT_H
