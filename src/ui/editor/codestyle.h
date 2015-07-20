#ifndef CODESTYLE_H
#define CODESTYLE_H

#include <QTextCharFormat>
#include <QColor>

struct TextFormat {
	uint32_t id;
	QTextFormat format;
	QColor color;
	bool operator==(uint32_t val) const {return val==id;}
	bool operator==(const TextFormat& val) const {return val.format == format && val.color == color;}
	bool operator<(uint32_t val) const {return val<id;}
	bool operator<(const TextFormat& val) const {return val.id < id;}
	bool operator>(const TextFormat& val) const {return val.id > id;}
};

#endif // CODESTYLE_H
