#-------------------------------------------------
#
# Project created by QtCreator 2015-04-11T15:31:26
#
#-------------------------------------------------

cache()

QMAKE_CXXFLAGS += -pedantic -std=c++11

macx {
	QMAKE_CXXFLAGS += -stdlib=libc++
	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DevGarden
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/ui/dgwindow.cpp

HEADERS  += \
    src/ui/dgwindow.h

FORMS    +=
