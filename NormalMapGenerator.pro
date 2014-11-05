#-------------------------------------------------
#
# Project created by QtCreator 2014-10-13T00:19:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NormalMapGenerator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fragmentpane.cpp

HEADERS  += mainwindow.h \
    fragmentpane.h

FORMS    += mainwindow.ui

QT += opengl

QMAKE_CXXFLAGS += -std=c++11

OTHER_FILES += \
    vertexShader.vert \
    fragShader.frag

RESOURCES += \
    NormalMapGenerator.qrc
