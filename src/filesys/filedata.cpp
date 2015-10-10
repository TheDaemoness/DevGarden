#include "filedata.h"
#include "../langregistry.h"

bool FileData::shouldAutoClose() {
	return !ref_count && ((autoclose && fl.get()  && saved) || !doc.get());
}

void FileData::save() {
	if(fl.get())
		saved |= fl->save(*doc.get());
}

void FileData::load() {
	if(fl.get())
		saved |= fl->load(*doc.get());
}

void FileData::setFileLoader(FileLoader* nu, bool save) {
	if(save && !saved)
		this->save();
	fl.reset(nu);
	autoclose = nu->defaultAutoclose();
}

