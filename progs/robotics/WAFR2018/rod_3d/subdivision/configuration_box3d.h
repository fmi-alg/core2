/*
 * configuration_box3d.h
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef CONFIGURATION_BOX3D_H
#define CONFIGURATION_BOX3D_H

#include <assert.h>
#include <iostream>
#include <vector>

#include "geometry/corner.h"
#include "geometry/edge.h"
#include "geometry/wall.h"
#include "predicate/configuration_box3d_predicate.h"
#include "subdivision/box3d.h"
#include "subdivision/rot3d.h"
#include "subdivision/status.h"

class ConfBox3d;
class Set;
class ConfBox3dPredicate;

class ConfBox3d {
 private:
  Box3d* box_ = nullptr;
  Rot3d* rot_ = nullptr;

 public:
  Point3d center;
  Point3d rotation_center;
  Segment3d rod;

  double width;
  double rotation_width;

  int depth;
  static double rod_length;
  double rB;
  static int box_id_counter;
  int box_id;

  static ConfBox3dPredicate* predicate;

  std::vector<ConfBox3d*> children;
  std::vector<ConfBox3d*> neighbors;
  ConfBox3d* parent;

  Status status;

  Set* p_set;

  std::vector<Corner*> corners;
  std::vector<Edge*> edges;
  std::vector<Wall*> walls;

  std::vector<Corner*> vor_corners;
  std::vector<Edge*> vor_edges;
  std::vector<Wall*> vor_walls;

  Corner* nearest_corner;
  Edge* nearest_edge;
  Wall* nearest_wall;

  static std::vector<ConfBox3d*> boxes;

  bool is_in_source;
  int heap_id;
  ConfBox3d* prev;
  bool visited;
  bool in_queue;
  bool in_vor_queue;

  int bidirection;

  // for generate path
  bool in_source;
  double dist_Dijkstra;

  ConfBox3d(Point3d point, double w);
  ConfBox3d(Point3d point, double ww, Point3d rotation_point, double rot_ww);

  bool isLeaf();

  bool getRot(float rot[3]);

  ConfBox3d* getBox(Point3d point, Point3d rotation_point);
  ConfBox3d* getNeighborBox(ConfBox3d* neighbor);

  void addCorner(Corner* c);

  void addEdge(Edge* e);

  void addWall(Wall* w);

  bool is_free();

  Status getStatus();

  std::vector<ConfBox3d*> getChildren();

  std::vector<ConfBox3d*> getNeighbors();

  bool isNeighbor(ConfBox3d* other);
  bool isAdjacent(ConfBox3d* other);

  bool split(double epsilon);

  bool contains(ConfBox3d* other);
  bool rotIsOverlapped(ConfBox3d* other);

  static ConfBox3d* makePointBox(Point3d point, Point3d rotation_point);
};

#endif  // CONFIGURATION_BOX3D_H
