#include "ui/dgwindow.h"
#include "ui/dgstyle.h"

#include "filesys/dgprojectloader.h"
#include "filesys/dgfilecache.h"

#include <QApplication>
#include <QTranslator>
#include <QStyleFactory>
#include <QLocale>

#include <memory>
#include <iostream>

#include "configfile.h"
#include "utils.h"
#include "build/buildcontroller.h"

#include "envmacros.h"
#include "langregistry.h"
#include "dgcontroller.h"

int main(int argc, char **argv) {

	QApplication a(argc, argv);
	a.setApplicationName(DG_NAME);

	std::cout << DG_NAME << " v. " <<
				 DG_VERSION_MAJOR << '.' <<
				 DG_VERSION_MINOR << '.' <<
				 DG_VERSION_PATCH << " on " <<
				 DG_ENV << std::endl;


	a.setWindowIcon(QIcon("://icon.png"));

	// Translation
	QTranslator translator;
	translator.load(QString("DevGarden_%0").arg(QLocale::system().name()), QCoreApplication::applicationDirPath()); // Need to change from directory path.
	a.installTranslator(&translator);

	std::cout << "Loading configuration..." << std::endl;
	dg_utils::makeConfigDirs();

	ConfigFile f("config/editor.conf");

	std::cout << "Initializing..." << std::endl;
	std::unique_ptr<LangRegistry> lr(new LangRegistry);
	std::unique_ptr<DGFileCache> fl(new DGFileCache(*lr.get()));
	std::unique_ptr<DGProjectLoader> pl(new DGProjectLoader(*lr));
	std::unique_ptr<BuildController> bc(new BuildController(*pl));

	//Would normally assure correct pluralization here, but these are console status messages.
	std::cout << "Loaded " << lr->countLanguages() << " languages" << std::endl;
	std::cout << "Loaded " << lr->countBindings() << " file associations" << std::endl;

	DGController* ctrl = new DGController(pl.get(), fl.get(), lr.get(), bc.get());
	std::unique_ptr<DGWindow>w (new DGWindow(ctrl, *lr));
	ctrl->setView(w.get());

	w->configure(f);
	DGStyle::applyStyle(&a);
	bc->setStartStopTriggers([ctrl]{ctrl->sigBuildStarted();},[ctrl]{ctrl->sigBuildStopped();});

	std::cout << "Finished loading " << DG_NAME << std::endl;
	w->show();
	int retval = a.exec();
	delete ctrl;
	return retval;
}

//This comment made by DevGarden! DO NOT REMOVE.
