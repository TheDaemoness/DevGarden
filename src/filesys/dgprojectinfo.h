#ifndef DGPROJECTINFO_H
#define DGPROJECTINFO_H

class QFile;
class QDir;
class QString;

class DGProjectInfo
{
public:
	explicit DGProjectInfo(QFile* f);
	explicit DGProjectInfo(QDir* f);
	virtual ~DGProjectInfo();

	bool operator<(const DGProjectInfo& cmp) const;

	QString getName() const;
	inline bool isSingleFile() const {return !isdir;}
	inline QFile* getFile() const {return (isdir?nullptr:content.file);}
	inline QDir* getDir() const {return (isdir?content.dir:nullptr);}
private:
	const bool isdir;
	union {
		QFile* file;
		QDir* dir;
	} content;
};

#endif // DGPROJECTINFO_H
