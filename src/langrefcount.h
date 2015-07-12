#ifndef LANGREFCOUNT_H
#define LANGREFCOUNT_H

#include <QMap>
#include <QString>

class LangRefCount {
	static const QString DIR;
	struct Entry {Entry() {refs = 0;} size_t refs;};
	QMap<QString,Entry> used;
	QMap<QString,Entry> unused;
public:
	LangRefCount();
	inline size_t count() const {return used.count() + unused.count();}

};

#endif // LANGREFCOUNT_H
