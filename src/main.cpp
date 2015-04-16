#include "ui/dgwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char **argv) {
	QApplication a(argc, argv);

    // Translation
    QTranslator translator;
    translator.load(QString("DevGarden_{0}").arg(QLocale::system().name()), QCoreApplication::applicationDirPath()); // Need to change from directory path.
    a.installTranslator(&translator);

	DGController ctrl;
	DGWindow w(&ctrl);

	w.show();

	return a.exec();
}
