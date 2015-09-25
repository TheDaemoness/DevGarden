#include "filedata.h"

bool FileData::shouldAutoClose() {
	return autoclose && ((fl.get() != nullptr && !lost) || doc.isEmpty());
}

void FileData::setFileLoader(FileLoader* nu, bool save) {
	if(fl.get() && save && !saved && !doc.isEmpty())
		fl->save(this->doc);
	fl.reset(nu);
	lost = false;
}

