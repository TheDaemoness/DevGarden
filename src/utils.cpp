#include "utils.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QFileInfo>
#include <QProcess>
#include <QStringList>

namespace dg_utils {

QFileInfo* getUtilityFile(const QString& name) {
	QFileInfo* f = new QFileInfo();
	f->setFile(QDir::home().path()+'/'+DG_CONFIG_PREFIX_LOCAL+DG_NAME+'/'+name);
	if(!f->exists()) {
		f->setFile(QString(DG_CONFIG_PREFIX_GLOBAL)+DG_NAME+"/"+name);
		if(!f->exists()) {
			delete f;
			return nullptr;
		}
	}
	return f;
}

QFile* getUtilityFileRead(const QString& name) {
	QFile* retval = new QFile;
	retval->setFileName(QDir::home().path()+'/'+DG_CONFIG_PREFIX_LOCAL+DG_NAME+'/'+name);
	retval->open(QFile::ReadOnly);
	if(!retval->isOpen()) {
		retval->setFileName(QString(DG_CONFIG_PREFIX_GLOBAL)+DG_NAME+"/"+name);
		retval->open(QFile::ReadOnly);
	}
	if(retval->isOpen())
		return retval;
	delete retval;
	return nullptr;
}

QFile* getUtilityFileWrite(const QString& name) {
	QFile* retval = new QFile;
	retval->setFileName(QDir::home().path()+'/'+DG_CONFIG_PREFIX_LOCAL+DG_NAME+"/"+name);
	retval->open(QFile::WriteOnly);
	if(!retval->isOpen()) {
		delete retval;
		return nullptr;
	}
	return retval;
}

void makeConfigDirs() {
	QDir d = QDir::home();
#ifndef DG_CONFIG_PREFIX_LOCAL_NOCD
	d.cd(DG_CONFIG_PREFIX_LOCAL);
#endif
	if(!d.exists())
		d.mkpath(DG_CONFIG_PREFIX_LOCAL);
#ifndef DG_CONFIG_PREFIX_LOCAL_NOCD
	if(!d.exists(DG_NAME)) {
		d.mkdir(DG_NAME);
		d.cd(DG_NAME);
	}
#else
	if(!d.exists(QString(DG_CONFIG_PREFIX_LOCAL)+DG_NAME)) {
		d.mkdir(QString(DG_CONFIG_PREFIX_LOCAL)+DG_NAME);
		d.cd(QString(DG_CONFIG_PREFIX_LOCAL)+DG_NAME);
	}
#endif
}

static ConfigEntry* recurseGetConfigEntry(QFile& file, ConfigEntry* parent, size_t depth = 0) {
	size_t ws_count = depth;
	do {
		if(!file.atEnd()) {
			ConfigEntry* child = nullptr;
			if(ws_count == depth)
				child = new ConfigEntry(file.readLine());
			else if(ws_count > depth)
				child = recurseGetConfigEntry(file, parent->back(), ws_count);
			if(child)
				parent->push_back(child);
		}
		ws_count = 0;
		char val = file.peek(1)[0];
		while(val == '\t' || val == ' ' || val == '\n' || val == '\r') {
			file.read(1);
			if(val == '\n' || val == '\r')
				ws_count = 0;
			else
				++ws_count;
			val = file.peek(1)[0];
		}
	} while(ws_count >= depth && !file.atEnd());
	return parent;
}

ConfigEntry* getConfigEntry(QFile& file) {
	if(!file.isOpen() || file.atEnd())
		return nullptr;
	ConfigEntry* retval = new ConfigEntry;
	size_t ws_count;
	ws_count = 0;
	retval->setData(file.readLine());
	char val = file.peek(1)[0];
	while(val == '\t' || val == ' ') {
		file.read(1);
		++ws_count;
		val = file.peek(1)[0];
	}
	if(ws_count)
		return recurseGetConfigEntry(file, retval, ws_count);
	return retval;
}

bool runTool(const QString& name, QStringList* args, QByteArray* out, const QByteArray* in) {
	QFileInfo* retval = new QFileInfo;
	retval->setFile(QDir::home().path()+'/'+DG_CONFIG_PREFIX_LOCAL+DG_NAME+'/'+name);
	if(!retval->isExecutable())
		retval->setFile(QString(DG_CONFIG_PREFIX_GLOBAL)+DG_NAME+"/"+name);
	if(!retval->isExecutable()) {
		delete retval;
		return false;
	}
	QProcess proc;
	proc.setProgram(retval->absoluteFilePath());
	if(args)
		proc.setArguments(*args);
	proc.start();
	if(!proc.waitForStarted())
		return false;
	if(in)
		proc.write(*in);
	if(!proc.waitForFinished())
		return !out;
	if(out)
		*out = proc.readAllStandardOutput();
	return true;
}

std::set<QString> getConfigDirs(const QString& name) {
	std::set<QString> retval;
	QDir local = QDir(QDir::home().path()+'/'+DG_CONFIG_PREFIX_LOCAL+DG_NAME+'/'+name);
	QDir global = QDir(QString(DG_CONFIG_PREFIX_GLOBAL)+DG_NAME+"/"+name);
	QStringList v;
	v = local.entryList(QStringList(),QDir::Dirs|QDir::NoDotAndDotDot,QDir::Name);
	for(QString& s : v)
		retval.insert(s);
	v = global.entryList(QStringList(),QDir::Dirs|QDir::NoDotAndDotDot,QDir::Name);
	for(QString& s : v)
		retval.insert(s);
	return retval;
}

}
