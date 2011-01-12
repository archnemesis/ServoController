#-------------------------------------------------
#
# Project created by QtCreator 2010-12-31T13:39:15
#
#-------------------------------------------------

QT       += core gui

TARGET = ServoController
TEMPLATE = app

include(install.pri)

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/win_qextserialport.cpp \
    src/qextserialport.cpp \
    src/qextserialenumerator.cpp \
    src/qextserialbase.cpp \
    src/posix_qextserialport.cpp \
    src/aboutdialog.cpp

HEADERS  += src/mainwindow.h \
    src/win_qextserialport.h \
    src/qextserialport.h \
    src/qextserialenumerator.h \
    src/qextserialbase.h \
    src/posix_qextserialport.h \
    src/aboutdialog.h

FORMS    += src/mainwindow.ui \
    src/aboutdialog.ui
DEFINES += _TTY_POSIX_

RESOURCES += \
    src/resources.qrc
