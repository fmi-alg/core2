#-------------------------------------------------
#
# Project created by QtCreator 2015-08-15T12:35:03
#
#-------------------------------------------------

QT       += \
core gui \
opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = complex-general-qt
TEMPLATE = app

##########################
# This is used on Mac OS
#
#INCLUDEPATH += /System/Library/Frameworks/OpenGL.framework/Headers
#LIBS += -F/Library/Frameworks
#LIBS += -framework OpenGL

##########################
# This is used on Linux
#
LIBS += -lGLU

SOURCES +=\
    MainWindow.cpp \
    Box.cpp \
    Corner.cpp \
    Polygon.cpp \
    QuadTree.cpp \
    Timer.cpp \
    complex-general-qt.cpp \
    Wall.cpp \
    fileProcessor.cpp \
    Display.cpp \
    DisplayRobot.cpp \
    Triangulate.cpp \
    BasisShape.cpp

HEADERS  += MainWindow.h \
    Box.h \
    Corner.h \
    Graph.h \
    Line2d.h \
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
    Triangle.h \
    complex.h \
    complex-general-qt.h \
    BasisShape.h \
    Circle.h


FORMS    += \
MainWindow.ui
