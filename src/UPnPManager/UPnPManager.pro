QT += core
QT -= gui
QT += network

CONFIG += c++11

TARGET = UPnPManager
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../Shared/UPnP/upnpcommands.cpp \
    ../Shared/UPnP/upnpdiscovery.cpp \
    ../Shared/UPnP/upnpengine.cpp \
    ../Shared/UPnP/addportmappingresponse.cpp

HEADERS += \
    ../Shared/UPnP/upnpcommands.h \
    ../Shared/UPnP/upnpdiscovery.h \
    ../Shared/UPnP/upnpengine.h \
    ../Shared/General/finally.h \
    ../Shared/UPnP/upnpcommandresponse_copy.h \
    ../Shared/UPnP/addportmappingresponse.h
