#include "ui/dgwindow.h"
#include "ui/dgstyle.h"

#include "filesys/dgprojectloader.h"
#include "filesys/dgfileloader.h"

#include <QApplication>
#include <QTranslator>
#include <QStyleFactory>

#include <memory>
#include <iostream>

#include "configloader.h"
#include "envmacros.h"
#include "langregistry.h"

#include "dgdebug.hpp"

int main(int argc, char **argv) {

	QApplication a(argc, argv);
	a.setApplicationName(DG_NAME);
	a.setWindowIcon(QIcon("://icon.png"));

	// Translation
	QTranslator translator;
	translator.load(QString("DevGarden_%0").arg(QLocale::system().name()), QCoreApplication::applicationDirPath()); // Need to change from directory path.
	a.installTranslator(&translator);

	std::cout << "Loading configuration..." << std::endl;
	makeConfigDirs();

	ConfigFile f("config/editor.conf");

	std::cout << "Initializing..." << std::endl;
	std::unique_ptr<DGFileLoader> fl(new DGFileLoader);
	std::unique_ptr<DGProjectLoader> pl(new DGProjectLoader);
	std::unique_ptr<LangRegistry> lr(new LangRegistry);

	//Would normally assure correct pluralization here, but these are console status messages.
	std::cout << "Loaded " << lr->countLanguages() << " languages" << std::endl;
	std::cout << "Loaded " << lr->countBindings() << " file associations" << std::endl;

	DGController ctrl(pl.get(), fl.get(), lr.get());
	DGWindow w(&ctrl);
	ctrl.setView(&w);

	w.configure(f);

	DGStyle::applyStyle(&a);

	std::cout << "Finished loading " << DG_NAME << std::endl;
	w.show();
	return a.exec();
}

//Second edit made by DevGarden!
