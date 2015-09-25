#include "fileloader.h"

#include <QTextDocument>

FileLoader* FileLoader::create(const QFileInfo& path, FileLoaderType type) {
	return (type==TYPE_DEFERRED?
				static_cast<FileLoader*>(new FileLoaderDeferred(path)):
				static_cast<FileLoader*>(new FileLoaderFS(path)));
}

bool FileLoaderFS::do_reopen(const QFileInfo& file) {
	this->file.setFileName(file.absoluteFilePath());
	return true;
}

bool FileLoaderFS::load(QTextDocument& buffer) {
	if(this->file.open(QFile::ReadOnly | QFile::Text | QFile::Unbuffered))
		buffer.setPlainText(file.readAll());
	else
		return false;
	file.close();
	return true;
}

bool FileLoaderFS::save(const QTextDocument& buffer) {
	if(this->file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text | QFile::Unbuffered)) {
		if(file.write(buffer.toPlainText().toLocal8Bit()) == -1) {
			file.close();
			return false;
		}
	} else
		return false;
	file.close();
	return true;
}

bool FileLoaderDeferred::load(QTextDocument& buffer) {

}

bool FileLoaderDeferred::save(const QTextDocument& buffer) {

}

bool FileLoaderDeferred::do_reopen(const QFileInfo& file) {
	this->file.setFileName(file.absoluteFilePath());
	return true;
}
