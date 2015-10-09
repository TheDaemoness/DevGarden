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
class FileLoaderFS : public FileLoader {
protected:
	QFile file;
public:
	FileLoaderFS(const QFileInfo& p) : FileLoader(p) {
		file.setFileName(p.absoluteFilePath());
	}
	virtual ~FileLoaderFS() {file.close();}
	virtual bool load(QTextDocument& buffer);
	virtual bool save(const QTextDocument& buffer);
	QString getLoaderName() const {return "Filesystem";}
protected:
	bool do_reopen(const QFileInfo& file);
};

class FileLoaderDeferred : public FileLoader {
protected:
	QFile file;
public:
	FileLoaderDeferred(const QFileInfo& p) : FileLoader(p) {}
	virtual ~FileLoaderDeferred() {file.close();}
	virtual bool load(QTextDocument& buffer);
	virtual bool save(const QTextDocument& buffer);
	QString getLoaderName() const {return "Deferred";}
};

#endif // FILELOADER_H
