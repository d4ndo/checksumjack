# -------------------------------------------------
# Project created by QtCreator 2011-03-21T18:57:58
# -------------------------------------------------
TARGET = checksumJack
DESTDIR = ../build/
target.path = /usr/bin/
unix:INCLUDEPATH += "/usr/include"
unix:LIBS +=  -L"/usr/lib/" -lssl -lcrypto

unix:icon.files += icon/mime/checksumJack.png
unix:icon.files += icon/mime/*.svg
unix:icon.path += /usr/local/share/icons/

unix:mimetype.files += ../linux/checksumJack.xml
unix:mimetype.path = /usr/share/mime/packages

unix:shortcut.files += ../linux/checksumJack.desktop
unix:shortcut.path = /usr/share/applications/

INSTALLS += target
INSTALLS += icon
INSTALLS += mimetype
INSTALLS += shortcut
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
    properties.cpp \
    options.cpp \
    detector.cpp
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
    properties.h \
    options.h \
    detector.h
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

win32:INCLUDEPATH += "C:/openssl-win64/include/"
win32:LIBS += -L"C:/openssl-win64/lib" -llibeay32
win32:RC_FILE = checksumjack_rc.rc

RESOURCES = ResourceCheckSumJack.qrc

#/cygdrive/c/Qt/4.8.3/bin/rcc.exe -name ResourceCheckSumJack ResourceCheckSumJack.qrc -o debug/qrc_ResourceCheckSumJack.cpp
