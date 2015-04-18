#ifndef DGPROJECTINFO_H
#define DGPROJECTINFO_H

class QFile;
class QFileSystemModel;

class DGProjectInfo
{
public:
	explicit DGProjectInfo(QFile* f);
	explicit DGProjectInfo(QFileSystemModel* f);
	virtual ~DGProjectInfo();

	inline bool isSingleFile() {return !isdir;}
	inline QFile* getFile() {return (isdir?nullptr:content.file);}
	inline QFileSystemModel* getDir() {return (isdir?content.dir:nullptr);}
private:
	const bool isdir;
	union {
		QFile* file;
		QFileSystemModel* dir;
	} content;
};

#endif // DGPROJECTINFO_H
