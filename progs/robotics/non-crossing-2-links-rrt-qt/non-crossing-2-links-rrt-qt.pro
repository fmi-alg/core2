#
# non-crossing-2-links-rrt-qt.pro
#
#  Author: Ching-Hsiang (Tom) Hsu
#  Email: chhsu@nyu.edu
#  Modified: Oct, 31, 2016
#

QT       += \
core gui \
opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app


SOURCES += \
    main.cpp \
    #main_test.cpp \
    polygon.cpp \
    RRT.cpp \
    MainWindow.cpp \
    Display.cpp \
    m_Triangulate.cpp \
    ./itree/EndPoint.cpp \
    ./itree/Interval.cpp \
    ./itree/RectKD.cpp \
    ./mathtool/Basic.cpp \
    ./mathtool/Gauss.cpp \
    ./mathtool/Quaternion.cpp


HEADERS  += \
    main.h \
    intersection.h \
    polygon.h \
    prm-graph.h \
    RRT.h \
    segseg.h \
    MainWindow.h \
    Display.h \
    m_Triangulate.h \
    ./graph/BaseGraph.h \
    ./graph/BasicDefns.h \
    ./graph/Defines.h \
    ./graph/Graph.h \
    ./graph/GraphAlgo.h \
    ./itree/EndPoint.h \
    ./itree/Interval.h \
    ./itree/MiddleStructure.h \
    ./itree/RectangleTree.h \
    ./itree/RectKD.h \
    ./mathtool/Basic.h \
    ./mathtool/Gauss.h \
    ./mathtool/Matrix.h \
    ./mathtool/Point.h \
    ./mathtool/Quaternion.h \
    ./mathtool/Vector.h \
    ./utils/Timer.h


FORMS    += MainWindow.ui

INCLUDEPATH += ./FIST/

LIBS += ../non-crossing-2-links-rrt-qt/FIST/libFIST.a
TARGET = non-crossing-2-links-rrt-qt
