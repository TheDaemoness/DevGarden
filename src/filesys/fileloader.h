#ifndef FILELOADER_H
#define FILELOADER_H

/**
 * @brief Deferred file loading and saving class.
 * Useful for using CG scripts in DevGarden.
 */

#include <QFileInfo>
#include <QFile>
#include <memory>

class QTextDocument;

class FileLoader {
protected:
	QFileInfo path;
public:
	enum FileLoaderType {
		TYPE_DEFAULT = -1,
		TYPE_FILE = 0,
		TYPE_DEFERRED,
		TYPE_SSH
	};
	static FileLoader* create(const QFileInfo& path, FileLoaderType type = TYPE_DEFAULT);

	explicit FileLoader(const QFileInfo& file) : path(file) {}
	virtual ~FileLoader() {};
	inline const QFileInfo& info() const {return this->path;}
	virtual bool load(QTextDocument& buffer) = 0;
	virtual bool save(const QTextDocument& buffer) = 0;
	virtual bool defaultAutoclose() {return true;}
	virtual QString name() {return path.absoluteFilePath();}
	virtual QString getLoaderName() const = 0;
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
	FileLoaderFS(const QFileInfo& p) : BaseFileLoader<FileLoaderFS>(p) {}
	virtual ~FileLoaderFS() {file.close();}
	virtual bool load(QTextDocument& buffer);
	virtual bool save(const QTextDocument& buffer);
	QString getLoaderName() const {return "Filesystem";}
protected:
	friend BaseFileLoader<FileLoaderFS>;
	bool do_reopen(const QFileInfo& file);
};

class FileLoaderDeferred : public BaseFileLoader<FileLoaderDeferred> {
protected:
	QFile file;
public:
	FileLoaderDeferred(const QFileInfo& p) : BaseFileLoader<FileLoaderDeferred>(p) {}
	virtual ~FileLoaderDeferred() {file.close();}
	virtual bool load(QTextDocument& buffer);
	virtual bool save(const QTextDocument& buffer);
	QString getLoaderName() const {return "Deferred";}
protected:
	friend BaseFileLoader<FileLoaderDeferred>;
	bool do_reopen(const QFileInfo& file);
};

#endif // FILELOADER_H
