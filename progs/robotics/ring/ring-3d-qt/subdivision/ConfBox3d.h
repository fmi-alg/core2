#pragma once

#include <vector>
#include <list>
#include <assert.h>
#include <iostream>

#include "CORE/geom3d/circle3d.h"
#include "geometry/Wall.h"
#include "geometry/Edge.h"
#include "geometry/Corner.h"
#include "subdivision/Box3d.h"
#include "subdivision/Rot3dSide.h"
#include "predicate/ConfBox3dPredicate.h"
#include "subdivision/Status.h"

class ConfBox3d;
class Set;
class ConfBox3dPredicate;

class ConfBox3d {
 private:
  Box3d* box = 0;
  Rot3dSide* rot = 0;
 public:
  Point3d mB;
  Point3d rot_mB;
  Circle3d ring;

  double width;
  double rot_width;
  // special usage for square
  double rot_x_width;
  double rot_y_width;
  double rot_z_width;

  int depth;
  static double r0;
  double rB;
  static int boxIdCounter;
  int boxId;

  static ConfBox3dPredicate* predicate;
  vector<ConfBox3d*> children;
  vector<ConfBox3d*> neighbors;
  ConfBox3d* parent;

  Status status;

  Set* pSet;

  list<Corner*> Corners;
  list<Edge*> Edges;
  list<Wall*> Walls;
  list<Sphere*> spheres;

  list<Corner*> vorCorners;
  list<Edge*> vorEdges;
  list<Wall*> vorWalls;

  Corner* nearestC;
  Edge* nearestE;
  Wall* nearestW;

  static vector<ConfBox3d*> boxes;

  double dist2Source;
  int heapId;
  ConfBox3d* prev;
  bool visited;
  bool inQueue;
  bool inVorQueue;

  int bidirection;

  //for generate path
  bool inSource;
  double distDij;

  ConfBox3d(Point3d pp, double w);
  ConfBox3d(Point3d pp, double ww,
            Point3d rot_pp, double rot_ww);

  bool isLeaf();

  bool getRot(float rot[3]);

  ConfBox3d* getBox(Point3d pp, Point3d rot_pp);

  void addCorner(Corner* c);

  void addEdge(Edge* e);

  void addWall(Wall* w);

  void addSphere(Sphere* s);

  bool isFree();

  Status getStatus();

  vector<ConfBox3d*> getChildren();

  vector<ConfBox3d*> getNeighbors();

  bool sameDirectionAdjacent(Box3d* b, Rot3dSide* r);
  bool isNeighbor(ConfBox3d* other);

  bool split(double epsilon);

  bool contains(ConfBox3d* other);
  bool rotIsOverlapped(ConfBox3d* other);

  static ConfBox3d* makePointBox(Point3d pp, Point3d rot_pp);
};
