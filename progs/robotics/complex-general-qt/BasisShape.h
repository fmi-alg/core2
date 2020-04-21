/*
 * file: BasisShape.h
 * This is the file to define 0/1/2-basis shapes for complex robot
 *
 * 	Author: Bo Zhou
 * ***************************************************/

#pragma once

#include "Line2d.h"
#include "Wall.h"
#include "Corner.h"
#include "Circle.h"
#include "Triangle.h"

#include <vector>
#include <stdio.h>
#include <math.h>
using std::vector;

class BasisShape
{
    /* BasisShapes:
     * 0-basis shape: a half-space or a disk or complement of a disk
     * 1-basis shape: intersections of 0-basis shapes, defined by a list 0-basis shapes
     * 2-basis shape: unions of 1-basis shapes, defined by a list of 1-basis shapes
     */
    Line2d line; // the half-space is defined by the line ax+by+c<=0
    Circle circ;
    bool isComp;
public:
    int degree;
    vector<BasisShape> list;

    BasisShape() {}
    BasisShape(int _degree) {
        degree = _degree;
    }
    BasisShape(Line2d _line) {
        degree = 0;
        line = _line;
    }
    BasisShape(Circle _circ, bool comp) {
        degree = 0;
        circ = _circ;
        isComp = comp;
    }

    bool isIntersect(Wall &w);
    bool isIntersect(Corner &c);
    void chope(Wall &w, Line2d &lwall) ;
};

