/*
 * configuration_box3d.cpp
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#include "subdivision/configuration_box3d.h"
#include "strategy/box_priority_queue.h"

double ConfBox3d::rod_length;
int ConfBox3d::box_id_counter;
ConfBox3dPredicate* ConfBox3d::predicate;
std::vector<ConfBox3d*> ConfBox3d::boxes;

ConfBox3d::ConfBox3d(Point3d point, double ww)
    : depth(1),
      center(point),
      width(ww),
      rotation_center(Point3d(0, 0, 0)),
      rotation_width(-2),
      parent(0),
      status(UNKNOWN),
      p_set(0),
      is_in_source(false),
      heap_id(-1),
      prev(0),
      visited(false),
      in_queue(false),
      in_vor_queue(false),
      bidirection(0),
      in_source(false),
      dist_Dijkstra(-1) {
  box_ = new Box3d(point, ww);
  box_id_counter++;
  box_id = box_id_counter;
  rB = (ww * sqrt(3)) / 2;
  boxes.push_back(this);
  predicate = new ConfBox3dPredicate();
  children.clear();

  rod = Segment3d(center, center);
}

ConfBox3d::ConfBox3d(Point3d point, double ww, Point3d rotation_point,
                     double rot_ww)
    : depth(1),
      center(point),
      width(ww),
      rotation_center(rotation_point),
      rotation_width(rot_ww),
      parent(0),
      status(UNKNOWN),
      p_set(0),
      is_in_source(false),
      heap_id(-1),
      prev(0),
      visited(false),
      in_queue(false),
      in_vor_queue(false),
      bidirection(0),
      in_source(false),
      dist_Dijkstra(-1) {
  box_ = new Box3d(point, ww);
  rot_ = new Rot3d(rotation_point, rot_ww);
  box_id_counter++;
  box_id = box_id_counter;
  rB = (ww * sqrt(3)) / 2;
  boxes.push_back(this);
  predicate = new ConfBox3dPredicate();
  children.clear();

  rotation_point = Rot3d::cube2sphere(rotation_point);
  Vector direction(rotation_point.X(), rotation_point.Y(), rotation_point.Z());
  direction.normalize();
  Point3d apex(center + direction * rod_length);
  rod = Segment3d(center, apex);
}

bool ConfBox3d::isLeaf() { return children.empty(); }

bool ConfBox3d::getRot(float rot[3]) {
  if (this->rot_ == nullptr) {
    return false;
  }
  rot[0] = this->rot_->origin->X();
  rot[1] = this->rot_->origin->Y();
  rot[2] = this->rot_->origin->Z();
  return true;
}

ConfBox3d* ConfBox3d::getBox(Point3d point, Point3d rotation_point) {
  if (isLeaf()) {
    bool containsPoint =
        box_->containsPoint(point) &&
        (rot_ == nullptr || rot_->containsPoint(rotation_point));
    return containsPoint ? this : nullptr;
  } else {
    for (unsigned i = 0; i < children.size(); ++i) {
      ConfBox3d* b = children[i]->getBox(point, rotation_point);
      if (b != nullptr) {
        return b;
      }
    }
    return nullptr;
  }
}

ConfBox3d* ConfBox3d::getNeighborBox(ConfBox3d* neighbor) {
  if (isLeaf()) {
    bool isNeighborBox = isNeighbor(neighbor);
    return isNeighborBox ? this : nullptr;
  } else {
    for (unsigned i = 0; i < children.size(); ++i) {
      ConfBox3d* b = children[i]->getNeighborBox(neighbor);
      if (b != nullptr) {
        return b;
      }
    }
    return nullptr;
  }
}

void ConfBox3d::addCorner(Corner* c) {
  corners.push_back(c);
  if (search_type == VORONOI || search_type == BIVORONOI)
    vor_corners.push_back(c);
}

void ConfBox3d::addEdge(Edge* e) {
  edges.push_back(e);
  if (search_type == VORONOI || search_type == BIVORONOI)
    vor_edges.push_back(e);
}

void ConfBox3d::addWall(Wall* w) {
  walls.push_back(w);
  if (search_type == VORONOI || search_type == BIVORONOI)
    vor_walls.push_back(w);
}

bool ConfBox3d::is_free() { return status == FREE; }

Status ConfBox3d::getStatus() {
  if (status == UNKNOWN) predicate->classification(this, this->center);
  return status;
}

std::vector<ConfBox3d*> ConfBox3d::getChildren() { return children; }

std::vector<ConfBox3d*> ConfBox3d::getNeighbors() { return neighbors; }

bool ConfBox3d::contains(ConfBox3d* other) {
  return box_->containsPoint(other->center) &&
         (rot_ == nullptr || rot_->containsPoint(other->rotation_center));
}
bool ConfBox3d::rotIsOverlapped(ConfBox3d* other) {
  return (rot_ == nullptr) || rot_->isOverlap(other->rot_);
}

// When any of the boxes does not have restrictions on the rotational degrees of
// freedom, just compare their translation boxes; Otherwise, one of the
// components should be the same and the other adjacent
bool ConfBox3d::isNeighbor(ConfBox3d* other) {
  if (rot_ == nullptr || other->rot_ == nullptr) {
    return box_->isAdjacent(other->box_);
  } else {
    return (rot_->isIdentical(other->rot_) && box_->isAdjacent(other->box_)) ||
           (rot_->isAdjacent(other->rot_) && box_->isIdentical(other->box_));
  }
}

bool ConfBox3d::isAdjacent(ConfBox3d* other) {
  if (rot_ == nullptr || other->rot_ == nullptr) {
    // translational
    return box_->isAdjacent(other->box_);
  } else {
    // rotational
    return (box_->isAdjacent(other->box_) && rot_->isAdjacent(other->rot_));
  }
}

bool ConfBox3d::split(double epsilon) {
  if (!this->isLeaf() || this->status != MIXED) {
    // Subdivided already
    return false;
  }

  if (box_->split(epsilon)) {
    // Can still subdivide the translational component
    for (const auto& child : box_->children) {
      children.push_back(new ConfBox3d(*(child->origin), child->width));
    }
  } else if (rot_ == nullptr) {
    // Need to subdivide into six sides of rotational component
    for (int i = 0; i < 3; i++) {
      for (int j = -1; j < 3; j += 2) {
        double r[3] = {0, 0, 0};
        r[i] = j;
        children.push_back(new ConfBox3d(*(box_->origin), box_->width,
                                         Point3d(r[0], r[1], r[2]), 2));
      }
    }
  } else if (rot_->split(epsilon / rod_length)) {
    // Can split the rotational component
    for (const auto& child_rotation : rot_->children) {
      children.push_back(new ConfBox3d(*(box_->origin), box_->width,
                                       *(child_rotation->origin),
                                       child_rotation->width));
    }
  } else {
    // Cannot subdivide, epsilon reached
    return false;
  }

  for (int i = 0; i < children.size(); i++) {
    children[i]->parent = this;
    children[i]->depth = this->depth + 1;
  }

  for (unsigned i = 0; i < children.size(); ++i) {
    for (unsigned j = i + 1; j < children.size(); ++j) {
      if (children[i]->isNeighbor(children[j])) {
        children[i]->neighbors.push_back(children[j]);
        children[j]->neighbors.push_back(children[i]);
      }
    }
  }
  for (unsigned i = 0; i < children.size(); ++i) {
    for (unsigned j = 0; j < neighbors.size(); ++j) {
      if (children[i]->isNeighbor(neighbors[j])) {
        children[i]->neighbors.push_back(neighbors[j]);
        neighbors[j]->neighbors.push_back(children[i]);
      }
    }
  }

  return true;
}

ConfBox3d* ConfBox3d::makePointBox(Point3d point, Point3d rotation_point) {
  if (fabs(rotation_point.X()) * fabs(rotation_point.Y()) *
          fabs(rotation_point.Z()) >
      0)
    return new ConfBox3d(point, 0, rotation_point, 0);
  else
    return new ConfBox3d(point, 0);
}
