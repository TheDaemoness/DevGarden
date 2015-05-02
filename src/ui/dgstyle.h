#ifndef DGSTYLE_H
#define DGSTYLE_H

class QApplication;

#include <QColor>

namespace DGStyle {
const QColor COLOR_BASE = QColor(16, 32, 40);
const QColor COLOR_LOLIGHT = QColor(52, 58, 50);
const QColor COLOR_HILIGHT = QColor(32, 192, 128);
const QColor COLOR_BACKGROUND = QColor(24, 40, 32);

void applyStyle(QApplication* app);
}

#endif // DGSTYLE_H
