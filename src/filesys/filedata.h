#ifndef FILEDATA_H
#define FILEDATA_H

#include "fileloader.h"

#include <memory>
#include <QTextDocument>
#include <QPlainTextDocumentLayout>

class FileData {
	std::unique_ptr<FileLoader> fl;
	std::unique_ptr<QTextDocument> doc;
	bool autoclose;
	size_t ref_count;
	bool saved;

	QString lang;
public:
	FileData() : doc(new QTextDocument), saved(false) {
		ref_count = 1;
		doc->setDocumentLayout(new QPlainTextDocumentLayout(doc.get()));
		autoclose = true;
	}
	FileData(FileData&& fd) {
		fl.reset(fd.fl.get());
		autoclose = fd.autoclose;
		doc.reset(fd.doc.release());
		ref_count = 1;
		saved = fd.saved;
	}

	size_t operator++() {++ref_count; return ref_count;}
	size_t operator--() {ref_count?--ref_count:0; return ref_count;}

	virtual ~FileData() {};

	bool shouldAutoClose();

	void setFileLoader(FileLoader* nu, bool save = true);
	inline void closeLoader() {fl.reset();}
	inline bool hasLoader() {return fl.get() != nullptr;}

	void load();
	void save();
	inline bool isSaved() {return saved;}

	inline QTextDocument* getDocument() {return doc.get();} //Don't you dare free this.
	inline const QString& getLang() {return lang;}
	bool operator<(const QString& d);
	bool operator<(const FileData& d);
	void markUnsaved() {saved = false;}
};
#endif // FILEDATA_H
