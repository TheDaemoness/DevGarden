#include "fileloader.h"

#include <QTextDocument>

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

