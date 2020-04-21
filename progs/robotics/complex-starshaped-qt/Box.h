#pragma once

// standard library
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <cmath>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>
#include <float.h>
#include <iostream>

// custom library
#include "Polygon.h"
#include "Wall.h"
#include "Corner.h"
#include "line2d.h"
#include "Triangle.h"
#include "Pose.h"


class Set;
class Box;

const double PI = 3.14159265;
const double deg2rad = 1.0f/180.0f;
extern vector<Triangle> compRoboTriangles;

using namespace std;

class BoxNode
{
public:
    BoxNode(){ x=y=clearance=0; nearest_feature=NULL; }
    //list<Corner*> corners;
    //list<Wall*> walls;
    Feature* nearest_feature;
    double clearance;
    double x, y;
};

class Box
{
private:
    static bool isOverLimit(const Box* base, const Box* nextBox)
    {
        if ((nextBox->x > base->x - base->width / 2 && nextBox->x < base->x + base->width / 2)
            || (nextBox->y > base->y - base->height / 2 && nextBox->y < base->y + base->height / 2))
        {
            return false;
        }
        return true;
    }

    static bool isOneContainAnotherX(const Box* b1, const Box* b2)
    {
        double xmin1 = b1->x - b1->width / 2;
        double xmax1 = b1->x + b1->width / 2;
        double xmin2 = b2->x - b2->width / 2;
        double xmax2 = b2->x + b2->width / 2;

        if ((xmin1 <= xmin2 && xmax2 <= xmax1)
            || (xmin2 <= xmin1 && xmax1 <= xmax2))
        {
            return true;
        }
        return false;
    }

    static bool isOneContainAnotherY(const Box* b1, const Box* b2)
    {
        double ymin1 = b1->y - b1->height / 2;
        double ymax1 = b1->y + b1->height / 2;
        double ymin2 = b2->y - b2->height / 2;
        double ymax2 = b2->y + b2->height / 2;

        if ((ymin1 <= ymin2 && ymax2 <= ymax1)
            || (ymin2 <= ymin1 && ymax1 <= ymax2))
        {
            return true;
        }
        return false;
    }

    static bool isOneContainAnotherA(const Box* b1, const Box* b2)
    {
        if (isBothInArc1(b1, b2) || isBothInArc1(b2, b1))
        {
            return true;
        }
        return false;
    }

    static bool isEq(double x, double y)
    {
        double e = 10 * std::numeric_limits<double>::epsilon();
        if ( abs( double(x-y) ) < e)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //returns true is any edge of arc2 is within arc1
    static bool isAnyInArc1(double s1, double e1, double s2, double e2)
    {
        if (e1 > s1)
        {
            if ( e1 > s2 && s2 >= s1 )
            {
                return true;
            }

            if ( e1 >= e2 && e2 > s1 )
            {
                return true;
            }
        }
        else
        {
            if ( ((2 > s2) && (s2 >= s1)) || ((e1 > s2) && (s2 >= 0)) )
            {
                return true;
            }

            if ( ((2 > e2) && (e2 >= s1)) || ((e1 > e2) && (e2 >= 0)) )
            {
                return true;
            }
        }

        return false;
    }

    //returns true is both edges of arc2 is within arc1
    static bool isBothInArc1(const Box* b1, const Box* b2)
    {
        double s1 = b1->xi[0];
        double e1 = b1->xi[1];
        double s2 = b2->xi[0];
        double e2 = b2->xi[1];

        if (e1 > s1)
        {
            if ( e1 >= e2 && s2 >= s1 )
            {
                return true;
            }
        }
        else
        {
            if ( (2 >= e2 && s2 >= s1) || (e1 > e2 && s2 >= 0) )
            {
                return true;
            }
        }

        return false;
    }

    static bool isArcIntsct(double s1, double e1, double s2, double e2)
    {
        if ( isAnyInArc1(s1, e1, s2, e2) || isAnyInArc1(s2, e2, s1, e1) )
        {
            return true;
        }
        return false;
    }

    void updateStatusSmall();

    void updateStatusBig();

    static int oppositeDir[6];// = {2, 3, 0, 1, 6, 5};

public:
    friend class BoxIter;
    friend class QuadTree;
    int depth;
    double x, y;
    double width, height;
    int priority;
    bool isLeaf;
    static double r0;
    static double THETA_MIN;
    double rB;
    double cl_m; //clearance of the mid point of the box

    static vector<Box*>* pAllLeaf;

    vector<Box*> Nhbrs[6];
    bool isBig;
    int tChildID;
    int rChildID;
    enum{ LOWER, UPPER };
    double xi[2];


    static int counter;	// time of expansion (used in BFS strategy)

    Box* pParent; //parent in quadtree
    int classify_condition;
    enum Status { FREE, STUCK, MIXED, UNKNOWN };
    Status status;
    Set* pSet;
    vector<list<Corner*>> corners;
    vector<list<Wall*>> walls;
    list<Corner*> vorCorners;
    list<Wall*> vorWalls;

    //for shortest path
    double dist2Source;
    int heapId;
    Box* prev;
    bool visited;

    Box(double xx, double yy, double w, double h):
            depth(1), x(xx), y(yy), width(w), height(h), isLeaf(true),
        isBig(true), tChildID(-1), rChildID(-1),
        pParent(0), classify_condition(-1), status(UNKNOWN),
        pSet(0), dist2Source(-1),
        heapId(-1), prev(0), visited(false)
    {
      rB = sqrt(width*width + height*height)/2;
      priority = Box::counter;

      xi[0] = 0;
      xi[1] = 2;
      corners.resize(compRoboTriangles.size());
      walls.resize(compRoboTriangles.size());
    }

    static int isNhbr(Box* b1, Box* b2);

    void updateStatus() {
        if (this->isBig) {
            updateStatusBig();
        }
        else {
            updateStatusSmall();
        }
    }


    //find the nearest feature, and check
    Status checkChildStatus(double x, double y);

    void distribute_features2box(Box * child)
    {
        typedef list<Wall*>::iterator   WIT;
        typedef list<Corner*>::iterator CIT;

        //center x,y
        double x=child->x;
        double y=child->y;

        //clearance+2*radius of the box
        double cl2r=child->rB*2+child->cl_m; //clearance + 2*rB

        //
        //compute the separation to walls
        for (WIT iterW=vorWalls.begin(); iterW != vorWalls.end(); ++iterW)
        {
            Wall* w = *iterW;
            pt2line rst = w->distance2(x, y); //w->distance_star(x, y); //w->distance(x, y);

            if (rst.dist < cl2r) //within the distance range
            {
                {
                    bool zone=w->inZone_star(child); //true; //w->inZone(child); //w->inZone_star(child);
                    if(zone)
                        child->vorWalls.push_back(w);
                }
            }//end if
        }//end for

        //compute the separation to corners
        for (CIT iterC=vorCorners.begin(); iterC != vorCorners.end(); ++iterC)
        {
            Corner* c = *iterC;
            double dist = c->distance(x, y);
            //OK, close enough
            if (dist < cl2r)
            {
                {
                    //check with the Zone of the previous wall
                    bool zone=c->inZone_star(child); //true; //c->inZone(child); //c->inZone_star(child);
                    if(zone)
                        child->vorCorners.push_back(c);
                }
            }//end if
        }//end for

        //        return;

        //
        //check the closest features of the box corner
        //
        //the closest feature of the box corner should be inside the features
        //of this box
        //
        {

            BoxNode mid;
            mid.x=x;
            mid.y=y;
            determine_clearance(mid);
            if(mid.nearest_feature==NULL) return;

            list<Feature*> features;
            features.insert(features.end(),child->vorCorners.begin(), child->vorCorners.end());
            features.insert(features.end(),child->vorWalls.begin(), child->vorWalls.end());
            if( std::find(features.begin(), features.end(), mid.nearest_feature)==features.end() )
            {
                //some closest feature is lost....
                //this means the box is not in the zone of the closest feature....
                child->vorCorners.clear();
                child->vorWalls.clear();
                return;
            }
        }
    }


    void determine_clearance(BoxNode& node)
    {
        double x=node.x;
        double y=node.y;

        //compute the closest wall
        Wall* nearestWall=NULL;
        double mindistW=FLT_MAX;
        for (list<Wall*>::iterator iterW = vorWalls.begin(); iterW != vorWalls.end(); ++iterW)
        {
            Wall* w = *iterW;

            pt2line rst = w->distance2(x, y);

            if( fabs(rst.dist-mindistW) < 1e-10 )
            {
                if( w->distance_sign(x,y)==0 && w->isRight(x,y) ) //in zone
                    nearestWall = *iterW;
            }
            else if (rst.dist < mindistW) //shorter distance
            {
                mindistW = rst.dist;
                nearestWall = *iterW;
            }
        }

        //
        // compute a closest corner that is closer than  (mindistW +1) (?? why +1)
        //
        double mindistC = FLT_MAX; //mindistC may not exist, so init to a bigger number
        Corner* nearestCorner = NULL;
        for (list<Corner*>::iterator iterC = vorCorners.begin(); iterC != vorCorners.end(); ++iterC)
        {
            Corner* c = *iterC;
            double dist = c->distance(x, y);

            if( fabs(dist-mindistC)<1e-5 ) //if(dist == mindistC)
            {
                if( c->inZone_star(x,y) ) //in the zone
                {
                    nearestCorner = *iterC;
                }

            }
            else if (dist < mindistC)  //shorter distance
            {
                mindistC = dist;
                nearestCorner = *iterC;
            }
        }

        //
        // determine the feature and shortest
        //

        bool in_zone_w=false;
        bool in_zone_c=false;

        if(nearestWall!=NULL) in_zone_w=nearestWall->inZone_star(x,y);
        if(nearestCorner!=NULL) in_zone_c=nearestCorner->inZone_star(x,y);

        if( fabs(mindistW-mindistC)<1e-5 )
        {
            if(in_zone_w){
                node.clearance=mindistW;
                node.nearest_feature=nearestWall;
            }

            if(in_zone_c){
                node.clearance=mindistC;
                node.nearest_feature=nearestCorner;
            }
        }
        else if (mindistW < mindistC)
        {
            node.clearance=mindistW;
            if(nearestWall!=NULL){
                node.nearest_feature=nearestWall;
            }
        }
        else //mindistW > mindistC
        {
            node.clearance=mindistC;
            if(nearestCorner!=NULL){
                node.nearest_feature=nearestCorner;
            }
        }
        //
        //-----------------------------------
        //
    }

    void addCorner(Corner* c) {
        for(unsigned int k=0; k<compRoboTriangles.size(); ++k)
           corners[k].push_back(c);
    }

    void addWall(Wall* w) {
        for(unsigned int k=0; k<compRoboTriangles.size(); ++k)
           walls[k].push_back(w);
    }

    bool isFree() {
        return (status == FREE);
    }

    bool in(double qx, double qy) {
        if( qx<x-width/2 || qx>x+width/2 )  return false;
        if( qy<y-height/2 || qy>y+height/2) return false;
        return true;
    }

    bool contains(double x, double y) {
        if (this->x + width / 2 >= x  && this->x - width / 2 <= x
         && this->y + height / 2 >= y && this->y - height / 2 <= y) {
            return true;
        }
        return false;
    }

    bool contains(double x, double y, double a) {
        if (this->x + width / 2 >= x && this->x - width / 2 <= x
            && this->y + height / 2 >= y && this->y - height / 2 <= y)
        {
            if (xi[1] > xi[0])
            {
                if ( xi[1] > a && a >= xi[0] )
                {
                    return true;
                }
            }
            else
            {
                if ( (2 > a && a >= xi[0]) || (xi[1] > a && a >= 0) )
                {
                    return true;
                }
            }

            return false;
        }

        return false;
    }

    Status getStatus() {
        //updateStatus();
        return status;
    }

    bool split2D( double epsilon, vector<Box*>& chldn);
    bool split3D( double epsilon, vector<Box*>& chldn );
    bool splitAngle( vector<Box*>& chldn );

     //split(eps)
     //	returns false if we fail to split for some reason
    bool split(double epsilon, vector<Box*>& chldn);

    void recursiveSplitAngle(double epsilon, vector<Box*>& chldn, const int n, int m);

    void getRoundTriVerts(double& v01x, double& v01y, double& v02x, double& v02y,
                          double& v11x, double& v11y, double& v12x, double& v12y,
                          double& v21x, double& v21y, double& v22x, double& v22y);
    //void getRoundTriVerts(Triangle &Trixi0, Triangle &Trixi1);
    bool checkInsideTAS(Corner &c, Triangle &tri, bool mirrored = 0);

    bool checkIntersectTAS(Wall &c, Triangle &tri, bool mirrored = 0);
};//class Box
