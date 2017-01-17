QT += core
QT -= gui
QT += network

CONFIG += c++11

TARGET = UPnPManager
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    upnpdiscovery.cpp \
    upnpengine.cpp \
    upnpcommands.cpp

HEADERS += \
    upnpdiscovery.h \
    upnpengine.h \
    upnpcommands.h
