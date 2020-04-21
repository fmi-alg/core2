
QT       += \
  core \
  gui \
  opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = ring-3d-qt
TEMPLATE = app

# unorder_set (Tom)
CONFIG += c++11

SOURCES += \
  main.cpp \
  CORE/MpfrIO.cpp \
  CORE/linearAlgebra.cpp \
  CORE/geom3d/point3d.cpp \
  CORE/geom3d/line3d.cpp \
  CORE/geom3d/segment3d.cpp \
  CORE/geom3d/triangle3d.cpp \
  CORE/geom3d/circle3d.cpp \
  CORE/geom3d/polygon3d.cpp \
  CORE/geom3d/plane3d.cpp \
  geometry/Corner.cpp \
  geometry/Edge.cpp \
  geometry/Wall.cpp \
  geometry/Square.cpp \
  subdivision/ConfBox3d.cpp \
  sss/SoftSubdivisionSearch.cpp \
  predicate/ConfBox3dPredicate.cpp \
  path/Path.cpp \
  display/MainWindow.cpp \
  display/Display.cpp \
  utility/Timer.cpp \
  utility/fileProcessor.cpp

HEADERS  += \
  main.h \
  CORE/CORE.h \
  CORE/linearAlgebra.h \
  CORE/geom3d/point3d.h \
  CORE/geom3d/line3d.h \
  CORE/geom3d/plane3d.h \
  CORE/geom3d/segment3d.h \
  CORE/geom3d/triangle3d.h \
  CORE/geom3d/circle3d.h \
  CORE/geom3d/polygon3d.h \
  geometry/Corner.h \
  geometry/Edge.h \
  geometry/Wall.h \
  geometry/Square.h \
  subdivision/Box3d.h \
  subdivision/Rot3dSide.h \
  subdivision/Status.h \
  subdivision/ConfBox3d.h \
  sss/SoftSubdivisionSearch.h \
  sss/Graph.h \
  sss/UnionFind.h \
  predicate/ConfBox3dPredicate.h \
  strategy/PriorityQueue.h \
  path/Path.h \
  display/MainWindow.h \
  display/Display.h \
  display/Color.h \
  utility/Parser.h \
  utility/Timer.h


FORMS += \
  display/MainWindow.ui


INCLUDEPATH += \
  /usr/local/Cellar/gmp/6.1.1/include/ \
  /usr/local/Cellar/mpfr/3.1.5/include/


LIBS += \
  -L/usr/local/Cellar/gmp/6.1.1/lib/ -lgmp \
  -L/usr/local/Cellar/mpfr/3.1.5/lib/ -lmpfr
