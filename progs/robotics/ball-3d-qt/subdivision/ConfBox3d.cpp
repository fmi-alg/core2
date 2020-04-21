#include "./ConfBox3d.h"

double ConfBox3d::r0;
int ConfBox3d::boxIdCounter;
ConfBox3dPredicate* ConfBox3d::predicate;
vector<ConfBox3d*> ConfBox3d::boxes;


ConfBox3d::ConfBox3d(double xx, double yy, double zz, double w):
  depth(1), x(xx), y(yy), z(zz), width(w),
  parent(0), status(UNKNOWN),
  pSet(0), dist2Source(-1), heapId(-1), prev(0), visited(false) {
  box = new Box3d(xx, yy, zz, w);
  boxIdCounter++;
  boxId = boxIdCounter;
  rB = (w * sqrt(3))/2;
  boxes.push_back(this);
  predicate = new ConfBox3dPredicate();
  children.clear();
  /* cout << boxId << "\t" << xx << "\t" << yy << "\t" << zz << "\t" << width << endl; */
}

ConfBox3d::ConfBox3d(double xx, double yy, double zz, double w, double rot_xx, double rot_yy, double rot_zz, double rot_ww):
  depth(1), x(xx), y(yy), z(zz), width(w),
  rot_x(rot_xx), rot_y(rot_yy), rot_z(rot_zz), rot_width(rot_ww),
  parent(0), status(UNKNOWN),
  pSet(0), dist2Source(-1), heapId(-1), prev(0), visited(false) {
  box = new Box3d(xx, yy, zz, w);
  rot = new Rot3dSide(rot_xx, rot_yy, rot_zz, rot_ww);
  boxIdCounter++;
  boxId = boxIdCounter;
  rB = (w * sqrt(3))/2;
  boxes.push_back(this);
  predicate = new ConfBox3dPredicate();
  children.clear();
  //cout << boxId << "\t" << xx << "\t" << yy << "\t" << zz << "\t" << width << endl;
}

bool ConfBox3d::isLeaf() {
  return children.empty();
}

bool ConfBox3d::getRot(float rot[3]) {
  if (!this->rot) {
    return false;
  }
  rot[0] = this->rot->origin->x;
  rot[1] = this->rot->origin->y;
  rot[2] = this->rot->origin->z;
  return true;
}

ConfBox3d* ConfBox3d::getBox(double xx, double yy, double zz, double rot_xx, double rot_yy, double rot_zz) {
    if (isLeaf()) {
      bool containsPoint = box->containsPoint(xx, yy, zz) &&
        (!rot || rot->containsPoint(rot_xx, rot_yy, rot_zz));
      return containsPoint ? this : 0;
    } else {
      for (int i = 0; i < children.size(); i++) {
        ConfBox3d* b = children[i]->getBox(xx, yy, zz);
        if (b != 0) {
          return b;
        }
      }
      return 0;
    }
  }

void ConfBox3d::addCorner(Corner* c) {
  corners.push_back(c);
}

void ConfBox3d::addEdge(Edge* e) {
  Edges.push_back(e);
}

void ConfBox3d::addWall(Wall* w) {
  Walls.push_back(w);
}

void ConfBox3d::addSphere(Sphere* s) {
  spheres.push_back(s);
}

bool ConfBox3d::isFree() {
  return status == FREE;
}

Status ConfBox3d::getStatus() {
  predicate->classify(this);
  return status;
}

vector<ConfBox3d*> ConfBox3d::getChildren() {
  return children;
}

vector<ConfBox3d*> ConfBox3d::getNeighbors() {
  return neighbors;
}

// When any of the boxes does not have restrictions on the rotational degrees of freedom, just compare their translation boxes;
// Otherwise, one of the components should be the same and the other adjacent
bool ConfBox3d::isNeighbor(ConfBox3d* other) {
  if (!rot || !other->rot) {
    return box->isAdjacent(other->box);
  } else {
    return (rot->isIdentical(other->rot) && box->isAdjacent(other->box)) ||
      (rot->isAdjacent(other->rot) && box->isIdentical(other->box));
  }
}

bool ConfBox3d::split(double epsilon) {
  if (!children.empty()) {
    // Subdivided already
    return false;
  }
  if (box->split(epsilon)) {
    // Can still subdivide the translational component
    vector<Box3d*> box3dChildren = box->children;
    for (int i = 0; i < box3dChildren.size(); i++) {
      Box3d* b = box3dChildren[i];
      children.push_back(new ConfBox3d(b->origin->x, b->origin->y, b->origin->z, b->width));
    }
  } else if (!rot) {
    // Need to subdivide into six sides
    for (int i = 0; i < 3; i++) {
      for (int j = -1; j < 3; j += 2) {
        double r[3] = {0, 0, 0};
        r[i] = j;
        children.push_back(new ConfBox3d(box->origin->x, box->origin->y, box->origin->z, box->width, r[0], r[1], r[2], 1));
      }
    }
  } else if (rot->split(epsilon)) {
    // Can split the rotational component
    vector<Rot3dSide*> rotChildren = rot->children;
    for (int i = 0; i < rotChildren.size(); i++) {
      Rot3dSide* r = rotChildren[i];
      children.push_back(new ConfBox3d(box->origin->x, box->origin->y, box->origin->z, box->width, r->origin->x, r->origin->y, r->origin->z, r->width));
    }
  } else {
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
    children[i]->corners.insert(children[i]->corners.begin(),
                                corners.begin(),
                                corners.end());
    children[i]->spheres.insert(children[i]->spheres.begin(),
                                spheres.begin(),
                                spheres.end());
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
      bool isNeighbor = children[i]->isNeighbor(neighbors[j]);
      if (isNeighbor) {
        children[i]->neighbors.push_back(neighbors[j]);
        neighbors[j]->neighbors.push_back(children[i]);
      }
    }
  }
  return true;
}

ConfBox3d* ConfBox3d::makePointBox(double xx, double yy, double zz, double rot_xx, double rot_yy, double rot_zz) {
  return new ConfBox3d(xx, yy, zz, 0, rot_xx, rot_yy, rot_zz, 0);
}
