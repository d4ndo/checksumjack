#-------------------------------------------------
#
# Project created by QtCreator 2014-09-07T00:57:43
#
#-------------------------------------------------

QT       += widgets testlib core

TARGET = tst_unittest
DESTDIR = ../build_unittest/
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_unittest.cpp \
        ../src/fileio.cpp \
        ../src/hashfileio.cpp \
        ../src/detector.cpp

HEADERS += ../src/fileio.h \
           ../src/hashfileio.h \
           ../src/detector.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"
