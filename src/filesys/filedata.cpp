#include "filedata.h"
#include "../langregistry.h"

bool FileData::shouldAutoClose() {
	return !ref_count && ((autoclose && fl.get()  && saved) || !doc.get());
}

bool FileData::save() {
	if(fl.get())
		saved |= fl->save(*doc.get());
	return saved;
}

bool FileData::load() {
	if(fl.get())
		saved |= fl->load(*doc.get());
	return saved;
}

void FileData::setFileLoader(FileLoader* nu, bool save) {
	if(save && !saved)
		this->save();
	fl.reset(nu);
	autoclose = nu->defaultAutoclose();
}

