#include "Box.h"
#include "QuadTree.h"
#include "Triangle.h"
#include "Corner.h"




//
//#define OVERRIDE_NEW_DELETE
//#include "MemProCpp\MemPro.cpp"

//extern vector<Polygon> polygons;
//extern vector<int> srcInPolygons;
extern vector<Triangle> compRoboTriangles;

extern int twoStrategyOption;
extern QuadTree* QT;
extern int sizeOfPhiB;
extern unsigned int renderSteps;
extern bool step;
extern int numberForDisplay;

//extern FILE *fptr;

double Box::r0 = 0;
double Box::THETA_MIN = 0;
int Box::counter = 0;
int maxDep = 1;
vector<Box*>* Box::pAllLeaf = 0;
vector<int> expansions;
int Box::oppositeDir[6] = {2, 3, 0, 1, 5, 4};

bool Box::split2D( double epsilon, vector<Box*>& chldn )
{
    //todo do sth to return false
    if (this->height < epsilon || this->width < epsilon)
    {
        return 0;
    }

    if (!this->isLeaf || this->status == FREE || this->status == STUCK)
    {
        return 0;
    }

    Box* children[4];
    //same as quadrants I, II, III, IV
    children[2] = new Box(x - width / 4, y + height / 4, width / 2, height / 2);
    children[1] = new Box(x + width / 4, y + height / 4, width / 2, height / 2);
    children[0] = new Box(x + width / 4, y - height / 4, width / 2, height / 2);
    children[3] = new Box(x - width / 4, y - height / 4, width / 2, height / 2);
    for (int i = 0; i < 4; ++i)
    {
        children[i]->depth = this->depth + 1;
        children[i]->isBig = this->isBig;
        children[i]->pParent = this;
        children[i]->tChildID = i;

        children[i]->xi[0] = this->xi[0];
        children[i]->xi[1] = this->xi[1];

        maxDep = maxDep > children[i]->depth ? maxDep : children[i]->depth;
    }

    children[0]->Nhbrs[1].push_back(children[1]);
    children[0]->Nhbrs[2].push_back(children[3]);
    children[1]->Nhbrs[2].push_back(children[2]);
    children[1]->Nhbrs[3].push_back(children[0]);
    children[2]->Nhbrs[3].push_back(children[3]);
    children[2]->Nhbrs[0].push_back(children[1]);
    children[3]->Nhbrs[0].push_back(children[0]);
    children[3]->Nhbrs[1].push_back(children[2]);

    for (int i = 0; i < 6; ++i)
    {
        for (vector<Box*>::iterator iter = this->Nhbrs[i].begin(); iter != this->Nhbrs[i].end(); ++iter)
        {
            Box* b = *iter;
            int foundDst = 0;
            for (int j = 0; j < 4; ++j)
            {
                int idx = isNhbr(children[j], b);
                if (idx != -1)
                {
                    children[j]->Nhbrs[idx].push_back(b);
                    if (foundDst == 0)
                    {
                        for (vector<Box*>::iterator it = b->Nhbrs[oppositeDir[idx]].begin();
                            it != b->Nhbrs[oppositeDir[idx]].end(); ++it)
                        {
                            if (*it == this)
                            {
                                *it = children[j];
                            }
                        }
                    }
                    else
                    {
                        b->Nhbrs[oppositeDir[idx]].push_back(children[j]);
                    }
                    ++foundDst;
                }
            }
            assert(foundDst > 0);
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        //add all of parent's walls and corners to each child,
        //will be filtered later in updatestatus()
        for (unsigned int k = 0; k < compRoboTriangles.size(); ++k){
          children[i]->walls[k].insert(children[i]->walls[k].begin(), this->walls[k].begin(), this->walls[k].end());
          children[i]->corners[k].insert(children[i]->corners[k].begin(), this->corners[k].begin(), this->corners[k].end());
        }
        BoxNode node;
        node.x=children[i]->x;
        node.y=children[i]->y;
        determine_clearance(node);
        children[i]->cl_m=node.clearance;

        //distribute the feature
        distribute_features2box(children[i]);
    }

    this->isLeaf = false;

    for (int i = 0; i < 4; ++i)
    {
        pAllLeaf->push_back(children[i]);
        chldn.push_back(children[i]);
    }
    //list<Box*>::iterator it = find(pAllLeaf->begin(), pAllLeaf->end(), this);
    //pAllLeaf->erase(it);

    return true;
}

bool Box::split3D( double epsilon, vector<Box*>& chldn )
{
    vector<Box*> bv;
    if (this->splitAngle(bv))
    {

        for (int i = 0; i < 2; ++i)
        {
            if (!bv[i]->split2D(epsilon, chldn))
            {
                return false;
            }
        }
        return true;
    }
    return 0;
}

bool Box::splitAngle( vector<Box*>& chldn )
{
    //todo do sth to return false
    if ( this->xi[1] > this->xi[0] )
    {
        //todo, 0.028 is somehow random
        if ( (this->xi[1] - this->xi[0]) * PI < 0.014)
        {
            return 0;
        }
    }
    else
    {
        if ( (2 + this->xi[1] - this->xi[0]) * PI < 0.014)
        {
            return 0;
        }
    }

    if (!this->isLeaf || this->status == FREE || this->status == STUCK)
    {
        return 0;
    }

    Box* children[2];
    children[1] = new Box(x, y, width, height);
    children[0] = new Box(x, y, width, height);
    for (int i = 0; i < 2; ++i)
    {
        children[i]->depth = this->depth;
        children[i]->isBig = false;
        children[i]->pParent = this;
        children[i]->rChildID = i;
    }
    //todo, 0(2) is within [xi[0], xi[1]] shouldn't happen
    children[0]->xi[0] = this->xi[0];
    children[0]->xi[1] = this->xi[0] + (this->xi[1] - this->xi[0]) / 2;
    children[1]->xi[0] = this->xi[0] + (this->xi[1] - this->xi[0]) / 2;
    children[1]->xi[1] = this->xi[1];

    //simple case
    children[0]->Nhbrs[4].push_back(children[1]);
    children[1]->Nhbrs[5].push_back(children[0]);

    for (int i = 0; i < 6; ++i)
    {
        for (vector<Box*>::iterator iter = this->Nhbrs[i].begin(); iter != this->Nhbrs[i].end(); ++iter)
        {
            Box* b = *iter;
            int foundDst = 0;
            for (int j = 0; j < 2; ++j)
            {
                int idx = isNhbr(children[j], b);
                if (idx != -1)
                {
                    children[j]->Nhbrs[idx].push_back(b);
                    if (foundDst == 0)
                    {
                        for (vector<Box*>::iterator it = b->Nhbrs[oppositeDir[idx]].begin();
                            it != b->Nhbrs[oppositeDir[idx]].end(); ++it)
                        {
                            if (*it == this)
                            {
                                *it = children[j];
                                break;
                            }
                        }
                    }
                    else
                    {
                        b->Nhbrs[oppositeDir[idx]].push_back(children[j]);
                    }
                    ++foundDst;
                }
            }
            assert(foundDst > 0 && foundDst < 3);
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        //add all of parent's walls and corners to each child,
        //will be filtered later in updatestatus()
        for (unsigned int k = 0; k < compRoboTriangles.size(); k++) {
          children[i]->walls[k].insert(children[i]->walls[k].begin(),
              this->walls[k].begin(), this->walls[k].end());
          children[i]->corners[k].insert(
              children[i]->corners[k].begin(),
              this->corners[k].begin(), this->corners[k].end());
        }
        children[i]->vorWalls.insert(children[i]->vorWalls.begin(),
            this->vorWalls.begin(), this->vorWalls.end());
        children[i]->vorCorners.insert(children[i]->vorCorners.begin(),
            this->vorCorners.begin(), this->vorCorners.end());
    }

    this->isLeaf = false;
    for (int i = 0; i < 2; ++i) {
        pAllLeaf->push_back(children[i]);
        chldn.push_back(children[i]);
    }
    //list<Box*>::iterator it = find(pAllLeaf->begin(), pAllLeaf->end(), this);
    //pAllLeaf->erase(it);

    return true;
}

bool Box::split( double epsilon, vector<Box*>& chldn ) {
    if (!this->isBig) { // not isBig:
        return split3D(epsilon, chldn);
    }
    else// if( rB / 2 < r0 * 2 / THETA_MIN / 20 ) {// one-shot intermediate split before
                // doing split3D( ).  After this, isBig is false.
                // This intermediate step speeds up the code...
                // THETA_MIN is roughly the minimum angle of the triangle robot

        //int n = ceil( 2 / THETA_MIN );
        //int m = 1;
        //for (int i = 0; i < n; ++i)
        //{
        //	splitAngle(epsilon);
        //}
        //return splitAngle(epsilon);
      if (rB < epsilon) {
      //if (rB < r0) {
        int n = ceil( 2 / THETA_MIN );
        n = 32;
        int m = 1;
        recursiveSplitAngle(epsilon, chldn, n, m);
        return true;
    }
    else { // do translational degree
        return split2D(epsilon, chldn);
    }
}

void Box::recursiveSplitAngle( double epsilon, vector<Box*>& chldn, const int n, int m )
{
    if (m >= n) {
        chldn.push_back(this);
        return;
    }
    vector<Box*> bv;
    splitAngle(bv);
    for (int i = 0; i < (int)bv.size(); ++i) {
        bv[i]->recursiveSplitAngle(epsilon, chldn, n, 2*m);
    }
}

Box::Status Box::checkChildStatus( double x, double y ) {

    // 4/5 Tom:
    // this flag is true if
    // the closest feature is a wall
    // and it is on the corner(u = 0 | 1)
    // and it is a convex corner.
    bool flag = false;
    Wall* nearestWall = NULL;
    double mindistW = FLT_MAX;

    for (unsigned int k = 0; k < compRoboTriangles.size(); ++k) {
        if (walls[k].size() > 0) {
            for (list<Wall*>::iterator iterW = walls[k].begin(); iterW != walls[k].end(); ++iterW) {
                Wall* w = *iterW;
                pt2line rst = w->distance2(x, y);

                if (mindistW > rst.dist) {
                    mindistW = rst.dist;
                    nearestWall = *iterW;

                    flag = false;
                    if (rst.u == 0) flag = w->src->isConvex();
                    if (rst.u == 1) flag = w->dst->isConvex();
                }
            }
        }
    }

    Corner* nearestCorner = NULL;
    double mindistC = FLT_MAX;
    for (unsigned int k = 0; k < compRoboTriangles.size(); k++) {
        if (corners[k].size() > 0) {
            for (list<Corner*>::iterator iterC = corners[k].begin(); iterC != corners[k].end(); ++iterC) {
                Corner* c = *iterC;
                double dist = c->distance(x, y);

                if (mindistC > dist) {
                    mindistC = dist;
                    nearestCorner = *iterC;
                }
            }
        }
    }
    //nearest feature is a wall
    if (mindistW < mindistC){
        if (nearestWall->isRight(x, y) || flag){
            return FREE;
        }
    }
    //otherwise check the corner's convexity
    //if convex, out; if concave, in
    //note that if a wall and a corner are the same dist,
    //only need to take care of the corner
    else{
        if (nearestCorner->isConvex()) {
            return FREE;
        }
    }
    return STUCK;
}

int Box::isNhbr( Box* b1, Box* b2 ) {
    double xmin1 = b1->x - b1->width / 2;
    double xmax1 = b1->x + b1->width / 2;
    double ymin1 = b1->y - b1->height / 2;
    double ymax1 = b1->y + b1->height / 2;
    double xmin2 = b2->x - b2->width / 2;
    double xmax2 = b2->x + b2->width / 2;
    double ymin2 = b2->y - b2->height / 2;
    double ymax2 = b2->y + b2->height / 2;

    if (isEq(xmin1, xmax2) && isOneContainAnotherY(b1, b2) && isOneContainAnotherA(b1, b2)) {
        return 2;
    }
    if (isEq(xmax1, xmin2) && isOneContainAnotherY(b1, b2) && isOneContainAnotherA(b1, b2)) {
        return 0;
    }
    if (isEq(ymin1, ymax2) && isOneContainAnotherX(b1, b2) && isOneContainAnotherA(b1, b2)) {
        return 3;
    }
    if (isEq(ymax1, ymin2) && isOneContainAnotherX(b1, b2) && isOneContainAnotherA(b1, b2)) {
        return 1;
    }
    //down
    if (isEq(b1->xi[0], b2->xi[1]) && isOneContainAnotherX(b1, b2) && isOneContainAnotherY(b1, b2)) {
        return 5;
    }
    //up
    if (isEq(b1->xi[1], b2->xi[0]) && isOneContainAnotherX(b1, b2) && isOneContainAnotherY(b1, b2)) {
        return 4;
    }

    return -1;
}

void Box::updateStatusBig() {
    if (status != UNKNOWN) return;

    double outerDomain = r0 + rB;
    // innerDomain unused:
    // double innerDomain = r0 > rB ? r0 - rB : 0;
    int total_corners_left = 0;
    int total_walls_left = 0;

    for (unsigned int k = 0; k < compRoboTriangles.size(); k++) {
        for (list<Corner*>::iterator it = corners[k].begin(); it != corners[k].end();) {
            Corner* c = *it;
            if( c->distance(this->x, this->y) <= outerDomain ) {
                status = MIXED;
                ++it;
            }
            else {
                it = corners[k].erase(it);
            }
        }
        total_corners_left += corners[k].size();
    }


    for (unsigned int k = 0; k < compRoboTriangles.size(); k++) {
        for (list<Wall*>::iterator it = walls[k].begin(); it != walls[k].end();) {
            Wall* w = *it;
            pt2line rst = w->distance2(this->x, this->y);

            if (rst.dist <= outerDomain) {
                status = MIXED;
                ++it;
            }
            else {
                it = walls[k].erase(it);
            }
        }

        total_walls_left += walls[k].size();
    }

    if (total_corners_left == 0 && total_walls_left == 0) {
        if (!pParent) {
            status = FREE;
            classify_condition = 1;
        }
        else {
            status = pParent->checkChildStatus(this->x, this->y);
            classify_condition = 2;
        }
    }
}


bool Box::checkInsideTAS(Corner &c, Triangle &tri, bool mirrored) {
  // decide if a Corner is inside the Truncated Angular Set (TAS) for the Triangle given the box
  double Ra = sqrt((tri.a.x)*(tri.a.x) + (tri.a.y)*(tri.a.y)) * r0;
  double Rb = sqrt((tri.b.x)*(tri.b.x) + (tri.b.y)*(tri.b.y)) * r0;
  double Rc = sqrt((tri.c.x)*(tri.c.x) + (tri.c.y)*(tri.c.y)) * r0;

  double Ox = this->x, Oy = this->y;

  if (Rc<Rb) {
    // Point B is the furthest point in the triangle, mirror by y axis then compute
      Corner cnew = c;
      cnew.x = 2*Ox - c.x;
      Triangle trinew = tri;
      trinew.a.x = - tri.a.x;
      trinew.b.x = - tri.b.x;
      trinew.c.x = - tri.c.x;
      swap(trinew.b, trinew.c);
      return checkInsideTAS(cnew, trinew, 1);
  }


  double angle_start = xi[0], angle_end = xi[1];

  if (mirrored) {
     angle_start = 2 - xi[0];
     angle_end = 2 - xi[1];
     if (angle_start<=-2) angle_start = 0;
     swap(angle_start, angle_end);
  }


  Ra = Ra - rB;
  if (Ra<0) Ra = 0;
  Rc = Rc + rB;
  double dist = c.distance(Ox, Oy);
  // double angle = atan2(c.y - Oy, c.x - Ox)/PI;

  Corner a0(tri.a.x * r0 + Ox, tri.a.y * r0 + Oy);
  Corner b0(tri.b.x * r0 + Ox, tri.b.y * r0 + Oy);
  Corner c0(tri.c.x * r0 + Ox, tri.c.y * r0 + Oy);
  Corner a1 = a0, b1 = b0, c1 = c0;
  a0.rotate(angle_start*PI, Ox, Oy);
  b0.rotate(angle_start*PI, Ox, Oy);
  c0.rotate(angle_start*PI, Ox, Oy);
  a1.rotate(angle_end*PI, Ox, Oy);
  b1.rotate(angle_end*PI, Ox, Oy);
  c1.rotate(angle_end*PI, Ox, Oy);


  Line2d la0b0(a0, b0);
  la0b0.shiftRight(rB);
  Line2d lb0c0(b0, c0);
  lb0c0.shiftRight(rB);
  Line2d lc1a1(c1, a1);
  lc1a1.shiftRight(rB);

  return (dist <= Rc) && (dist >= Ra) && la0b0.isRight(c.x,c.y) && lb0c0.isRight(c.x,c.y) && lc1a1.isRight(c.x,c.y);
}

bool Box::checkIntersectTAS(Wall &w, Triangle &tri, bool mirrored) {
  // decide if a Wall intersects with the Truncated Angular Set (TAS) for the Triangle given the box
  double Ra = sqrt((tri.a.x)*(tri.a.x) + (tri.a.y)*(tri.a.y)) * r0;
  double Rb = sqrt((tri.b.x)*(tri.b.x) + (tri.b.y)*(tri.b.y)) * r0;
  double Rc = sqrt((tri.c.x)*(tri.c.x) + (tri.c.y)*(tri.c.y)) * r0;
  double Ox = this->x, Oy = this->y;

  if (Rc<Rb) {
    // Point B is the furthest point in the triangle, mirror by y axis then compute
      Corner srcnew = *w.src;
      Corner dstnew = *w.dst;
      srcnew.x = 2*Ox - srcnew.x;
      dstnew.x = 2*Ox - dstnew.x;
      Wall wnew(&srcnew, &dstnew);

      Triangle trinew = tri;
      trinew.a.x = -tri.a.x;
      trinew.b.x = -tri.b.x;
      trinew.c.x = -tri.c.x;
      swap(trinew.b, trinew.c);
      return checkIntersectTAS(wnew, trinew, 1);
  }

  double angle_start = xi[0], angle_end = xi[1];
  if (mirrored) {
     angle_start = 2 - xi[0];
     angle_end = 2 - xi[1];
     if (angle_start<=-2) angle_start = 0;
     swap(angle_start, angle_end);
  }

  // finished mirroring
  // start checking
  Ra = Ra - rB;
  if (Ra<0) Ra = 0;
  Rc = Rc + rB;

  Corner a0(tri.a.x * r0 + Ox, tri.a.y * r0 + Oy);
  Corner b0(tri.b.x * r0 + Ox, tri.b.y * r0 + Oy);
  Corner c0(tri.c.x * r0 + Ox, tri.c.y * r0 + Oy);
  Corner a1 = a0, b1 = b0, c1 = c0;
  a0.rotate(angle_start*PI, Ox, Oy);
  b0.rotate(angle_start*PI, Ox, Oy);
  c0.rotate(angle_start*PI, Ox, Oy);
  a1.rotate(angle_end*PI, Ox, Oy);
  b1.rotate(angle_end*PI, Ox, Oy);
  c1.rotate(angle_end*PI, Ox, Oy);


  Line2d la0b0(a0, b0);
  la0b0.shiftRight(rB);
  Line2d lb0c0(b0, c0);
  lb0c0.shiftRight(rB);
  Line2d lc1a1(c1, a1);
  lc1a1.shiftRight(rB);

  Line2d lwall(w.src->x, w.src->y, w.dst->x, w.dst->y);

  Corner B0_ext;
  la0b0.intersection(lb0c0, B0_ext);
  Circle outer_circle(Ox, Oy, Rc);
  Corner C0_ext1, C0_ext2;
  outer_circle.intersectLine(lb0c0, C0_ext1, C0_ext2);

  Wall seg_B0C0(B0_ext, C0_ext1);
  if (Line2d::lineSegIntsct(w, seg_B0C0))
    return true; // intersect with extended side BC

  double thetaC0 = outer_circle.getTheta(C0_ext1);


  Corner C1_ext1, C1_ext2;
  outer_circle.intersectLine(lc1a1, C1_ext2, C1_ext1);
  double thetaC1 = outer_circle.getTheta(C1_ext1);

  Corner w_outer_circ1, w_outer_circ2;
  outer_circle.intersectLine(lwall,w_outer_circ1,w_outer_circ2);

  auto angleBetween = [](double a, double b0, double b1) {
      if (b0<=b1) return (a>=b0 && a<=b1);
      else return !(a>=b1 && a<=b0);
  };

  if (angleBetween(outer_circle.getTheta(w_outer_circ1), thetaC0 , thetaC1)
        && w.isOnSegment(w_outer_circ1))
    return true; // intersect with extended arc CC'

  if (angleBetween(outer_circle.getTheta(w_outer_circ2), thetaC0 , thetaC1)
        && w.isOnSegment(w_outer_circ2))
    return true; // intersect with extended arc CC'

  if (Ra > 0) { // has inner circle
      Circle inner_circle(Ox, Oy, Ra);

      Corner A0_ext1, A0_ext2;

      inner_circle.intersectLine(la0b0, A0_ext1, A0_ext2);

      Wall seg_B0A0(B0_ext, A0_ext1);
      if (Line2d::lineSegIntsct(w, seg_B0A0))
        return true; // intersect with extended side BA

      double thetaA0 = inner_circle.getTheta(A0_ext1);


      Corner A1_ext1, A1_ext2;
      inner_circle.intersectLine(lc1a1, A1_ext2, A1_ext1);
      double thetaA1 = inner_circle.getTheta(A1_ext1);

      Wall seg_C1A1(C1_ext1, A1_ext1);
      if (Line2d::lineSegIntsct(w, seg_C1A1 ))
        return true; // intersect with extended side C'A'

      Corner w_inner_circ1, w_inner_circ2;
      inner_circle.intersectLine(lwall,w_inner_circ1, w_inner_circ2);

      if (angleBetween(outer_circle.getTheta(w_inner_circ1), thetaA0 , thetaA1)
            && w.isOnSegment(w_inner_circ1))
        return true; // intersect with extended arc AA'

      if (angleBetween(outer_circle.getTheta(w_inner_circ2), thetaA0 , thetaA1)
            && w.isOnSegment(w_inner_circ2))
        return true; // intersect with extended arc AA'


  }
  else {// no inner circle
      Corner A0_ext;
      la0b0.intersection(lc1a1, A0_ext);

      Wall seg_B0A0(B0_ext, A0_ext);
      if (Line2d::lineSegIntsct(w, seg_B0A0))
        return true; // intersect with extended side BA
      Wall seg_C1A0(C1_ext1, A0_ext);
      if (Line2d::lineSegIntsct(w, seg_C1A0))
        return true; // intersect with extended side C'A
  }


  return false;
}


void Box::updateStatusSmall() {
  if (status != UNKNOWN) return;

  // outerDomain SHOULD BE USED TO SPEED CODE!!!   (TO DO)
  //double outerDomain = r0 + rB;
  
  //  bool isSTUCK = false;

  int total_corners_left = 0;
  int total_walls_left = 0;

  for (unsigned int k = 0; k < compRoboTriangles.size(); k++) {
      for (list<Corner*>::iterator it = corners[k].begin(); it != corners[k].end();) {
          Corner* c = *it;

          if (checkInsideTAS(*c, compRoboTriangles[k])) {
            status = MIXED;
            ++it;
          }
          else {
            it = corners[k].erase(it);
          }
      }
      total_corners_left += corners[k].size();
  }

  for (unsigned int k = 0; k < compRoboTriangles.size(); k++) {
      for (list<Wall*>::iterator it = walls[k].begin(); it != walls[k].end();) {
          Wall* w = *it;

          if (checkIntersectTAS(*w, compRoboTriangles[k])) {
            status = MIXED;
            ++it;
          }
          else {
            it = walls[k].erase(it);
          }
      }
      total_walls_left += walls[k].size();
  }

  if (total_corners_left == 0 && total_walls_left == 0) {
      if (!pParent) {
          status = FREE;
          classify_condition = 6;
      }
      else {
          status = pParent->checkChildStatus(this->x, this->y);
          classify_condition = 7;
      }
  }

}




/*
void Box::updateStatusSmall() {
    if (status != UNKNOWN) return;

    //Triangle Trixi0, Trixi1;
    double v01x, v01y, v02x, v02y, v11x, v11y, v12x, v12y, v21x, v21y, v22x, v22y;

    getRoundTriVerts(v01x, v01y, v02x, v02y, v11x, v11y, v12x, v12y, v21x, v21y, v22x, v22y);
    //getRoundTriVerts(Trixi0, Trixi1);

    // unused:
    // bool expandSuccess = L1.expand(rB, L2, L3) && L2.expand(rB, L1, L3) && L3.expand(rB, L1, L2) && !L1.isNegative(L2, L3);
    //assert(expandSuccess);



    // This is creating the inner triangle
    Line2d L1a(v01x, v01y, v12x, v12y);
    Line2d L2a(v11x, v11y, v22x, v22y);
    Line2d L3a(v21x, v21y, v02x, v02y);
    // the box may be too big to form a shrinked triangle
    bool shrinkSuccess = L1a.expand(-rB, L2a, L3a)
                      && L2a.expand(-rB, L3a, L1a)
                      && L3a.expand(-rB, L1a, L2a)
                      && !L1a.isNegative(L2a, L3a); // check the validation of the shrinked triangle



    // April 2016: Chee&Tom:
    // It is now clear that the logic below is causing unnecessary splitting of the rotational degree of freedom
    //    We did this experiment: if the triangle has a small minimum angle, then you are forced to do more splits of rotational DOF
    //       Triangle 120deg, 240deg (i.e., equilateral triangle)   -- time is 1.3seconds
    //       Triangle 150deg, 210deg (i.e., triangle with 30degree)   -- time is 4.5seconds
    //       Triangle 165deg, 195deg (i.e., triangle with 15degree)   -- time is 16.5seconds
    //       Triangle 172deg, 188deg (i.e., triangle with 8degree)   -- time is 12.5seconds
    //            (somehow, this breaks the trend, probably related to epsilon)
    // UPSHOT: must rewrite the logic below:

    Line2d L1(v02x, v02y, v11x, v11y);
    Line2d L2(v12x, v12y, v21x, v21y);
    Line2d L3(v22x, v22y, v01x, v01y);

    L1.expand2(rB);
    L2.expand2(rB);
    L3.expand2(rB);
    bool expandSuccess = !L1.isRight(0,0) && !L2.isRight(0,0) && !L3.isRight(0,0); // (0,0) is the center of the box

    // fomr the big triangle (expanded lines)
    double X12x, X12y;
    L1.intersection(L2, X12x, X12y);
    double X23x, X23y;
    L2.intersection(L3, X23x, X23y);
    double X31x, X31y;
    L3.intersection(L1, X31x, X31y);


    for (list<Corner*>::iterator it = corners.begin(); it != corners.end(); ) {
        Corner* c = *it;
        double cx = c->x - x;
        double cy = c->y - y;

        if (sqrt(cx*cx + cy*cy) < r0 + rB) {


            // corner c is inside the shrinked triangle
            if (shrinkSuccess &&
                !L1a.isRight(cx, cy) && !L2a.isRight(cx, cy) && !L3a.isRight(cx, cy)) {
                status = STUCK;
                classify_condition = 3;
                return ;
            }

            if (expandSuccess) {
                // inside the circumcircle and inside the big triangle
                if(!Line2d::isRight(X31x, X31y, X12x, X12y, cx, cy)
                   && !Line2d::isRight(X12x, X12y, X23x, X23y, cx, cy)
                   && !Line2d::isRight(X23x, X23y, X31x, X31y, cx, cy)){
                    status = MIXED;
                    ++it;
                }
                else {
                    it = corners.erase(it);
                }
            }
            else {
                if (!L1.isRight(0,0)) {
                    if (L2.isRight(0,0) && L3.isRight(0,0)) {
                        if (L1.isRight(cx, cy)) {
                            it = corners.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else if (L2.isRight(0,0) && !L3.isRight(0,0)) {
                        if (L1.isRight(cx, cy) || L3.isRight(cx, cy)) {
                            it = corners.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else if (!L2.isRight(0,0) && L3.isRight(0,0)) {
                        if (L1.isRight(cx, cy) || L2.isRight(cx, cy)) {
                            it = corners.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else { // !L2.isRight(0,0) && !L3.isRight(0,0) => expandSuccess
                    }
                }
                else if (!L2.isRight(0,0)) {
                    if (L1.isRight(0,0) && L3.isRight(0,0)) {
                        if (L2.isRight(cx, cy)) {
                            it = corners.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else if (L1.isRight(0,0) && !L3.isRight(0,0)) {
                        if (L2.isRight(cx, cy) || L3.isRight(cx, cy)) {
                            it = corners.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else if (!L1.isRight(0,0) && L3.isRight(0,0)) {
                        if (L1.isRight(cx, cy) || L2.isRight(cx, cy)) {
                            it = corners.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else { // !L1.isRight(0,0) && !L3.isRight(0,0) => expandSuccess
                    }
                }
                else if (!L3.isRight(0,0)){
                    if (L1.isRight(0,0) && L2.isRight(0,0)) {
                        if (L3.isRight(cx, cy)) {
                            it = corners.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else if (L1.isRight(0,0) && !L2.isRight(0,0)) {
                        if (L2.isRight(cx, cy) || L3.isRight(cx, cy)) {
                            it = corners.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else if (!L1.isRight(0,0) && L2.isRight(0,0)) {
                        if (L1.isRight(cx, cy) || L3.isRight(cx, cy)) {
                            it = corners.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else { // !L1.isRight(0,0) && !L2.isRight(0,0) => expandSuccess
                    }
                }
            }

            // expandSuccess
            // && !Line2d::isRight(X31x, X31y, X12x, X12y, cx, cy)
            // && !Line2d::isRight(X12x, X12y, X23x, X23y, cx, cy)
            // && !Line2d::isRight(X23x, X23y, X31x, X31y, cx, cy)
        }
        else { // far away (outside the circumcircle)
            it = corners.erase(it);
        }
    }

    for (list<Wall*>::iterator it = walls.begin(); it != walls.end(); ) {
        Wall* w = *it;
        pt2line rst = w->distance2(x, y);

        if (rst.dist < r0 + rB) {
            double srcx = w->src->x - x;
            double srcy = w->src->y - y;
            double dstx = w->dst->x - x;
            double dsty = w->dst->y - y;

            //quick stuck detection
            if (shrinkSuccess) { //inner triangle (before shrink rB) is not negative
                double x23s, y23s;
                L2a.intersection(L3a, x23s, y23s);
                double x31s, y31s;
                L3a.intersection(L1a, x31s, y31s);
                double x12s, y12s;
                L1a.intersection(L2a, x12s, y12s);

                //if src or dst is in the shrinked triangle
                if ( (!L1a.isRight(srcx, srcy) && !L2a.isRight(srcx, srcy) && !L3a.isRight(srcx, srcy)) ||
                     (!L1a.isRight(dstx, dsty) && !L2a.isRight(dstx, dsty) && !L3a.isRight(dstx, dsty)) ) {
                    status = STUCK;
                    classify_condition = 4;
                    return;
                }
                // or wall seg (src,dst) intersects any edge of the shrinked triangle
                else if ( Line2d::lineSegIntsct(x23s, y23s, x31s, y31s, srcx, srcy, dstx, dsty)
                       || Line2d::lineSegIntsct(x31s, y31s, x12s, y12s, srcx, srcy, dstx, dsty)
                       || Line2d::lineSegIntsct(x12s, y12s, x23s, y23s, srcx, srcy, dstx, dsty) ) {
                    status = STUCK;
                    classify_condition = 5;
                    return;
                }
            }

            if (expandSuccess) {
                //if src or dst is in the big triangle
                if ( (!Line2d::isRight(X31x, X31y, X12x, X12y, srcx, srcy) && !Line2d::isRight(X12x, X12y, X23x, X23y, srcx, srcy) && !Line2d::isRight(X23x, X23y, X31x, X31y, srcx, srcy)) ||
                     (!Line2d::isRight(X31x, X31y, X12x, X12y, dstx, dsty) && !Line2d::isRight(X12x, X12y, X23x, X23y, dstx, dsty) && !Line2d::isRight(X23x, X23y, X31x, X31y, dstx, dsty))) {
                    status = MIXED;
                    ++it;
                }
                // or line seg (src,dst) intersects any edge of the big triangle
                else if ( Line2d::lineSegIntsct(X31x, X31y, X12x, X12y, srcx, srcy, dstx, dsty)
                       || Line2d::lineSegIntsct(X12x, X12y, X23x, X23y, srcx, srcy, dstx, dsty)
                       || Line2d::lineSegIntsct(X23x, X23y, X31x, X31y, srcx, srcy, dstx, dsty) ) {
                    status = MIXED;
                    ++it;
                }
                else {
                    it = walls.erase(it);
                }
            }
            else {
                if (!L1.isRight(0,0)) {
                    if (L2.isRight(0,0) && L3.isRight(0,0)) {
                        if (L1.isRight(srcx, srcy) && L1.isRight(dstx, dsty)) {
                            it = walls.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else if (L2.isRight(0,0) && !L3.isRight(0,0)) {
                        if ((L1.isRight(srcx, srcy) && L1.isRight(dstx, dsty)) ||
                            (L3.isRight(srcx, srcy) && L3.isRight(dstx, dsty)) ||
                            (L1.isRight(srcx, srcy) && L3.isRight(dstx, dsty)) ||
                            (L1.isRight(dstx, dsty) && L3.isRight(srcx, srcy))) {
                            it = walls.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else if (!L2.isRight(0,0) && L3.isRight(0,0)) {
                        if ((L1.isRight(srcx, srcy) && L1.isRight(dstx, dsty)) ||
                            (L2.isRight(srcx, srcy) && L2.isRight(dstx, dsty)) ||
                            (L1.isRight(srcx, srcy) && L2.isRight(dstx, dsty)) ||
                            (L1.isRight(dstx, dsty) && L2.isRight(srcx, srcy))) {
                            it = walls.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else { // !L2.isRight(0,0) && !L3.isRight(0,0) => expandSuccess
                    }
                }
                else if (!L2.isRight(0,0)) {
                    if (L1.isRight(0,0) && L3.isRight(0,0)) {
                        if (L2.isRight(srcx, srcy) && L2.isRight(dstx, dsty)) {
                            it = walls.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else if (L1.isRight(0,0) && !L3.isRight(0,0)) {
                        if ((L2.isRight(srcx, srcy) && L2.isRight(dstx, dsty)) ||
                            (L3.isRight(srcx, srcy) && L3.isRight(dstx, dsty)) ||
                            (L2.isRight(srcx, srcy) && L3.isRight(dstx, dsty)) ||
                            (L2.isRight(dstx, dsty) && L3.isRight(srcx, srcy))) {
                            it = walls.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else if (!L1.isRight(0,0) && L3.isRight(0,0)) {
                        if ((L1.isRight(srcx, srcy) && L1.isRight(dstx, dsty)) ||
                            (L2.isRight(srcx, srcy) && L2.isRight(dstx, dsty)) ||
                            (L1.isRight(srcx, srcy) && L2.isRight(dstx, dsty)) ||
                            (L1.isRight(dstx, dsty) && L2.isRight(srcx, srcy))) {
                            it = walls.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else { // !L1.isRight(0,0) && !L3.isRight(0,0) => expandSuccess
                    }
                }
                else if (!L3.isRight(0,0)){
                    if (L1.isRight(0,0) && L2.isRight(0,0)) {
                        if (L3.isRight(srcx, srcy) && L3.isRight(dstx, dsty)) {
                            it = walls.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else if (L1.isRight(0,0) && !L2.isRight(0,0)) {
                        if ((L2.isRight(srcx, srcy) && L2.isRight(dstx, dsty)) ||
                            (L3.isRight(srcx, srcy) && L3.isRight(dstx, dsty)) ||
                            (L2.isRight(srcx, srcy) && L3.isRight(dstx, dsty)) ||
                            (L2.isRight(dstx, dsty) && L3.isRight(srcx, srcy))) {
                            it = walls.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else if (!L1.isRight(0,0) && L2.isRight(0,0)) {
                        if ((L1.isRight(srcx, srcy) && L1.isRight(dstx, dsty)) ||
                            (L3.isRight(srcx, srcy) && L3.isRight(dstx, dsty)) ||
                            (L1.isRight(srcx, srcy) && L3.isRight(dstx, dsty)) ||
                            (L1.isRight(dstx, dsty) && L3.isRight(srcx, srcy))) {
                            it = walls.erase(it);
                        }
                        else {
                            status = MIXED;
                            ++it;
                        }
                    }
                    else { // !L1.isRight(0,0) && !L2.isRight(0,0) => expandSuccess
                    }
                }
            }
        } // far away (outside the circumcircle)
        else {
            it = walls.erase(it);
        }
    }

    if (corners.size() == 0 && walls.size() == 0) {
        if (!pParent) {
            status = FREE;
            classify_condition = 6;
        }
        else {
            status = pParent->checkChildStatus(this->x, this->y);
            classify_condition = 7;
        }
    }
}
*/
