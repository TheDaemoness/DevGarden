#include "fileloader.h"

#include <QTextDocument>
#include <QStringList>

FileLoader* FileLoader::create(const QFileInfo& path, FileLoaderType type) {
	FileLoader* retval = (type==TYPE_DEFERRED?
				static_cast<FileLoader*>(new FileLoaderDeferred(path)):
				static_cast<FileLoader*>(new FileLoaderFS(path)));
	if(retval->isInvalid()) {
		delete retval;
		return nullptr;
	}
	return retval;
}

void FileLoaderFS::setFileSystemWatcher(decltype(fw) fsw) {
	fw=fsw;
}

QFileSystemWatcher* FileLoaderFS::fw = nullptr;

FileLoaderFS::FileLoaderFS(const QFileInfo& p) : FileLoader(p) {
	file.setFileName(p.absoluteFilePath());
	raced = false;
	mod = p.lastModified();
	if(!fw->files().contains(p.absoluteFilePath()))
		fw->addPath(p.absoluteFilePath());
	else
		invalidate();
	auto cb = std::bind(&FileLoaderFS::onFSChanged,this,std::placeholders::_1);
	fwconnect = QObject::connect(fw, &QFileSystemWatcher::fileChanged, cb);
}

FileLoaderFS::~FileLoaderFS() {
	file.close();
	QObject::disconnect(fwconnect);
	fw->removePath(path.absoluteFilePath());
}

bool FileLoaderFS::testValid() {
	path.refresh();
	if(!path.exists() || !path.isReadable() || !path.isWritable()) {
		invalidate();
		return false;
	}
	if(path.lastModified() != mod) {
		mod = path.lastModified();
		raced = true;
		cbStateUpdate();
	}
	return true;
}

bool FileLoaderFS::load(QTextDocument& buffer) {
	fw->removePath(path.absoluteFilePath());
	if(this->file.open(QFile::ReadOnly | QFile::Text | QFile::Unbuffered))
		buffer.setPlainText(file.readAll());
	else {
		fw->addPath(path.absoluteFilePath());
		return false;
	}
	file.close();
	path.refresh();
	mod = path.lastModified();
	raced = false;
	fw->addPath(path.absoluteFilePath());
	return true;
}

QString FileLoaderFS::getStatusString() const {
	return QString(raced?"File Edited":"");
}

bool FileLoaderFS::save(const QTextDocument& buffer) {
	fw->removePath(path.absoluteFilePath());
	if(this->file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text | QFile::Unbuffered)) {
		if(file.write(buffer.toPlainText().toLocal8Bit()) == -1) {
			file.close();
			fw->addPath(path.absoluteFilePath());
			return false;
		}
	} else {
		fw->addPath(path.absoluteFilePath());
		return false;
	}
	file.close();
	path.refresh();
	mod = path.lastModified();
	fw->addPath(path.absoluteFilePath());
	raced = false;
	return true;
}

void FileLoaderFS::onFSChanged(const QString& p) {
	QFileInfo fi(p);
	if(fi == path)
		testValid();
}


bool FileLoaderDeferred::testValid() {

}

bool FileLoaderDeferred::load(QTextDocument& buffer) {

}

bool FileLoaderDeferred::save(const QTextDocument& buffer) {

}
