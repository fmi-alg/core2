/* **************************************
   File: point-inl.h

   Description: 
	Point code factored out from cxy.cpp.


   HISTORY: 2008: Long Lin (Java Code for cxy paper)
	    Apr, 2009: Shuxing Lu (converted Java code to Core2)
	    Jun, 2010: Narayan Kamath (refactored code into independent units)

   Since Core Library  Version 2.0
   $Id: point-inl.h,v 1.2 2010/06/10 22:03:09 exact Exp $
 ************************************** */

#ifndef CXY2_POINT_INL_H_
#define CXY2_POINT_INL_H_

#include "cxy-defs.h"

// A representation of a point in two dimensions. This
// class is immutable.
//
// NOTE : Apart from the contains function , this could
// well have been represented by std::pair<double, double> ,
// this implementation is preserved in the case that we
// need to add more (geometric) functionality to this
// class.
class Point {
 public:
  Point(const double &x, const double &y) :
  x_(x), y_(y) {

  }

  const double getX() const {
  return x_;
  }

  const double getY() const {
  return y_;
  }

  // Return true if this point is contained in the edge
  // <a, b>
  const bool contains(const Point &a, const Point &b) const {
    bool ret_val = false;
    if (x_ == a.getX() && x_ == b.getX()) {
    if ((a.getY() < y_ && y_ < b.getY()) ||
      (a.getY() > y_ && y_ > b.getY())) {
    ret_val = true;
    }
  }

    if (y_ == a.getY() && y_ == b.getY()) {
    if ((a.getX() < x_ && x_ < b.getX()) ||
      (a.getX() > x_ && x_ > b.getX())) {
    ret_val = true;
    }
    }

    return ret_val;
  }

  // Return the arithmetic mean of a & b .
  static const Point mid(const Point &a, const Point &b) {
  return Point((a.getX()+b.getX())/2, (a.getY()+b.getY())/2);
  }
  // Overloaded version of the above, pure syntactic sugar.
  static const Point mid(const Point *a, const Point *b) {
    return mid(*a, *b);
  }

 private:
  const double x_;
  const double y_;
};

#endif   // CXY2_POINT_INL_H_
