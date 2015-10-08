#include "filedata.h"

bool FileData::shouldAutoClose() {
	return !ref_count && ((autoclose && fl.get() != nullptr && saved) || doc.get() == nullptr);
}

void FileData::save() {
	if(fl.get() != nullptr)
		fl->save(*doc.get());
	saved = true;
}

void FileData::load() {
	if(fl.get() != nullptr)
		fl->load(*doc.get());
}

void FileData::setFileLoader(FileLoader* nu, bool save) {
	if(fl.get() && save && !saved && !doc->isEmpty())
		fl->save(*doc.get());
	fl.reset(nu);
}

