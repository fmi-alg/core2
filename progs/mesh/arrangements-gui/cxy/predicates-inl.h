/* **************************************
   File: predicate-inl.h

   Description: 
	Predicate code factored out from cxy.cpp.

   HISTORY: 2008: Long Lin (Java Code for cxy paper)
	    Apr, 2009: Shuxing Lu (converted Java code to Core2)
	    Jun, 2010: Narayan Kamath (refactored code into independent units)

   Since Core Library  Version 2.0
   $Id: predicates-inl.h,v 1.6 2010/09/20 16:07:59 exact Exp $
 ************************************** */

#ifndef CXY2_PREDICATES_INL_H_
#define CXY2_PREDICATES_INL_H_

#include "cxy-defs.h"
#include "box.h"
#include "point-inl.h"
#include "CORE/poly/Curves.h"

namespace cxy {

// Makes a copy of the curve
template <typename T> class CxyPredicates {
public:
  CxyPredicates(Curve<T> &curve,
                const unsigned int min_gen,
                const unsigned int max_gen) : curve_(curve) {
    // Unfortunately, differentiateFoo( ) seems to be a self
    // modifying operator, and therefore we need to make
    // temporary copies of the curve in order to not mess
    // things up.
    Curve<T> tmp = curve, tmp1 = curve;
    diff_x_ = tmp.differentiateX();
    diff_y_ = tmp1.differentiateY();
  }

  // This is the C0 predicate.
  const SignCondition Exclude(const Box &b) const {
    return curve_.eval<Interval>(*(b.getX()), *(b.getY())).sign();
  }

  // add the Max_Size and Min_Size criterion.
  //
  // (b->cxy())&&(b->getSize()<Max_Size)&&(b->getSize()>Min_Size)
  // This is the cxy predicate.
  //
  // This used to have the getSize( ) terms as well, but those
  // dont really belong here.
  const bool Include(const Box &b) const {
    Interval diff_x = diff_x_.eval<Interval>(*(b.getX()), *(b.getY()));
    Interval diff_y = diff_y_.eval<Interval>(*(b.getX()), *(b.getY()));

    return ((!diff_x.zero()) || (!diff_y.zero()));
  }

  // Suspicious if the curve evaluates to the same sign on
  // all four corners of this box.
  const bool Suspicious(const Box &b) const {
    const vector<Point *> &p = *(b.getCorners());
    return (Sign(*(p[0])) == Sign(*(p[1])) &&
            Sign(*(p[1])) == Sign(*(p[2])) &&
            Sign(*(p[2])) == Sign(*(p[3])));
  }

  // Ambiguous if the curve evaluates the same sign on all corners
  // and the curve claims to intersect the box twice.
  const bool Ambiguous(const Box &b) const {
    return Suspicious(b) && (b.getCurveVertices()->size() == 2);
  }

  // Note that this returns sign = 1 for value == 0 as well, this
  // is taken from the original implementation.
  const int Sign(const Point &p) const {
    double value = curve_.yPolynomial(p.getX()).eval(p.getY());
    if (value < 0) {
      return -1;
    }

    return 1;
  }

  // Compute the curve vertices on the sides of the box b.
  void ComputeVertices(Box *box) const {
    // The corners of the box.
    const vector<Point *> &p = *(box->getCorners());
    // The list of curve vertices (might be empty).
    vector<Point *> *verts = box->mutableCurveVertices();
    // The list of neighbors in case we need to add vertices
    // to the neighbors as well.
    const vector<Box *> *neighbors = box->getNeighbors();

    for (unsigned int i=0; i < p.size(); ++i) {
      // If the sign between two vertices of the side
      // of a square differs, then we add a curve vertex
      // at the midpoint of that side.
      const Point &a = *(p.at(i));
      const Point &b = *(p.at((i+1) % 4));
      if (Sign(a) != Sign(b)) {
        // If this vertex does not already exist. Hmmmm. I wonder
        // if this test could be made more efficient.
        if (!box->curveVertexExists(a, b)){
          // Add this vertex to the list of curve intersections.
          Point *v = new Point(Point::mid(a, b));
          verts->push_back(v);
          // Also make sure that our friendly neighbors have the same
          // vertex.
          for (unsigned int j=0; j < neighbors->size(); ++j){
            Box  *n_box = neighbors->at(j);
            // If b does not already contain this vertex, then
            // add it.
            if (n_box->contains(*v) && !n_box->curveVertexExists(*v)) {
              n_box->mutableCurveVertices()->push_back(v);
            }
          }
        }
      }
    }
  }
private:
  // These classes do not appear to be const correct, hence this
  // horribleness with the mutable declaration.
  mutable Curve<T> curve_;
  Curve<T> diff_x_;
  Curve<T> diff_y_;
};


} //namespace cxy

#endif  // CXY2_PREDICATES_INL_H_
