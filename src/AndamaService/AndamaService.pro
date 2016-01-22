QT += core
QT -= gui

TARGET = AndamaService
CONFIG += console
CONFIG -= app_bundle
include(../QtService/src/qtservice.pri)
include(../Andama/Andama.pri)

TEMPLATE = app

SOURCES += main.cpp \
    andamaservice.cpp

#INCLUDEPATH += /Users/yiannis/Projects/AndamaRemoteDesktop/AndamaGitHub/Andama-Remote-Desktop/src/QtService

HEADERS += \
    andamaservice.h

#on mac you should set DYLD_LIBRARY_PATH for the OSXObjectiveCBridge build folder like: DYLD_LIBRARY_PATH = /Users/your user name/Projects/AndamaRemoteDesktop/Builds/OSXObjectiveCBridge/Debug  (on Projects > Run > Run Environment)
macx: LIBS += -L$$PWD/../../../../Builds/OSXObjectiveCBridge/Release/ -lOSXObjectiveCBridge
INCLUDEPATH += $$PWD/../OSXObjectiveCBridge
DEPENDPATH += $$PWD/../OSXObjectiveCBridge

DEFINES += SERVICE


