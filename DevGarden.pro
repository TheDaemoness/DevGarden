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
	src/configloader.cpp \
    src/ui/editor/codeeditorwidget.cpp \
	src/ui/editor/syntaxhighlighter.cpp \
    src/filesys/dgprojectloader.cpp \
	src/filesys/dgprojectinfo.cpp

HEADERS  += \
    src/ui/dgwindow.h \
    src/dgcontroller.h \
    src/util/stringtree.h \
    src/ui/dgcentralwidget.hpp \
    src/configloader.h \
	src/envmacros.h \
    src/ui/editor/codeeditorwidget.h \
    src/ui/editor/linenumberarea.h \
	src/ui/editor/syntaxhighlighter.h \
    src/filesys/dgprojectloader.h \
	src/filesys/dgprojectinfo.h

TRANSLATIONS = DevGarden_fr.ts

RESOURCES += \
    assets/icons.qrc

OTHER_FILES += \
    src/CODESTYLE.md
