/* **************************************
   File: box.h

   Description: 
	Code factored out from cxy.cpp.

   HISTORY: 2008: Long Lin (Java Code for cxy paper)
	    Apr, 2009: Shuxing Lu (converted Java code to Core2)
	    Jun, 2010: Narayan Kamath (refactored code into independent units)

   Since Core Library  Version 2.0
   $Id: box.h,v 1.9 2010/09/15 15:10:31 exact Exp $
 ************************************** */

#ifndef CXY2_BOX_H_
#define CXY2_BOX_H_

#include <list>
#include <vector>

#include "cxy-defs.h"
#include "point-inl.h"

using namespace std;

class Box {
 public:
  // Construct a box with the specified extents.
  // a is the "bottom left" corner of the box, and
  // b is the "top right" corner of the box.
  // Define our own delete box to free memory.
  ~Box();
  Box(const Point &a, const Point &b);
  // Split the box into four equal sized boxes that
  // become the boxes children. Returns the vector
  // of children.
  const vector<Box *> *split();

  // Accessor for various properties of the box such as
  // the x and y extents (intervals), the corners of the box,
  // the children of this box (if split has been called), the
  // neighbours of the box and its size.
  inline const Interval *getX() const {
    return &x_;
  }
  inline const Interval *getY() const {
    return &y_;
  }
  inline const vector<Point *> *getCorners() const {
    return &corners_;
  }
  inline const vector<Box *> *getChildren() const {
    return &children_;
  }
  inline const vector<Box *> *getNeighbors() const {
    return &neighbors_;
  }
  const double getSize() const;
  const unsigned int getGeneration() const {
    return generation_;
  }
  // Returns the vertices of the curve that are within this
  // box.
  inline vector<Point *> *mutableCurveVertices() {
    return &curve_vertices_;
  }
  inline const vector<Point *> *getCurveVertices() const {
    return &curve_vertices_;
  }
  // return true if the edge (a,b) already contains curve
  // vertex..
  const bool curveVertexExists(const Point &a, const Point &b) const;
  const bool curveVertexExists(const Point &a) const;

  // return true if this box contains point a.
  const bool contains(const Point &a) const;
  // Returns true iff this box is the same as the box
  // "ref".
  const bool operator==(const Box &ref) const;

  // These functions allow us to set a mark which is an
  // integer identifier. This is not used by the box in anyway
  // but might be used by the algorithm that uses the box class.
  const bool getMark(const unsigned int mark) const;
  void setMark(const unsigned int mark);

  // This function is extremely fishy, need to investigate if it
  // is really needed and what this is used for.
  static void RemoveFirst(vector<Box *> &vec, const Box *b);

  static const unsigned int INACTIVE_BOX = 0x1;
  static const unsigned int OUT_BOX_POS = 0x2;
  static const unsigned int OUT_BOX_NEG = 0x8; // historical reason for "8" (Chee)
  static const unsigned int IN_BOX = 0x4;

  void setIterator(const list<Box *>::iterator it) {
    it_ = it;
  }

  list<Box *>::iterator getIterator() const {
    return it_;
  }
 private:
  // Can be used only by this class, for instance by split.
  // External parties must create a box using Box(Point, Point).
  Box();

  Interval x_;
  Interval y_;

  // These are the the corners of the box, not to be confused by
  // the other vector of Points< > that specifies the set of vertices
  // that pass through this box.
  vector<Point *> corners_;
  vector<Box *> children_;
  vector<Box *> neighbors_;
  // This vector specifies the points of the curve that passes through
  // this box.
  vector<Point *> curve_vertices_;

  unsigned int mark_;
  unsigned int generation_;

  // Optional field, each box knows the location of itself in
  // the priority queue.
  list<Box *>::iterator it_;

  // The earlier implementation had a parent field as well, but that
  // was rather misleading, since the "parent" could point to a pointer
  // that had already been freed, which would cause all kind of nastiness.
  // Since it was not really being used too much, a decision was made to
  // get rid of it.
  //
  //box * parent;
};

#endif  // CXY2_BOX_H_
