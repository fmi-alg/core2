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

    }
    ;

};
std::vector<int> expansions;

int Box::oppositeDir[5] = { 2, 3, 0, 1, 4 };

bool hasIntersectionLR(double xs, double ys, double xe, double ye, double xleft,
        double ytop, double xr, double yb);
void getExpandedLine(Wall* wall, double width, double height, double& newSrcX,
        double& newSrcY, double& newDstX, double& newDstY);
AngleRange calcAngleRangeCB(double l, double t, double xc, double yc, Box* b);
double getSeparation(double xa, double ya, double xb, double yb, double x,
        double y);
vector<Point> calcIntersectionCW(double l, double xc, double yc, Wall* wall,
        bool& checkSrc, bool& checkDst);
double calcAngle(double srcX, double srcY, double dstX, double dstY);
AngleRange calcAngleRange(map<double, double> angles, double l, double t);
vector<AngleRange> calcZone(vector<AngleRange>& srcAngleRanges);
vector<AngleRange> calcOppoZone(vector<AngleRange>& srcAngleRanges);
double getPointDistance(double x1, double y1, double x2, double y2);
void calcOppoAngle(double& angle);

//void Box::getRoundTriVerts(double& v01x, double& v01y, double& v02x,
//		double& v02y, double& v11x, double& v11y, double& v12x, double& v12y,
//		double& v21x, double& v21y, double& v22x, double& v22y) {
//	double r = r0;
////	double theta1 = triRobo[0];
////	double theta2 = triRobo[1];
//	double theta1 = 0.833333333;
//	double theta2 = 1.0;
//	double xi1 = this->xi[0];
//	double xi2 = this->xi[1];
//
//	v01x = r * cos(xi1 * PI);
//	v01y = r * sin(xi1 * PI);
//	v02x = r * cos(xi2 * PI);
//	v02y = r * sin(xi2 * PI);
//
//	v11x = r * cos((xi1 + theta1) * PI);
//	v11y = r * sin((xi1 + theta1) * PI);
//	v12x = r * cos((xi2 + theta1) * PI);
//	v12y = r * sin((xi2 + theta1) * PI);
//
//	v21x = r * cos((xi1 + theta2) * PI);
//	v21y = r * sin((xi1 + theta2) * PI);
//	v22x = r * cos((xi2 + theta2) * PI);
//	v22y = r * sin((xi2 + theta2) * PI);
//}

bool Box::split2D(double epsilon, vector<Box*>& chldn) {
    shouldSplit2D = false;
//	//todo do sth to return false
//	if (this->height < epsilon || this->width < epsilon) {
////		std::cout << "split2D 94" << endl;
//		return 0;
//	}

    if (!this->isLeaf || this->status == FREE || this->status == STUCK) {

        return 0;
    }

//	std::cout << "split2D" << endl;
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
        for (vector<Box*>::iterator iter = this->Nhbrs[i].begin();
                iter != this->Nhbrs[i].end(); ++iter) {
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
    //list<Box*>::iterator it = find(pAllLeaf->begin(), pAllLeaf->end(), this);
    //pAllLeaf->erase(it);

    return true;
}

//bool Box::split3D(double epsilon, vector<Box*>& chldn) {
//	vector<Box*> bv;
//	if (this->splitAngle(epsilon, bv)) {
//		for (int i = 0; i < 2; ++i) {
//			if (!bv[i]->split2D(epsilon, chldn)) {
//				return false;
//			}
//		}
//		return true;
//	}
//	return 0;
//}

bool Box::splitAngle(double epsilon, vector<Box*>& chldn) {

//	std::cout << "Box::splitAngle begin" << endl;
//	std::cout << "box  " << this->x << "  " << this->y << endl;

    //extern vector<Polygon> polygons;
    //extern vector<int> srcInPolygons;
//	cout << "polygons.size() " << polygons.size() << endl;
//	for (unsigned int i = 0; i < polygons.size(); i++) {
////		cout << "polygons.size() " << polygons.size() << endl;
//		if (pointInPolygon(x - width / 2, y - width / 2, polygons[i])
//				!= srcInPolygons[i]) {
//			this->status = STUCK;
//			return 1;
//		}
//		if (pointInPolygon(x - width / 2, y + width / 2, polygons[i])
//				!= srcInPolygons[i]) {
//			this->status = STUCK;
//			return 1;
//		}
//		if (pointInPolygon(x + width / 2, y - width / 2, polygons[i])
//				!= srcInPolygons[i]) {
//			this->status = STUCK;
//			return 1;
//		}
//		if (pointInPolygon(x + width / 2, y + width / 2, polygons[i])
//				!= srcInPolygons[i]) {
//			this->status = STUCK;
//			return 1;
//		}
//	}

//	//todo do sth to return false
//	for (int i = 0; i < 3; i += 2) {
//		if (this->xi[i + 1] > this->xi[i]) {
//			//todo, 0.028 is somehow random
//			if ((this->xi[i + 1] - this->xi[i]) * PI < 0.014) {
//				return 0;
//			}
//		} else {
//			if ((2 + this->xi[i + 1] - this->xi[i]) * PI < 0.014) {
//				return 0;
//			}
//		}
//	}

    //if (!this->isLeaf || this->status != MIXED)
    if (!this->isLeaf || this->status == FREE || this->status == STUCK) {

        return 0;
    }

    vector<AngleRange> l1AngleRanges;
    vector<AngleRange> l2AngleRanges;
    vector<AngleRange> l1ForbidenZone;
    vector<AngleRange> l2ForbidenZone;
    vector<AngleRange> l1SafeZone;
    vector<AngleRange> l2SafeZone;
    for (list<Wall*>::iterator itW = walls.begin(); itW != walls.end(); itW++) {
//		std::cout << "walls.size()=" << walls.size() << endl;

        Wall* w = *itW;

//		std::cout << "walls =" << w->src->x << " " << w->src->y << " "
//				<< w->dst->x << " " << w->dst->y << endl;
//		double distWall = w->distance(this->x, this->y);
        // when wall intersects with box
        if (hasIntersectionLR((double) w->src->x, (double) w->src->y,
                (double) w->dst->x, (double) w->dst->y,
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

        getExpandedLine(w, BOARDWIDTH, BOARDHEIGHT, newSrcX, newSrcY, newDstX,
                newDstY);
        if (hasIntersectionLR(newSrcX, newSrcY, newDstX, newDstY,
                this->x - this->width / 2, this->y + this->height / 2,
                this->x + this->width / 2, this->y - this->height / 2)) {
//			std::cout << "hasIntersectionLR == true" << endl;

            // find the closer endpoint
            if (min(this->x - this->width / 2, this->x + this->width / 2)
                    > max(w->src->x, w->dst->x)) {
                if (w->src->x > w->dst->x) {
                    closerX = w->src->x;
                    closerY = w->src->y;
                } else {
                    closerX = w->dst->x;
                    closerY = w->dst->y;
                }
            }

            if (min(this->y - this->height / 2, this->y + this->height / 2)
                    > max(w->src->y, w->dst->y)) {
                if (w->src->y > w->dst->y) {
                    closerX = w->src->x;
                    closerY = w->src->y;
                } else {
                    closerX = w->dst->x;
                    closerY = w->dst->y;
                }
            }

            if (max(this->x - this->width / 2, this->x + this->width / 2)
                    < min(w->src->x, w->dst->x)) {
                if (w->src->x < w->dst->x) {
                    closerX = w->src->x;
                    closerY = w->src->y;
                } else {
                    closerX = w->dst->x;
                    closerY = w->dst->y;
                }
            }

            if (max(this->y - this->height / 2, this->y + this->height / 2)
                    < min(w->src->y, w->dst->y)) {
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
                AngleRange tempAngleRange = calcAngleRangeCB(tempL, thickness,
                        closerX, closerY, this);
//				std::cout << "l1 ==" << l1 << endl;
//				std::cout << "l2 ==" << l2 << endl;
//				std::cout << "tempAngleRangel" << i + 1 << " lowerbound ="
//						<< tempAngleRange.lowerBound << " upperbound ="
//						<< tempAngleRange.upperBound << endl;

                if (tempAngleRange.lowerBound != 0
                        || tempAngleRange.upperBound != 0) {
                    if (i == 0) {
                        l1AngleRanges.push_back(tempAngleRange);
                    } else {
                        l2AngleRanges.push_back(tempAngleRange);
                    }
                }
            }
        } else {

//			std::cout << "hasIntersectionLR == false" << endl;

            double closestCornerX = 0;
            double closestCornerY = 0;
            double tempSeparation = FLT_MAX;
            // find the closest corner of box to the wall
            double temp = getSeparation((double) w->src->x, (double) w->src->y,
                    (double) w->dst->x, (double) w->dst->y,
                    this->x - this->width / 2, this->y - this->height / 2);
            if (temp <= tempSeparation) {
                tempSeparation = temp;
                closestCornerX = this->x - this->width / 2;
                closestCornerY = this->y - this->height / 2;
            }

            temp = getSeparation((double) w->src->x, (double) w->src->y,
                    (double) w->dst->x, (double) w->dst->y,
                    this->x - this->width / 2, this->y + this->height / 2);
            if (temp <= tempSeparation) {
                tempSeparation = temp;
                closestCornerX = this->x - this->width / 2;
                closestCornerY = this->y + this->height / 2;
            }

            temp = getSeparation((double) w->src->x, (double) w->src->y,
                    (double) w->dst->x, (double) w->dst->y,
                    this->x + this->width / 2, this->y - this->height / 2);
            if (temp <= tempSeparation) {
                tempSeparation = temp;
                closestCornerX = this->x + this->width / 2;
                closestCornerY = this->y - this->height / 2;
            }

            temp = getSeparation((double) w->src->x, (double) w->src->y,
                    (double) w->dst->x, (double) w->dst->y,
                    this->x + this->width / 2, this->y + this->height / 2);
            if (temp <= tempSeparation) {
                tempSeparation = temp;
                closestCornerX = this->x + this->width / 2;
                closestCornerY = this->y + this->height / 2;
            }

//			std::cout << "closestCorner= " << closestCornerX << " "
//					<< closestCornerY << endl;

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
                    tempIntersection = calcIntersectionCW(tempL, closestCornerX,
                            closestCornerY, w, checkSrc, checkDst);
                    //TODO modified by Zhongdi 08/22/2013, we should always check both endpoints
                    if (checkSrc) {
                        Point tempPoint(w->src->x, w->src->y);
                        tempIntersection.push_back(tempPoint);
//						std::cout << "checkSrc x= " << w->src->x << " y="
//								<< w->src->y << endl;
                    }
                    //TODO modified by Zhongdi 08/22/2013, we should always check both endpoints
                    if (checkDst) {
                        Point tempPoint(w->dst->x, w->dst->y);
                        tempIntersection.push_back(tempPoint);
//						std::cout << "checkDst x= " << w->dst->x << " y="
//								<< w->dst->y << endl;
                    }

                    AngleRange tempAngleRange(0, 0);
                    map<double, double> tempAngles;
                    for (vector<Point>::iterator itP = tempIntersection.begin();
                            itP != tempIntersection.end(); itP++) {
                        Point temp = *itP;
                        double tempAngle = calcAngle(closestCornerX,
                                closestCornerY, temp.x, temp.y);
                        tempAngles.insert(
                                pair<double, double>(tempAngle,
                                        getPointDistance(closestCornerX,
                                                closestCornerY, temp.x,
                                                temp.y)));
//						std::cout << "tempAngle= " << tempAngle << " tempL="
//								<< tempL << endl;
                    }
                    tempAngleRange = calcAngleRange(tempAngles, tempL,
                            thickness);
//					std::cout << "tempAngleRange= " << tempAngleRange.lowerBound
//							<< " " << tempAngleRange.upperBound << endl;
                    if (tempAngleRange.lowerBound != 0
                            || tempAngleRange.upperBound != 0) {
                        if (i == 0) {
                            l1AngleRanges.push_back(tempAngleRange);
                        } else {
                            l2AngleRanges.push_back(tempAngleRange);
                        }
                    }
                }
                //TODO modified by Zhongdi 08/22/2013, we should always check both endpoints
                if (true || checkSrc) {
//					std::cout << "checkSrc == true " << endl;
                    AngleRange tempAngleRange = calcAngleRangeCB(tempL,
                            thickness, (double) w->src->x, (double) w->src->y,
                            this);
//					std::cout << "checkSrc tempAngleRange= " << tempAngleRange.lowerBound
//												<< " " << tempAngleRange.upperBound << endl;

                    if (tempAngleRange.lowerBound != 0
                            || tempAngleRange.upperBound != 0) {
                        if (i == 0) {
                            l1AngleRanges.push_back(tempAngleRange);
                        } else {
                            l2AngleRanges.push_back(tempAngleRange);
                        }
                    }
                }
                //TODO modified by Zhongdi 08/22/2013, we should always check both endpoints
                if (true || checkDst) {
//					std::cout << "checkDst == true " << endl;
                    AngleRange tempAngleRange = calcAngleRangeCB(tempL,
                            thickness, (double) w->dst->x, (double) w->dst->y,
                            this);

//					std::cout << "checkDst tempAngleRange= " << tempAngleRange.lowerBound
//							<< " " << tempAngleRange.upperBound << endl;

                    if (tempAngleRange.lowerBound != 0
                            || tempAngleRange.upperBound != 0) {
                        if (i == 0) {
                            l1AngleRanges.push_back(tempAngleRange);
                        } else {
                            l2AngleRanges.push_back(tempAngleRange);
                        }
                    }
                }
            }
        }
    }
    l1ForbidenZone = calcZone(l1AngleRanges);
//	std::cout << "l1ForbidenZone.size= " << l1ForbidenZone.size() << endl;
//	std::cout << "l1ForbidenZone[0]= " << l1ForbidenZone[0].lowerBound << " "
//			<< l1ForbidenZone[0].upperBound << endl;
    l2ForbidenZone = calcZone(l2AngleRanges);
//	std::cout << "l2ForbidenZone.size= " << l2ForbidenZone.size() << endl;
    l1SafeZone = calcOppoZone(l1ForbidenZone);
//	std::cout << "l1SafeZone[0]= " << l1SafeZone[0].lowerBound << " "
//			<< l1SafeZone[0].upperBound << endl;
    l2SafeZone = calcOppoZone(l2ForbidenZone);
//	std::cout << "l1SafeZone[0]= " << l1SafeZone[0].lowerBound << " "
//				<< l1SafeZone[0].upperBound << endl;

//	std::cout << "Box::splitAngle safezone calculated" << endl;
    if (twoStrategyOption == 2) {
        if (l1SafeZone.size() <= l2SafeZone.size()) {
            safeRanges = l2SafeZone.size();
//			std::cout << "l2" << l2SafeZone.size()<< endl;
        } else {
            safeRanges = l1SafeZone.size();
//			std::cout << "l1" << l1SafeZone.size()<< endl;
        }
    }

    if (l1SafeZone.empty() || l2SafeZone.empty()) {
        this->status = STUCK;
        return 0;
    }

    this->isLeaf = false;
//	vector<Box*> children;
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
//				it = l1SafeZone.erase(it);

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
                //				it = l1SafeZone.erase(it);

            } else {
                if (it->upperBound - it->lowerBound > 0.01) {
                    AngleRange tempRange(it->lowerBound, it->upperBound);
                    tempZone2.push_back(tempRange);
                }
            }
        }
        l2SafeZone = tempZone2;
    }

//	double oldL1LowerBound = -1;
//	double oldL1UpperBound = -1;
//	double oldL2LowerBound = -1;
//	double oldL2UpperBound = -1;

    for (int i = 0; i < (int) l1SafeZone.size(); i++) {
        for (int j = 0; j < (int) l2SafeZone.size(); j++) {
            for (int l = 0; l < crossingOption + 1; l++) {

                Box* child = new Box(x, y, width, height);
//					child->depth = this->depth + 1;
                child->isBig = false;
                child->pParent = this;
                child->rChildID = i * j * l + j * l + l;
                child->status = FREE;

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
//						if (angleDistance(child->xi[1], child->xi[2])
//								< bandwidth
//								&& abs(child->xi[1] - child->xi[2]) > 180
//								&& child->xi[1] < child->xi[2])
                        if(child->xi[2] - child->xi[1] >= 360 - bandwidth)
                        {
//							cout << child->xi[1] << " " << child->xi[2] << endl;
                            continue;
                        }
                    } else {
                        child->order = GT;
                        if (child->xi[2] + bandwidth >= child->xi[1]) {
                            continue;
                        }
//						if (angleDistance(child->xi[0], child->xi[3])
//								< bandwidth
//								&& abs(child->xi[0] - child->xi[3]) > 180
//								&& child->xi[0] > child->xi[3])
                        if(child->xi[0] - child->xi[3] >= 360 - bandwidth)
                        {
                            continue;
                        }
                    }
                    for (vector<Box*>::iterator it = chldn.begin();
                            it != chldn.end(); it++) {
                        if ((*it)->xi[1] == 360 && child->xi[0] == 0
                                && (*it)->xi[2] == child->xi[2]
                                && (*it)->xi[3] == child->xi[3]
                                && (*it)->order != child->order) {
//							if (child->Nhbrs[4].size() == 0) {
                            child->Nhbrs[4].push_back(*it);
//							}
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
//							if (child->Nhbrs[4].size() == 0) {
                            child->Nhbrs[4].push_back(*it);
//							}
                            if (std::find((*it)->Nhbrs[4].begin(),
                                    (*it)->Nhbrs[4].end(), child)
                                    == (*it)->Nhbrs[4].end()) {
                                (*it)->Nhbrs[4].push_back(child);
                            }
                        }
                    }
                }
//			this->status = FREE;

//			std::cout << "Box::splitAngle i=" << i << endl;
//			std::cout << "Box::splitAngle j=" << j << endl;
//			std::cout << "Box::splitAngle xi=" << child->xi[0] << " "
//					<< child->xi[1] << " " << child->xi[2] << " "
//					<< child->xi[3] << endl;

//			children.push_back(child);

                for (int k = 0; k < 4; ++k) {
                    for (vector<Box*>::iterator itBNhbrs =
                            this->Nhbrs[k].begin();
                            itBNhbrs != this->Nhbrs[k].end(); ++itBNhbrs) {
                        Box* b = *itBNhbrs;
//					int foundDst = 0;
//					for (int j = 0; j < 2; ++j) {
                        int idx = isNhbr(child, b);
                        if (idx != -1) {
                            child->Nhbrs[idx].push_back(b);
//						if (foundDst == 0) {
                            //TODO commented by Zhongdi 08/27/2013
//						for (vector<Box*>::iterator itBNhbrsOppo =
//								b->Nhbrs[oppositeDir[idx]].begin();
//								itBNhbrsOppo != b->Nhbrs[oppositeDir[idx]].end();
//								++itBNhbrsOppo) {
//							if (*itBNhbrsOppo == this) {
//								*itBNhbrsOppo = child;
//								//TODO deleted by Zhongdi 08/27/2013 need to further check this line when using smarter strategy
////								b->Nhbrs[oppositeDir[idx]].erase(itBNhbrsOppo);
//								break;
//							}
//						}

//						} else {
                            b->Nhbrs[oppositeDir[idx]].push_back(child);
//						}
//						++foundDst;
                        }
//					}
//					assert(foundDst > 0 && foundDst < 3);
                    }
                }

                pAllLeaf->push_back(child);
                expansions.push_back(pAllLeaf->size());
                chldn.push_back(child);
            }
//			oldL2LowerBound = l2SafeZone[i].lowerBound;
//			oldL2UpperBound = l2SafeZone[i].upperBound;
        }
//		oldL1LowerBound = l1SafeZone[i].lowerBound;
//		oldL1UpperBound = l1SafeZone[i].upperBound;
    }

//	Box* children[2];
//	children[1] = new Box(x, y, width, height);
//	children[0] = new Box(x, y, width, height);
//	for (int i = 0; i < 2; ++i) {
//		//children[i]->depth = this->depth + 1;
//		children[i]->isBig = false;
//		children[i]->pParent = this;
//		children[i]->rChildID = i;
//	}
////todo, 0(2) is within [xi[0], xi[1]] shouldn't happen
//	if (this->xi[1] - this->xi[0] >= this->xi[3] - this->xi[2]) {
//
//		children[0]->xi[0] = this->xi[0];
//		children[0]->xi[1] = this->xi[0] + (this->xi[1] - this->xi[0]) / 2;
//		children[0]->xi[2] = this->xi[2];
//		children[0]->xi[3] = this->xi[3];
//
//		children[1]->xi[0] = this->xi[0] + (this->xi[1] - this->xi[0]) / 2;
//		children[1]->xi[1] = this->xi[1];
//		children[1]->xi[2] = this->xi[2];
//		children[1]->xi[3] = this->xi[3];
//
//	} else {
//		children[0]->xi[0] = this->xi[0];
//		children[0]->xi[1] = this->xi[1];
//		children[0]->xi[2] = this->xi[2];
//		children[0]->xi[3] = this->xi[2] + (this->xi[3] - this->xi[2]) / 2;
//
//		children[1]->xi[0] = this->xi[0];
//		children[1]->xi[1] = this->xi[1];
//		children[1]->xi[2] = this->xi[2] + (this->xi[3] - this->xi[2]) / 2;
//		children[1]->xi[3] = this->xi[3];
//	}
//
//////simple case
////	children[0]->Nhbrs[4].push_back(children[1]);
////	children[1]->Nhbrs[5].push_back(children[0]);
//
//	for (int i = 0; i < 6; ++i) {
//		for (vector<Box*>::iterator iter = this->Nhbrs[i].begin();
//				iter != this->Nhbrs[i].end(); ++iter) {
//			Box* b = *iter;
//			int foundDst = 0;
//			for (int j = 0; j < 2; ++j) {
//				int idx = isNhbr(children[j], b);
//				if (idx != -1) {
//					children[j]->Nhbrs[idx].push_back(b);
//					if (foundDst == 0) {
//						for (vector<Box*>::iterator it =
//								b->Nhbrs[oppositeDir[idx]].begin();
//								it != b->Nhbrs[oppositeDir[idx]].end(); ++it) {
//							if (*it == this) {
//								*it = children[j];
//								break;
//							}
//						}
//					} else {
//						b->Nhbrs[oppositeDir[idx]].push_back(children[j]);
//					}
//					++foundDst;
//				}
//			}
//			assert(foundDst > 0 && foundDst < 3);
//		}
//	}
//
//	for (int i = 0; i < 2; ++i) {
//		//add all of parent's walls and corners to each child,
//		//will be filtered later in updatestatus()
//		children[i]->walls.insert(children[i]->walls.begin(),
//				this->walls.begin(), this->walls.end());
//		children[i]->corners.insert(children[i]->corners.begin(),
//				this->corners.begin(), this->corners.end());
//
//		children[i]->vorWalls.insert(children[i]->vorWalls.begin(),
//				this->vorWalls.begin(), this->vorWalls.end());
//		children[i]->vorCorners.insert(children[i]->vorCorners.begin(),
//				this->vorCorners.begin(), this->vorCorners.end());
//	}

//	this->isLeaf = false;
//
//	for (int i = 0; i < 2; ++i) {
//		pAllLeaf->push_back(children[i]);
//		chldn.push_back(children[i]);
//	}
//list<Box*>::iterator it = find(pAllLeaf->begin(), pAllLeaf->end(), this);
//pAllLeaf->erase(it);

//	std::cout << "Box::splitAngle end" << endl;
    return true;
}

bool Box::split(double epsilon, vector<Box*>& chldn) {
    if ((this->height / 2 < epsilon && this->width / 2 < epsilon)
            || (twoStrategyOption > 0
                    && (int) walls.size() + (int) corners.size() <= sizeOfPhiB
                    && !shouldSplit2D)) {

//		std::cout << "splitangle" << endl;
        if (this->height / 2 < epsilon && this->width / 2 < epsilon) {
//			std::cout << "split 603" << endl;
            return splitAngle(epsilon, chldn);

        } else {
            QT->PQ->push(this);
//			if(!PQ->empty()){
//				std::cout  << endl;
//			}

//			std::cout << "split 711" << endl;
            bool result = splitAngle(epsilon, chldn);

            isLeaf = true;
            status = MIXED;
//			if(this->height / 2 < epsilon && this->width / 2 <epsilon){
//				shouldSplit2D = false;
//			}else{
            shouldSplit2D = true;
//			}
//			visited = false;
            dist2Source = -1;
//			std::cout << "split 719" << endl;
//			std::cout << "split 720" << endl;
            return result;

        }

    }
//	else if (rB / 2 < r0 * 2 / THETA_MIN / 20) {
//		//int n = ceil( 2 / THETA_MIN );
//		//int m = 1;
//		//for (int i = 0; i < n; ++i)
//		//{
//		//	splitAngle(epsilon);
//		//}
//		//return splitAngle(epsilon);
//
//		int n = ceil(2 / THETA_MIN);
//		//n = 64;
//		int m = 1;
//		recursiveSplitAngle(epsilon, chldn, n, m);
//		return true;
//	}
    else {
//		std::cout << "split 741" << endl;
        return split2D(epsilon, chldn);
    }
}

//void Box::recursiveSplitAngle(double epsilon, vector<Box*>& chldn, const int n,
//		int m) {
//	if (m >= n) {
//		chldn.push_back(this);
//		return;
//	}
//	vector<Box*> bv;
//	splitAngle(epsilon, bv);
//	for (int i = 0; i < (int) bv.size(); ++i) {
//		bv[i]->recursiveSplitAngle(epsilon, chldn, n, 2 * m);
//	}
//}

Box::Status Box::checkChildStatus(double x, double y, int width, bool small) {
//assert(walls.size());

    Wall* nearestWall;
    list<Wall*>::iterator iterW = walls.begin();
//	cout<<"checkChildStatus 711"<<endl;
//	cout<<"checkChildStatus 712 "<< walls.size()<<" "<<corners.size()<< endl;
//	cout<<"checkChildStatus 713 "<<
    double mindistW = (*iterW)->distance(x, y);
//	cout<<"checkChildStatus 713"<< "  mindistW = "<< mindistW<<endl;
    nearestWall = *iterW;
    ++iterW;
    for (; iterW != walls.end(); ++iterW) {
//		cout<<"checkChildStatus 715"<<endl;
        Wall* w = *iterW;
        double dist = w->distance(x, y);
        if (dist < mindistW) {
            mindistW = dist;
            nearestWall = *iterW;
        }
    }
//	cout<<"checkChildStatus 722"<<endl;
    double mindistC = mindistW + 1;	//mindistC may not exist, so init to a bigger number
    Corner* nearestCorner = NULL;
    if (corners.size()) {
        list<Corner*>::iterator iterC = corners.begin();
        mindistC = (*iterC)->distance(x, y);
        nearestCorner = *iterC;
        ++iterC;
        for (; iterC != corners.end(); ++iterC) {
            Corner* c = *iterC;
            double dist = c->distance(x, y);
            if (dist < mindistC) {
                mindistC = dist;
                nearestCorner = *iterC;
            }
        }
    }

    Status tempStatus = UNKNOWN;
//	cout<<"checkChildStatus 741"<<endl;
//nearest feature is a wall
    if (mindistW < mindistC) {
//		std::cout << "checkChildStatus 686" << endl;
//		std::cout << "checkChildStatus nearestWall " << nearestWall->src->x
//				<< " " << nearestWall->src->y << " " << nearestWall->dst->x
//				<< " " << nearestWall->dst->y << endl;
        if (nearestWall->isRight(x, y)
                && mindistW > r0 + rB / 2 + thickness / 2) {
//			std::cout << "checkChildStatus 688" << endl;
            tempStatus = FREE;
            return tempStatus;
        } else if (!nearestWall->isRight(x, y)
                && mindistW > rB / 2 - thickness / 2) {
            tempStatus = STUCK;
            return tempStatus;
        }
        if (!nearestWall->isRight(x + width / 2, y + width / 2)
                && !nearestWall->isRight(x - width / 2, y + width / 2)
                && !nearestWall->isRight(x + width / 2, y - width / 2)
                && !nearestWall->isRight(x - width / 2, y - width / 2)) {
            tempStatus = STUCK;
            return tempStatus;
        }

    }
//otherwise check the corner's convexity
//if convex, out; if concave, in
//note that if a wall and a corner are the same dist,
//only need to take care of the corner
    else {
//		std::cout << "checkChildStatus 695" << endl;
//		if (nearestCorner->isConvex() && mindistC > r0+ rB) {
//			tempStatus = FREE;
//		}

        if (nearestCorner->isConvex()
                && mindistC > r0 + rB / 2 + thickness / 2) {
            tempStatus = FREE;
        } else if (!nearestCorner->isConvex()
                && mindistC > rB / 2 - thickness / 2) {
            tempStatus = STUCK;
        }

//		else if(!nearestCorner->isConvex() && mindistC > rB){
//			tempStatus = STUCK;
//		}
    }

//	cout<<"polygons.size() "<<polygons.size()<<endl;
//	if (small) {
//		//extern vector<Polygon> polygons;
//		//extern vector<int> srcInPolygons;
//		cout<<"polygons.size() "<<polygons.size()<<endl;
//		for (int i = 0; i < polygons.size(); i++) {
//			cout<<"polygons.size() "<<polygons.size()<<endl;
//			if (pointInPolygon(x - width / 2, y - width / 2, polygons[i])
//					!= srcInPolygons[i]) {
//				isFree = false;
//				break;
//			}
//			if (pointInPolygon(x - width / 2, y + width / 2, polygons[i])
//					!= srcInPolygons[i]) {
//				isFree = false;
//				break;
//			}
//			if (pointInPolygon(x + width / 2, y - width / 2, polygons[i])
//					!= srcInPolygons[i]) {
//				isFree = false;
//				break;
//			}
//			if (pointInPolygon(x + width / 2, y + width / 2, polygons[i])
//					!= srcInPolygons[i]) {
//				isFree = false;
//				break;
//			}
//		}
//	}

//	if (isFree) {
//		return FREE;
//	}
////	std::cout << "checkChildStatus 703" << endl;
//	return STUCK;
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

    if (!isArcIntsct(b1->xi[0], b1->xi[1], b2->xi[0], b2->xi[1])
            || !isArcIntsct(b1->xi[2], b1->xi[3], b2->xi[2], b2->xi[3])) {
        return -1;
    }
    if (isEq(xmin1, xmax2) && isOneContainAnotherY(b1, b2)
            && (!crossingOption || (b1->order == b2->order && b1->order != NON)
                    || b1->order == ALL || b2->order == ALL)) {
        return 2;
    }
    if (isEq(xmax1, xmin2) && isOneContainAnotherY(b1, b2)
            && (!crossingOption || (b1->order == b2->order && b1->order != NON)
                    || b1->order == ALL || b2->order == ALL)) {
        return 0;
    }
    if (isEq(ymin1, ymax2) && isOneContainAnotherX(b1, b2)
            && (!crossingOption || (b1->order == b2->order && b1->order != NON)
                    || b1->order == ALL || b2->order == ALL)) {
        return 3;
    }
    if (isEq(ymax1, ymin2) && isOneContainAnotherX(b1, b2)
            && (!crossingOption || (b1->order == b2->order && b1->order != NON)
                    || b1->order == ALL || b2->order == ALL)) {
        return 1;
    }
    // for case 4, we are not using this function to check the connectivity. We maintain the connectivity just after the angler splitting

//	//down
//	if (isEq(b1->xi[0], b2->xi[1]) && isOneContainAnotherX(b1, b2)
//			&& isOneContainAnotherY(b1, b2)) {
//		return 5;
//	}
//	//up
//	if (isEq(b1->xi[1], b2->xi[0]) && isOneContainAnotherX(b1, b2)
//			&& isOneContainAnotherY(b1, b2)) {
//		return 4;
//	}

    return -1;
}

double angleDistance(double a1, double a2) {
    return fmin(abs(a1 - a2), 360 - abs(a1 - a2));
}

void Box::updateStatusBig() {
    if (status != UNKNOWN) {
        return;
    }
//	cout<<"updateStatusBig 872"<<endl;
    if (pParent) {
        status = pParent->checkChildStatus(this->x, this->y, this->width,
                false);
        //if (status == FREE) {
        //	order = ALL;
        //}
    } else {
        status = MIXED;
    }
//	cout<<"updateStatusBig 879"<<endl;
    if (status == STUCK || status == FREE) {
        return;
    }
//	cout<<"updateStatusBig 881"<<endl;
// TODO should add the clearance filter to determine a box stuck or free directly. this part is unnecessary.
    double outerDomain = r0 + rB + thickness / 2;
    double innerDomain = thickness / 2 > rB ? thickness / 2 - rB : 0;
    for (list<Corner*>::iterator it = corners.begin(); it != corners.end();) {
        Corner* c = *it;
//		if (c->x < this->x + this->width / 2 && c->x > this->x - this->width / 2
//				&& c->y < this->y + this->height / 2
//				&& c->y > this->y - this->height / 2) {
//			status = MIXED;
////			return;
//			++it;
//		} else

        if (c->distance(this->x, this->y) <= outerDomain) {
            status = MIXED;
            ++it;
        } else if (c->distance(this->x, this->y) < innerDomain) {
            status = STUCK;
            ++it;
        } else {
            it = corners.erase(it);
        }
    }
//	cout<<"updateStatusBig 902"<<endl;
    for (list<Wall*>::iterator it = walls.begin(); it != walls.end();) {
        Wall* w = *it;
        double distWall = w->distance(this->x, this->y);

//		if (hasIntersectionLR((double) w->src->x, (double) w->src->y,
//				(double) w->dst->x, (double) w->dst->y,
//				this->x - this->width / 2, this->y + this->height / 2,
//				this->x + this->width / 2, this->y - this->height / 2)) {
//			status = MIXED;
//			++it;
////			return;
//		} else

        if (distWall <= outerDomain) {
            status = MIXED;
            ++it;
        } else if (distWall < innerDomain) {
            status = STUCK;
            ++it;
        } else {
            it = walls.erase(it);
        }
    }

//	if (corners.size() == 0 && walls.size() == 0) {
//		if (!pParent) {
//			status = FREE;
//		} else {
////			std::cout << "updateStatusBig 791" << endl;
//			status = pParent->checkChildStatus(this->x, this->y, this->width,
//					false);
////			std::cout << "updateStatusBig 793  " << status << endl;
//		}
//	}
}

void Box::updateStatusSmall() {
    if (status != UNKNOWN) {
        return;
    } else {
        cout << "updateStatusSmall 942942" << endl;
    }

//	double v01x, v01y, v02x, v02y, v11x, v11y, v12x, v12y, v21x, v21y, v22x,
//			v22y;
//
//	getRoundTriVerts(v01x, v01y, v02x, v02y, v11x, v11y, v12x, v12y, v21x, v21y,
//			v22x, v22y);
//
//	Line2d L1(v02x, v02y, v11x, v11y);
//	Line2d L2(v12x, v12y, v21x, v21y);
//	Line2d L3(v22x, v22y, v01x, v01y);
//	bool expandSuccess = L1.expand(rB, L2, L3) && L2.expand(rB, L1, L3)
//			&& L3.expand(rB, L1, L2) && !L1.isNegative(L2, L3);
//	assert(expandSuccess);
//
//	Line2d L1a(v01x, v01y, v12x, v12y);
//	Line2d L2a(v11x, v11y, v22x, v22y);
//	Line2d L3a(v21x, v21y, v02x, v02y);
//	bool shrinkSuccess = L1a.expand(-rB, L2a, L3a) && L2a.expand(-rB, L1a, L3a)
//			&& L3a.expand(-rB, L1a, L2a) && !L1a.isNegative(L2a, L3a);
//
//	double x23, y23;
//	L2a.intersection(L3a, x23, y23);
//
//	double X12x, X12y;
//	L1.intersection(L2, X12x, X12y);
//	double X23x, X23y;
//	L2.intersection(L3, X23x, X23y);
//	double X31x, X31y;
//	L3.intersection(L1, X31x, X31y);

//	//TODO consider the conners or not
//	for (list<Corner*>::iterator it = corners.begin(); it != corners.end();) {
//		Corner* c = *it;
//		double cx = c->x - this->x;
//		double cy = c->y - this->y;
//		if (c->x < this->x + this->width / 2 && c->x > this->x - this->width / 2
//				&& c->y < this->y + this->height / 2
//				&& c->y > this->y - this->height / 2) {
//			status = STUCK;
//			return;
//		}

//
//		if (shrinkSuccess && !L1a.isRight(cx, cy) && !L2a.isRight(cx, cy)
//				&& !L3a.isRight(cx, cy)) {
//			status = STUCK;
//			return;
//		}
//
//		if (!(sqrt(cx * cx + cy * cy) < r0 + rB
//				&& !Line2d::isRight(X31x, X31y, X12x, X12y, cx, cy)
//				&& !Line2d::isRight(X12x, X12y, X23x, X23y, cx, cy)
//				&& !Line2d::isRight(X23x, X23y, X31x, X31y, cx, cy))) {
//			it = corners.erase(it);
//		} else {
//			status = MIXED;
//			++it;
//		}

//	}

//		if (distWall < r0 + rB) {
//			double srcx = w->src->x - this->x;
//			double srcy = w->src->y - this->y;
//			double dstx = w->dst->x - this->x;
//			double dsty = w->dst->y - this->y;
//
//			//quick stuck detection
//			if (shrinkSuccess) //inner triangle (before shrink rB) is not negative
//			{
//				double x23s, y23s;
//				L2a.intersection(L3a, x23s, y23s);
//				double x13s, y13s;
//				L1a.intersection(L3a, x13s, y13s);
//				double x12s, y12s;
//				L1a.intersection(L2a, x12s, y12s);
//
//				//if src or dst is in triangle
//				if ((!L1a.isRight(srcx, srcy) && !L2a.isRight(srcx, srcy)
//						&& !L3a.isRight(srcx, srcy))
//						|| (!L1a.isRight(dstx, dsty) && !L2a.isRight(dstx, dsty)
//								&& !L3a.isRight(dstx, dsty))) {
//					status = STUCK;
//					return;
//				}
//				// or line seg (src,dst) intersects any edge of triangle
//				else if (Line2d::lineSegIntsct(x23s, y23s, x13s, y13s, srcx,
//						srcy, dstx, dsty)
//						|| Line2d::lineSegIntsct(x13s, y13s, x12s, y12s, srcx,
//								srcy, dstx, dsty)
//						|| Line2d::lineSegIntsct(x12s, y12s, x23s, y23s, srcx,
//								srcy, dstx, dsty)) {
//					status = STUCK;
//					return;
//				}
//			}
//
//			//if src or dst is in triangle
//			if ((!Line2d::isRight(X31x, X31y, X12x, X12y, srcx, srcy)
//					&& !Line2d::isRight(X12x, X12y, X23x, X23y, srcx, srcy)
//					&& !Line2d::isRight(X23x, X23y, X31x, X31y, srcx, srcy))
//					|| (!Line2d::isRight(X31x, X31y, X12x, X12y, dstx, dsty)
//							&& !Line2d::isRight(X12x, X12y, X23x, X23y, dstx,
//									dsty)
//							&& !Line2d::isRight(X23x, X23y, X31x, X31y, dstx,
//									dsty))) {
//				status = MIXED;
//				++it;
//			}
//			// or line seg (src,dst) intersects any edge of triangle
//			else if (Line2d::lineSegIntsct(X31x, X31y, X12x, X12y, srcx, srcy,
//					dstx, dsty)
//					|| Line2d::lineSegIntsct(X12x, X12y, X23x, X23y, srcx, srcy,
//							dstx, dsty)
//					|| Line2d::lineSegIntsct(X23x, X23y, X31x, X31y, srcx, srcy,
//							dstx, dsty)) {
//				status = MIXED;
//				++it;
//			} else {
//				it = walls.erase(it);
//			}
//		} else {
//			it = walls.erase(it);
//		}

//	if (corners.size() == 0 && walls.size() == 0) {
//		if (!pParent) {
//			status = FREE;
//		} else {
//			status = pParent->checkChildStatus(this->x, this->y, this->width,
//					true);
//		}
//	}
}
// check whether a line segment intersects with a rectangle
bool hasIntersectionLR(double xs, double ys, double xe, double ye, double xleft,
        double ytop, double xr, double yb) {

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

void getExpandedLine(Wall* wall, double boardWidth, double boardHeight,
        double& newSrcX, double& newSrcY, double& newDstX, double& newDstY) {
//	double l = 0;
//	if(width > height){
//		l = width;
//	}else{
//		l = height;
//	}
    double xVariation = wall->dst->x - wall->src->x;

    if (xVariation != 0) {
        double slope = (wall->dst->y - wall->src->y)
                / (wall->dst->x - wall->src->x);
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
vector<Point> calcIntersectionCB(double l, double xc, double yc, double xleft,
        double ytop, double xr, double yb) {
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
//				std::cout << "pointCB l=" << l << " x=" << xTemp << " y=" << y1
//						<< endl;
            }
            if (y2 <= ytop && y2 >= yb) {
                Point intersection(xTemp, y2);
                points.push_back(intersection);
//				std::cout << "pointCB l=" << l << " x=" << xTemp << " y=" << y2
//						<< endl;
            }
        }
//		// intersect at 1 point
//		else if (temp1 < 0.0001 && temp1 >= -0.0001) {
//			Point intersection(xTemp, yc);
//			points.push_back(intersection);
//		}
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
//				std::cout << "pointCB l=" << l << " x=" << x1 << " y=" << yTemp
//						<< endl;
            }
            if (x2 <= xr && x2 >= xleft) {
                Point intersection(x2, yTemp);
                points.push_back(intersection);
//				std::cout << "pointCB l=" << l << " x=" << x2 << " y=" << yTemp
//						<< endl;
            }
        }

//		// intersect at 1 point
//		else if (temp1 < 0.0001 && temp1 >= -0.0001) {
//			Point intersection(xc, yTemp);
//			points.push_back(intersection);
//		}
    }

//	std::cout << "calcIntersectionCB end" << endl;
    return points;
}

AngleRange calcAngleRangeCB(double l, double t, double xc, double yc, Box* b) {
    vector<Point> tempPoints;
    tempPoints = calcIntersectionCB(l, xc, yc, b->x - b->width / 2,
            b->y + b->height / 2, b->x + b->width / 2, b->y - b->height / 2);

    double distance = getPointDistance(xc, yc, b->x - b->width / 2,
            b->y - b->height / 2);
    if (distance <= l) {
        Point tempPoint(b->x - b->width / 2, b->y - b->height / 2);
        tempPoints.push_back(tempPoint);
    }

    distance = getPointDistance(xc, yc, b->x - b->width / 2,
            b->y + b->height / 2);
    if (distance <= l) {
        Point tempPoint(b->x - b->width / 2, b->y + b->height / 2);
        tempPoints.push_back(tempPoint);
    }

    distance = getPointDistance(xc, yc, b->x + b->width / 2,
            b->y - b->height / 2);
    if (distance <= l) {
        Point tempPoint(b->x + b->width / 2, b->y - b->height / 2);
        tempPoints.push_back(tempPoint);
    }

    distance = getPointDistance(xc, yc, b->x + b->width / 2,
            b->y + b->height / 2);
    if (distance <= l) {
        Point tempPoint(b->x + b->width / 2, b->y + b->height / 2);
        tempPoints.push_back(tempPoint);
    }

// calculate the anglerange of the specific wall
    AngleRange tempAngleRange(0, 0);
    map<double, double> tempAngles;
    for (vector<Point>::iterator it = tempPoints.begin();
            it != tempPoints.end(); it++) {
        Point temp = *it;
        double tempAngle = calcAngle(xc, yc, temp.x, temp.y);
        calcOppoAngle(tempAngle);
        tempAngles.insert(
                pair<double, double>(tempAngle,
                        getPointDistance(xc, yc, temp.x, temp.y)));
    }
    tempAngleRange = calcAngleRange(tempAngles, l, t);
//	std::cout << "calcAngleRangeCB  tempAngleRange ="
//			<< tempAngleRange.lowerBound << " " << tempAngleRange.upperBound
//			<< endl;
    return tempAngleRange;
}

// calculate the intersection between circle and wall
vector<Point> calcIntersectionCW(double l, double xc, double yc, Wall* wall,
        bool& checkSrc, bool& checkDst) {
    vector<Point> points;

// quick check the separation and r0, if r0 < separation, no intersection
    if (l
            < getSeparation((double) wall->src->x, (double) wall->src->y,
                    (double) wall->dst->x, (double) wall->dst->y, xc, yc)) {
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
//		a = wall->dst->x;

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

        //	if(a < 0.0001 && a >= -0.0001){
        //		double temp1 = r0 * r0 - (b - yc) * (b - yc);
        //	}

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

//	// intersect at 1 point
//	else if (temp < 0.0001 && temp >= -0.0001) {
//		x1 = (2 * a * yc + 2 * xc - 2 * a * b) / (2 * (a * a + 1));
//		y1 = a * x1 + b;
//		if (x1 <= xMax && x1 >= xMin) {
//			Point intersection(x1, y1);
//			points.push_back(intersection);
//		}
//	}
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
AngleRange calcAngleRange(map<double, double> angles, double l, double t) {
    AngleRange angleRange(0, 0);
    if (angles.empty()) {
        return angleRange;
    }
    double minAngle = 540, maxAngle = 0;
    double minAngleDistance = 0, maxAngleDistance = 0;
    for (map<double, double>::iterator it = angles.begin(); it != angles.end();
            it++) {
        double tempAngle = it->first;
        double tempDistance = it->second;
//		if (temp < 0) {
//			temp = temp + 360;
//		}
        if (tempAngle <= minAngle) {
            minAngle = tempAngle;
            minAngleDistance = tempDistance;
        }
        if (tempAngle >= maxAngle) {
            maxAngle = tempAngle;
            maxAngleDistance = tempDistance;
        }
    }
//	if (maxAngle - minAngle > 180) {
//		double temp2 = minAngle;
//		minAngle = maxAngle - 360;
//		maxAngle = temp2;
//	}

    if (maxAngle - minAngle < 180) {
        angleRange.lowerBound = minAngle;
        angleRange.upperBound = maxAngle;

    } else {
        minAngle = 540, maxAngle = 0;
        for (map<double, double>::iterator it = angles.begin();
                it != angles.end(); it++) {
//			double temp = *it;
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
    if (minAngleDistance < t / 2) {
        isStuck = 1;
    }
//	else if (minAngleDistance + t / 2 < l - t / 2) {
//		fixRange1 = asin(t / 2 / minAngleDistance) * 180 / PI;
//	} else {
//		fixRange1 = acos(
//				((l - t / 2) * (l - t / 2) + minAngleDistance * minAngleDistance
//						- (t / 2) * (t / 2))
//						/ (2 * (l - t / 2) * minAngleDistance)) * 180 / PI;
//	}
//
    double fixRange2 = 0;
    if (maxAngleDistance < t / 2) {
        isStuck = 1;
    }
//	else if (maxAngleDistance + t / 2 < l - t / 2) {
//		fixRange2 = asin(t / 2 / maxAngleDistance) * 180 / PI;
//	} else {
//		fixRange2 = acos(
//				((l - t / 2) * (l - t / 2) + maxAngleDistance * maxAngleDistance
//						- (t / 2) * (t / 2))
//						/ (2 * (l - t / 2) * maxAngleDistance)) * 180 / PI;
//	}

    fixRange1 = asin(t / 2 / minAngleDistance) * 180 / PI;
    fixRange2 = asin(t / 2 / maxAngleDistance) * 180 / PI;
//	cout << t << endl;

//	cout
//			<< ((l - t / 2) * (l - t / 2) + minAngleDistance * minAngleDistance
//					- (t / 2) * (t / 2)) / (2 * (l - t / 2) * minAngleDistance)
//			<< endl;
//	cout
//			<< ((l - t / 2) * (l - t / 2) + maxAngleDistance * maxAngleDistance
//					- (t / 2) * (t / 2)) / (2 * (l - t / 2) * maxAngleDistance)
//			<< endl;
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
//		std::cout << "srcAngleRanges.empty() == true" << endl;
        return dstAngleRanges;
    }
    std::sort(srcAngleRanges.begin(), srcAngleRanges.end(), sortAngleRanges);

    double maxUpperBound = 0;
    for (int i=0;i<(int)srcAngleRanges.size();++i) {
        AngleRange temp = srcAngleRanges[i];
        if (temp.lowerBound > temp.upperBound) {
            if(maxUpperBound < temp.upperBound) maxUpperBound = temp.upperBound;
            temp.upperBound += 360;
        }
        newSrcAngleRanges.push_back(AngleRange(temp.lowerBound, temp.upperBound));
    }



    for (int i=0;i<(int)newSrcAngleRanges.size();++i) {
        if (newSrcAngleRanges[i].lowerBound < maxUpperBound) {
            newSrcAngleRanges[i].lowerBound += 360;
            newSrcAngleRanges[i].upperBound += 360;
        }
    }
    std::sort(newSrcAngleRanges.begin(), newSrcAngleRanges.end(), sortAngleRanges);

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
        interval.upperBound -= 360;
        if(interval.upperBound < newSrcAngleRanges[0].lowerBound){
            dstAngleRanges.push_back(AngleRange(interval.lowerBound, interval.upperBound));
        }
        else{
            dstAngleRanges[0].lowerBound = interval.lowerBound;
        }
    }
    else{
        dstAngleRanges.push_back(AngleRange(interval.lowerBound, interval.upperBound));
    }

    for (int i=0;i<(int)dstAngleRanges.size();++i) {
        AngleRange temp = dstAngleRanges[i];
        if (temp.lowerBound >= 360) temp.lowerBound -= 360;
        if (temp.upperBound > 360)  temp.upperBound -= 360;
        newDstAngleRanges.push_back(AngleRange(temp.lowerBound, temp.upperBound));
    }
    return newDstAngleRanges;

////	std::cout << "srcAngleRanges.size() = " << srcAngleRanges.size() << endl;
//    for (vector<AngleRange>::iterator it = srcAngleRanges.begin();
//            it != srcAngleRanges.end(); it++) {
//        AngleRange temp = *it;
//        if (temp.lowerBound > temp.upperBound) {
//            temp.upperBound += 360;
//        }
//        AngleRange tempRange(temp.lowerBound, temp.upperBound);
//        newSrcAngleRanges.push_back(tempRange);
////		std::cout << "hahahahaha src angle range =" << temp.lowerBound << " "
////				<< temp.upperBound << endl;
//    }

//    double tempLowerBound = 540, tempUpperBound = 0;
////	double lowerBound = 360;
////	double totalAngle = 0;
//    for (vector<AngleRange>::iterator it = newSrcAngleRanges.begin();
//            it != newSrcAngleRanges.end(); it++) {
//        AngleRange temp = *it;
//        if (it == newSrcAngleRanges.begin()) {
//            if (temp.lowerBound <= tempLowerBound) {
//                tempLowerBound = temp.lowerBound;
////				lowerBound = temp.lowerBound;
//            }
//            if (temp.upperBound >= tempUpperBound) {

//                tempUpperBound = temp.upperBound;

//            }

//            if (newSrcAngleRanges.size() == 1) {
//                AngleRange tempAngleRange(tempLowerBound, tempUpperBound);
//                dstAngleRanges.push_back(tempAngleRange);
//            }
////			if(tempLowerBound < tempUpperBound){
////				totalAngle += tempUpperBound - tempLowerBound;
////			}else{
////				totalAngle += tempUpperBound + 360 - tempLowerBound;
////			}
//            continue;
//        }
////		std::cout << "temp.lowerBound = " << temp.lowerBound << endl;
////		std::cout << "temp.upperBound = " << temp.upperBound << endl;
//        if (temp.lowerBound <= tempUpperBound) {
//            if (temp.upperBound >= tempUpperBound) {
//                tempUpperBound = temp.upperBound;
//            }
//        } else {
//            AngleRange tempAngleRange(tempLowerBound, tempUpperBound);
//            dstAngleRanges.push_back(tempAngleRange);
//            tempLowerBound = temp.lowerBound;
//            tempUpperBound = temp.upperBound;
//        }

//        if (it == newSrcAngleRanges.end() - 1) {
//            AngleRange tempAngleRange(tempLowerBound, tempUpperBound);
////			std::cout << "tempLowerBound = " << tempLowerBound << endl;
////			std::cout << "tempUpperBound = " << tempUpperBound << endl;
//            dstAngleRanges.push_back(tempAngleRange);
//        }
//    }

////	std::cout << "dstAngleRanges.size() = " << dstAngleRanges.size() << endl;
//    for (vector<AngleRange>::iterator it = dstAngleRanges.begin();
//            it != dstAngleRanges.end(); it++) {
//        //AngleRange temp = *it;
////		std::cout << "hahahahaha dst angle range before=" << temp.lowerBound
////				<< " " << temp.upperBound << endl;
//    }

//    if (dstAngleRanges[dstAngleRanges.size() - 1].upperBound
//            - dstAngleRanges[0].lowerBound >= 360) {

//        if (dstAngleRanges.size() >= 2) {

//            tempLowerBound =
//                    dstAngleRanges[dstAngleRanges.size() - 1].lowerBound;
//            tempUpperBound = dstAngleRanges[0].upperBound;
//            vector<AngleRange>::iterator it = dstAngleRanges.end() - 1;
//            dstAngleRanges.erase(it);
////			if (dstAngleRanges.size() > 0) {
//            it = dstAngleRanges.begin();
//            dstAngleRanges.erase(it);
////			}
//            AngleRange angleRange(tempLowerBound, tempUpperBound);
//            dstAngleRanges.push_back(angleRange);

//        } else {
//            vector<AngleRange>::iterator it = dstAngleRanges.begin();
//            dstAngleRanges.erase(it);
//            AngleRange angleRange(0, 360);
//            dstAngleRanges.push_back(angleRange);
//        }
//    }
////	std::cout << "dstAngleRanges.size() = " << dstAngleRanges.size() << endl;
//    for (vector<AngleRange>::iterator it = dstAngleRanges.begin();
//            it != dstAngleRanges.end(); it++) {
//        AngleRange temp = *it;
//        if (temp.lowerBound >= 360) {
//            temp.lowerBound -= 360;
//        }
//        if (temp.upperBound > 360) {
//            temp.upperBound -= 360;
//        }
//        AngleRange tempRange(temp.lowerBound, temp.upperBound);
//        newDstAngleRanges.push_back(tempRange);
////		std::cout << "hahahahaha dst angle range =" << temp.lowerBound << " "
////				<< temp.upperBound << endl;
//    }
////	if (dstAngleRanges.size() == 1) {
////		if (dstAngleRanges[0].upperBound - dstAngleRanges[0].lowerBound
////				>= 360) {
////			dstAngleRanges.erase(dstAngleRanges.begin());
////			AngleRange angleRange(0, 360);
////			dstAngleRanges.push_back(angleRange);
////		}
////
////	}
//    return newDstAngleRanges;
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

    for (vector<AngleRange>::iterator it = srcAngleRanges.begin();
            it != srcAngleRanges.end(); it++) {
        //AngleRange temp = *it;
    }
    std::sort(srcAngleRanges.begin(), srcAngleRanges.end(), sortAngleRanges);

    double tempLowerBound = 540, tempUpperBound = 0;

    for (vector<AngleRange>::iterator it = srcAngleRanges.begin();
            it != srcAngleRanges.end(); it++) {
        AngleRange temp = *it;
        if (it == srcAngleRanges.begin()) {

//			if (temp.upperBound <= tempLowerBound) {
            tempLowerBound = temp.upperBound;
//			}
            continue;
        }

//		if (temp.Bound >= tempLowerBound) {
        tempUpperBound = temp.lowerBound;
        AngleRange angleRange(tempLowerBound, tempUpperBound);
        dstAngleRanges.push_back(angleRange);
        tempLowerBound = temp.upperBound;
//		}
    }
    tempUpperBound = (*srcAngleRanges.begin()).lowerBound;
//	if (tempUpperBound <= tempLowerBound) {
//		if (tempUpperBound <= 180) {
//			tempUpperBound += 360;
//		}
//		if (tempLowerBound >= 180) {
//			tempLowerBound -= 360;
//		}
//	}
    AngleRange angleRange(tempLowerBound, tempUpperBound);
    dstAngleRanges.push_back(angleRange);
    return dstAngleRanges;
}

//double calcDistance(double x1, double y1, double x2, double y2) {
//	return sqrt((x2 - x1) * (x2 - x1) - (y2 - y1) * (y2 - y1));
//}
