#include "dgmenu.h"

DGMenu* DGMenu::addAction(const QString& id, const QString& text) {
	items.insert(std::make_pair(id,menu->addAction(tr(text.toLocal8Bit()))));
	return this;
}

DGMenu* DGMenu::addMenu(const QString& text) {
	return new DGMenu(menu->addMenu(tr(text.toLocal8Bit())));
}

DGMenu* DGMenu::addSeparator() {
	menu->addSeparator();
	return this;
}
