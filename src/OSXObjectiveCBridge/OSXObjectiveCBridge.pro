#-------------------------------------------------
#
# Project created by QtCreator 2015-10-27T03:10:42
#
#-------------------------------------------------

QT       -= gui

TARGET = OSXObjectiveCBridge
TEMPLATE = lib

mac:LIBS += -framework Foundation

DEFINES += LIBOSXOBJECTIVECBRIDGE_LIBRARY

SOURCES +=

HEADERS += osxobjectivecbridge.h\
        osxobjectivecbridge_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OBJECTIVE_SOURCES += \
    osxobjectivecbridge.mm
