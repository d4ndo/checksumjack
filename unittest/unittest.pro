#-------------------------------------------------
#
# Project created by QtCreator 2014-09-07T00:57:43
#
#-------------------------------------------------

QT       += widgets script testlib

TARGET = tst_unittest
DESTDIR = ../build_unittest/
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_unittest.cpp \
        ../src/fileio.cpp \
        ../src/hashfileio.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
