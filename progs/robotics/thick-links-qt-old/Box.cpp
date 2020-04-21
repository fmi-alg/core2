#include "Box.h"
#include <assert.h>
#include <math.h>
#include <iostream>
#include "Polygon.h"
#include "QuadTree.h"
#include <map>
//
//#define OVERRIDE_NEW_DELETE
//#include "MemProCpp\MemPro.cpp"

//extern double triRobo[3];

//extern vector<Polygon> polygons;
//extern vector<int> srcInPolygons;

extern FILE *fp;

extern int twoStrategyOption;
extern int crossingOption;
extern QuadTree* QT;
extern int sizeOfPhiB;
extern double bandwidth;

double Box::r0 = 0;
double Box::l1 = 0;
double Box::l2 = 0;
double Box::thickness = 0;
//double Box::THETA_MIN = 0;
int Box::counter = 0;

vector<Box*>* Box::pAllLeaf = 0;

class Point {
public:
    double x;
    double y;
    Point(double xx, double yy) :
            x(xx), y(yy) {
    }
    ;
};

class AngleRange {
public:
    double lowerBound;
    double upperBound;
    AngleRange(double lower, double upper) :
            lowerBound(lower), upperBound(upper) {
    };
};
std::vector<int> expansions;

int Box::oppositeDir[5] = { 2, 3, 0, 1, 4 };

bool hasIntersectionLR(double xs, double ys, double xe, double ye, double xleft, double ytop, double xr, double yb);
void getExpandedLine(Wall* wall, double width, double height, double& newSrcX, double& newSrcY, double& newDstX, double& newDstY);
AngleRange calcAngleRangeCB(double l, double t, double xc, double yc, Box* b);
double getSeparation(double xa, double ya, double xb, double yb, double x, double y);
vector<Point> calcIntersectionCW(double l, double xc, double yc, Wall* wall, bool& checkSrc, bool& checkDst);
double calcAngle(double srcX, double srcY, double dstX, double dstY);
AngleRange calcAngleRange(map<double, double> angles, double thickness);
vector<AngleRange> calcZone(vector<AngleRange>& srcAngleRanges);
vector<AngleRange> calcOppoZone(vector<AngleRange>& srcAngleRanges);
double getPointDistance(double x1, double y1, double x2, double y2);
void calcOppoAngle(double& angle);


bool Box::split2D(vector<Box*>& chldn) {
    shouldSplit2D = false;

    if (!this->isLeaf || this->status == FREE || this->status == STUCK) {
        return 0;
    }

    Box* children[4];
    //same as quadrants I, II, III, IV
    children[2] = new Box(x - width / 4, y + height / 4, width / 2, height / 2);
    children[1] = new Box(x + width / 4, y + height / 4, width / 2, height / 2);
    children[0] = new Box(x + width / 4, y - height / 4, width / 2, height / 2);
    children[3] = new Box(x - width / 4, y - height / 4, width / 2, height / 2);
    for (int i = 0; i < 4; ++i) {
        children[i]->depth = this->depth + 1;
        children[i]->isBig = this->isBig;
        children[i]->pParent = this;
        children[i]->tChildID = i;

        children[i]->xi[0] = this->xi[0];
        children[i]->xi[1] = this->xi[1];
        children[i]->xi[2] = this->xi[2];
        children[i]->xi[3] = this->xi[3];

        children[i]->order = ALL;
    }

    children[0]->Nhbrs[1].push_back(children[1]);
    children[0]->Nhbrs[2].push_back(children[3]);
    children[1]->Nhbrs[2].push_back(children[2]);
    children[1]->Nhbrs[3].push_back(children[0]);
    children[2]->Nhbrs[3].push_back(children[3]);
    children[2]->Nhbrs[0].push_back(children[1]);
    children[3]->Nhbrs[0].push_back(children[0]);
    children[3]->Nhbrs[1].push_back(children[2]);

    for (int i = 0; i < 4; ++i) {
        for (vector<Box*>::iterator iter = this->Nhbrs[i].begin(); iter != this->Nhbrs[i].end(); ++iter) {
            Box* b = *iter;
            int foundDst = 0;
            for (int j = 0; j < 4; ++j) {
                int idx = isNhbr(children[j], b);
                if (idx != -1) {
                    children[j]->Nhbrs[idx].push_back(b);
                    if (foundDst == 0) {
                        for (vector<Box*>::iterator it =
                                b->Nhbrs[oppositeDir[idx]].begin();
                                it != b->Nhbrs[oppositeDir[idx]].end(); ++it) {
                            if (*it == this) {
                                *it = children[j];
                            }
                        }
                    } else {
                        b->Nhbrs[oppositeDir[idx]].push_back(children[j]);
                    }
                    ++foundDst;
                }
            }
            assert(foundDst > 0);
        }
    }

    for (int i = 0; i < 4; ++i) {
        //add all of parent's walls and corners to each child,
        //will be filtered later in updatestatus()

        //TODO try to improve this part. Should filter the features before add them to its children.
        children[i]->walls.insert(children[i]->walls.begin(),
                this->walls.begin(), this->walls.end());
        children[i]->corners.insert(children[i]->corners.begin(),
                this->corners.begin(), this->corners.end());

        BoxNode node;
        node.x = children[i]->x;
        node.y = children[i]->y;
        // get the clearance for voronoi
        determine_clearance(node);
        children[i]->cl_m = node.clearance;

        //distribute the feature for voronoi
        distribute_features2box(children[i]);
    }

    this->isLeaf = false;

    for (int i = 0; i < 4; ++i) {
        pAllLeaf->push_back(children[i]);
        chldn.push_back(children[i]);
    }
    expansions.push_back(pAllLeaf->size());

    return true;
}

bool Box::splitAngle(vector<Box*>& chldn) {

    if (!this->isLeaf || this->status == FREE || this->status == STUCK) {
        return 0;
    }

    vector<AngleRange> l1AngleRanges; // multiple forbidden intervals(angle ranges)
    vector<AngleRange> l2AngleRanges;
    vector<AngleRange> l1ForbidenZone;
    vector<AngleRange> l2ForbidenZone;
    vector<AngleRange> l1SafeZone;
    vector<AngleRange> l2SafeZone;

    double link_length[2];
    link_length[0] = l1;
    link_length[1] = l2;

    for (list<Wall*>::iterator itW = walls.begin(); itW != walls.end(); itW++) {

        Wall* w = *itW;

        // when wall intersects with box
        if (hasIntersectionLR((double) w->src->x, (double) w->src->y,(double) w->dst->x, (double) w->dst->y,
                              this->x - this->width / 2, this->y + this->height / 2,
                              this->x + this->width / 2, this->y - this->height / 2)) {
            this->status = MIXED;
            return 0;
        }

        double closerX = 0;
        double closerY = 0;
        // when expanded wall intersects with box
        double newSrcX = w->src->x;
        double newSrcY = w->src->y;
        double newDstX = w->dst->x;
        double newDstY = w->dst->y;

        getExpandedLine(w, BOARDWIDTH, BOARDHEIGHT, newSrcX, newSrcY, newDstX, newDstY);
        // closest feature to the box is a corner
        if (hasIntersectionLR(newSrcX, newSrcY, newDstX, newDstY,
                              this->x - this->width / 2, this->y + this->height / 2,
                              this->x + this->width / 2, this->y - this->height / 2)) {

            // find the closer endpoint
            if (min(this->x - this->width / 2, this->x + this->width / 2) > max(w->src->x, w->dst->x)) {
                if (w->src->x > w->dst->x) {
                    closerX = w->src->x;
                    closerY = w->src->y;
                } else {
                    closerX = w->dst->x;
                    closerY = w->dst->y;
                }
            }

            if (min(this->y - this->height / 2, this->y + this->height / 2) > max(w->src->y, w->dst->y)) {
                if (w->src->y > w->dst->y) {
                    closerX = w->src->x;
                    closerY = w->src->y;
                } else {
                    closerX = w->dst->x;
                    closerY = w->dst->y;
                }
            }

            if (max(this->x - this->width / 2, this->x + this->width / 2) < min(w->src->x, w->dst->x)) {
                if (w->src->x < w->dst->x) {
                    closerX = w->src->x;
                    closerY = w->src->y;
                } else {
                    closerX = w->dst->x;
                    closerY = w->dst->y;
                }
            }

            if (max(this->y - this->height / 2, this->y + this->height / 2) < min(w->src->y, w->dst->y)) {
                if (w->src->y < w->dst->y) {
                    closerX = w->src->x;
                    closerY = w->src->y;
                } else {
                    closerX = w->dst->x;
                    closerY = w->dst->y;
                }
            }

            // calculate the anglerange of l1, l2
            for (int i = 0; i < 2; i++) {
                double tempL = 0;
                if (i == 0) {
                    tempL = l1 + thickness / 2;
                } else {
                    tempL = l2 + thickness / 2;
                }

// old version from Zhongdi
                AngleRange tempAngleRange = calcAngleRangeCB(tempL, thickness, closerX, closerY, this);

                if (tempAngleRange.lowerBound != 0 || tempAngleRange.upperBound != 0) {
                    if (i == 0) {
                        l1AngleRanges.push_back(tempAngleRange);
                    } else {
                        l2AngleRanges.push_back(tempAngleRange);
                    }
                }
            }
        }
        // closest feature to the box is a wall
        else {

// old version from Zhongdi
            double closestCornerX = 0;
            double closestCornerY = 0;
            double tempSeparation = FLT_MAX;

            // find the closest corner of box to the wall
            double temp;
            temp = getSeparation(w->src->x, w->src->y, w->dst->x, w->dst->y, this->x-this->width/2, this->y-this->height/2);
            if (temp <= tempSeparation) {
                tempSeparation = temp;
                closestCornerX = this->x - this->width/2;
                closestCornerY = this->y - this->height/2;
            }

            temp = getSeparation(w->src->x, w->src->y, w->dst->x, w->dst->y, this->x-this->width/2, this->y+this->height/2);
            if (temp <= tempSeparation) {
                tempSeparation = temp;
                closestCornerX = this->x - this->width / 2;
                closestCornerY = this->y + this->height / 2;
            }

            temp = getSeparation(w->src->x, w->src->y, w->dst->x, w->dst->y, this->x+this->width/2, this->y-this->height/2);
            if (temp <= tempSeparation) {
                tempSeparation = temp;
                closestCornerX = this->x + this->width / 2;
                closestCornerY = this->y - this->height / 2;
            }

            temp = getSeparation(w->src->x, w->src->y, w->dst->x, w->dst->y, this->x+this->width/2, this->y+this->height/2);
            if (temp <= tempSeparation) {
                tempSeparation = temp;
                closestCornerX = this->x + this->width / 2;
                closestCornerY = this->y + this->height / 2;
            }

            // calculate the anglerange of l1, l2
            for (int i = 0; i < 2; i++) {
                double tempL = 0;
                if (i == 0) {
                    tempL = l1 + thickness / 2;
                } else {
                    tempL = l2 + thickness / 2;
                }
                bool checkSrc = true;
                bool checkDst = true;
                if (tempSeparation < tempL) {
                    vector<Point> tempIntersection;
                    tempIntersection = calcIntersectionCW(tempL, closestCornerX, closestCornerY, w, checkSrc, checkDst);

                    //Should always check both endpoints
                    if (checkSrc) {
                        Point tempPoint(w->src->x, w->src->y);
                        tempIntersection.push_back(tempPoint);
                    }
                    if (checkDst) {
                        Point tempPoint(w->dst->x, w->dst->y);
                        tempIntersection.push_back(tempPoint);
                    }

                    AngleRange tempAngleRange(0, 0);
                    map<double, double> tempAngles;
                    for (vector<Point>::iterator itP = tempIntersection.begin(); itP != tempIntersection.end(); itP++) {
                        Point temp = *itP;
                        double tempAngle = calcAngle(closestCornerX, closestCornerY, temp.x, temp.y);
                        tempAngles.insert( pair<double, double>(tempAngle, getPointDistance(closestCornerX, closestCornerY, temp.x, temp.y)));
                    }
                    tempAngleRange = calcAngleRange(tempAngles, thickness);
                    if (tempAngleRange.lowerBound != 0 || tempAngleRange.upperBound != 0) {
                        if (i == 0) {
                            l1AngleRanges.push_back(tempAngleRange);
                        } else {
                            l2AngleRanges.push_back(tempAngleRange);
                        }
                    }
                }

                //Should always check both endpoints
                AngleRange tempAngleRange_src = calcAngleRangeCB(tempL, thickness, (double) w->src->x, (double) w->src->y, this);

                if (tempAngleRange_src.lowerBound != 0 || tempAngleRange_src.upperBound != 0) {
                    if (i == 0) {
                        l1AngleRanges.push_back(tempAngleRange_src);
                    } else {
                        l2AngleRanges.push_back(tempAngleRange_src);
                    }
                }
                AngleRange tempAngleRange_dst = calcAngleRangeCB(tempL, thickness, (double) w->dst->x, (double) w->dst->y, this);

                if (tempAngleRange_dst.lowerBound != 0 || tempAngleRange_dst.upperBound != 0) {
                    if (i == 0) {
                        l1AngleRanges.push_back(tempAngleRange_dst);
                    } else {
                        l2AngleRanges.push_back(tempAngleRange_dst);
                    }
                }
            }
        }
    }

    l1ForbidenZone = calcZone(l1AngleRanges);
    l2ForbidenZone = calcZone(l2AngleRanges);


    l1SafeZone = calcOppoZone(l1ForbidenZone);
    l2SafeZone = calcOppoZone(l2ForbidenZone);


    if (twoStrategyOption == 2) {
        if (l1SafeZone.size() <= l2SafeZone.size()) {
            safeRanges = l2SafeZone.size();
        } else {
            safeRanges = l1SafeZone.size();
        }
    }

    if (l1SafeZone.empty() || l2SafeZone.empty()) {

        condition = 2;

        this->status = STUCK;
        return 0;
    }

    this->isLeaf = false;

    if (crossingOption) {
        vector<AngleRange> tempZone1;
        for (vector<AngleRange>::iterator it = l1SafeZone.begin();
                it != l1SafeZone.end(); it++) {
            if (it->lowerBound > it->upperBound) {
                if (360 - it->lowerBound > 0.01) {
                    AngleRange tempRange1(it->lowerBound, 360);
                    tempZone1.push_back(tempRange1);
                }
                if (it->upperBound > 0.01) {
                    AngleRange tempRange2(0, it->upperBound);
                    tempZone1.push_back(tempRange2);
                }
            } else {
                if (it->upperBound - it->lowerBound > 0.01) {
                    AngleRange tempRange(it->lowerBound, it->upperBound);
                    tempZone1.push_back(tempRange);
                }
            }

        }
        l1SafeZone = tempZone1;
        vector<AngleRange> tempZone2;
        for (vector<AngleRange>::iterator it = l2SafeZone.begin();
                it != l2SafeZone.end(); it++) {
            if (it->lowerBound > it->upperBound) {
                if (360 - it->lowerBound > 0.01) {
                    AngleRange tempRange1(it->lowerBound, 360);
                    tempZone2.push_back(tempRange1);
                }
                if (it->upperBound > 0.01) {
                    AngleRange tempRange2(0, it->upperBound);
                    tempZone2.push_back(tempRange2);
                }
            } else {
                if (it->upperBound - it->lowerBound > 0.01) {
                    AngleRange tempRange(it->lowerBound, it->upperBound);
                    tempZone2.push_back(tempRange);
                }
            }
        }
        l2SafeZone = tempZone2;
    }


    for (int i = 0; i < (int) l1SafeZone.size(); i++) {
        for (int j = 0; j < (int) l2SafeZone.size(); j++) {
            for (int l = 0; l < crossingOption + 1; l++) {

                Box* child = new Box(x, y, width, height);
                child->isBig = false;
                child->pParent = this;
                child->rChildID = i * j * l + j * l + l;
                child->status = FREE;

                condition = 6;

                child->xi[0] = l1SafeZone[i].lowerBound;
                child->xi[1] = l1SafeZone[i].upperBound;
                child->xi[2] = l2SafeZone[j].lowerBound;
                child->xi[3] = l2SafeZone[j].upperBound;

                if (crossingOption) {
                    if (l == 0) {
                        child->order = LT;
                        if (child->xi[0] + bandwidth >= child->xi[3]) {
                            continue;
                        }
                        if(child->xi[2] - child->xi[1] >= 360 - bandwidth) {
                            continue;
                        }
                    } else {
                        child->order = GT;
                        if (child->xi[2] + bandwidth >= child->xi[1]) {
                            continue;
                        }
                        if(child->xi[0] - child->xi[3] >= 360 - bandwidth) {
                            continue;
                        }
                    }
                    for (vector<Box*>::iterator it = chldn.begin(); it != chldn.end(); it++) {
                        if ((*it)->xi[1] == 360 && child->xi[0] == 0
                                && (*it)->xi[2] == child->xi[2]
                                && (*it)->xi[3] == child->xi[3]
                                && (*it)->order != child->order) {
                            child->Nhbrs[4].push_back(*it);
                            if (std::find((*it)->Nhbrs[4].begin(),
                                    (*it)->Nhbrs[4].end(), child)
                                    == (*it)->Nhbrs[4].end()) {
                                (*it)->Nhbrs[4].push_back(child);
                            }
                        }
                        if ((*it)->xi[3] == 360 && child->xi[2] == 0
                                && (*it)->xi[0] == child->xi[0]
                                && (*it)->xi[1] == child->xi[1]
                                && (*it)->order != child->order) {
                            child->Nhbrs[4].push_back(*it);
                            if (std::find((*it)->Nhbrs[4].begin(),
                                    (*it)->Nhbrs[4].end(), child)
                                    == (*it)->Nhbrs[4].end()) {
                                (*it)->Nhbrs[4].push_back(child);
                            }
                        }
                    }
                }

                for (int k = 0; k < 4; ++k) {
                    for (vector<Box*>::iterator itBNhbrs =
                            this->Nhbrs[k].begin();
                            itBNhbrs != this->Nhbrs[k].end(); ++itBNhbrs) {
                        Box* b = *itBNhbrs;
                        int idx = isNhbr(child, b);
                        if (idx != -1) {
                            child->Nhbrs[idx].push_back(b);
                            b->Nhbrs[oppositeDir[idx]].push_back(child);
                        }
                    }
                }

                pAllLeaf->push_back(child);
                expansions.push_back(pAllLeaf->size());
                chldn.push_back(child);
            }
        }
    }

    return true;
}

bool Box::split(double epsilon, vector<Box*>& chldn) {
    if ((this->height / 2 < epsilon && this->width / 2 < epsilon) || (twoStrategyOption > 0
        && (int) walls.size() + (int) corners.size() <= sizeOfPhiB && !shouldSplit2D)) {

        if (this->height / 2 < epsilon && this->width / 2 < epsilon) {
            return splitAngle(chldn);
        } else {
            QT->PQ->push(this);
            bool result = splitAngle(chldn);

            isLeaf = true;
            status = MIXED;
            shouldSplit2D = true;
            dist2Source = -1;
            return result;
        }
    }
    else {
        return split2D(chldn);
    }
}


Box::Status Box::checkChildStatus(double x, double y, double width) {


    Wall* nearestWall = NULL;
    double mindistW = FLT_MAX;
    if(walls.size()){
        for (list<Wall*>::iterator iterW = walls.begin(); iterW != walls.end(); ++iterW) {
            Wall* w = *iterW;
            double dist = w->distance(x, y);
            if (dist < mindistW) {
                mindistW = dist;
                nearestWall = *iterW;
            }
        }
    }

    double mindistC = FLT_MAX;
    Corner* nearestCorner = NULL;
    if(corners.size()){
        for(list<Corner*>::iterator iterC = corners.begin(); iterC != corners.end(); ++iterC) {
            Corner* c = *iterC;
            double dist = c->distance(x, y);
            if (dist < mindistC) {
                mindistC = dist;
                nearestCorner = *iterC;
            }
        }
    }

    if(x == 134 && y == 114){
        fprintf(fp, "r0 %lf  rB %lf  width %lf\n", r0, rB, width);
        if(nearestWall == NULL && nearestCorner == NULL)
            fprintf(fp, "......\n");
        else{
            if(nearestWall != NULL && nearestCorner != NULL){
                fprintf(fp, "%lf %lf\n", mindistW, mindistC);
                fprintf(fp, "%lf %lf -> %lf %lf\n", nearestWall->src->x, nearestWall->src->y, nearestWall->dst->x, nearestWall->dst->y);
                fprintf(fp, "%d   %d %d %d %d\n", nearestWall->isRight(x, y),
                                nearestWall->isRight(x + width / 2, y + width / 2),
                                nearestWall->isRight(x - width / 2, y + width / 2),
                                nearestWall->isRight(x + width / 2, y - width / 2),
                                nearestWall->isRight(x - width / 2, y - width / 2));
            }
            else{
                if(nearestWall != NULL){
                    fprintf(fp, "wall %lf\n", mindistW);
                }
                else{
                    fprintf(fp, "corner %lf\n", mindistC);
                }
            }
        }
    }

    Status tempStatus = UNKNOWN;
    if(nearestWall == NULL && nearestCorner == NULL) return tempStatus;


    if (mindistW < mindistC) {
        if (nearestWall->isRight(x, y) && mindistW > r0+rB/2+thickness/2) {
            tempStatus = FREE;
        }
        else if(!nearestWall->isRight(x, y) && mindistW > rB/2-thickness/2) {
            tempStatus = STUCK;
        }
        else if (!nearestWall->isRight(x + width / 2, y + width / 2)
              && !nearestWall->isRight(x - width / 2, y + width / 2)
              && !nearestWall->isRight(x + width / 2, y - width / 2)
              && !nearestWall->isRight(x - width / 2, y - width / 2)) {
            tempStatus = STUCK;
        }
    }
    //otherwise check the corner's convexity
    //if convex, out; if concave, in
    //note that if a wall and a corner are the same dist,
    //only need to take care of the corner
    else{
        if(nearestCorner->isConvex() && mindistC > r0+rB/2+thickness/2) {
            tempStatus = FREE;
        } else if (!nearestCorner->isConvex() && mindistC > rB/2-thickness/2) {
            tempStatus = STUCK;
        }
    }

    if(x == 134 && y == 114){
        fprintf(fp, "status %d\n", tempStatus);
    }

    return tempStatus;
}

//    -1: not adjacent
//    0: b2 is on the right of b1
//    1: b2 is above b1
//    2: b2 is on the left of b1
//    3  b2 is under b1
//    4  b1 and b2 have the same translational position
int Box::isNhbr(Box* b1, Box* b2) {
    double xmin1 = b1->x - b1->width / 2;
    double xmax1 = b1->x + b1->width / 2;
    double ymin1 = b1->y - b1->height / 2;
    double ymax1 = b1->y + b1->height / 2;
    double xmin2 = b2->x - b2->width / 2;
    double xmax2 = b2->x + b2->width / 2;
    double ymin2 = b2->y - b2->height / 2;
    double ymax2 = b2->y + b2->height / 2;

    if (!isArcIntsct(b1->xi[0], b1->xi[1], b2->xi[0], b2->xi[1]) || !isArcIntsct(b1->xi[2], b1->xi[3], b2->xi[2], b2->xi[3])) {
        return -1;
    }
    if (isEq(xmin1, xmax2) && isOneContainAnotherY(b1, b2) && (!crossingOption || (b1->order == b2->order && b1->order != NON)
        || b1->order == ALL || b2->order == ALL)) {
        return 2;
    }
    if (isEq(xmax1, xmin2) && isOneContainAnotherY(b1, b2) && (!crossingOption || (b1->order == b2->order && b1->order != NON)
        || b1->order == ALL || b2->order == ALL)) {
        return 0;
    }
    if (isEq(ymin1, ymax2) && isOneContainAnotherX(b1, b2)  && (!crossingOption || (b1->order == b2->order && b1->order != NON)
        || b1->order == ALL || b2->order == ALL)) {
        return 3;
    }
    if (isEq(ymax1, ymin2) && isOneContainAnotherX(b1, b2) && (!crossingOption || (b1->order == b2->order && b1->order != NON)
        || b1->order == ALL || b2->order == ALL)) {
        return 1;
    }

    return -1;
}

double angleDistance(double a1, double a2) {
    return fmin(abs(a1 - a2), 360 - abs(a1 - a2));
}

void Box::updateStatusBig() {
    if (status != UNKNOWN) {
        return;
    }
    if (pParent) {
        condition = 3;
        status = pParent->checkChildStatus(this->x, this->y, this->width);
    } else {
        status = MIXED;
    }
    if (status == STUCK || status == FREE) {
        return;
    }

    double outerDomain = r0 + rB + thickness / 2;
    double innerDomain = thickness / 2 > rB ? thickness / 2 - rB : 0;
    for (list<Corner*>::iterator it = corners.begin(); it != corners.end();) {
        Corner* c = *it;

        if (c->distance(this->x, this->y) <= outerDomain) {
            status = MIXED;
            ++it;
        } else if (c->distance(this->x, this->y) < innerDomain) {
            condition = 0;
            status = STUCK;
            ++it;
        } else {
            it = corners.erase(it);
        }
    }

    for (list<Wall*>::iterator it = walls.begin(); it != walls.end();) {
        Wall* w = *it;
        double distWall = w->distance(this->x, this->y);
        if (distWall <= outerDomain) {
            status = MIXED;
            ++it;
        } else if (distWall < innerDomain) {
            condition = 1;
            status = STUCK;
            ++it;
        } else {
            it = walls.erase(it);
        }
    }
}

void Box::updateStatusSmall() {
    if (status != UNKNOWN) {
        return;
    }
    else {
    }
}

// check whether a line segment intersects with a rectangle
bool hasIntersectionLR(double xs, double ys, double xe, double ye, double xleft, double ytop, double xr, double yb) {

    double a = ys - ye, b = xe - xs, c = xs * ye - xe * ys;
    if ((a * xleft + b * ytop + c >= 0 && a * xr + b * yb + c <= 0)
            || (a * xleft + b * ytop + c <= 0 && a * xr + b * yb + c >= 0)
            || (a * xleft + b * yb + c >= 0 && a * xr + b * ytop + c <= 0)
            || (a * xleft + b * yb + c <= 0 && a * xr + b * ytop + c >= 0)) {
        if (xleft > xr)
            swap(xleft, xr);
        if (ytop < yb)
            swap(ytop, yb);
        if ((xs < xleft && xe < xleft) || (xs > xr && xe > xr)
                || (ys > ytop && ye > ytop) || (ys < yb && ye < yb))
            return false;
        else
            return true;
    } else
        return false;
}

void getExpandedLine(Wall* wall, double boardWidth, double boardHeight, double& newSrcX, double& newSrcY, double& newDstX, double& newDstY) {

    double xVariation = wall->dst->x - wall->src->x;

    if (xVariation != 0) {
        double slope = (wall->dst->y - wall->src->y) / (wall->dst->x - wall->src->x);
        double yChanged = boardWidth * slope;
        if (wall->dst->x > wall->src->x) {
            newDstX = wall->dst->x + boardWidth;
            newSrcX = wall->src->x - boardWidth;
            newDstY = wall->dst->y + yChanged;
            newSrcY = wall->src->y - yChanged;
        } else {
            newDstX = wall->src->x + boardWidth;
            newSrcX = wall->dst->x - boardWidth;
            newDstY = wall->src->y + yChanged;
            newSrcY = wall->dst->y - yChanged;
        }

    } else {
        if (wall->dst->y > wall->src->y) {
            newDstY = wall->dst->y + boardHeight;
            newSrcY = wall->src->y - boardHeight;
        } else {
            newDstY = wall->dst->y - boardHeight;
            newSrcY = wall->src->y + boardHeight;
        }
    }
}

double getPointDistance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double getSeparation(double xa, double ya, double xb, double yb, double x,
        double y) {
    double a, b, c;
    a = getPointDistance(xa, ya, x, y);
    if (a <= 0)
        return 0.0;
    b = getPointDistance(xb, yb, x, y);
    if (b <= 0)
        return 0.0;
    c = getPointDistance(xa, ya, xb, yb);
    if (c <= 0)
        return a;

    if (a * a > b * b + c * c)
        return b;
    if (b * b > a * a + c * c)
        return a;

    double l = (a + b + c) / 2;
    double s = sqrt(l * (l - a) * (l - b) * (l - c));  //Heron's formula
    return 2 * s / c;
}

// calculate the intersection between circle and box
vector<Point> calcIntersectionCB(double l, double xc, double yc, double xleft, double ytop, double xr, double yb) {
    vector<Point> points;
    double temp1 = 0;
    double x1 = 0, x2 = 0, y1 = 0, y2 = 0;
//calclate the intersection between circle and x = xleft / x = xr
    for (int i = 0; i < 2; i++) {
        double xTemp = 0;
        if (i == 0) {
            xTemp = xleft;

        } else {
            xTemp = xr;
        }
        temp1 = l * l - (xTemp - xc) * (xTemp - xc);
        // intersect at 2 points
        if (temp1 > 0) {
            y1 = -sqrt(temp1) + yc;
            y2 = sqrt(temp1) + yc;
            if (y1 <= ytop && y1 >= yb) {
                Point intersection(xTemp, y1);
                points.push_back(intersection);
            }
            if (y2 <= ytop && y2 >= yb) {
                Point intersection(xTemp, y2);
                points.push_back(intersection);
            }
        }
    }

//calclate the intersection between circle and y = ytop / y = yb
    for (int i = 0; i < 2; i++) {
        double yTemp = 0;
        if (i == 0) {
            yTemp = ytop;

        } else {
            yTemp = yb;

        }
        temp1 = l * l - (yTemp - yc) * (yTemp - yc);
        // intersect at 2 points
        if (temp1 > 0) {
            x1 = -sqrt(temp1) + xc;
            x2 = sqrt(temp1) + xc;
            if (x1 <= xr && x1 >= xleft) {
                Point intersection(x1, yTemp);
                points.push_back(intersection);
            }
            if (x2 <= xr && x2 >= xleft) {
                Point intersection(x2, yTemp);
                points.push_back(intersection);
            }
        }
    }

    return points;
}

AngleRange calcAngleRangeCB(double l, double t, double xc, double yc, Box* b) {
    vector<Point> tempPoints;
    tempPoints = calcIntersectionCB(l, xc, yc, b->x - b->width / 2, b->y + b->height / 2, b->x + b->width / 2, b->y - b->height / 2);

    double distance = getPointDistance(xc, yc, b->x - b->width / 2, b->y - b->height / 2);
    if (distance <= l) {
        Point tempPoint(b->x - b->width / 2, b->y - b->height / 2);
        tempPoints.push_back(tempPoint);
    }

    distance = getPointDistance(xc, yc, b->x - b->width / 2, b->y + b->height / 2);
    if (distance <= l) {
        Point tempPoint(b->x - b->width / 2, b->y + b->height / 2);
        tempPoints.push_back(tempPoint);
    }

    distance = getPointDistance(xc, yc, b->x + b->width / 2, b->y - b->height / 2);
    if (distance <= l) {
        Point tempPoint(b->x + b->width / 2, b->y - b->height / 2);
        tempPoints.push_back(tempPoint);
    }

    distance = getPointDistance(xc, yc, b->x + b->width / 2, b->y + b->height / 2);
    if (distance <= l) {
        Point tempPoint(b->x + b->width / 2, b->y + b->height / 2);
        tempPoints.push_back(tempPoint);
    }

    // calculate the angle range of the specific wall
    AngleRange tempAngleRange(0, 0);
    map<double, double> tempAngles;
    for (vector<Point>::iterator it = tempPoints.begin();it != tempPoints.end(); it++) {
        Point temp = *it;
        double tempAngle = calcAngle(xc, yc, temp.x, temp.y);
        calcOppoAngle(tempAngle);
        tempAngles.insert(pair<double, double>(tempAngle, getPointDistance(xc, yc, temp.x, temp.y)));
    }
    tempAngleRange = calcAngleRange(tempAngles, t);

    return tempAngleRange;
}

// calculate the intersection between circle and wall
vector<Point> calcIntersectionCW(double l, double xc, double yc, Wall* wall, bool& checkSrc, bool& checkDst) {
    vector<Point> points;

// quick check the separation and r0, if r0 < separation, no intersection
    if (l < getSeparation((double) wall->src->x, (double) wall->src->y, (double) wall->dst->x, (double) wall->dst->y, xc, yc)) {
        return points;
    }

    double x1, x2, y1, y2;
// get the expression of wall, in form of y = ax + b
    double a, b;
    double xMax = max(wall->src->x, wall->dst->x);
    double xMin = min(wall->src->x, wall->dst->x);
    double yMax = max(wall->src->y, wall->dst->y);
    double yMin = min(wall->src->y, wall->dst->y);
    double temp1 = wall->dst->x - wall->src->x;
    if (temp1 == 0) {
        double temp = l * l - (wall->dst->x - xc) * (wall->dst->x - xc);
        if (temp > 0) {
            x1 = wall->dst->x;
            x2 = wall->dst->x;
            y1 = -sqrt(temp) + yc;
            y2 = sqrt(temp) + yc;

            if (y1 <= yMax && y1 >= yMin) {
                Point intersection(x1, y1);
                points.push_back(intersection);
            }
            if (y2 <= yMax && y2 >= yMin) {
                Point intersection(x2, y2);
                points.push_back(intersection);
            }

            if (wall->src->y <= y2 && wall->src->y >= y1) {
                checkSrc = true;
            } else {
                checkSrc = false;
            }
            if (wall->dst->y <= y2 && wall->dst->y >= y1) {
                checkDst = true;
            } else {
                checkDst = false;
            }
        }
    } else {
        a = (wall->dst->y - wall->src->y) / temp1;
        b = (wall->dst->x * wall->src->y - wall->src->x * wall->dst->y) / temp1;

        // check B^2 - 4*A*C
        double temp = pow((2 * a * b - 2 * a * yc - 2 * xc), 2)
                - 4 * (a * a + 1)
                        * (b * b - 2 * b * yc + yc * yc + xc * xc - l * l);
        // intersect at 2 points
        if (temp > 0) {
            x1 = ((2 * a * yc + 2 * xc - 2 * a * b) - sqrt(temp))
                    / (2 * (a * a + 1));
            x2 = ((2 * a * yc + 2 * xc - 2 * a * b) + sqrt(temp))
                    / (2 * (a * a + 1));
            y1 = a * x1 + b;
            y2 = a * x2 + b;

            if (x1 <= xMax && x1 >= xMin) {
                Point intersection(x1, y1);
                points.push_back(intersection);
            }
            if (x2 <= xMax && x2 >= xMin) {
                Point intersection(x2, y2);
                points.push_back(intersection);
            }
            if (wall->src->x <= x2 && wall->src->x >= x1) {
                checkSrc = true;
            } else {
                checkSrc = false;
            }
            if (wall->dst->x <= x2 && wall->dst->x >= x1) {
                checkDst = true;
            } else {
                checkDst = false;
            }
        }
    }

    return points;
}

// calculate the angle between the vector from src to dst and the X axis
double calcAngle(double srcX, double srcY, double dstX, double dstY) {
    double temp = atan2(dstY - srcY, dstX - srcX) * 180 / PI;
    if (temp < 0) {
        temp += 360;
    }
    return temp;
}

// calculate the opposite angle
void calcOppoAngle(double& angle) {
    angle += 180;
    if (angle >= 360) {
        angle -= 360;
    }
}

// calculate the angleRange spanned by several angles, in our case the angleRange is no more than 180 degree
// 0 < lowerbound < upperbound < 360
AngleRange calcAngleRange(map<double, double> angles, double thickness) {
    AngleRange angleRange(0, 0);
    if (angles.empty()) {
        return angleRange;
    }
    double minAngle = 540, maxAngle = 0;
    double minAngleDistance = 0, maxAngleDistance = 0;
    for (map<double, double>::iterator it = angles.begin(); it != angles.end(); it++) {
        double tempAngle = it->first;
        double tempDistance = it->second;
        if (tempAngle <= minAngle) {
            minAngle = tempAngle;
            minAngleDistance = tempDistance;
        }
        if (tempAngle >= maxAngle) {
            maxAngle = tempAngle;
            maxAngleDistance = tempDistance;
        }
    }

    if (maxAngle - minAngle < 180) {
        angleRange.lowerBound = minAngle;
        angleRange.upperBound = maxAngle;

    } else {
        minAngle = 540, maxAngle = 0;
        for (map<double, double>::iterator it = angles.begin(); it != angles.end(); it++) {
            double tempAngle = it->first;
            double tempDistance = it->second;
            // important here
            if (tempAngle < 180) {
                tempAngle = tempAngle + 360;
            }

            if (tempAngle <= minAngle) {
                minAngle = tempAngle;
                minAngleDistance = tempDistance;
            }
            if (tempAngle >= maxAngle) {
                maxAngle = tempAngle;
                maxAngleDistance = tempDistance;
            }
        }

        // TODO, can avoid this check
        if (maxAngle - minAngle < 180) {
            angleRange.lowerBound = minAngle;
            angleRange.upperBound = maxAngle - 360;
        }
    }

// if robot has thickness, expand the forbidden range.
    bool isStuck = 0;
    double fixRange1 = 0;
    if (minAngleDistance < thickness / 2) {
        isStuck = 1;
    }

    double fixRange2 = 0;
    if (maxAngleDistance < thickness / 2) {
        isStuck = 1;
    }

    fixRange1 = asin(thickness / 2 / minAngleDistance) * 180 / PI;
    fixRange2 = asin(thickness / 2 / maxAngleDistance) * 180 / PI;

    if (isStuck) {
        angleRange.lowerBound = 0;
        angleRange.upperBound = 360;
    } else {
        angleRange.lowerBound -= fixRange1;
        if (angleRange.lowerBound < 0) {
            angleRange.lowerBound += 360;
        }

        angleRange.upperBound += fixRange2;
        if (angleRange.upperBound > 360) {
            angleRange.upperBound -= 360;
        }
    }

    return angleRange;
}
// sorting function
bool sortAngleRanges(const AngleRange &v1, const AngleRange &v2) {
    return v1.lowerBound < v2.lowerBound;
}

// merge several angle ranges.
vector<AngleRange> calcZone(vector<AngleRange> &srcAngleRanges) {
    vector<AngleRange> dstAngleRanges;
    vector<AngleRange> newSrcAngleRanges;
    vector<AngleRange> newDstAngleRanges;
    if (srcAngleRanges.empty()) {
        return dstAngleRanges;
    }
    std::sort(srcAngleRanges.begin(), srcAngleRanges.end(), sortAngleRanges);

    for (int i=0;i<(int)srcAngleRanges.size();++i) {
        AngleRange temp = srcAngleRanges[i];
        if (temp.lowerBound > temp.upperBound) {
            temp.upperBound += 360;
        }
        newSrcAngleRanges.push_back(AngleRange(temp.lowerBound, temp.upperBound));
    }

    AngleRange interval(newSrcAngleRanges[0].lowerBound, newSrcAngleRanges[0].upperBound);
    for(int i=1;i<(int)newSrcAngleRanges.size();++i){
        if(newSrcAngleRanges[i].lowerBound <= interval.upperBound){
            if(interval.upperBound < newSrcAngleRanges[i].upperBound) interval.upperBound = newSrcAngleRanges[i].upperBound;
        }
        else{
            dstAngleRanges.push_back(AngleRange(interval.lowerBound, interval.upperBound));
            interval.lowerBound = newSrcAngleRanges[i].lowerBound;
            interval.upperBound = newSrcAngleRanges[i].upperBound;
        }
    }
    // need to check last interval
    if(interval.upperBound >= 360){
        interval.upperBound -= 360.0f;
        if(interval.upperBound < newSrcAngleRanges[0].lowerBound){
            dstAngleRanges.push_back(AngleRange(interval.lowerBound, interval.upperBound));

            for (int i=0;i<(int)dstAngleRanges.size();++i) {
                AngleRange temp = dstAngleRanges[i];
                if (temp.lowerBound >= 360) temp.lowerBound -= 360;
                if (temp.upperBound > 360)  temp.upperBound -= 360;
                newDstAngleRanges.push_back(AngleRange(temp.lowerBound, temp.upperBound));
            }
        }
        else{
            int i=0;
            while(i < (int)dstAngleRanges.size() && dstAngleRanges[i].lowerBound <= interval.upperBound){
                if(interval.upperBound < dstAngleRanges[i].upperBound){
                    interval.upperBound = dstAngleRanges[i].upperBound;
                }
                ++i;
            }
            if(interval.upperBound > interval.lowerBound){
                newDstAngleRanges.push_back(AngleRange(0, 360));
            }
            else{
                newDstAngleRanges.push_back(AngleRange(interval.lowerBound, interval.upperBound));

                while(i < (int)dstAngleRanges.size()){
                    AngleRange temp = dstAngleRanges[i];
                    if (temp.lowerBound >= 360) temp.lowerBound -= 360;
                    if (temp.upperBound > 360)  temp.upperBound -= 360;
                    newDstAngleRanges.push_back(AngleRange(temp.lowerBound, temp.upperBound));
                    ++i;
                }
            }
        }
    }
    else{
        dstAngleRanges.push_back(AngleRange(interval.lowerBound, interval.upperBound));

        for (int i=0;i<(int)dstAngleRanges.size();++i) {
            AngleRange temp = dstAngleRanges[i];
            if (temp.lowerBound >= 360) temp.lowerBound -= 360;
            if (temp.upperBound > 360)  temp.upperBound -= 360;
            newDstAngleRanges.push_back(AngleRange(temp.lowerBound, temp.upperBound));
        }
    }

    return newDstAngleRanges;
}

// calculate the opposite of the existing zone, the lower and upperbound of the angle ranges will between 0 and 360
vector<AngleRange> calcOppoZone(vector<AngleRange> &srcAngleRanges) {
    vector<AngleRange> dstAngleRanges;
    if (srcAngleRanges.empty()) {
        AngleRange angleRange(0, 360);
        dstAngleRanges.push_back(angleRange);
        return dstAngleRanges;
    }
    if (srcAngleRanges.begin()->lowerBound == 0
            && srcAngleRanges.begin()->upperBound == 360) {
        return dstAngleRanges;
    }

    std::sort(srcAngleRanges.begin(), srcAngleRanges.end(), sortAngleRanges);

    double tempLowerBound = 540, tempUpperBound = 0;

    for (vector<AngleRange>::iterator it = srcAngleRanges.begin();
            it != srcAngleRanges.end(); it++) {
        AngleRange temp = *it;
        if (it == srcAngleRanges.begin()) {
            tempLowerBound = temp.upperBound;
            continue;
        }

        tempUpperBound = temp.lowerBound;
        AngleRange angleRange(tempLowerBound, tempUpperBound);
        dstAngleRanges.push_back(angleRange);
        tempLowerBound = temp.upperBound;
    }
    tempUpperBound = (*srcAngleRanges.begin()).lowerBound;

    AngleRange angleRange(tempLowerBound, tempUpperBound);
    dstAngleRanges.push_back(angleRange);
    return dstAngleRanges;
}
