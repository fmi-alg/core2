QT       += \
  core \
  gui \
  opengl

greaterThan(QT_MINOR_VERSION, 5): QT += widgets

TARGET = disc-qt
TEMPLATE = app

CONFIG += c++11


SOURCES += \
  disc-qt.cpp \
  CORE/MpfrIO.cpp \
  CORE/linearAlgebra.cpp \
  CORE/geom2d/point2d.cpp \
  CORE/geom2d/line2d.cpp \
  CORE/geom2d/segment2d.cpp \
  CORE/geom2d/circle2d.cpp \
  CORE/geom2d/triangle2d.cpp \
  CORE/geom2d/polygon2d.cpp \
  geometry/Corner.cpp \
  geometry/Wall.cpp \
  subdivision/Box.cpp \
  subdivision/QuadTree.cpp \
  path/path.cpp \
  display/MainWindow.cpp \
  display/Display.cpp \
  utility/Timer.cpp \
  utility/fileProcessor.cpp


HEADERS  += \
  CORE/CORE.h \
  CORE/linearAlgebra.h \
  CORE/geom2d/point2d.h \
  CORE/geom2d/line2d.h \
  CORE/geom2d/segment2d.h \
  CORE/geom2d/circle2d.h \
  CORE/geom2d/triangle2d.h \
  CORE/geom2d/polygon2d.h \
  geometry/Corner.h \
  geometry/Wall.h \
  subdivision/Box.h \
  subdivision/status.h \
  subdivision/QuadTree.h \
  sss/Graph.h \
  sss/UnionFind.h \
  strategy/PriorityQueue.h \
  path/path.h \
  display/MainWindow.h \
  display/Display.h \
  utility/Timer.h


FORMS += \
  display/MainWindow.ui


INCLUDEPATH += \
  /usr/local/Cellar/gmp/6.1.1/include/ \
  /usr/local/Cellar/mpfr/3.1.5/include/


LIBS += \
  -L/usr/local/Cellar/gmp/6.1.1/lib/ -lgmp \
  -L/usr/local/Cellar/mpfr/3.1.5/lib/ -lmpfr
