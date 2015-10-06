#-------------------------------------------------
#
# Project created by some lunatics in April 2015.
#
#-------------------------------------------------

cache()

QMAKE_CXXFLAGS += -pedantic -std=c++14

macx {
	QMAKE_CXXFLAGS += -stdlib=libc++
	QMAKE_LFLAGS += -lc++
	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
}

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DevGarden
TEMPLATE = app

macx:ICON = assets/icon.icns
win32:RC_ICONS += assets/icon.ico

SOURCES += \
    src/main.cpp \
    src/ui/dgwindow.cpp \
    src/dgcontroller.cpp \
    src/ui/dgcentralwidget.cpp \
    src/ui/editor/codeeditorwidget.cpp \
    src/filesys/dgprojectloader.cpp \
	src/filesys/dgprojectinfo.cpp \
    src/ui/dgstyle.cpp \
    src/configentry.cpp \
    src/ui/editor/oldsynhighlighter.cpp \
    src/ui/editor/syntaxhighlighter.cpp \
    src/textmatcher.cpp \
    src/langregistry.cpp \
    src/configfile.cpp \
    src/ui/dgmenu.cpp \
    src/utils.cpp \
    src/filesys/dgfilecache.cpp \
    src/filesys/fileloader.cpp \
    src/filesys/filedata.cpp \
    src/build/target.cpp \
    src/build/buildcontroller.cpp \
    src/asyncwatcher.cpp

HEADERS  += \
    src/ui/dgwindow.h \
    src/dgcontroller.h \
    src/ui/dgcentralwidget.hpp \
	src/envmacros.h \
    src/ui/editor/codeeditorwidget.h \
    src/ui/editor/linenumberarea.h \
    src/filesys/dgprojectloader.h \
	src/filesys/dgprojectinfo.h \
    src/dgdebug.hpp \
    src/ui/editor/codestyle.h \
    src/ui/dgstyle.h \
    src/configentry.h \
    src/ui/editor/oldsynhighlighter.h \
    src/ui/editor/syntaxhighlighter.h \
    src/textmatcher.h \
    src/langregistry.h \
    src/configfile.h \
    src/ui/dgmenu.h \
    src/utils.h \
    src/filesys/dgfilecache.h \
    src/filesys/fileloader.h \
    src/filesys/filedata.h \
    src/build/target.h \
    src/build/buildcontroller.h \
    src/consts.h \
    src/asyncwatcher.h

TRANSLATIONS = DevGarden_fr.ts

RESOURCES += \
    assets/icons.qrc

OTHER_FILES +=
