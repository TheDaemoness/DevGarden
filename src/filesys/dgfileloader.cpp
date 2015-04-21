#include "dgfileloader.h"

#include <QString>

DGFileLoader::DGFileLoader() {
}

void DGFileLoader::loadFile(DGProjectInfo* inf, const QFileInfo& fi) {
	if(files.count(fi.absolutePath()))
		++files[fi.absolutePath()].second;
	else {
		QTextDocument* tex = new QTextDocument();
		QFile file(fi.absolutePath());
		file.open(QFile::ReadOnly);
		QString data = QString(file.readAll());
		file.close();
		tex->setPlainText(data);
		files[fi.absolutePath()] = std::make_pair(tex,size_t(1));
	}

}

void DGFileLoader::unloadFile(DGProjectInfo* inf, const QFileInfo& fi) {
	if(files.count(fi.absolutePath())) {
		auto t = files[fi.absolutePath()];
		--t.second;
		if(!t.second) {
			delete t.first;
			files.erase(fi.absolutePath());
		}
	}
}
