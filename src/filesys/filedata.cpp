#include "filedata.h"

bool FileData::shouldAutoClose() {
	return !ref_count && ((autoclose && fl.get() != nullptr && saved) || doc.get() == nullptr);
}

void FileData::save() {
	if(fl.get() != nullptr)
		saved = fl->save(*doc.get());
}

void FileData::load() {
	if(fl.get() != nullptr)
		saved = fl->load(*doc.get());
}

void FileData::setFileLoader(FileLoader* nu, bool save) {
	if(fl.get() && save && !saved && !doc->isEmpty())
		fl->save(*doc.get());
	fl.reset(nu);
}

