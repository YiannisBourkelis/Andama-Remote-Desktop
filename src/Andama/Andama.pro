#-------------------------------------------------
#
# Project created by QtCreator 2014-09-27T04:27:25
#
#-------------------------------------------------
# * ***********************************************************************
# * Andama
# * (C) 2014 by Yiannis Bourkelis (hello@andama.org)
# *
# * This file is part of Andama.
# *
# * Andama is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * Andama is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with Andama.  If not, see <http://www.gnu.org/licenses/>.
# * ***********************************************************************/

#QMAKE_MAC_SDK = macosx10.11

include(mousekeyb/mousekeyb.pri)

QT         += core gui quick
windows:QT += winextras

CONFIG += c++11

linux:CONFIG += static
# ./configure -static -prefix /home/yiannis/Qt5.3.2-src -qt-xcb
# make
# make install
#linux

mac:QMAKE_LFLAGS += -F /System/Library/Frameworks/CoreFoundation.framework/
mac:LIBS += -framework CoreFoundation
mac:LIBS += -framework ApplicationServices
mac:LIBS += -framework IOKit #xreiazetai gia na ksypnaei to monitor

#@--gia na ginei compile se ubuntu 14 x64 prepei na orisw ta paths tis opengl gia kapoio logo
linux:QMAKE_LIBS_OPENGL     = /usr/lib/x86_64-linux-gnu/mesa/libGL.so.1
linux:QMAKE_LIBS_OPENGL_QT  = /usr/lib/x86_64-linux-gnu/mesa/libGL.so.1

#se windows build me minGW xreiazetai na prosthetw tis vivliothikes
win32-g++ {
LIBS += -lgdi32
LIBS += -lws2_32
}

#QMAKE_MAC_SDK = macosx10.11

#na ginetai comment afti i grammi an thelw na to treksw mesa apo ton QT Creator
#release:QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:\"level=\'asInvoker\' uiAccess=\'true\'\"
#LINK=/SUBSYSTEM:WINDOWS,5.01

linux:LIBS += -lXtst -lX11
windows:LIBS += -luser32
windows:LIBS += -lgdi32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

TARGET = Andama
TEMPLATE = app
VERSION = 0.1.0

DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
#mac:DEFINES += APP_MAC

SOURCES += main.cpp\
    clientserver.cpp \
    mainwindow.cpp \
    screenshotsworker.cpp \
    helperfuncs.cpp \
    keepalive.cpp \
    PasswordProtection.cpp \
    About.cpp \
    engine.cpp \
    screenshotprovider.cpp \
    p2pserver.cpp

HEADERS  += clientserver.h \
    mainwindow.h \
    screenshotsworker.h \
    helperfuncs.h \
    keepalive.h \
    PasswordProtection.h \
    About.h \
    engine.h \
    screenshotprovider.h \
    p2pserver.h

FORMS    += mainwindow.ui \
    About.ui

RESOURCES += \
    resources.qrc

OTHER_FILES +=


#on mac you should set DYLD_LIBRARY_PATH for the OSXObjectiveCBridge build folder like: DYLD_LIBRARY_PATH = /Users/your user name/Projects/AndamaRemoteDesktop/Builds/OSXObjectiveCBridge/Debug  (on Projects > Run > Run Environment)
macx: LIBS += -L$$PWD/../../../../Builds/OSXObjectiveCBridge/Release/ -lOSXObjectiveCBridge
INCLUDEPATH += $$PWD/../OSXObjectiveCBridge
DEPENDPATH += $$PWD/../OSXObjectiveCBridge

DISTFILES += \
    main.qml
