#include "fileloader.h"

bool FileLoaderFS::do_reopen(const QFileInfo& file) {
	this->file.setFileName(file.absoluteFilePath());
	return true;
}

bool FileLoaderFS::load(QString& buffer) {
	if(this->file.open(QFile::ReadOnly | QFile::Text | QFile::Unbuffered))
		buffer = file.readAll();
	else
		return false;
	file.close();
	return true;
}

bool FileLoaderFS::save(const QString& buffer) {
	if(this->file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text | QFile::Unbuffered)) {
		if(file.write(buffer.toLocal8Bit()) == -1) {
			file.close();
			return false;
		}
	} else
		return false;
	file.close();
	return true;
}

