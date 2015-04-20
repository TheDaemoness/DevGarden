#include "ui/dgwindow.h"
#include "envmacros.h"

#include "filesys/dgprojectloader.h"

#include <QApplication>
#include <QTranslator>

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

	DGProjectLoader loader;
	DGController ctrl(&loader);
	DGWindow w(&ctrl);

	w.show();

	return a.exec();
}
