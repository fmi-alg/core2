#
# rod_3d_qt.pro
#
# Created on: June 30, 2018
# Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
#
# Copyright (c) 2015-2018 Ching-Hsiang Hsu
#

QT       += \
  core \
  gui \
  opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = rod_3d
TEMPLATE = app

# unorder_set (Ching-Hsiang Hsu)
CONFIG += c++11

SOURCES += \
  main.cpp \
  CORE/MpfrIO.cpp \
  CORE/linearAlgebra.cpp \
  CORE/geom3d/point3d.cpp \
  CORE/geom3d/line3d.cpp \
  CORE/geom3d/segment3d.cpp \
  CORE/geom3d/triangle3d.cpp \
  CORE/geom3d/polygon3d.cpp \
  CORE/geom3d/plane3d.cpp \
  path/path.cpp \
    geometry/corner.cpp \
    geometry/edge.cpp \
    geometry/wall.cpp \
    predicate/configuration_box3d_predicate.cpp \
    sss/soft_subdivision_search.cpp \
    subdivision/configuration_box3d.cpp \
    utility/timer.cpp \
    display/display.cpp \
    display/main_window.cpp

HEADERS  += \
  main.h \
  CORE/CORE.h \
  CORE/linearAlgebra.h \
  CORE/geom3d/point3d.h \
  CORE/geom3d/line3d.h \
  CORE/geom3d/plane3d.h \
  CORE/geom3d/segment3d.h \
  CORE/geom3d/triangle3d.h \
  CORE/geom3d/polygon3d.h \
    geometry/feature.h \
    geometry/edge.h \
    geometry/corner.h \
    geometry/wall.h \
    utility/timer.h \
    utility/parser.h \
    subdivision/status.h \
    subdivision/rot3d.h \
    subdivision/configuration_box3d.h \
    subdivision/box3d.h \
    strategy/box_priority_queue.h \
    sss/union_find.h \
    sss/soft_subdivision_search.h \
    sss/graph.h \
    predicate/configuration_box3d_predicate.h \
    path/path.h \
    display/main_window.h \
    display/display.h \
    display/color.h


FORMS += \
    display/main_window.ui


INCLUDEPATH += \
  /usr/local/Cellar/gmp/6.1.1/include/ \
  /usr/local/Cellar/mpfr/3.1.5/include/


LIBS += \
  -L/usr/local/Cellar/gmp/6.1.1/lib/ -lgmp \
  -L/usr/local/Cellar/mpfr/3.1.5/lib/ -lmpfr
