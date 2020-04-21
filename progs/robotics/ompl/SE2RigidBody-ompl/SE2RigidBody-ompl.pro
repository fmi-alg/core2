#
#   Author: Ching-Hsiang (Tom) Hsu
#   Email: chhsu@nyu.edu
#   Modified: Oct. 31, 2016
#
#

QT       += \
core gui \
opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SE2RigidBody-ompl
TEMPLATE = app


SOURCES += \
    main.cpp

LIBS += -L"/opt/local/lib/" -lompl -lompl_app_base

INCLUDEPATH += \
    /opt/local/include/ \
    /

CONFIG += c++11
