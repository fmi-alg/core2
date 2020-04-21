#include "Box.h"
#include "QuadTree.h"
#include "Triangle.h"
#include "Corner.h"
#include "BasisShape.h"

//
//#define OVERRIDE_NEW_DELETE
//#include "MemProCpp\MemPro.cpp"

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
            double dist = c->distance(this->x, this->y);
            if( dist <= outerDomain ) {
                status = MIXED;
                ++it;
            }
            else {
                it = corners[k].erase(it);
            }
        }
        total_corners_left += corners[k].size();

        for (list<Wall*>::iterator it = walls[k].begin(); it != walls[k].end();) {
            Wall* w = *it;
            double infimum = w->distance(this->x, this->y);
            if (infimum <= outerDomain) {
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


BasisShape Box::getExpandedNSS(Triangle tri, bool &mirrored) {
  // compute the expanded Nicely Swept Set (NSS) for the Triangle given the Box
  // mirrored is also one of the returned value, which is set to True if the output is mirrored by the line x=box.x
  double Ra = sqrt((tri.a.x)*(tri.a.x) + (tri.a.y)*(tri.a.y)) * r0;
  double Rb = sqrt((tri.b.x)*(tri.b.x) + (tri.b.y)*(tri.b.y)) * r0;
  double Rc = sqrt((tri.c.x)*(tri.c.x) + (tri.c.y)*(tri.c.y)) * r0;

  double Ox = this->x, Oy = this->y;

  if (Rc<Rb) {
      // Point B is the furthest point in the triangle, mirror the Triangle by its y-axis
      mirrored = true;
      tri.a.x = -tri.a.x;
      tri.b.x = -tri.b.x;
      tri.c.x = -tri.c.x;
      swap(tri.b, tri.c);
      swap(Rb, Rc);
  }

  double angle_start = xi[0], angle_end = xi[1];
  if (mirrored) {
     angle_start = 2 - xi[0];
     angle_end = 2 - xi[1];
     // if (angle_start<=-2) angle_start = 0;
     swap(angle_start, angle_end);
  }

  // The expanded NSS consists a triangle, a truncated strip and a sector
  // and 5 disks at the vertices a0, b0, c0, a1, c1

  BasisShape expanded_triangle(1);
  BasisShape expanded_sector(1);
  BasisShape expanded_truncated_strip(1);
  BasisShape expanded_NSS(2);

  // a0, b0, c0 are the vertices of the starting-position triangle (rotated by angle_start)
  // a1, b2, c2 are the vertices of the ending-position triangle (rotated by angle_end)
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

  // c2 is the intersection of line(a1, c2) and circle(c), where line(a1, c2) is parallel to line(a0, c0)
  Line2d l_a0c0(a0, c0);
  Line2d l_a1c2 = l_a0c0;
  l_a1c2.srcx = a1.x;
  l_a1c2.srcy = a1.y;
  l_a1c2.c = - (l_a1c2.a * a1.x + l_a1c2.b * a1.y);
  Circle outer_circle(Ox, Oy, Rc);
  Corner c2, c2_neg;
  outer_circle.intersectLine(l_a1c2, c2, c2_neg);
  l_a1c2.dstx = c2.x;
  l_a1c2.dsty = c2.y;

  // 5 disks
  Circle c_a0(a0.x, a0.y, rB);
  Circle c_b0(b0.x, b0.y, rB);
  Circle c_c0(c0.x, c0.y, rB);
  Circle c_a1(a1.x, a1.y, rB);
  Circle c_c1(c1.x, c1.y, rB);

  // 5 0-basis shapes for the disks
  BasisShape bs0_disk_a0(c_a0, 0);
  BasisShape bs0_disk_b0(c_b0, 0);
  BasisShape bs0_disk_c0(c_c0, 0);
  BasisShape bs0_disk_a1(c_a1, 0);
  BasisShape bs0_disk_c1(c_c1, 0);

  // 5 1-basis shapes for the disks
  BasisShape bs1_disk_a0(1);
  BasisShape bs1_disk_b0(1);
  BasisShape bs1_disk_c0(1);
  BasisShape bs1_disk_a1(1);
  BasisShape bs1_disk_c1(1);
  bs1_disk_a0.list.push_back(bs0_disk_a0);
  bs1_disk_b0.list.push_back(bs0_disk_b0);
  bs1_disk_c0.list.push_back(bs0_disk_c0);
  bs1_disk_a1.list.push_back(bs0_disk_a1);
  bs1_disk_c1.list.push_back(bs0_disk_c1);

  // add to the expanded NSS
  expanded_NSS.list.push_back(bs1_disk_a0);
  expanded_NSS.list.push_back(bs1_disk_b0);
  expanded_NSS.list.push_back(bs1_disk_c0);
  expanded_NSS.list.push_back(bs1_disk_a1);
  expanded_NSS.list.push_back(bs1_disk_c1);

  // The expanded triangle is defined by: a0, b0, c0
  // intersection of 6 half-spaces
  // UNION 3 disks at vertices (already added to NSS)
  Line2d l_a0b0(a0, b0);
  Line2d l_b0c0(b0, c0);
  Line2d l_c0a0(c0, a0);
  l_a0b0.shiftRight(rB);
  l_b0c0.shiftRight(rB);
  l_c0a0.shiftRight(rB);
  vector<Corner> triangle_vertices(6);
  triangle_vertices[0] = Corner(l_a0b0.srcx, l_a0b0.srcy);
  triangle_vertices[1] = Corner(l_a0b0.dstx, l_a0b0.dsty);
  triangle_vertices[2] = Corner(l_b0c0.srcx, l_b0c0.srcy);
  triangle_vertices[3] = Corner(l_b0c0.dstx, l_b0c0.dsty);
  triangle_vertices[4] = Corner(l_c0a0.srcx, l_c0a0.srcy);
  triangle_vertices[5] = Corner(l_c0a0.dstx, l_c0a0.dsty);

  Line2d l_a0_0(triangle_vertices[5], triangle_vertices[0]);
  Line2d l_b0_0(triangle_vertices[1], triangle_vertices[2]);
  Line2d l_c0_0(triangle_vertices[3], triangle_vertices[4]);
  BasisShape bs_a0b0(l_a0b0);
  BasisShape bs_b0c0(l_b0c0);
  BasisShape bs_c0a0(l_c0a0);
  BasisShape bs_a0_0(l_a0_0);
  BasisShape bs_b0_0(l_b0_0);
  BasisShape bs_c0_0(l_c0_0);
  expanded_triangle.list.push_back(bs_a0b0);
  expanded_triangle.list.push_back(bs_b0c0);
  expanded_triangle.list.push_back(bs_c0a0);
  expanded_triangle.list.push_back(bs_a0_0);
  expanded_triangle.list.push_back(bs_b0_0);
  expanded_triangle.list.push_back(bs_c0_0);

  expanded_NSS.list.push_back(expanded_triangle);

  // The expanded sector is defined by: a1, c2, c1
  // intersection of 5 half-spaces and a disk
  // UNION 3 disks at vertices (the disk at c2 is omitted, the other 2 already added to NSS)

  // Line2d l_a1c2(a1, c2); // defined above
  Circle expanded_outer_circle(Ox, Oy, Rc+rB);
  Line2d l_c1a1(c1, a1);
  l_a1c2.shiftRight(rB);
  l_c1a1.shiftRight(rB);
  vector<Corner> sector_vertices(6);
  sector_vertices[0] = Corner(l_a1c2.srcx, l_a1c2.srcy);
  sector_vertices[1] = Corner(l_a1c2.dstx, l_a1c2.dsty);
  sector_vertices[2] = expanded_outer_circle.projectionOf(c2);
  sector_vertices[3] = expanded_outer_circle.projectionOf(c1);
  sector_vertices[4] = Corner(l_c1a1.srcx, l_c1a1.srcy);
  sector_vertices[5] = Corner(l_c1a1.dstx, l_c1a1.dsty);

  Line2d l_a1_1(sector_vertices[5], sector_vertices[0]);
  Line2d l_c2_1(sector_vertices[1], sector_vertices[2]);
  Line2d l_c1_1(sector_vertices[3], sector_vertices[4]);
  BasisShape bs_a1c2(l_a1c2);
  BasisShape bs_c2c1(expanded_outer_circle, 0);
  BasisShape bs_c1a1(l_c1a1);
  BasisShape bs_a1_1(l_a1_1);
  BasisShape bs_c2_1(l_c2_1);
  BasisShape bs_c1_1(l_c1_1);
  expanded_sector.list.push_back(bs_a1c2);
  expanded_sector.list.push_back(bs_c2c1);
  expanded_sector.list.push_back(bs_c1a1);
  expanded_sector.list.push_back(bs_a1_1);
  expanded_sector.list.push_back(bs_c2_1);
  expanded_sector.list.push_back(bs_c1_1);

  expanded_NSS.list.push_back(expanded_sector);


  // The expanded truncated strip is defined by: a0, c0, c2, a1
  // intersection of 6 half-spaces and a disk and a complement of disk
  // UNION 4 disks at vertices (the disk at c2 is omitted, the other 3 already added to NSS)

  // no truncated strip shape for star-shaped robot (Ra = 0)
  if (Ra == 0) return expanded_NSS;

  // Line2d l_a0c0(a0, c0); // defined above
  // Circle expanded_outer_circle(Ox, Oy, Rc+rB); // defined above
  Line2d l_c2a1(c2, a1);
  double Ra_exp = Ra-rB;
  if (Ra_exp <0) Ra_exp = 0;
  Circle expanded_inner_circle(Ox, Oy, Ra_exp);
  l_a0c0.shiftRight(rB);
  l_c2a1.shiftRight(rB);

  vector<Corner> strip_vertices(8);
  strip_vertices[0] = Corner(l_a0c0.srcx, l_a0c0.srcy);
  strip_vertices[1] = Corner(l_a0c0.dstx, l_a0c0.dsty);
  strip_vertices[2] = expanded_outer_circle.projectionOf(c0);
  strip_vertices[3] = expanded_outer_circle.projectionOf(c2);
  strip_vertices[4] = Corner(l_c2a1.srcx, l_c2a1.srcy);
  strip_vertices[5] = Corner(l_c2a1.dstx, l_c2a1.dsty);
  strip_vertices[6] = expanded_inner_circle.projectionOf(a1);
  strip_vertices[7] = expanded_inner_circle.projectionOf(a0);

  Line2d l_a0_2(strip_vertices[7], strip_vertices[0]);
  Line2d l_c0_2(strip_vertices[1], strip_vertices[2]);
  Line2d l_c2_2(strip_vertices[3], strip_vertices[4]);
  Line2d l_a1_2(strip_vertices[5], strip_vertices[6]);

  BasisShape bs_a0_2(l_a0_2);
  BasisShape bs_c0_2(l_c0_2);
  BasisShape bs_c2_2(l_c2_2);
  BasisShape bs_a1_2(l_a1_2);

  BasisShape bs_a0c0(l_a0c0);
  BasisShape bs_c0c2(expanded_outer_circle, 0);
  BasisShape bs_c2a1(l_c2a1);
  BasisShape bs_a1a0(expanded_inner_circle, 1);

  expanded_truncated_strip.list.push_back(bs_a0_2);
  expanded_truncated_strip.list.push_back(bs_c0_2);
  expanded_truncated_strip.list.push_back(bs_c2_2);
  expanded_truncated_strip.list.push_back(bs_a1_2);
  expanded_truncated_strip.list.push_back(bs_a0c0);
  expanded_truncated_strip.list.push_back(bs_c0c2);
  expanded_truncated_strip.list.push_back(bs_c2a1);
  if (Ra_exp > 0) expanded_truncated_strip.list.push_back(bs_a1a0);

  expanded_NSS.list.push_back(expanded_truncated_strip);

  return expanded_NSS;
}


bool Box::checkIntersectNSS(Corner c, BasisShape &expanded_NSS, bool mirrored) {
  // decide if a Corner is inside the expanded Nicely Swept Set (NSS)

  double angle_start = xi[0], angle_end = xi[1];

  if (mirrored) {
    // mirror by the line x=this->x
    c.x = 2*x - c.x;

    angle_start = 2 - xi[0];
    angle_end = 2 - xi[1];
    // if (angle_start<=-2) angle_start = 0;
    swap(angle_start, angle_end);
  }

  return expanded_NSS.isIntersect(c);
}

bool Box::checkIntersectNSS(Wall w, BasisShape &expanded_NSS, bool mirrored) {
  // decide if a Wall intersect with the expanded Nicely Swept Set (NSS)

  double angle_start = xi[0], angle_end = xi[1];

  if (mirrored) {
    // mirror by the line x=this->x
    Corner srcnew = *w.src;
    Corner dstnew = *w.dst;
    srcnew.x = 2*x - srcnew.x;
    dstnew.x = 2*x - dstnew.x;
    w = Wall(&srcnew, &dstnew);

    angle_start = 2 - xi[0];
    angle_end = 2 - xi[1];
    // if (angle_start<=-2) angle_start = 0;
    swap(angle_start, angle_end);
  }

  return expanded_NSS.isIntersect(w);
}

void Box::updateStatusSmall() {
  if (status != UNKNOWN) return;

  // outerDomain SHOULD BE USED TO SPEED CODE!!!   (TO DO)
  // double outerDomain = r0 + rB;
  
  //  bool isSTUCK = false;

  int total_corners_left = 0;
  int total_walls_left = 0;

  for (unsigned int k = 0; k < compRoboTriangles.size(); k++) {
      bool mirrored = false;
      BasisShape expanded_NSS = getExpandedNSS(compRoboTriangles[k], mirrored);

      for (list<Corner*>::iterator it = corners[k].begin(); it != corners[k].end();) {
          Corner* c = *it;

          if (checkIntersectNSS(*c, expanded_NSS, mirrored)) {
            status = MIXED;
            ++it;
          }
          else {
            it = corners[k].erase(it);
          }
      }
      total_corners_left += corners[k].size();

      for (list<Wall*>::iterator it = walls[k].begin(); it != walls[k].end();) {
          Wall* w = *it;

          if (checkIntersectNSS(*w, expanded_NSS, mirrored)) {
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
