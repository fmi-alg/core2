/* **************************************
   File: box.cpp

   Description: 
	Geometry code factored out from cxy.cpp.

   HISTORY: 2008: Long Lin (Java Code for cxy paper)
	    Apr, 2009: Shuxing Lu (converted Java code to Core2)
	    Jun, 2010: Narayan Kamath (refactored code into independent units)

   Since Core Library  Version 2.0
   $Id: box.cpp,v 1.10 2010/11/01 03:24:30 exact Exp $
 ************************************** */

#include "box.h"

namespace cxy {

Box::Box(const Point &a, const Point &b) :
	  x_(Interval(a.getX(), b.getX())),
	  y_(Interval(a.getY(), b.getY())),
	  mark_(0),
	  generation_(0) {
	
	  // This was the old mapping of v1, v2, v3, v4 . This is
	  // not terribly useful, as this information can be got
	  // from the intervals X and Y.
	  //
	  // v1=a.getX(); v2=b.getX(); v3=a.getY(); v4=b.getY();
	
	  // Note that the points are stored in counter clockwise
	  // order starting with the top left corner of the box.
	  corners_.push_back(new Point(a.getX(), b.getY()));
	  corners_.push_back(new Point(a.getX(), a.getY()));
	  corners_.push_back(new Point(b.getX(), a.getY()));
	  corners_.push_back(new Point(b.getX(), b.getY()));
	}

Box::~Box() {
	  for (unsigned int i = 0; i < corners_.size(); ++i) {
	    delete corners_[i];
	  }
	 
	  for (unsigned int i = 0; i < neighbors_.size(); ++i) {
	    vector<Box *> &n_list = neighbors_[i]->neighbors_;
	    vector<Box *>::iterator it = n_list.begin();
	    while (it != n_list.end()) {
	      if (*it == this) {
	        it = n_list.erase(it);
	      } else {
	        ++it;
	      }
	    }
	  }
	}

const bool Box::operator ==(const Box & ref) const {
	  if (&ref == this) {
	  return true;
	  }
	
	  const Interval *r_x = ref.getX();
	  const Interval *r_y = ref.getY();
	
	  if (r_x->getL() == x_.getL() && r_x->getR() == x_.getR() &&
	    r_y->getL() == y_.getL() && r_y->getR() == y_.getR()) {
	  return true;
	  }
	
	  return false;
	}

const bool Box::getMark(const unsigned int mark) const {
  return mark_ & mark;
}

void Box::setMark(const unsigned int mark) {
  mark_ = mark_ | mark;
}

template <typename NT>
inline const bool ThickOverlap(const IntervalT<NT> &s, const IntervalT<NT> &t) {
  return !(s.getR() <= t.getL() || s.getL() >= t.getR());
}

const vector<Box *> *Box::split() {
	  // Create the four new boxes.
	  // c1 | c4
	  // --------
	  // c2 | c3
	  Box *c1 = new Box(Point::mid(corners_[0], corners_[1]),
	                Point::mid(corners_[0], corners_[3]));
	  Box *c2 = new Box(*(corners_[1]),
	                Point::mid(corners_[1], corners_[3]));
	  Box *c3 = new Box(Point::mid(corners_[1], corners_[2]),
	                Point::mid(corners_[2], corners_[3]));
	  Box *c4 = new Box(Point::mid(corners_[1], corners_[3]),
	                *(corners_[3]));
	
	  // Add the boxes to each others neighbour list.
	  // C1 -> C2, C4
	  c1->neighbors_.push_back(c2); c1->neighbors_.push_back(c4);
	  // C2 -> C1, C3
	  c2->neighbors_.push_back(c1); c2->neighbors_.push_back(c3);
	  // C3 -> C4, C2
	  c3->neighbors_.push_back(c2); c3->neighbors_.push_back(c4);
	  // C4 -> C1, C3
	  c4->neighbors_.push_back(c3); c4->neighbors_.push_back(c1);
	
	  const unsigned int c_gen = generation_ + 1;
	  c1->generation_ = c_gen;
	  c2->generation_ = c_gen;
	  c3->generation_ = c_gen;
	  c4->generation_ = c_gen;
	
	  vector<Box *>::const_iterator it = neighbors_.begin();
	
	  while (it != neighbors_.end()) {
	    Box *neighbor = (*it);
	    RemoveFirst(neighbor->neighbors_, this);
	    const vector<Point *> *corners = neighbor->getCorners();
	
	    if (corners->at(1)->getY() == corners_.at(0)->getY()) {
	      // A neighbor that falls above this box.
	      if (ThickOverlap(neighbor->x_, c1->x_)) {
	        c1->neighbors_.push_back(neighbor);
	        neighbor->neighbors_.push_back(c1);
	      }
	      if (ThickOverlap(neighbor->x_, c4->x_)){
	        c4->neighbors_.push_back(neighbor);
	        neighbor->neighbors_.push_back(c4);
	      }
	    } else if (corners->at(2)->getX() == corners_.at(1)->getX()) {
	      // A neighbor that falls to the left of this box.
	      if (ThickOverlap(neighbor->y_, c1->y_)) {
	        c1->neighbors_.push_back(neighbor);
	        neighbor->neighbors_.push_back(c1);
	      }
	      if (ThickOverlap(neighbor->y_, c2->y_)) {
	        c2->neighbors_.push_back(neighbor);
	        neighbor->neighbors_.push_back(c2);
	      }
	    } else if (corners->at(3)->getY() == corners_.at(2)->getY()) {
	      // A neighbor that falls to the bottom of this box.
	      if (ThickOverlap(neighbor->x_, c2->x_)){
	        c2->neighbors_.push_back(neighbor);
	        neighbor->neighbors_.push_back(c2);;
	      }
	      if (ThickOverlap(neighbor->x_, c3->x_)) {
	        c3->neighbors_.push_back(neighbor);
	        neighbor->neighbors_.push_back(c3);
	      }
	    } else if (corners->at(0)->getX() == corners_.at(3)->getX()) {
	      // A neighbor on the right hand side of this box.
	      if (ThickOverlap(neighbor->y_, c3->y_)){
	        c3->neighbors_.push_back(neighbor);
	        neighbor->neighbors_.push_back(c3);
	      }
	      if (ThickOverlap(neighbor->y_, c4->y_)){
	        c4->neighbors_.push_back(neighbor);
	        neighbor->neighbors_.push_back(c4);
	      }
	    }
	    ++it;
	  }
	
	  children_.push_back(c1);
	  children_.push_back(c2);
	  children_.push_back(c3);
	  children_.push_back(c4);
	
	  return &children_;
	}

/* static */
void Box::RemoveFirst(vector<Box *> &vec, const Box *b){
	  // Hmmph. There has to be a better way to do this.
	  vector<Box *>::iterator it = vec.begin();
	  for (; it != vec.end(); ++it) {
	    if (b->operator==(**it)) {
	      vec.erase(it);
	      return;
	    }
	  }
	}

/*
 * Old implementation pasted below : Wonder why this is implemented
 * this way ?
  double tmp = v2-v1;
    if (tmp>= 0)
      return tmp;
    else
      return -tmp;
 */
const double Box::getSize() const {
  return x_.width();
}

const bool Box::curveVertexExists(const Point &a, const Point &b) const {
	  vector<Point *>::const_iterator it = curve_vertices_.begin();
	
	  while (it != curve_vertices_.end()) {
	    const Point *v = *it;
	    if ((v->getX() == a.getX() && v->getX() == b.getX()) ||
	        (v->getY() == a.getY() && v->getY() == b.getY())) {
	      return true;
	    }
	    ++it;
	  }
	
	  return false;
	}

const bool Box::curveVertexExists(const Point &a) const {
	  vector<Point *>::const_iterator it = curve_vertices_.begin();
	
	  while (it != curve_vertices_.end()) {
	    const Point *v = *it;
	    if (v->getX()==a.getX() && v->getY()==a.getY()) {
	      return true;
	    }
	    ++it;
	  }
	
	  return false;
	}

// Check whether a given point is contained in each of the sides
// of the box.
const bool Box::contains(const Point &a) const {
	  unsigned int sz = corners_.size();
	  for (unsigned int i=0; i < sz; i++) {
	    if (a.contains(*(corners_[i]), *(corners_[(i+1)%4]))) {
	      return true;
	    }
	  }
	
	  return false;
	}

} //namespace cxy 