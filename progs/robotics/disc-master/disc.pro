#-------------------------------------------------
#
# Project created by QtCreator 2015-07-29T15:47:06
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 3)
greaterThan(QT_MINOR_VERSION, 3): QT += widgets

TARGET = disc
TEMPLATE = app

#QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11

SOURCES +=\
        ControlWindow.cpp \
    Box.cpp \
    Corner.cpp \
    disc.cpp \
    QuadTree.cpp \
    Timer.cpp \
    Wall.cpp \
    fileProcessor.cpp \
    Display.cpp \
    shader.cpp \
    ShaderException.cpp


HEADERS  += ControlWindow.h \
    Box.h \
    Corner.h \
    QuadTree.h \
    Timer.h \
    Wall.h \
    Corelo.h \
    Graph.h \
    PriorityQueue.h \
    UnionFind.h \
    Display.h \
    shader.h \
    ShaderException.h

DISTFILES += \
    envGeneratory.py \
    simple.vert \
    simple.frag

FORMS += \
    ControlWindow.ui

RESOURCES += \
    core-profile.qrc
