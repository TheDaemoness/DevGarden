#-------------------------------------------------
#
# Project created by QtCreator 2015-04-11T15:31:26
#
#-------------------------------------------------

cache()

QMAKE_CXXFLAGS += -pedantic -std=c++11

macx {
	QMAKE_CXXFLAGS += -stdlib=libc++
	QMAKE_LFLAGS += -lc++
	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DevGarden
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/ui/dgwindow.cpp \
    src/dgcontroller.cpp \
    src/ui/dgcentralwidget.cpp \
    src/configloader.cpp \
    src/ui/editor/codeeditorwidget.cpp \
    src/ui/editor/syntaxhighlighter.cpp

HEADERS  += \
    src/ui/dgwindow.h \
    src/dgcontroller.h \
    src/util/stringtree.h \
    src/ui/dgcentralwidget.hpp \
    src/configloader.h \
    src/envmacros.h \
    src/ui/editor/codeeditorwidget.h \
    src/ui/editor/linenumberarea.h \
    src/ui/editor/syntaxhighlighter.h

FORMS    +=

TRANSLATIONS = DevGarden_fr.ts
