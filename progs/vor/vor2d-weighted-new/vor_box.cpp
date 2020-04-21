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

#include "vor_box.h"
#include "vor_qt.h"
#include "BiPoly.h"

#include <algorithm>
#include <assert.h>
#include <set>

#define DEBUG 0

namespace vor2d {

using std::set;

vor_box::vor_box(int depth, int indicator, double center[], vor_qt* tree)
  : depth_(depth), indicator_(indicator), center_(center), tree_(tree),
    children_(nullptr), num_children_(0), is_active_(false), is_degen_(false),
    bx(TOP), by(TOP) /* Real initialization is below; forced by C++. */ {
  width_ = pow(2, -depth_) * tree_->width();
  bx = Interval(center[0] - width_ / 2.0, center[0] + width_ / 2.0);
  by = Interval(center[1] - width_ / 2.0, center[1] + width_ / 2.0);  
  neighbors_ = new vor_box*[2 * dimension()];
}

vor_box::~vor_box() {
  // If the box is split then recurse to children.
  if (!is_leaf()) {
    for (int i = 0; i < (1 << dimension()); i++) {
      delete children_[i];
    }
    delete[] children_;
  }

  delete[] neighbors_;
  delete[] center_;
}

void vor_box::smooth_split() {
  if (!is_leaf()) {
    return;
  }
  smooth_split_aux();
  tree_->inc_smooth_splits();
}

double vor_box::width() const {
  return width_;
}

double vor_box::radius() const {
  return width_ / sqrt(2);
}

double* vor_box::center() const {
  return center_;
}

vor_box** vor_box::children() const {
  return children_;
}

bool vor_box::is_leaf() const {
  return children_ == nullptr;
}

vor_box* vor_box::principal_neighbor(int dir) const {
  int axis = fabs(dir);
  int sign = (dir < 0) ? 0 : 1;
  return neighbors_[(axis - 1) + sign * dimension()];
}

shared_ptr<vector<vor_box*>> vor_box::leaf_neighbors_dir(int dir) {
  shared_ptr<vector<vor_box*>> neighbors (new vector<vor_box*>());
  vor_box* cur_neighbor = principal_neighbor(dir);

  if (cur_neighbor == nullptr) {
    return neighbors;
  } else if (cur_neighbor->is_leaf()) {
    neighbors->push_back(cur_neighbor);
    return neighbors;
  } else { // cur_neighbor is non-null and split.
    return cur_neighbor->enumerate_halfspace_leaf_descendants(-dir);
  }
}

int vor_box::depth() const {
  return depth_;
}

vor_box** vor_box::neighbors() const {
  return neighbors_;
}

shared_ptr<vector<vor_box*>> vor_box::enumerate_halfspace_leaf_descendants(int dir) {
  assert(!is_leaf());

  shared_ptr<vector<vor_box*>> neighbors (new vector<vor_box*>());
  int axis = fabs(dir);
  int dir_bit = (dir < 0 ? 0 : 1) << (axis - 1);
  int bottom_mask = (1 << (axis - 1)) - 1;
  int top_mask = (1 << dimension()) - 1 - bottom_mask;

  for (int i = 0; i < (1 << (dimension() - 1)); i++) {
    int ind = ((i & top_mask) << 1) | dir_bit | (i & bottom_mask);
    assert(ind >= 0 && ind < 1 << dimension());
    vor_box* cur_child = children_[ind];
    if (cur_child->is_leaf()) {
      neighbors->push_back(cur_child);
    } else {
      shared_ptr<vector<vor_box*>> descendants = cur_child->enumerate_halfspace_leaf_descendants(dir);
      neighbors->insert(neighbors->begin(), descendants->begin(), descendants->end());
    }
  }

  return neighbors;
}

const int vor_box::dimension() const {
  return tree_->dimension();
}

bool vor_box::scaled_intersect(const vor_box& other, double scale) const {
  assert(scale > 0);

  double sw = scale * width() / 2;
  double osw = scale * other.width() / 2;
  Interval x1 = Interval(center()[0] - sw, center()[0] + sw);
  Interval y1 = Interval(center()[1] - sw, center()[1] + sw);
  Interval x2 = Interval(other.center()[0] - osw, other.center()[0] + osw);
  Interval y2 = Interval(other.center()[1] - osw, other.center()[1] + osw);
  Interval* x_inter = x1.intersect(x2);
  Interval* y_inter = y1.intersect(y2);
  bool intersect = x_inter != NULL && y_inter != NULL;

  if (x_inter != NULL) {
    delete x_inter;
  }

  if (y_inter != NULL) {
    delete y_inter;
  }
  
  return intersect;
}

void vor_box::split() {
  num_children_ = 1 << dimension();
  double* center;

  // Initialize the children.
  children_ = new vor_box*[num_children_];
  for (int i = 0; i < num_children_; i++) {

    // Compute the center of the new child.
    center = new double[dimension()];
    for (int j = 0; j < dimension(); j++) {
      center[j] = center_[j] + ((i & (1 << j)) == 0 ? -1 : 1) * width_ / 4.0;
    }
    children_[i] = new vor_box(depth_ + 1, i, center, tree_);

    // Compute active features for the new child.
    // TODO: Check the use of child vs. parents boxes here.
    double lip = max_lipschitz(); // Maximum Lipschitz constant of relevant features.
    Point2d mid_point(center[0], center[1]);
    double child_clearance = clearance(mid_point);
    Edge* e;

    for (auto it = objects_.begin(); it != objects_.end(); ++it) {
      if ((*it)->distance(mid_point) <= child_clearance + 2 * lip * children_[i]->radius()) {
    	children_[i]->add_object(*it);
      }
    }

    for (auto it = features_.begin(); it != features_.end(); ++it) {
      e = dynamic_cast<Edge*>(*it);
      // if (e != nullptr && !e->interior_active(bx, by)) {
      // 	continue;
      // }
      
      if ((*it)->distance(mid_point) <= child_clearance + 2 * lip * children_[i]->radius()) {
#if DEBUG
	// cout << *it << " " << (*it)->parent() << " " << (dynamic_cast<Edge*>(*it) != nullptr) << "\n";
#endif
	children_[i]->add_feature(*it);

#if DEBUG
	// cout << (*it)->distance(mid_point) << " "
	//      << (*it)->parent()->distance(mid_point) << "\n";

	// if (e) {
	//   cout << e->get_tstar().eval(bx, by) << " "
	//        << e->get_tstar().eval(center[0], center[1]) << "\n";
	// }

	assert((*it)->distance(mid_point) >= (*it)->parent()->distance(mid_point));
	assert(std::find(children_[i]->get_objects()->begin(),
			 children_[i]->get_objects()->end(), (*it)->parent())
	       != children_[i]->get_objects()->end());
#endif
      }
    }
  }
  
#if DEBUG
  for (int i = 0; i < num_children(); i++) {
    assert(children_[i]->num_features() > 0);
    assert(children_[i]->num_features() >= children_[i]->num_objects());
  }
#endif

  // Initialize neighbors of new children.
  // Each box has 2 * d principal neighbor pointers, one in each semi-axis direction.
  // Specifically, each box has exactly one sibling neighbor and one non-sibling or null neighbor
  // in the opposite direction along each axis.
  for (int child_ind = 0; child_ind < num_children_; child_ind++) {
    vor_box* cur_child = children_[child_ind];
    for (int d = 0; d < dimension(); d++) {
      int neighbor_child_ind = child_ind ^ (1 << d);
      int sibling_dir_ind = (neighbor_child_ind & (1 << d)) == 0 ? 0 : 1;

      // Set sibling neighbor.
      int sib_dim_ind = d + dimension() * sibling_dir_ind;
      cur_child->neighbors_[sib_dim_ind] = children_[neighbor_child_ind];

      // Set non-sibling neighbor and update neighbor's pointers.
      int non_sib_dim_ind = d + dimension() * (1 - sibling_dir_ind);
      vor_box* cur_neighbor = neighbors_[non_sib_dim_ind];

      if (cur_neighbor == nullptr || cur_neighbor->is_leaf()) {
	cur_child->neighbors()[non_sib_dim_ind] = cur_neighbor;
      } else {
	cur_neighbor = cur_neighbor->children()[neighbor_child_ind];
	cur_child->neighbors()[non_sib_dim_ind] = cur_neighbor;
	cur_neighbor->neighbors()[sib_dim_ind] = cur_child;
      }
    }
  }
  tree_->inc_splits();
}

void vor_box::smooth_split_aux() {
  assert(is_leaf());
  for (int d = 0; d < dimension(); d++) {
    int non_sib_neighbor_ind = d + (((indicator_ & (1 << d)) == 0) ? 0 : 1) * dimension();
    vor_box* cur_neighbor = neighbors_[non_sib_neighbor_ind];
    if (cur_neighbor != nullptr && cur_neighbor->depth() < depth_) {
      cur_neighbor->smooth_split_aux();
    }
  }
  split();
}

void vor_box::add_feature(Feature* feature) {
  features_.push_back(feature);
}

void vor_box::add_object(Object* object) {
  objects_.push_back(object);
}

vector<Feature*>* vor_box::get_features() {
  return &features_;
}

vector<Object*>* vor_box::get_objects() {
  return &objects_;
}

const vector<vor_seg*>* vor_box::get_segments() const {
  return &segments_;
}

int vor_box::num_children() const {
  return num_children_;
}

double vor_box::clearance(const Point2d& point) const {
  double min_sep = std::numeric_limits<double>::max();
  
  // Compute clearance in terms of objects.
  for (auto it = objects_.begin(); it != objects_.end(); ++it) {
    double dist = (*it)->distance(point);
    if (dist < min_sep) {
      min_sep = dist;
    }
  }

  return min_sep;
}

double vor_box::midpoint_clearance() const {
  Point2d mid_point(center_[0], center_[1]);
  return clearance(mid_point);
}

double vor_box::max_lipschitz() const {
  double lip = 0.0;

  for (auto it = features_.begin(); it != features_.end(); ++it) {
    double l = (*it)->lipschitz();
    if (l > lip) {
      lip = l;
    }
  }

  return lip;
}

int vor_box::num_features() const {
  return features_.size();
}

int vor_box::num_objects() const {
  return objects_.size();
}

void vor_box::set_active(bool is_active) {
  is_active_ = is_active;
}

bool vor_box::is_active() const {
  return is_active_;
}

void vor_box::set_degen(bool is_degen) {
  is_degen_ = is_degen;
}

bool vor_box::is_degen() const {
  return is_degen_;
}

/***********************************************************
 *** Box predicates ****************************************
************************************************************/

// New versions.

bool vor_box::cpv() const {
  if (bisectors_.empty()) {
    return true;
  }

  Interval gx(TOP), gy(TOP);
  double hw = width_ / 2.0;

  for (BiPoly bisector : bisectors_) {
    gx = bisector.gradient().first.eval(bx, by);
    gy = bisector.gradient().second.eval(bx, by);

    if (((gx * gx) + (gy * gy)).contains(0)) {
      return false;
    }
  }

  return true;
}

bool vor_box::cjc(double scale) const {
  // TODO: Improve this to work with degenerate intersections.
  if (num_features() <= 2) {
    return true;
  }

  for (int i = 0; i < bisectors_.size(); i++) {
    BiPoly bs1 = bisectors_[i];
    BiPoly bs1_x = bs1.gradient().first;
    BiPoly bs1_y = bs1.gradient().second;
    
    for (int j = i + 1; j < bisectors_.size(); j++) {
      BiPoly bs2 = bisectors_[j];
      BiPoly bs2_x = bs2.gradient().first;
      BiPoly bs2_y = bs2.gradient().second;
      BiPoly det_poly = (bs1_x * bs2_y) - (bs1_y * bs2_x);

      if (!(i == 0 && j == 1)) {
	return true;
      }
      if (det_poly.eval(bx, by).contains(0)) {
#if DEBUG
	cout << "CJC:\n";
	cout << bs1.to_string()   << "\n";
	cout << bs1_x.to_string() << ", " << bs1_y.to_string() << "\n";
	cout << bs2.to_string()   << "\n";
	cout << bs2_x.to_string() << ", " << bs2_y.to_string() << "\n";
	cout << det_poly.eval(bx, by) << "\n";
#endif
	return false;
      }
    }
  }
  
  return true;
}

bool vor_box::cmk(double scale) const {
  if (num_features() <= 2) {
    return true;
  }

  double mx = center()[0];
  double my = center()[1];
  double sw = scale * width_ / 2;
  Interval lft_x(mx - sw);
  Interval rgt_x(mx + sw);
  Interval bot_y(my - sw);
  Interval top_y(my + sw);
  Interval span_x(mx - sw, mx + sw);
  Interval span_y(my - sw, my + sw);
  
  for (int i = 0; i < bisectors_.size(); i++) {
    BiPoly f1 = bisectors_[i];
    BiPoly f11 = f1.gradient().first;
    BiPoly f12 = f1.gradient().second;
    
    for (int j = i + 1; j < bisectors_.size(); j++) {
      BiPoly f2 = bisectors_[j];
      BiPoly f21 = f2.gradient().first;
      BiPoly f22 = f2.gradient().second;

      assert(f1.eval(bx, by).contains(0) && f2.eval(bx, by).contains(0));

      // Evaluate Jacobian at midpoint.
      double z11 = f11.eval(mx, my);
      double z12 = f12.eval(mx, my);
      double z21 = f21.eval(mx, my);
      double z22 = f22.eval(mx, my);
      double zdet = ((z11 * z22) - (z12 * z21));

      if (zdet == 0.0) {
	return false;
      }
      double izdet = 1.0 / zdet;

      // Inverse determinant.
      double y11 = izdet * z22;
      double y12 = izdet * (-z12);
      double y21 = izdet * (-z21);
      double y22 = izdet * z11;

      // Modified system and derivatives (G and G').
      BiPoly g1  = y11 * f1  + y12 * f2;
      BiPoly g2  = y21 * f1  + y22 * f2;
      BiPoly g11 = y11 * f11 + y12 * f21;
      BiPoly g12 = y11 * f12 + y12 * f22;
      BiPoly g21 = y21 * f11 + y22 * f21;
      BiPoly g22 = y21 * f12 + y22 * f22;

#if DEBUG
      cout << "MK applied to box: " << mx << " " << my << " " << sw << "\n";
  
      cout << "MK original system F:\n";
      cout << f1.to_string() << "\n";
      cout << f2.to_string() << "\n";
      cout << f11.to_string() << ", "
	   << f12.to_string() << "\n";
      cout << f21.to_string() << ", "
	   << f22.to_string() << "\n";
  
      cout << "MK modified system G:\n";
      cout << g1.to_string() << "\n";
      cout << g2.to_string() << "\n";
      cout << g11.to_string() << ", "
	   << g12.to_string() << "\n";
      cout << g21.to_string() << ", "
	   << g22.to_string() << "\n";

      cout << "MK modified system evaluated on box midpoint:\n";
      cout << g11.eval(mx, my) << ", "
	   << g12.eval(mx, my) << "\n";
      cout << g21.eval(mx, my) << ", "
	   << g22.eval(mx, my) << "\n";
      
      assert(g1.gradient().first == g11 && g1.gradient().second == g12);
      assert(g2.gradient().first == g21 && g2.gradient().second == g22);
#endif

      // Evaluations.
      double g1p = g1.eval(mx + sw, my);
      double g1m = g1.eval(mx - sw, my);
      double g2p = g2.eval(mx, my + sw);
      double g2m = g2.eval(mx, my - sw);

#if DEBUG
      cout << "center: " << center_[0] << " " << center_[1]
	   << " f1-mid: " << f1.eval(mx, my)
	   << ", f2-mid: " << f2.eval(mx, my)
	   << " g1-mid: " << g1.eval(mx, my)
	   << ", g2-mid: " << g2.eval(mx, my)
	   << "\n";
      cout << "f1 box: " << f1.eval(bx, by)
	   << ", f2 box: " << f2.eval(bx, by)
	   << " g1 box: " << g1.eval(bx, by)
	   << ", g2 box: " << g2.eval(bx, by)
	   << "\n";
      cout << "g1p: " << g1p << " g1m: " << g1m << " g2p: " << g2p << " g2m: " << g2m << "\n";
      cout << "g12+: "  << sw * g12.eval(rgt_x, span_y) << " g21+: " << sw * g21.eval(span_x, top_y)
	   << " g12-: " << sw * g12.eval(lft_x, span_y) << " g21-: " << sw * g21.eval(span_x, bot_y) << "\n";
#endif
  
      // Conditions 2.11.
      if (!(g1p * g1m <= 0)) {
#if DEBUG
	cout << "Condition 1\n";
#endif
	return false;
      }
      if (!(g2p * g2m <= 0)) {
#if DEBUG
	cout << "Condition 2\n";
#endif
	return false;
      }

      // Conditions 2.12.  
      if (!((sw * g12.eval(rgt_x, span_y)).mag() <= fabs(g1p))) {
#if DEBUG
	cout << "Condition 3: " << sw * g12.eval(rgt_x, span_y) << "\n";
#endif
	return false;
      }
      if (!((sw * g21.eval(span_x, top_y)).mag() <= fabs(g2p))) {
#if DEBUG
	cout << "Condition 4: " << sw * g21.eval(span_x, top_y) << "\n";
#endif
	return false;
      }
  
      // Conditions 2.13.
      if (!((sw * g12.eval(lft_x, span_y)).mag() <= fabs(g1m))) {
#if DEBUG
	cout << "Condition 5: " << sw * g12.eval(lft_x, span_y) << "\n";
#endif
	return false;
      }
      if (!((sw * g21.eval(span_x, bot_y)).mag() <= fabs(g2m))) {
#if DEBUG
	cout << "Condition 6: " << sw * g21.eval(span_x, bot_y) << "\n";
#endif
	return false;
      }
    }
  }

#if DEBUG
  cout << "MK succeeds.\n";
  cout << mx << " " << my << "\n";
#endif
  
  return true;
}

/***********************************************************/

Object* vor_box::nearest_obj(const Point2d& point) const {
  double min_dist = std::numeric_limits<double>::max();
  Object* nearest = nullptr;

  for (Object* obj : objects_) {
    double cur_dist = obj->distance(point);
    if (cur_dist < min_dist) {
      min_dist = cur_dist;
      nearest = obj;
    } else if (cur_dist == min_dist) {
      cout << "Warning: possible degeneracy: " << center_[0] << " " << center_[1] << "\n";
    }
  }

  return nearest;
}

Point2d* get_midpoint(const Point2d& p, const Point2d& q) {
  return new Point2d((p[0] + q[0]) / 2.0, (p[1] + q[1]) / 2.0);
}

int vor_box::num_corner_obj() const {
  double x = center_[0];
  double y = center_[1];
  double hw = width_ / 2.0;
  set<Object*> objects;

  Point2d corners[4];
  corners[0] = Point2d(x + hw, y + hw);
  corners[1] = Point2d(x + hw, y - hw);
  corners[2] = Point2d(x - hw, y - hw);
  corners[3] = Point2d(x - hw, y + hw);

  assert(is_leaf());

  // Compute nearest objects for each corner.
  Object* objs[4];
  for (int i = 0; i < 4; i++) {
    objs[i] = nearest_obj(corners[i]);
    objects.insert(objs[i]);
  }

  return objects.size();
}

int vor_box::get_dir(int index) {
  // TODO: Improve direction computation logic.
  switch(index) {
  case 0:
    return 2;
  case 1:
    return 1;
  case 2:
    return -2;
  case 3:
    return -1;
  default:
    cout << "Warning: Failed to compute direction.\n";
    return -1;
  }
}

// TODO: Factor construction logic out into a separate file?
void vor_box::gen_vertices() {
  double x = center_[0];
  double y = center_[1];
  double hw = width_ / 2.0;
  set<Object*> objects;

  Point2d corners[4];
  corners[0] = Point2d(x + hw, y + hw);
  corners[1] = Point2d(x + hw, y - hw);
  corners[2] = Point2d(x - hw, y - hw);
  corners[3] = Point2d(x - hw, y + hw);

  assert(is_leaf());

  // Compute nearest objects for each corner.
  Object* objs[4];
  for (int i = 0; i < 4; i++) {
    objs[i] = nearest_obj(corners[i]);
    objects.insert(objs[i]);
  }

  if (objects.size() == 1) {
    return;
  }

  for (int i = 0; i < 4; i++) {
    int j = (i + 1) % 4;

    // Add a node iff adjacent corners have different labels.
    if (objs[i] == objs[j]) {
      continue;
    }

    int dir = get_dir(j);
    Point2d* midpoint = get_midpoint(corners[i], corners[j]);
    Object* mid_closest = nearest_obj(*midpoint);
    vor_box* neighbor = principal_neighbor(dir);
    if (neighbor != nullptr && !neighbor->is_leaf()) {
      // TODO: Clean up.
      Point2d* sub_midpoint;
      if (objs[i] != mid_closest) {
	sub_midpoint = get_midpoint(corners[i], *midpoint);
	nodes_.push_back(sub_midpoint);
	nodes_map_[dir] = sub_midpoint;
      } else if (objs[j] != mid_closest) {
	sub_midpoint = get_midpoint(corners[j], *midpoint);
	nodes_.push_back(sub_midpoint);
	nodes_map_[dir] = sub_midpoint;
      } else {
	cout << "Warning: No node added.\n";
      }
    } else {
      nodes_.push_back(midpoint);
      nodes_map_[dir] = midpoint;
    }
  }
  
  // Generate Voronoi segments.
  // TODO: Fix logic for nodes_.size() > 2. Update: Why is it wrong?
  if (nodes_.size() == 2) {
    segments_.push_back(new vor_seg(*nodes_[0], *nodes_[1]));
  } else {
    Point2d center(center_[0], center_[1]);
    for (Point2d* node : nodes_) {
      segments_.push_back(new vor_seg(*node, center));
    }
  }
}

bool vor_box::contained_in(const vor_box& other, double scale) const {
  double mx = center()[0];
  double my = center()[1];
  double hw = width() / 2.0;

  double ox = other.center()[0];
  double oy = other.center()[1];
  double ow = scale * other.width() / 2.0;

  return (mx - hw >= ox - ow) &&
    (mx + hw <= ox + ow) &&
    (my - hw >= oy - ow) &&
    (my + hw <= oy + ow);
}

bool vor_box::contained_in_polygon() const {
  for (Object* object : objects_) {
    if (object->contains(*this)) {
      return true;
    }
  }
}

vector<BiPoly>* vor_box::get_active_bisectors() {
  assert(objects_.size() >= 2);
  if (bisectors_.empty()) {
    activate_bisectors();
  }

  return &bisectors_;
}

#define PSEUDO_VOR 0
void vor_box::activate_bisectors() {
#if DEBUG
  cout << "Bisectors in box " << center_[0] << " " << center_[1] << " " << width_ / 2.0 << ":\n";
#endif
  for (int i = 0; i < features_.size(); i++) {
    Feature* f1 = features_[i];
    for (int j = i + 1; j < features_.size(); j++) {
      Feature* f2 = features_[j];
      if (f1->parent() != f2->parent() || (PSEUDO_VOR && f1 != f2 && (f1->is_edge() && f2->is_edge()))) {
	BiPoly bisector = f1->dfun_sq() - f2->dfun_sq();
	if (bisector.eval(bx, by).contains(0)) {
	  bisectors_.push_back(bisector);
	}
#if DEBUG
	cout << (f1->dfun_sq() - f2->dfun_sq()).to_string() << "\n";
#endif
      }
    }
  }
#if DEBUG
  cout << "\n";
#endif
}

#define MAX_ACTIVE_FEATURES_PER_OBJECT 3
bool vor_box::few_active_features_per_object() {
  map<Object*, int> counts;

  for(Feature* f : features_) {
    assert(std::find(objects_.begin(), objects_.end(), f->parent())
	   != objects_.end());
    
    if (counts.find(f->parent()) == counts.end()) {
      counts[f->parent()] = 0;
    }
    counts[f->parent()] += 1;

    if (counts[f->parent()] > MAX_ACTIVE_FEATURES_PER_OBJECT) {
      return false;
    }
  }

#if DEBUG
  // for (auto it = counts.begin(); it != counts.end(); ++it) {
  //   cout << it->second << ", ";
  // }
  // cout << "\n";
#endif

  // TODO: Activating bisectors here is a hack. Fix.
  activate_bisectors();
  return true;
}

Interval vor_box::get_bx() const {
  return bx;
}

Interval vor_box::get_by() const {
  return by;
}

} // namespace vor2d
