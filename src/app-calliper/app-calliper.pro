#-------------------------------------------------
#
# Project created by QtCreator 2016-12-14T08:19:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calliper
TEMPLATE = app


SOURCES += main.cpp\
        widgets/mainwindow.cpp \
    widgets/projectfiledockwidget.cpp

HEADERS  += widgets/mainwindow.h \
    widgets/projectfiledockwidget.h

FORMS    += mainwindow.ui
