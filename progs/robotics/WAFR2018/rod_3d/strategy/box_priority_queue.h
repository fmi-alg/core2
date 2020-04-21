/*
 * box_priority_queue.h
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef BOX_PRIORITY_QUEUE_H
#define BOX_PRIORITY_QUEUE_H

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <queue>
#include <unordered_set>
#include <vector>

#include "sss/graph.h"
#include "sss/union_find.h"
#include "subdivision/configuration_box3d.h"

enum QTYPE {
  RANDOM = 0,
  BFS,
  GREEDY,
  BIGREEDY,
  GREEDY_SIZE,
  BIGREEDY_SIZE,
  VORONOI,
  BIVORONOI
};

extern int search_type;
extern bool move_away;

extern Point3d start;
extern Point3d goal;
extern Point3d start_rotation;
extern Point3d goal_rotation;

extern double rod_length;

static double rotationLength(Point3d ra, Point3d rb) {
  Point3d tmp(Rot3d::cube2sphere(ra));
  Vector a(tmp.X(), tmp.Y(), tmp.Z());
  tmp = Rot3d::cube2sphere(rb);
  Vector b(tmp.X(), tmp.Y(), tmp.Z());
  double radian = acos(a * b);
  assert(a.norm() > 0);
  assert(b.norm() > 0);

  if (floor(ra.X()) * floor(rb.X()) + floor(ra.Y()) * floor(rb.Y()) +
          floor(ra.Z()) * floor(rb.Z()) <
      0)
    radian = M_PI - radian;
  if (radian < 0) radian = 0;

  return radian * rod_length;
}

class PathCmp {
 public:
  bool operator()(ConfBox3d* a, ConfBox3d* b) {
    return a->dist_Dijkstra > b->dist_Dijkstra;
  }
};

class PQCmp {
 public:
  bool operator()(const ConfBox3d* a, const ConfBox3d* b) {
    // use depth for now
    if (a->depth > b->depth) {
      return true;
    }
    // if same depth, expand box created earlier first
    if (a->depth == b->depth) {
      return a->box_id > b->box_id;
    }
    return false;
  }
};

// won't work with std pq, as this comparison is not transitional!
class DistCmp {
 public:
  bool operator()(const ConfBox3d* a, const ConfBox3d* b) {
    double distA = a->center.distance(goal);
    double distB = b->center.distance(goal);

    if (a->rotation_width != -2 && b->rotation_width != -2) {
      distA += rotationLength(a->rotation_center, goal_rotation);
      distB += rotationLength(b->rotation_center, goal_rotation);
    } else if (a->rotation_width == -2 && b->rotation_width != -2) {
      distB += rotationLength(b->rotation_center, goal_rotation);
    } else if (a->rotation_width != -2 && b->rotation_width == -2) {
      distA += rotationLength(a->rotation_center, goal_rotation);
    }

    return distA - distB > 0;
  }
};

// won't work with std pq, as this comparison is not transitional!
class BiDistCmp {
 public:
  bool operator()(const ConfBox3d* a, const ConfBox3d* b) {
    Point3d pa, pb, pa_rot, pb_rot;
    if (a->bidirection > 0) {
      pa = goal;
      pa_rot = goal_rotation;
    } else {
      pa = start;
      pa_rot = start_rotation;
    }
    if (b->bidirection > 0) {
      pb = goal;
      pb_rot = goal_rotation;
    } else {
      pb = start;
      pb_rot = start_rotation;
    }

    double distA = a->center.distance(pa);
    double distB = b->center.distance(pb);

    if (a->rotation_width != -2 && b->rotation_width != -2) {
      distA += rotationLength(a->rotation_center, pa_rot);
      distB += rotationLength(b->rotation_center, pb_rot);
    } else if (a->rotation_width == -2 && b->rotation_width != -2) {
      distB += rotationLength(b->rotation_center, pb_rot);
    } else if (a->rotation_width != -2 && b->rotation_width == -2) {
      distA += rotationLength(a->rotation_center, pa_rot);
    }

    return distA - distB > 0;
  }
};

// won't work with std pq, as this comparison is not transitional!
class DistSizeCmp {
 public:
  bool operator()(const ConfBox3d* a, const ConfBox3d* b) {
    double distA = a->center.distance(goal);
    double distB = b->center.distance(goal);

    if (a->rotation_width != -2 && b->rotation_width != -2) {
      distA += rotationLength(a->rotation_center, goal_rotation);
      distB += rotationLength(b->rotation_center, goal_rotation);
    } else if (a->rotation_width == -2 && b->rotation_width != -2) {
      distB += rotationLength(b->rotation_center, goal_rotation);
    } else if (a->rotation_width != -2 && b->rotation_width == -2) {
      distA += rotationLength(a->rotation_center, goal_rotation);
    }

    return distA * a->depth - distB * b->depth > 0;
  }
};

// won't work with std pq, as this comparison is not transitional!
class BiDistSizeCmp {
 public:
  bool operator()(const ConfBox3d* a, const ConfBox3d* b) {
    Point3d pa, pb, pa_rot, pb_rot;
    if (a->bidirection > 0) {
      pa = goal;
      pa_rot = goal_rotation;
    } else {
      pa = start;
      pa_rot = start_rotation;
    }
    if (b->bidirection > 0) {
      pb = goal;
      pb_rot = goal_rotation;
    } else {
      pb = start;
      pb_rot = start_rotation;
    }

    double distA = a->center.distance(pa);
    double distB = b->center.distance(pb);

    if (a->rotation_width != -2 && b->rotation_width != -2) {
      distA += rotationLength(a->rotation_center, pa_rot);
      distB += rotationLength(b->rotation_center, pb_rot);
    } else if (a->rotation_width == -2 && b->rotation_width != -2) {
      distB += rotationLength(b->rotation_center, pb_rot);
    } else if (a->rotation_width != -2 && b->rotation_width == -2) {
      distA += rotationLength(a->rotation_center, pa_rot);
    }

    return distA * a->depth - distB * b->depth > 0;
  }
};

// won't work with std pq, as this comparison is not transitional!
class VorCmp {
 public:
  bool operator()(const ConfBox3d* a, const ConfBox3d* b) {
    double distA = a->center.distance(goal);
    double distB = b->center.distance(goal);

    if (a->rotation_width != -2 && b->rotation_width != -2) {
      distA += rotationLength(a->rotation_center, goal_rotation);
      distB += rotationLength(b->rotation_center, goal_rotation);
    } else if (a->rotation_width == -2 && b->rotation_width != -2) {
      distB += rotationLength(b->rotation_center, goal_rotation);
    } else if (a->rotation_width != -2 && b->rotation_width == -2) {
      distA += rotationLength(a->rotation_center, goal_rotation);
    }

    double a_v =
        a->vor_corners.size() + a->vor_edges.size() + a->vor_walls.size();
    double b_v =
        b->vor_corners.size() + b->vor_edges.size() + b->vor_walls.size();

    return distA / a_v > distB / b_v;
    // return dista*dista/a_v > distb*distb/b_v;
    // return pow(dista, a_v) > pow(distb, b_v);
    // return dista/(a_v*a_v) > distb/(b_v*b_v);
    // return dista*a_v > distb*b_v;
    // return pow(dista, 1.0f/a_v) > pow(distb, 1.0f/b_v);

    // if this is true,
    // it means a's priority is lower than b's.
  }
};

// won't work with std pq, as this comparison is not transitional!
class BiVorCmp {
 public:
  bool operator()(const ConfBox3d* a, const ConfBox3d* b) {
    Point3d pa, pb, pa_rot, pb_rot;
    if (a->bidirection > 0) {
      pa = goal;
      pa_rot = goal_rotation;
    } else {
      pa = start;
      pa_rot = start_rotation;
    }
    if (b->bidirection > 0) {
      pb = goal;
      pb_rot = goal_rotation;
    } else {
      pb = start;
      pb_rot = start_rotation;
    }

    double distA = a->center.distance(pa);
    double distB = b->center.distance(pb);

    if (a->rotation_width != -2 && b->rotation_width != -2) {
      distA += rotationLength(a->rotation_center, pa_rot);
      distB += rotationLength(b->rotation_center, pb_rot);
    } else if (a->rotation_width == -2 && b->rotation_width != -2) {
      distB += rotationLength(b->rotation_center, pb_rot);
    } else if (a->rotation_width != -2 && b->rotation_width == -2) {
      distA += rotationLength(a->rotation_center, pa_rot);
    }

    double a_v =
        a->vor_corners.size() + a->vor_edges.size() + a->vor_walls.size();
    double b_v =
        b->vor_corners.size() + b->vor_edges.size() + b->vor_walls.size();

    //    if(a_v == 2 && b_v == 2)
    //      return dista > distb;
    //    else if(a_v == 2)
    //      return false;
    //    else if(b_v == 2)
    //      return true;
    //    else
    //      return a_v > b_v;

    return distA / a_v > distB / b_v;
  }
};

// won't work with std pq, as this comparison is not transitional!
class DistCmpSpecial {
 public:
  bool operator()(const ConfBox3d* a, const ConfBox3d* b) {
    Point3d pa, pb;
    if (a->bidirection > 0)
      pa = goal;
    else
      pa = start;
    if (b->bidirection > 0)
      pb = goal;
    else
      pb = start;

    double distA = std::numeric_limits<double>::max();
    if (a->nearest_corner != nullptr) {
      distA = a->center.distance(a->nearest_corner->point());
    }
    if (a->nearest_edge != nullptr &&
        distA > a->nearest_edge->segment().distance(a->center)) {
      distA = a->nearest_edge->segment().distance(a->center);
    }
    if (a->nearest_wall != nullptr &&
        distA > a->nearest_wall->triangle().distance(a->center)) {
      distA = a->nearest_wall->triangle().distance(a->center);
    }

    double distB = std::numeric_limits<double>::max();
    if (b->nearest_corner != nullptr) {
      distB = b->center.distance(b->nearest_corner->point());
    }
    if (b->nearest_edge != nullptr &&
        distB > b->nearest_edge->segment().distance(b->center)) {
      distB = b->nearest_edge->segment().distance(b->center);
    }
    if (b->nearest_wall != nullptr &&
        distB > b->nearest_wall->triangle().distance(b->center)) {
      distB = b->nearest_wall->triangle().distance(b->center);
    }

    if (fabs(distA - distB) < EPS)
      return (a->center.distance(pa) - b->center.distance(pb) > 0);
    else
      return (distA - distB < 0);
  }
};

class BoxQueue {
 public:
  BoxQueue(void) {}

  virtual void push(ConfBox3d* b) = 0;

  virtual ConfBox3d* extract() = 0;

  virtual bool empty() = 0;

  virtual int size() = 0;

  virtual void clear() = 0;

  virtual void process(ConfBox3d* b) {
    if (b->getStatus() == MIXED) push(b);
  }

  ~BoxQueue(void) {}
};

class SeqQueue : public BoxQueue {
 private:
  std::priority_queue<ConfBox3d*, std::vector<ConfBox3d*>, PQCmp> PQ;

 public:
  void push(ConfBox3d* b) { PQ.push(b); }

  ConfBox3d* extract() {
    ConfBox3d* r = PQ.top();
    PQ.pop();
    return r;
  }

  bool empty() { return PQ.empty(); }

  int size() { return PQ.size(); }

  void clear() {
    PQ = std::priority_queue<ConfBox3d*, std::vector<ConfBox3d*>, PQCmp>();
  }
};

class RandQueue : public BoxQueue {
 private:
  std::list<ConfBox3d*> L;

 public:
  RandQueue() {
    // srand( Qseed );
  }

  void push(ConfBox3d* b) { L.push_back(b); }

  ConfBox3d* extract() {
    unsigned long i = static_cast<unsigned long>(rand()) % L.size();
    std::list<ConfBox3d*>::iterator iter = L.begin();
    advance(iter, i);
    ConfBox3d* r = *iter;
    L.erase(iter);
    return r;
  }

  bool empty() { return L.empty(); }

  int size() { return L.size(); }

  void clear() { L.clear(); }
};

template <typename Cmp>
class DijkstraQueue : public BoxQueue {
 private:
  std::vector<ConfBox3d*> bv;

 public:
  DijkstraQueue() {}

  void push(ConfBox3d* b) {
    distHeap<Cmp>::insert(bv, b);

    if (search_type != VORONOI && search_type != BIVORONOI)
      distHeap<Cmp>::insert(bv, b);
    else {
      if (move_away ||
          (b->vor_corners.size() + b->vor_edges.size() + b->vor_walls.size() >
           2)) {
        distHeap<Cmp>::insert(bv, b);
      }
    }
  }

  ConfBox3d* extract() {
    ConfBox3d* b = distHeap<Cmp>::extractMin(bv);
    return b;
  }

  bool empty() { return bv.empty(); }

  int size() { return bv.size(); }

  void clear() { bv.clear(); }

  void process(ConfBox3d* b) {}
};

#endif  // BOX_PRIORITY_QUEUE_H
