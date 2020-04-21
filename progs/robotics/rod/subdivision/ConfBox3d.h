#pragma once

#include <vector>
#include <list>
#include <assert.h>
#include "../geometry/Wall.h"
#include "../geometry/Edge.h"
#include "../geometry/Corner.h"
#include "./Box3d.h"
#include "./Rot3dSide.h"
#include <iostream>
#include "../predicate/ConfBox3dPredicate.h"
#include "./Status.h"

class ConfBox3d;
class Set;
class ConfBox3dPredicate;
/* class ConfBox3dPredicate { */
 /* public: */
  /* void classify(ConfBox3d* b); */
  /* Status checkChildStatus(ConfBox3d* b, double x, double y, double z); */

/* }; */

class ConfBox3d {
 private:
  Box3d* box = 0;
  Rot3dSide* rot = 0;
 public:
  double x;
  double y;
  double z;
  double rot_x;
  double rot_y;
  double rot_z;
  double width;
  double rot_width;
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
  list<Corner*> corners;
  list<Edge*> Edges;
  list<Wall*> Walls;
  list<Sphere*> spheres;

  static vector<ConfBox3d*> boxes;

  double dist2Source;
  int heapId;
  ConfBox3d* prev;
  bool visited;

  ConfBox3d(double xx, double yy, double zz, double w);
  ConfBox3d(double xx, double yy, double zz, double w, double rot_xx, double rot_yy, double rot_zz, double rot_ww);

  bool isLeaf();

  bool getRot(float rot[3]);

  ConfBox3d* getBox(double xx, double yy, double zz, double rot_xx = 0, double rot_yy = 1, double rot_zz = 0);

  void addCorner(Corner* c);

  void addEdge(Edge* e);

  void addWall(Wall* w);

  void addSphere(Sphere* s);

  bool isFree();

  Status getStatus();

  vector<ConfBox3d*> getChildren();

  vector<ConfBox3d*> getNeighbors();

  bool isNeighbor(ConfBox3d* other);

  bool split(double epsilon);

  static ConfBox3d* makePointBox(double xx, double yy, double zz, double rot_xx = 0, double rot_yy = 1, double rot_zz = 0);
};
