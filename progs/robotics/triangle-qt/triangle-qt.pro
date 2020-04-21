#-------------------------------------------------
#
# Project created by QtCreator 2015-08-15T12:35:03
#
#-------------------------------------------------

QT       += \
core gui \
opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = triangle-qt
TEMPLATE = app


SOURCES +=\
    MainWindow.cpp \
    Box.cpp \
    Corner.cpp \
    Polygon.cpp \
    QuadTree.cpp \
    Timer.cpp \
    triangle-qt.cpp \
    Wall.cpp \
    fileProcessor.cpp \
    Display.cpp \
    DisplayRobot.cpp \
    triangulate.cpp

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
    Color.h \
    Pose.h \
    Vertex.h \
    DisplayRobot.h \
    triangulate.h \
    Triangle.h


FORMS    += \
MainWindow.ui
