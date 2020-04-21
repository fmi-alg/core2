/* **************************************
   File: construct-inl.h

   Description: 
	Code factored out from cxy.cpp.

   HISTORY: 2008: Long Lin (Java Code for cxy paper)
	    Apr, 2009: Shuxing Lu (converted Java code to Core2)
	    Jun, 2010: Narayan Kamath (refactored code into independent units)

   Since Core Library  Version 2.0
   $Id: display-list-inl.h,v 1.1 2010/06/18 16:24:13 exact Exp $
 ************************************** */

#ifndef CXY2_DISPLAY_LIST_INL_H_
#define CXY2_DISPLAY_LIST_INL_H_

#include <utility>
#include <vector>

#include "cxy-defs.h"
#include "box.h"
#include "point-inl.h"

namespace display_list {

inline double Distance(const Point *a, const Point *b) {
  return sqrt((b->getX()-a->getX())*(b->getX()-a->getX()) +
      (b->getY()-a->getY())*(b->getY()-a->getY()));
}

void ConstructCurve(const vector<Box *> &input,
    vector<pair<Point *, Point *> > *points) {
  vector<Box *>::const_iterator it = input.begin();

  // Any reason a reverse iterator is used ?? and not a
  // forward iterator.
  while (it != input.end()) {
    Box *b = *it;
    const vector<Point *> *verts = b->getCurveVertices();
    const int size = verts->size();
    if (size == 2) {
      // Draw a line between the two vertices on this square.
      points->push_back(std::make_pair(verts->at(0), verts->at(1)));
    } else if (size == 4) {
      int s1=5, s2 =5, d1 = 5, d2 = 5;

      const vector<Point *> *pts = b->getCorners();
      // For the next three for loops, surely there is a better way
      // to do this.
      for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++){
          if ((verts->at(i)->getX() == verts->at(j)->getX()) &&
              ((verts->at(i)->getX() == pts->at(0)->getX()) ||
                  (verts->at(i)->getX() == pts->at(2)->getX()))) {
            s1=i;
            s2=j;
          }
          if ((verts->at(i)->getY() == verts->at(j)->getY()) &&
              ((verts->at(i)->getY() == pts->at(0)->getY()) ||
                  (verts->at(i)->getY()==pts->at(2)->getY()))){
            s1=i;
            s2=j;
          }
        }
      }
      for (int m=0; m<4; m++) {
        if ((m!=s1)&&(m!=s2)) {
          d1=m;
        }
      }
      for (int n=0; n<4; n++) {
        if ((n!=s1)&&(n!=s2)&&(n!=d1)) {
          d2=n;
        }
      }

      if ((Distance(verts->at(s1), verts->at(d1)) + Distance(verts->at(s2), verts->at(d2))) <
          (Distance(verts->at(s1), verts->at(d2)) + Distance(verts->at(s2), verts->at(d1)))) {
        points->push_back(std::make_pair(verts->at(s1), verts->at(d1)));
        points->push_back(std::make_pair(verts->at(s2), verts->at(d2)));
      } else{
        points->push_back(std::make_pair(verts->at(s1), verts->at(d2)));
        points->push_back(std::make_pair(verts->at(s2), verts->at(d1)));
      }
    }
    ++it;
  }
}

}

#endif  // CXY2_CONSTRUCT_INL_H_
