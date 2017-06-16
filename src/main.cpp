#include "ui/dgwindow.h"
#include "ui/dgstyle.h"

#include "async/asyncwatcher.h"
#include "async/executor.h"

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

#include "envmacros.h"
#include "langregistry.h"
#include "dgcontroller.h"

namespace dg_consts {
	const QString STRING_EMPTY = "";
	const QString STRING_DIR_BUILD = "%tools/build/";
}

int main(int argc, char **argv) {
	QApplication a(argc, argv);
	a.setApplicationName(DG_NAME);

	std::cout << DG_NAME << " v. " <<
                 DG_VERSION_MAJOR << '.' <<
                 DG_VERSION_MINOR << '.' <<
                 DG_VERSION_PATCH << " on " <<
                 DG_ENV << std::endl;

	if(a.arguments().length() == 2 && a.arguments().at(1) == "--selftest") {
		std::cout << "Begin self-test." << std::endl;
		dg_utils::GlobalFlags::setTesting();
	}

	a.setWindowIcon(QIcon("://icon.png"));

	// Translation
	std::cout << "Loading translation..." << std::endl;
	QTranslator translator;
	translator.load(QString("DevGarden_%0").arg(QLocale::system().name()), QCoreApplication::applicationDirPath()); // Need to change from directory path.
	a.installTranslator(&translator);

	std::cout << "Loading configuration..." << std::endl;
	dg_utils::makeConfigDirs();
	ConfigFile f("config/editor.conf");

	std::cout << "Initializing..." << std::endl;
		DG_EC_SELFTEST_ECHO("init filesystem watcher")
	QFileSystemWatcher fsw;
	FileLoaderFS::setFileSystemWatcher(&fsw);
		DG_EC_SELFTEST_ECHO("init LangRegistry")
	std::unique_ptr<LangRegistry> lr(new LangRegistry);
		DG_EC_SELFTEST_ECHO("init AsyncWatcher")
	std::unique_ptr<AsyncWatcher> aw(new AsyncWatcher);
		DG_EC_SELFTEST_ECHO("init Executor")
	std::unique_ptr<Executor> exe(new Executor);
		DG_EC_SELFTEST_ECHO("init FileCache")
	std::unique_ptr<DGFileCache> fc(new DGFileCache(*lr));
		DG_EC_SELFTEST_ECHO("TEST: init ProjectLoader")
	std::unique_ptr<DGProjectLoader> pl(new DGProjectLoader(*lr));

	//Would normally assure correct pluralization here, but these are console status messages.
	std::cout << "Loaded " << lr->countLanguages() << " languages" << std::endl;
	std::cout << "Loaded " << lr->countBindings() << " file associations" << std::endl;

		DG_EC_SELFTEST_ECHO("TEST: init DGController")
	DGController* ctrl = new DGController(pl.get(), fc.get(), lr.get(), exe.get());
		DG_EC_SELFTEST_ECHO("TEST: init DGWindow")
	std::unique_ptr<DGWindow>w (new DGWindow(ctrl, *lr, exe.get()));
	ctrl->setView(w.get());
	fc->bindController(ctrl);
	w->configure(f);
	DGStyle::applyStyle(&a);

	DG_EC_SELFTEST_ECHO("PASS: init")
	else {
		std::cout << "Finished loading " << DG_NAME << std::endl;
		w->show();
		int retval = a.exec();
		delete ctrl;
		return retval;
	}
	//TODO: More tests.
	delete ctrl;
	return 0;
}

//This comment made by DevGarden! DO NOT REMOVE.
