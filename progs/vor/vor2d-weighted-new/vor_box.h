/*
SubVor, a program for computing Voronoi diagrams via subdivision.
Copyright (C) 2016 Huck Bennett, Evanthia Papadopoulou, and Chee Yap.
For comments or questions, please contact Huck Bennett at hbennett@cs.nyu.edu.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "BiPoly.h"
#include "Corner.h"
#include "Edge.h"
#include "Interval.h"
#include "Object.h"
#include "Point.h"
#include "vor2d.h"

#include <limits>
#include <map>
#include <memory>
#include <typeinfo>
#include <utility>
#include <vector>

using std::shared_ptr;
using std::vector;

namespace vor2d {

typedef map<pair<Object*, Object*>, vector<BiPoly>*> BisecObjMap;

class vor_qt;

struct vor_seg {
  vor_seg(Point2d p, Point2d q) : p_(p), q_(q) {}
  Point2d p_;
  Point2d q_;
};

class vor_box {
 public:
  vor_box(int depth, int indicator, double center[], vor_qt* tree);
  ~vor_box();
  void smooth_split();
  double width() const;
  double radius() const;
  double* center() const;
  vor_box** children() const;
  bool is_leaf() const;
  vor_box* principal_neighbor(int dir) const;
  const int dimension() const;
  bool scaled_intersect(const vor_box& other, double scale) const;
  int depth() const;
  int num_children() const;
  static int get_dir(int index);
  shared_ptr<vector<vor_box*>> leaf_neighbors_dir(int dir);
  shared_ptr<vector<vor_box*>> enumerate_halfspace_leaf_descendants(int dir);
  
  // Voronoi specific methods.
  // void add_corner(Corner* corner);
  // void add_edge(Edge* edge);
  void add_object(Object* object);
  // vector<Corner*>* get_corners();
  // vector<Edge*>* get_edges();
  void add_feature(Feature* feature);
  vector<Object*>* get_objects();
  vector<Feature*>* get_features();
  vector<BiPoly>* get_active_bisectors();
  const vector<vor_seg*>* get_segments() const;
  int num_features() const;
  double max_lipschitz() const;
  double midpoint_clearance() const;
  double clearance(const Point2d&) const;
  int num_objects() const;
  void set_active(bool is_active);
  bool is_active() const;
  void set_degen(bool is_degen);
  bool is_degen() const;
  int num_corner_obj() const;
  void gen_vertices();
  bool contained_in(const vor_box& other, double scale) const;
  bool contained_in_polygon() const;
  bool few_active_features_per_object();
  void activate_bisectors();
  Interval get_bx() const;
  Interval get_by() const;

  // Box predicates.
  bool cpv() const;
  bool cmk(double scale) const;
  bool cjc(double scale) const;

 protected:
  vor_box** neighbors() const;
  void split();
  void smooth_split_aux();
  Object* nearest_obj(const Point2d&) const;

  // Fields.
  bool pv_; // TODO: Use this.
  bool jc_; // TODO: Use this.
  bool is_active_;
  bool is_degen_;
  const int depth_;
  const int indicator_;
  double width_;
  double* center_;
  int num_children_;
  vor_qt* tree_;
  Interval bx;
  Interval by;
  // double max_lipschitz_;

  // Collections.
  vor_box** children_;
  vor_box** neighbors_;
  map<int, Point2d*> nodes_map_;
  vector<Point2d*> nodes_;
  vector<vor_seg*> segments_;

  // Active things.
  // vector<Corner*> corners_;
  // vector<Edge*> edges_;
  vector<Feature*> features_;
  vector<Object*> objects_;
  vector<BiPoly> bisectors_;
  
  // vector<BiPoly*> bisectors_;
  // vector<BiPoly*> bisector_grads_;

  friend class vor_qt;
};

} // namespace vor2d
