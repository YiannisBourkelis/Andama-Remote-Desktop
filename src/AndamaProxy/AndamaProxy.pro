#-------------------------------------------------
#
# Project created by QtCreator 2014-10-01T16:32:26
#
#-------------------------------------------------

QMAKE_MAC_SDK = macosx10.12

QT       += core
CONFIG   += c++11

QT       -= gui

TARGET = AndamaProxy
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../Shared/AndamaHeaders/byte_functions.cpp \
    ../Shared/AndamaHeaders/socket_functions.cpp \
    ../Shared/AndamaHeaders/exception_helper.cpp

HEADERS += \
    ../Shared/AndamaHeaders/shared_constants.h \
    ../Shared/AndamaHeaders/byte_functions.h \
    ../Shared/AndamaHeaders/shared_enums.h \
    clientinfo.h \
    ipprotection.h \
    ../Shared/AndamaHeaders/socket_functions.h \
    ../Shared/AndamaHeaders/exception_helper.h
