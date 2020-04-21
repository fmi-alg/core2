QT       += \
core \
gui \
opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = thick-links-qt-windows
TEMPLATE = app


SOURCES +=\
    MainWindow.cpp \
    Box.cpp \
    Corner.cpp \
    Polygon.cpp \
    QuadTree.cpp \
    Timer.cpp \
    thick-links-qt-windows.cpp \
#    thick-links-test.cpp \
    Wall.cpp \
    fileProcessor.cpp \
    Display.cpp \
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
    Vertex.h \
    Pose.h \
    triangulate.h \
    Triangle.h

FORMS    += MainWindow.ui

LIBS += -lopengl32 -lglu32
