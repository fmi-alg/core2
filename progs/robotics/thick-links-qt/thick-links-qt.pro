#
#  thick-links-qt.pro
#  
#  Author: Ching-Hsiang (Tom) Hsu
#  Email: chhsu@nyu.edu
#  Modified: Ching-Hsiang (Tom) Hsu, Oct. 31, 2016
#


QT       += \
core gui \
opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = thick-links-qt
TEMPLATE = app


SOURCES +=\
    MainWindow.cpp \
    Box.cpp \
    Corner.cpp \
    Polygon.cpp \
    QuadTree.cpp \
    Timer.cpp \
    thick-links-qt.cpp \
    #thick-links-test.cpp \
    Wall.cpp \
    fileProcessor.cpp \
    Display.cpp \
    triangulate.cpp

HEADERS  += \
    MainWindow.h \
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
    Vertex.h \
    Pose.h \
    triangulate.h \
    Triangle.h

FORMS    += MainWindow.ui
