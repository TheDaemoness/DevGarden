#include "dgstyle.h"

#include <QApplication>
#include <QStyleFactory>
#include <QPalette>

#include "../dgdebug.hpp"

void DGStyle::applyStyle(QApplication* app) {

	app->setStyle(QStyleFactory::create("Fusion"));

	QPalette darkPalette;
	darkPalette.setColor(QPalette::Window, DGStyle::COLOR_BASE);
	darkPalette.setColor(QPalette::WindowText, Qt::cyan);
	darkPalette.setColor(QPalette::Base, DGStyle::COLOR_BASE);
	darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
	darkPalette.setColor(QPalette::ToolTipBase, Qt::darkGray);
	darkPalette.setColor(QPalette::ToolTipText, Qt::cyan);
	darkPalette.setColor(QPalette::Text, Qt::cyan);
	darkPalette.setColor(QPalette::Button, DGStyle::COLOR_BASE);
	darkPalette.setColor(QPalette::Background, DGStyle::COLOR_BASE);
	darkPalette.setColor(QPalette::ButtonText, Qt::cyan);
	darkPalette.setColor(QPalette::BrightText, Qt::green);
	darkPalette.setColor(QPalette::Link, DGStyle::COLOR_HILIGHT);

	darkPalette.setColor(QPalette::Highlight, DGStyle::COLOR_HILIGHT);
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);

	app->setPalette(darkPalette);

	app->setStyleSheet("QToolTip { color: #00ffff; background-color: #004090; border: 1px solid white; }");

}
