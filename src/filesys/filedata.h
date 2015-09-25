#ifndef FILEDATA_H
#define FILEDATA_H

#include "fileloader.h"

#include <memory>
#include <QTextDocument>

class FileData {
	std::unique_ptr<FileLoader> fl;
	bool autoclose;
	QTextDocument doc;
public:
	bool shouldAutoClose();
	void setFileLoader(FileLoader* nu, bool save = true);
	inline QTextDocument* getDocument() {return &doc;} //Don't you dare free this.
	inline FileLoader* getLoader() {return fl.get();}

	QString lang;
	bool saved, lost;
	size_t ref_count;
	bool operator<(const QString& d);
	bool operator<(const FileData& d);
};
#endif // FILEDATA_H
