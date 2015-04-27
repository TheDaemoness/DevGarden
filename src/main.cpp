#include "ui/dgwindow.h"
#include "envmacros.h"

#include "filesys/dgprojectloader.h"
#include "filesys/dgfileloader.h"

#include <QApplication>
#include <QTranslator>

#include <memory>

#include "configloader.h"

int main(int argc, char **argv) {
	QApplication a(argc, argv);
	a.setApplicationName("DevGarden");
	a.setWindowIcon(QIcon("://icon.png"));\

	// Translation
	QTranslator translator;
	translator.load(QString("DevGarden_{0}").arg(QLocale::system().name()), QCoreApplication::applicationDirPath()); // Need to change from directory path.
	a.installTranslator(&translator);

	makeConfigDirs();

	std::unique_ptr<DGFileLoader> fl;
	fl.reset(new DGFileLoader());
	DGProjectLoader loader;
	DGController ctrl(&loader, fl.get());
	DGWindow w(&ctrl);

	ctrl.setView(&w);
	w.show();

	return a.exec();
}

//Second edit made by DevGarden!
