# -------------------------------------------------
# Project created by QtCreator 2011-03-21T18:57:58
# -------------------------------------------------
TARGET = checksumJack
DESTDIR = ../build/
target.path = /usr/bin/
INSTALLS += target
TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    about.cpp \
    filtersettings.cpp \
    xmlconfigio.cpp \
    fileio.cpp \
    verifydialog.cpp \
    reportitem.cpp \
    displayrandomart.cpp \
    hashfileio.cpp \
    rootpath.cpp \
    properties.cpp
HEADERS += \
    about.h \
    filtersettings.h \
    xmlconfigio.h \
    globaldefs.h \
    fileio.h \
    verifydialog.h \
    reportitem.h \
    mainwindow.h \
    displayrandomart.h \
    hashfileio.h \
    rootpath.h \
    properties.h
FORMS += mainwindow.ui \
    about.ui \
    filtersettings.ui \
    verifydialog.ui \
    properties.ui

TRANSLATIONS = lang/checksumjack_de.ts
CODECFORTR = UTF-8
CONFIG += QT
QT *= xml
QT += widgets

unix:QMAKE_CXXFLAGS_RELEASE -= -O2
unix:QMAKE_CXXFLAGS_RELEASE += -O3
win32:QMAKE_CXXFLAGS_RELEASE += -Zi -Oy -Ox -GL
win32:QMAKE_LFLAGS_RELEASE += /LTCG

unix:INCLUDEPATH += "/usr/include"
unix:LIBS +=  -L"/usr/lib/" -lssl -lcrypto

win32:INCLUDEPATH += "C:/openssl-win64/include/"
win32:LIBS += -L"C:/openssl-win64/lib" -llibeay32
win32:RC_FILE = checksumjack_rc.rc

RESOURCES = ResourceCheckSumJack.qrc

#/cygdrive/c/Qt/4.8.3/bin/rcc.exe -name ResourceCheckSumJack ResourceCheckSumJack.qrc -o debug/qrc_ResourceCheckSumJack.cpp
