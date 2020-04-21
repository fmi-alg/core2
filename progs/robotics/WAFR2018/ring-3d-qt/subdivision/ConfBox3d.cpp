#include "subdivision/ConfBox3d.h"

extern FILE *g_fptr;
extern bool verbose;

double ConfBox3d::r0;
int ConfBox3d::boxIdCounter;
ConfBox3dPredicate* ConfBox3d::predicate;
vector<ConfBox3d*> ConfBox3d::boxes;


ConfBox3d::ConfBox3d(Point3d pp, double ww):
  depth(1), mB(pp), width(ww),
  parent(0), status(UNKNOWN),
  pSet(0), dist2Source(-1), heapId(-1), prev(0), visited(false), inQueue(false), inVorQueue(false), bidirection(0),
  inSource(false), distDij(-1){
  box = new Box3d(pp, ww);
  rot_mB.set(0,0,0);
  rot_x_width = rot_y_width = rot_z_width = rot_width = -2;
  boxIdCounter++;
  boxId = boxIdCounter;
  rB = (ww * sqrt(3))/2;
  boxes.push_back(this);
  predicate = new ConfBox3dPredicate();
  children.clear();

  ring = Circle3d(pp, 0, Vector(0,0,0));
}

ConfBox3d::ConfBox3d(Point3d pp, double ww, Point3d rot_pp, double rot_ww):
  depth(1), mB(pp), width(ww), rot_mB(rot_pp), rot_width(rot_ww),
  parent(0), status(UNKNOWN),
  pSet(0), dist2Source(-1), heapId(-1), prev(0), visited(false), inQueue(false), inVorQueue(false), bidirection(0),
  inSource(false), distDij(-1){
  box = new Box3d(pp, ww);
  rot = new Rot3dSide(rot_pp, rot_ww);
  boxIdCounter++;
  boxId = boxIdCounter;
  rB = (ww * sqrt(3))/2;
  boxes.push_back(this);
  predicate = new ConfBox3dPredicate();
  children.clear();

  rot_pp = Rot3dSide::cube2sphere(rot_pp);
  Vector direction(rot_pp.X(), rot_pp.Y(), rot_pp.Z());
  direction.normalize();
  ring = Circle3d(pp, r0, direction);
}

bool ConfBox3d::isLeaf() {
  return children.empty();
}

bool ConfBox3d::getRot(float rot[3]) {
  if (!this->rot) {
    return false;
  }
  rot[0] = this->rot->origin->X();
  rot[1] = this->rot->origin->Y();
  rot[2] = this->rot->origin->Z();
  return true;
}

ConfBox3d* ConfBox3d::getBox(Point3d pp, Point3d rot_pp) {
  if (isLeaf()) {
    bool containsPoint = box->containsPoint(pp) && (!rot || rot->containsPoint(rot_pp));
    return containsPoint ? this : 0;
  }
  else {
    for (unsigned i = 0; i < children.size(); ++i) {
      ConfBox3d* b = children[i]->getBox(pp, rot_pp);
      if (b != 0) {
        return b;
      }
    }
    return 0;
  }
}

void ConfBox3d::addCorner(Corner* c) {
  Corners.push_back(c);
  vorCorners.push_back(c);
}

void ConfBox3d::addEdge(Edge* e) {
  Edges.push_back(e);
  vorEdges.push_back(e);
}

void ConfBox3d::addWall(Wall* w) {
  Walls.push_back(w);
  vorWalls.push_back(w);
}

void ConfBox3d::addSphere(Sphere* s) {
  //spheres.push_back(s);
}

bool ConfBox3d::isFree() {
  return status == FREE;
}

Status ConfBox3d::getStatus() {
  if(status == UNKNOWN) predicate->classification(this, this->ring);
  return status;
}

vector<ConfBox3d*> ConfBox3d::getChildren() {
  return children;
}

vector<ConfBox3d*> ConfBox3d::getNeighbors() {
  return neighbors;
}

bool ConfBox3d::contains(ConfBox3d* other){
  return box->containsPoint(other->mB) &&
       (!rot || rot->containsPoint(other->rot_mB));
}
bool ConfBox3d::rotIsOverlapped(ConfBox3d* other){
  return !rot || rot->isOverlap(other->rot);
}

bool ConfBox3d::sameDirectionAdjacent(Box3d* b, Rot3dSide* r) {

  double ori = (*rot->orientation) * (*(r->orientation));
  // must be same orientation
  if (ori == 1) {
    bool adj;
    if(abs(rot->orientation->X()) == 1)
      adj = box->isSameDirectionAdjacent(b, 1);
    else if(abs(rot->orientation->Y()) == 1)
      adj = box->isSameDirectionAdjacent(b, 2);
    else
      adj = box->isSameDirectionAdjacent(b, 3);

    return adj && rot->contains(r);
  }
  else return false;
}

// When any of the boxes does not have restrictions on the rotational degrees of freedom, just compare their translation boxes;
// Otherwise, one of the components should be the same and the other adjacent
bool ConfBox3d::isNeighbor(ConfBox3d* other) {
  if (!rot || !other->rot) {
    return box->isAdjacent(other->box);
  }
  else {
    return (rot->isIdentical(other->rot) && box->isAdjacent(other->box)) ||
           (other->rot->contains(rot) && box->isAdjacent(other->box)) ||
           (rot->isAdjacent(other->rot) && box->isIdentical(other->box));
  }
}

bool ConfBox3d::split(double epsilon) {

  if (!children.empty()) {
    // Subdivided already
    return false;
  }

  //fprintf(g_fptr, "split\n");

  if (box->split(epsilon)) {
    // Can still subdivide the translational component
    vector<Box3d*> box3dChildren = box->children;
    for (unsigned i = 0; i < box3dChildren.size(); ++i) {
      Box3d* b = box3dChildren[i];
      children.push_back(new ConfBox3d(*b->origin, b->width));
    }
  }
  else if (!rot) {

    //fprintf(g_fptr, "split face\n");

    // Need to subdivide into six sides
    for (int i = 0; i < 3; i++) {
      for (int j = -1; j < 3; j += 2) {
        double r[3] = {0, 0, 0};
        r[i] = j;
        children.push_back(new ConfBox3d(*box->origin, box->width, Point3d(r[0], r[1], r[2]), 2));
      }
    }
  }
  else if (rot->split(epsilon/r0)) {
    // Can split the rotational component
    vector<Rot3dSide*> rotChildren = rot->children;
    for (unsigned i = 0; i < rotChildren.size(); ++i) {
      Rot3dSide* r = rotChildren[i];
      children.push_back(new ConfBox3d(*box->origin, box->width, *r->origin, r->width));
    }
  }
  else {
    // Cannot subdivide, epsilon reached
    return false;
  }


  for (int i = 0; i < children.size(); i++) {
    children[i]->parent = this;
    children[i]->depth = this->depth + 1;

    children[i]->Walls.insert(children[i]->Walls.begin(),
                              Walls.begin(),
                              Walls.end());
    children[i]->Edges.insert(children[i]->Edges.begin(),
                              Edges.begin(),
                              Edges.end());
    children[i]->Corners.insert(children[i]->Corners.begin(),
                                Corners.begin(),
                                Corners.end());


    children[i]->vorWalls.insert(children[i]->vorWalls.begin(),
                                 vorWalls.begin(),
                                 vorWalls.end());
    children[i]->vorEdges.insert(children[i]->vorEdges.begin(),
                                 vorEdges.begin(),
                                 vorEdges.end());
    children[i]->vorCorners.insert(children[i]->vorCorners.begin(),
                                   vorCorners.begin(),
                                   vorCorners.end());
  }


  for (int i = 0; i < children.size(); i++) {
    for (int j = i + 1; j < children.size(); j++) {
      if (children[i]->isNeighbor(children[j])) {
        children[i]->neighbors.push_back(children[j]);
        children[j]->neighbors.push_back(children[i]);
      }
    }
  }
  for (int i = 0; i < children.size(); i++) {
    for (int j = 0; j < neighbors.size(); j++) {
      if (children[i]->isNeighbor(neighbors[j])) {
        children[i]->neighbors.push_back(neighbors[j]);
        neighbors[j]->neighbors.push_back(children[i]);
      }
    }
  }

  return true;
}

ConfBox3d* ConfBox3d::makePointBox(Point3d pp, Point3d rot_pp) {
  if(abs(rot_pp.X())*abs(rot_pp.Y())*abs(rot_pp.Z()) > 0)
    return new ConfBox3d(pp, 0, rot_pp, 0);
  else
    return new ConfBox3d(pp, 0);
}
