#include "dgstyle.h"

#include <QApplication>
#include <QStyleFactory>
#include <QPalette>

#include "../dgdebug.hpp"

void DGStyle::applyStyle(QApplication* app) {

	app->setStyle(QStyleFactory::create("Fusion"));

	QPalette darkPalette;
	darkPalette.setColor(QPalette::Window, QColor(16,32,40));
	darkPalette.setColor(QPalette::WindowText, Qt::cyan);
	darkPalette.setColor(QPalette::Base, QColor(16,32,40));
	darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
	darkPalette.setColor(QPalette::ToolTipBase, QColor(32, 192, 128));
	darkPalette.setColor(QPalette::ToolTipText, Qt::cyan);
	darkPalette.setColor(QPalette::Text, Qt::cyan);
	darkPalette.setColor(QPalette::Button, QColor(16,32,40));
	darkPalette.setColor(QPalette::Background, QColor(16,32,40));
	darkPalette.setColor(QPalette::ButtonText, Qt::cyan);
	darkPalette.setColor(QPalette::BrightText, Qt::green);
	darkPalette.setColor(QPalette::Link, QColor(32, 192, 128));

	darkPalette.setColor(QPalette::Highlight, QColor(32, 192, 128));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);

	app->setPalette(darkPalette);

	app->setStyleSheet("QToolTip { color: #00ffff; background-color: #004090; border: 1px solid white; }");

}
