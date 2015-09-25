#include "dgmenu.h"

DGMenu* DGMenu::addAction(const QString& id, const QString& text, const QKeySequence& keyseq, bool enabled) {
	QAction* act = menu->addAction(tr(text.toLocal8Bit()));
	act->setShortcut(keyseq);
	act->setEnabled(enabled);
	items.insert(std::make_pair(id,act));
	return this;
}

DGMenu* DGMenu::addAction(const QString& id, const QString& text, bool enabled) {
	QAction* act = menu->addAction(tr(text.toLocal8Bit()));
	act->setEnabled(enabled);
	items.insert(std::make_pair(id,act));
	return this;
}

DGMenu* DGMenu::addMenu(const QString& text) {
	return new DGMenu(menu->addMenu(tr(text.toLocal8Bit())));
}

DGMenu* DGMenu::addSeparator() {
	menu->addSeparator();
	return this;
}
