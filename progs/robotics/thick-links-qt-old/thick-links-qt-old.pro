QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = thick-links-qt-old
TEMPLATE = app


SOURCES +=\
    MainWindow.cpp \
    Box.cpp \
    Corner.cpp \
    Polygon.cpp \
    QuadTree.cpp \
    Timer.cpp \
    thick-links-qt-old.cpp \
    Wall.cpp \
    fileProcessor.cpp \
    Display.cpp \
    shader.cpp \
    ShaderException.cpp

HEADERS  += MainWindow.h \
    Box.h \
    Corner.h \
    Graph.h \
    line2d.h \
    Polygon.h \
    QuadTree.h \
    Timer.h \
    UnionFind.h \
    PriorityQueue.h \
    Feature.h \
    Wall.h \
    Display.h \
    shader.h \
    ShaderException.h

FORMS    += MainWindow.ui

DISTFILES += \
    simple.frag \
    simple.vert
