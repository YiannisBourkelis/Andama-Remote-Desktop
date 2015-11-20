#-------------------------------------------------
#
# Project created by QtCreator 2014-09-30T00:22:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testwindows
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientserver.cpp \
    cs_thread.cpp \
    screenshotworker.cpp

HEADERS  += mainwindow.h \
    clientserver.h \
    cs_thread.h \
    screenshotworker.h

FORMS    += mainwindow.ui
