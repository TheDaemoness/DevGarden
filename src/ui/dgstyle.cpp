#include "dgstyle.h"

#include <QApplication>
#include <QStyleFactory>
#include <QPalette>

void DGStyle::applyStyle(QApplication* app) {

	app->setStyle(QStyleFactory::create("Fusion"));

	QPalette darkPalette;
	darkPalette.setColor(QPalette::Window, DGStyle::COLOR_BASE);
	darkPalette.setColor(QPalette::WindowText, DGStyle::COLOR_TEXT);
	darkPalette.setColor(QPalette::Base, DGStyle::COLOR_BASE);
	darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
	darkPalette.setColor(QPalette::ToolTipBase, Qt::darkGray);
	darkPalette.setColor(QPalette::ToolTipText, DGStyle::COLOR_TEXT);
	darkPalette.setColor(QPalette::Text, DGStyle::COLOR_TEXT);
	darkPalette.setColor(QPalette::Button, DGStyle::COLOR_BASE);
	darkPalette.setColor(QPalette::Background, DGStyle::COLOR_BASE);
	darkPalette.setColor(QPalette::ButtonText, DGStyle::COLOR_TEXT);
	darkPalette.setColor(QPalette::BrightText, DGStyle::COLOR_TEXT_BRIGHT);
	darkPalette.setColor(QPalette::Link, DGStyle::COLOR_HILIGHT);

	darkPalette.setColor(QPalette::Disabled, QPalette::Base, DGStyle::COLOR_BASE_DISABLED);
	darkPalette.setColor(QPalette::Disabled, QPalette::Background, DGStyle::COLOR_BASE_DISABLED);
	darkPalette.setColor(QPalette::Disabled, QPalette::Text, DGStyle::COLOR_TEXT_DISABLED);
	darkPalette.setColor(QPalette::Disabled, QPalette::Foreground, DGStyle::COLOR_TEXT_DISABLED);
	darkPalette.setColor(QPalette::Disabled, QPalette::Button, DGStyle::COLOR_BASE_DISABLED);
	darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, DGStyle::COLOR_TEXT_DISABLED);

	darkPalette.setColor(QPalette::ToolTipText, DGStyle::COLOR_TEXT);
	darkPalette.setColor(QPalette::ToolTipBase, DGStyle::COLOR_BASE_DISABLED);
	darkPalette.setColor(QPalette::Shadow, DGStyle::COLOR_TEXT_HILIGHTED);
	darkPalette.setColor(QPalette::Highlight, DGStyle::COLOR_HILIGHT);
	darkPalette.setColor(QPalette::HighlightedText, DGStyle::COLOR_TEXT_HILIGHTED);

	app->setPalette(darkPalette);

	app->setStyleSheet("QToolTip { border: 1px solid black; }");

}
