#include "configentry.h"

ConfigEntry* ConfigEntry::setData(const QString& val) {
	data.clear();
	data.push_back(val.trimmed());
	return this;
}

QString ConfigEntry::firstWord() {
	return data.at(0).section(' ',0,0);
}

size_t ConfigEntry::getInherit() {
	auto divide = data.at(0).split(':');
	data.clear();
	data.push_back(divide.at(0).trimmed());
	if(divide.size() > 1) {
		inherit=divide.at(1).split(',');
		for(QString& str : inherit)
			str=str.trimmed();
	}
}

size_t ConfigEntry::split() {
	getInherit();
	data=data.at(0).split(' ');
}
