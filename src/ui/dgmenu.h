#ifndef DGMENUBARMENU_H
#define DGMENUBARMENU_H

#include <map>

#include <QMenu>
#include <QString>

/**
 * @brief DevGarden menu action management class.
 * Improves ease of management of actions in QMenus in DevGarden.
 * Does NOT own the menus that it manages.
 */
class DGMenu : public QObject {
	Q_OBJECT
public:
	explicit DGMenu(QMenu* menu) {this->menu = menu;}
	inline QMenu& getMenu() {return *menu;}

	DGMenu* addAction(const QString& id, const QString& text, const QKeySequence& keyseq, bool enabled = false);
	DGMenu* addAction(const QString& id, const QString& text, bool enabled);
	DGMenu* addMenu(const QString& text); //Uses new, remember to free the DGMenu returned by this.
	DGMenu* addSeparator();

	inline DGMenu*  addAction(const QString& text, const QKeySequence& keyseq, bool enabled = false)
		{return addAction(text,text,keyseq,enabled);}
	inline DGMenu*  addAction(const QString& text, bool enabled = false)
		{return addAction(text,text,enabled);}
	inline QAction* getAction(const QString& id)   {return items.at(id);}
private:
	std::map<QString,QAction*> items;
	QMenu* menu;
};

#endif // DGMENUBARMENU_H
