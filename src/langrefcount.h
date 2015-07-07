#ifndef LANGREFCOUNT_H
#define LANGREFCOUNT_H

#include <QMap>
#include <QString>

class LangRefCount {
	static const QString DIR;
	struct Entry {size_t refs;};
public:
	LangRefCount();
private:
	QMap<QString,Entry> used;
	QMap<QString,Entry> unused;

};

#endif // LANGREFCOUNT_H
