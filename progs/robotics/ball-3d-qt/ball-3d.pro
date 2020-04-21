#-------------------------------------------------
#
# Project created by QtCreator 2015-10-14T20:36:30
#
#-------------------------------------------------

# add opengl (Tom)
QT       += \
core gui \
opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ball-3d
TEMPLATE = app

# unorder_set (Tom)
CONFIG += c++11

SOURCES += \
main.cpp \
Display.cpp \
#Shader.cpp \
#ShaderException.cpp \
./geometry/Corner.cpp \
./geometry/Edge.cpp \
./geometry/Wall.cpp \
./path/Path.cpp \
./predicate/ConfBox3dPredicate.cpp \
./sss/SoftSubdivisionSearch.cpp \
./subdivision/ConfBox3d.cpp \
./utility/fileProcessor.cpp \
./utility/Timer.cpp \
    MainWindow.cpp

HEADERS  += \
main.h \
Display.h \
#Shader.h \
#ShaderException.h \
./geometry/Corner.h \
./geometry/Edge.h \
./geometry/Obstacle.h \
./geometry/Vector.h \
./geometry/Wall.h \
./geometry/./geometry/Corner.cppector.h \
./path/Path.h \
./predicate/ConfBox3dPredicate.h \
./robot/ball.h \
./robot/rod.h \
./sss/Graph.h \
./sss/SoftSubdivisionSearch.h \
./sss/UnionFind.h \
./strategy/PriorityQueue.h \
./subdivision/Box3d.h \
./subdivision/ConfBox3d.h \
./subdivision/Rot3dSide.h \
./subdivision/Status.h \
./utility/Parser.h \
./utility/Timer.h \
    MainWindow.h

FORMS    += \
MainWindow.ui

DISTFILES += \
input_interpreter.py
#simple.frag \
#simple.vert
