#ifndef FILEDATA_H
#define FILEDATA_H

#include "fileloader.h"

#include <memory>
#include <QTextDocument>
#include <QPlainTextDocumentLayout>

#include "../consts.h"
#include "../langregistry.h"

class FileData {
	std::unique_ptr<FileLoader> fl;
	std::unique_ptr<QTextDocument> doc;
	bool autoclose;
	size_t ref_count;
	bool saved;
	LangRegistry::LangID lang;
public:
    FileData() : doc{new QTextDocument}, ref_count{1}, saved{false}, lang{LangRegistry::ID_UNKNOWN} {
		doc->setDocumentLayout(new QPlainTextDocumentLayout(doc.get()));
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
	void closeLoader() {fl.reset();}
	inline const FileLoader* getLoader() {return fl.get();}

	bool load();
	bool save();
	inline bool isSaved() {return saved;}

	inline void setLang(LangRegistry::LangID lid) {lang = lid;}

	inline QTextDocument* getDocument() {return doc.get();} //Don't you dare free this.
	inline QTextDocument* releaseDocument() {return doc.release();} //Don't you dare free this.
	inline LangRegistry::LangID getLang() {return lang;}
	bool operator<(const QString& d);
	bool operator<(const FileData& d);
	void markUnsaved() {saved = false;}
};
#endif // FILEDATA_H
