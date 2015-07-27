#ifndef DGPROJECTINFO_H
#define DGPROJECTINFO_H

#include <array>

class QFileInfo;
class QDir;
class QString;

/**
 * @brief Stores metadata about one project.
 */
class DGProjectInfo {
public:
	explicit DGProjectInfo(QFileInfo* f);
	explicit DGProjectInfo(QDir* f);
	virtual ~DGProjectInfo();

	bool operator<(const DGProjectInfo& cmp) const;

	QString getName() const;
	inline bool isSingleFile() const {return !dir;}
	inline QFileInfo* getFile() const {return file;}
	inline QDir* getDir() const {return dir;}
private:
	QFileInfo* file;
	QDir* dir;
};

#endif // DGPROJECTINFO_H
