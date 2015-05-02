#include "configentry.h"

bool operator==(const ConfigEntry* ptr, const QString& word) {
	 return ptr->firstWord() == word;
}

ConfigEntry* ConfigEntry::setData(const QString& val) {
	data.clear();
	data.push_back(val.trimmed());
	return this;
}

QString ConfigEntry::firstWord() const {
	return data.at(0).section(' ',0,0);
}

size_t ConfigEntry::getInherit() {
	if(!inherit.isEmpty())
		return inherit.size();
	auto temp = data.first();
	data.clear();
	data.push_back(temp.section(':',0,0).trimmed());
	inherit=temp.section(':',1,1).split(',');
	for(QString& str : inherit)
		str=str.trimmed();
	return inherit.size();
}

size_t ConfigEntry::split() {
	getInherit();
	if(data.size()==1)
		data=data.at(0).split(' ');
	return data.size();
}
