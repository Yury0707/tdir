#-------------------------------------------------
#
# Project created by QtCreator 2018-08-03T07:58:36
#
#-------------------------------------------------

VERSION = 1.0.0.1

QT       += core

QT       -= gui

TARGET = tdir
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += APP_VERSION=\\\"$$VERSION\\\"\
    APP_NAME=\\\"$$TARGET\\\"\
    APP_AUTHOR="\"\\\"Yury Osipov\\\"\""\
    APP_E_MAIL=\\\"yusosipov@yandex.ru\\\"

# Uncomment it to see debug messages
# DEFINES += __TDIR_DEBUG__

SOURCES += main.cpp \
    fileenumerator.cpp

HEADERS += \
    fileenumerator.h \
    exitcodes.h \
    options.h

QMAKE_TARGET_PRODUCT = tdir
QMAKE_TARGET_DESCRIPTION = Directory file enumerator
QMAKE_TARGET_COPYRIGHT = (c) Yury Osipov 2018

CONFIG -= debug_and_release debug_and_release_target

# Setting paths for compiled binaries
CONFIG(debug, debug|release):DESTDIR = bin/debug
CONFIG(release, debug|release):DESTDIR = bin

# Temporary moc files destination
MOC_DIR = moc

# Object files destination
OBJECTS_DIR = obj
