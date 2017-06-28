!include("../../retroshare.pri"): error("Could not include file ../../retroshare.pri")

TARGET = retroshare-android-notify-service

QT += core network qml
QT -= gui

CONFIG += c++11
CONFIG += dll

HEADERS += rsqmlappengine.h

RESOURCES += qml.qrc


TEMPLATE = app

android-g++ {
    TEMPLATE = lib
    QT += androidextras
    SOURCES += NativeCalls.cpp
    HEADERS += NativeCalls.h
}

HEADERS += libresapilocalclient.h notificationsbridge.h
SOURCES += libresapilocalclient.cpp main.cpp \
    rsqmlappengine.cpp

DEPENDPATH *= ../../libretroshare/src
INCLUDEPATH *= ../../libretroshare/src
PRE_TARGETDEPS *= ../../libretroshare/src/lib/libretroshare.a
LIBS *= ../../libretroshare/src/lib/libretroshare.a
