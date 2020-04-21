#include "Box.h"
#include "QuadTree.h"




//
//#define OVERRIDE_NEW_DELETE
//#include "MemProCpp\MemPro.cpp"

//extern vector<Polygon> polygons;
//extern vector<int> srcInPolygons;
extern double triRobo[2];
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

void Box::getRoundTriVerts(double& v01x, double& v01y, double& v02x, double& v02y,
                           double& v11x, double& v11y, double& v12x, double& v12y,
                           double& v21x, double& v21y, double& v22x, double& v22y) {
    double r = r0;
    double theta1 = triRobo[0];
    double theta2 = triRobo[1];
    double xi1 = this->xi[0];
    double xi2 = this->xi[1];

    double temp1 = xi1;
    //while (temp1 > 2) temp1 -= 2;
    double temp2 = xi2;
    //while (temp2 > 2) temp2 -= 2;
    v01x = r * cos(temp1 * PI);
    v01y = r * sin(temp1 * PI);
    v02x = r * cos(temp2 * PI);
    v02y = r * sin(temp2 * PI);

    temp1 = xi1 + theta1;
    //while (temp1 > 2) temp1 -= 2;
    temp2 = xi2 + theta1;
    //while (temp2 > 2) temp2 -= 2;
    v11x = r * cos(temp1 * PI);
    v11y = r * sin(temp1 * PI);
    v12x = r * cos(temp2 * PI);
    v12y = r * sin(temp2 * PI);

    temp1 = xi1 + theta2;
    //while (temp1 > 2) temp1 -= 2;
    temp2 = xi2 + theta2;
    //while (temp2 > 2) temp2 -= 2;
    v21x = r * cos(temp1 * PI);
    v21y = r * sin(temp1 * PI);
    v22x = r * cos(temp2 * PI);
    v22y = r * sin(temp2 * PI);
}

//void Box::getRoundTriVerts(Triangle &Trixi0, Triangle &Trixi1) {
//    double r = r0;
//    double theta1 = triRobo[0];
//    double theta2 = triRobo[1];
//    double xi1 = this->xi[0];
//    double xi2 = this->xi[1];

//    double temp1 = xi1;
//    //while (temp1 > 2) temp1 -= 2;
//    double temp2 = xi2;
//    //while (temp2 > 2) temp2 -= 2;
//    Trixi0.a = Pose(r * cos(temp1 * PI), r * sin(temp1 * PI));
//    Trixi1.a = Pose(r * cos(temp2 * PI), r * sin(temp2 * PI));

//    temp1 = xi1 + theta1;
//    //while (temp1 > 2) temp1 -= 2;
//    temp2 = xi2 + theta1;
//    //while (temp2 > 2) temp2 -= 2;
//    Trixi0.b = Pose(r * cos(temp1 * PI), r * sin(temp1 * PI));
//    Trixi1.b = Pose(r * cos(temp2 * PI), r * sin(temp2 * PI));

//    temp1 = xi1 + theta2;
//    //while (temp1 > 2) temp1 -= 2;
//    temp2 = xi2 + theta2;
//    //while (temp2 > 2) temp2 -= 2;
//    Trixi0.c = Pose(r * cos(temp1 * PI), r * sin(temp1 * PI));
//    Trixi1.c = Pose(r * cos(temp2 * PI), r * sin(temp2 * PI));
//}

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
        children[i]->walls.insert(children[i]->walls.begin(), this->walls.begin(), this->walls.end());
        children[i]->corners.insert(children[i]->corners.begin(), this->corners.begin(), this->corners.end());

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
        children[i]->walls.insert(children[i]->walls.begin(),
            this->walls.begin(), this->walls.end());
        children[i]->corners.insert(
            children[i]->corners.begin(),
            this->corners.begin(), this->corners.end());

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
    else if( rB / 2 < r0 * 2 / THETA_MIN / 20 ) {// one-shot intermediate split before
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

        int n = ceil( 2 / THETA_MIN );
        //n = 64;
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
    if (walls.size() > 0) {
        for (list<Wall*>::iterator iterW = walls.begin(); iterW != walls.end(); ++iterW) {
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

    Corner* nearestCorner = NULL;
    double mindistC = FLT_MAX;
    if (corners.size() > 0) {
        for (list<Corner*>::iterator iterC = corners.begin(); iterC != corners.end(); ++iterC) {
            Corner* c = *iterC;
            double dist = c->distance(x, y);

            if (mindistC > dist) {
                mindistC = dist;
                nearestCorner = *iterC;
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
    for (list<Corner*>::iterator it = corners.begin(); it != corners.end();) {
        Corner* c = *it;
        if( c->distance(this->x, this->y) <= outerDomain ) {
            status = MIXED;
            ++it;
        }
        else {
            it = corners.erase(it);
        }
    }

    for (list<Wall*>::iterator it = walls.begin(); it != walls.end();) {
        Wall* w = *it;
        pt2line rst = w->distance2(this->x, this->y);

        if (rst.dist <= outerDomain) {
            status = MIXED;
            ++it;
        }
        else {
            it = walls.erase(it);
        }
    }

    if (corners.size() == 0 && walls.size() == 0) {
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
        else { /* far away (outside the circumcircle) */
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
        } /* far away (outside the circumcircle) */
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
