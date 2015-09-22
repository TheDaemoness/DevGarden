#ifndef FILELOADER_H
#define FILELOADER_H

/**
 * @brief Deferred file loading and saving class.
 * Useful for using CG scripts in DevGarden.
 */

#include <QFileInfo>
#include <QFile>
#include <memory>

class FileLoader {
protected:
	QFileInfo path;
public:
	explicit FileLoader(const QFileInfo& file) : path(file) {}
	virtual ~FileLoader() {};
	virtual bool load(QString& buffer) = 0;
	virtual bool save(const QString& buffer) = 0;
};

template <typename T>
class BaseFileLoader : public FileLoader {
public:
	explicit BaseFileLoader(const QFileInfo& file) : FileLoader(file) {
		reopen(file);
	}
	bool reopen(const QFileInfo& file) {
		return static_cast<T*>(this)->do_reopen(file);
	}

	virtual ~BaseFileLoader() {};
};

class FileLoaderFS : public BaseFileLoader<FileLoaderFS> {
protected:
	QFile file;
public:
	virtual ~FileLoaderFS() {file.close();}
	virtual bool load(QString& buffer);
	virtual bool save(const QString& buffer);
protected:
	bool do_reopen(const QFileInfo& file);
};

#endif // FILELOADER_H
