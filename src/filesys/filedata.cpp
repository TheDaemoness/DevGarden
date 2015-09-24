#include "filedata.h"

void FileData::setFileLoader(FileLoader* nu, bool save) {
	   if(fl.get() && save && !saved)
		   fl->save(this->doc);
	   fl.reset(nu);
	   lost = false;
}

