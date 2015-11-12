#ifndef FILELOADER_H
#define FILELOADER_H

/**
 * @brief Deferred file loading and saving class.
 * Useful for using CG scripts in DevGarden.
 */

#include <QFileInfo>
#include <QFile>
#include <QDateTime>
#include <QFileSystemWatcher>

#include <memory>
#include <functional>

class QTextDocument;

class FileLoader {
	std::atomic_bool invalid;
protected:
	QFileInfo path;
	inline void invalidate() {invalid.store(true); cbStateUpdate();}
	std::function<void()> cbStateUpdate;
public:
	enum FileLoaderType {
		TYPE_DEFAULT = -1,
		TYPE_FILE = 0,
		TYPE_DEFERRED,
		TYPE_SSH
	};
	static FileLoader* create(const QFileInfo& path, FileLoaderType type = TYPE_DEFAULT);

	explicit FileLoader(const QFileInfo& file) : path(file), invalid(false) {}
	virtual ~FileLoader() {};
	inline bool isInvalid() const {return invalid.load();}
	inline const QFileInfo& info() const {return this->path;}

	void setStateUpdateCallback(decltype(cbStateUpdate)&& cb) {cbStateUpdate = cb;}
	virtual bool testValid() = 0;
	virtual bool load(QTextDocument& buffer) = 0;
	virtual bool save(const QTextDocument& buffer) = 0;
	virtual bool defaultAutoclose() {return true;}
	virtual QString getStatusString() const {return "";}
	virtual QString getAddress() const {return path.absoluteFilePath();}
	virtual QString getLoaderName() const = 0;
};
class FileLoaderFS : public FileLoader {
	void onFSChanged(const QString& p);
protected:
	static QFileSystemWatcher* fw;
	QFile file;
	bool raced;
	QDateTime mod;
	QMetaObject::Connection fwconnect;
public:
	static void setFileSystemWatcher(decltype(fw) fsw);
	FileLoaderFS(const QFileInfo& p);
	virtual ~FileLoaderFS();
	virtual bool testValid() override;
	virtual bool load(QTextDocument& buffer) override;
	virtual bool save(const QTextDocument& buffer) override;
	virtual QString getStatusString() const override;
	QString getLoaderName() const override {return "Filesystem";}
};

class FileLoaderDeferred : public FileLoader {
protected:
	QFile file;
public:
	FileLoaderDeferred(const QFileInfo& p) : FileLoader(p) {}
	virtual ~FileLoaderDeferred() {file.close();}
	virtual bool testValid() override;
	virtual bool load(QTextDocument& buffer) override;
	virtual bool save(const QTextDocument& buffer) override;
	virtual QString getStatusString() const override {return "Deferred";}
	QString getLoaderName() const override {return "Deferred";}
};

#endif // FILELOADER_H
