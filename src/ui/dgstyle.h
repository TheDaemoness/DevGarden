#ifndef DGSTYLE_H
#define DGSTYLE_H

class QApplication;

#include <QColor>

namespace DGStyle {
const QColor COLOR_BASE = QColor(16, 32, 40);
const QColor COLOR_BASE_DISABLED = QColor(32, 8, 16);
const QColor COLOR_TEXT = Qt::cyan;
const QColor COLOR_TEXT_BRIGHT = Qt::green;
const QColor COLOR_TEXT_DISABLED = Qt::darkGray;
const QColor COLOR_TEXT_EDITOR = Qt::lightGray;
const QColor COLOR_TEXT_HILIGHTED = Qt::black;
const QColor COLOR_LOLIGHT = QColor(52, 58, 50);
const QColor COLOR_HILIGHT = QColor(32, 192, 128);
const QColor COLOR_BACKGROUND = QColor(24, 40, 32);
const QColor COLOR_TEXT_SYNTAX_KEYWORD = QColor(255, 127, 196);
const QColor COLOR_TEXT_SYNTAX_LITERAL = QColor(96, 192, 96);
const QColor COLOR_TEXT_SYNTAX_COMMENT = QColor(127, 96, 96);
const QColor COLOR_TEXT_SYNTAX_TYPE = QColor(127, 255, 127);
const QColor COLOR_TEXT_SYNTAX_VARIABLE = QColor(127, 127, 255);
const QColor COLOR_TEXT_SYNTAX_CONSTANT = QColor(127, 192, 255);
const QColor COLOR_TEXT_SYNTAX_FUNCTON = QColor(255, 192, 127);

void applyStyle(QApplication* app);
}

#endif // DGSTYLE_H
