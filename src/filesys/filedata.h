#ifndef FILEDATA_H
#define FILEDATA_H

#include "fileloader.h"

#include <memory>
#include <QTextDocument>
#include <QPlainTextDocumentLayout>

#include "../consts.h"

class LangRegistry;

class FileData {
	std::unique_ptr<FileLoader> fl;
	std::unique_ptr<QTextDocument> doc;
	bool autoclose;
	size_t ref_count;
	bool saved;
	const QString* lang;
public:
	FileData() : doc(new QTextDocument), saved(false) {
		ref_count = 1;
		doc->setDocumentLayout(new QPlainTextDocumentLayout(doc.get()));
		lang = &dg_consts::STRING_EMPTY;
	}
	FileData(FileData&& fd) {
		fl.reset(fd.fl.release());
		autoclose = fd.autoclose;
		if(fd.doc.get())
			doc.reset(fd.doc.release());
		ref_count = 1;
		saved = fd.saved;
		lang = fd.lang;
	}

	size_t operator++() {++ref_count; return ref_count;}
	size_t operator--() {ref_count?--ref_count:0; return ref_count;}

	bool shouldAutoClose();

	void setFileLoader(FileLoader* nu, bool save = true);
	inline void closeLoader() {fl.reset();}
	inline bool hasLoader() {return fl.get() != nullptr;}

	void load();
	void save();
	inline bool isSaved() {return saved;}

	inline void setLang(const QString* lname) {lang = lname;}

	inline QTextDocument* getDocument() {return doc.get();} //Don't you dare free this.
	inline QTextDocument* releaseDocument() {return doc.release();} //Don't you dare free this.
	inline const QString& getLang() {return *lang;}
	bool operator<(const QString& d);
	bool operator<(const FileData& d);
	void markUnsaved() {saved = false;}
};
#endif // FILEDATA_H
