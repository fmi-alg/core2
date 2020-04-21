//
// Box in Quadtree
//
//  REMARKS: the use of FLT_MAX and 1e-5 and 1e-10 below need to be
//      fixed to make this code acceptable to all CORE LEVELS.
//
//  Boxex are classified with the enum "Status" type with values
//   IN: the VOR is inside the box but doesn't intersect the border of the box
//   OUT: the VOR is outside
//   ON: the VOR is cross the border
//   TRICKY: tricky case (see def below)
//
// $Id$
//

#pragma once

#define ACTUAL 0

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <vector>
#include <list>
#include <set>
#include <float.h>

#include "Wall.h"
#include "Corner.h"
#include "UnionFind.h"
#include "Intersection.h"

extern bool c1; //c1 predicate (true is the new version and false is the old version)
extern bool c2; //c2 predicate


//
// keep these as options
//
// (1) pseudo ( cl(m)>r_B && inseparable )
// (2) internal medial axis
//

class Set;
class Box;

using namespace std;

//Box iterator
class BoxIter
{
private:
	const Box* b;
	int direction;
	int prev;
	int next;
	int cross;
	Box* neighbor;
public:
	
	BoxIter(const Box* bb, int direc);

	Box* First();

	Box* Next();

	Box* End();
}; //class BoxIter

class BoxNode
{
public:
    BoxNode(){ clearance=0; nearest_feature=NULL; }
    //list<Corner*> corners;
    //list<Wall*> walls;
    Feature* nearest_feature;
    double clearance;
    Point2d pos; //position
};

class VorSegment
{
public:
    //double p[2], q[2];
    Point2d p, q;
};

struct BdSeg //border segment
{
    Point2d n1, //start
            n2, //end
            x;  //crossing point

    void approxX()
    {
        if(type=='1'){ x[0]=(n1[0]+n2[0])/2;  x[1]=(n1[1]+n2[1])/2; }
//        else if(type=='s'){ x=n1; }
//        else if(type=='t'){ x=n2; }
    }

    char type; //0, 1, p, s or t
};

class Box
{

public:
	
	friend class BoxIter;
	friend class QuadTree;
	
	//
	// these data should be moved to private
	//
	int depth;

	///center of the box
	Point2d o;

	////dimentions of the box
	double width, height;

    //Pointers to children, but when no children (i.e., leaf),
    //  the pointers are used as neighbor pointers
    // where
    //  0 = NW, 1 = EN, 2 = SE, 3 = WS (if pChildren are kids)
    //  0 = N, 1=E, 2=S, 3=3 (for neighbors)
    Box* pChildren[4];

	int priority;
	bool isLeaf;
	
	//radius of box
	double rB;

	//clearance of the mid point of the box
	double cl_m;

	// time of expansion
	static int counter;


	list<VorSegment> vor_segments;

	Box* pParent; //parent in quadtree
	enum Status { IN, OUT, ON, UNKNOWN };
	                                      // IN: the VOR is inside the box but does not intersect the border of the box
	                                      // OUT: the VOR is outside
	                                      // ON: the VOR is cross the border
	                                      // UNKNOWN: uninitialized

	Status status; //current status of this box; see Status above

	//these are the features contained in this box
	list<Corner*> corners;
	list<Wall*> walls;

	//
	// constructor
	//
	Box(const Point2d& center, double w, double h):
	    depth(1), width(w), height(h), isLeaf(true),
		pParent(0), status(UNKNOWN)
	{
	    o=center;

		for (int i = 0; i < 4; ++i)
		{
			pChildren[i] = 0;
		}

		rB = sqrt(width*width + height*height)/2;
		//priority = Box::counter;
	}

	//
	// Methods for accessing data in this box
	//

	//compute the corners (vertices) of this box from its center and dimensions
	// UL: upper-left vertex, UR: upper-right vertex
	// LR: lower-right vertex, LL: lower-left vertex
	void getCorners(Point2d& UL, Point2d& UR, Point2d& LR, Point2d& LL );

	//compute the corners (vertices) of this box from its center and dimensions
	// UL: upper-left vertex, UR: upper-right vertex
	// LR: lower-right vertex, LL: lower-left vertex
	void getCorners(BoxNode& UL, BoxNode& UR, BoxNode& LR, BoxNode& LL );

	// add a corner feature to this box
    void addCorner(Corner* c)
    {
        corners.push_back(c);
    }

    // add a wall feature to this box
    void addWall(Wall* w)
    {
        walls.push_back(w);
    }

    // return current status of this box
    Status getStatus()
    {
        updateStatus();
        return status;
    }

	//
	// determine the status of the box: UNKNOWN, IN, OUT, ON
	//
	void updateStatus();

	//
	// distribute the features in this box to the given child box
	//
	// Note: "child" must be a child of this box
	//
	void distribute_features2box(Box * child);

	//
	// compute the clearance and the witness of this clearance (i.e. nearest feature)
	//
	void determine_clearance(BoxNode& node);


	//
	//  split (eps, maxEpsilon)
	//
	// 	--returns false if we fail to split for some reason
	//
	bool split(double epsilon, double maxEpsilon);

	//
	// determine if the corners and walls in this class are separable
	// return true if separable
	// otherwise return false
	//
	int separable_features_count(list<Wall*>& walls, list<Corner*>& corners);

	//same as above but using the features in this class
    int separable_features_count();

	//
	// given a set of features (both corner and wall)
	// determine a set of inseparable features
    //
    // Note: after this call, pSet of features (Wall or Corner) will point to the root of the set
	//
	void getInseparableFeatures(list<Wall*>& walls, list<Corner*>& corners, list<Feature*>& insep);

	//check if a given point p is inside or on the boundary of this box
	bool in(const Point2d& p);

	//
	// find a box that contains point p (i.e. p is in the box)
	// see the definition of "in" above
	//
	Box * find(const Point2d& p);

	//
	// obtain all leaves of this box
	//
	void getLeaves(list<Box*>& leaves);


	// count the number of features that are "f"
	int feature_count(Feature* f, list<Feature*>& all_features);

	//
	// create a Voronoi segment (curve) by connecting two points n1 & n2
	//
	VorSegment createVorSegment(const Point2d& n1, const Point2d& n2);

	//call cutBy_complex or cutBy_simplified
	char cutBy(Feature * f, Feature * g, const Point2d& n1, const Point2d& n2);

    //
    // check if the edge (n1,n2) is cut by the separator of features f and g
	//
    // this function is correct unless (1) f is an edge and g is a corner and
    // (2) n1 and n2 are closer to f but the separator still cuts through the
	// interior of the edge
	//
    // This case is prevented by predicate C3
	//
	// return value:
	//  '0': the separator does not touch (n1,n2)
	//  '1': the separator intersects the interior of (n1,n2) in the regular way
    //  'p': the separator go through both n1 and n2
	//  's':the separator go through n1
	//  't':the separator go through n2
	//
	char cutBy_complex(Feature * f, Feature * g, const Point2d& n1, const Point2d& n2);

    //
    // check if the edge (n1,n2) is cut by the separator of features f and g
    //
    // this function is correct unless (1) f is an edge and g is a corner and
    // (2) n1 and n2 are closer to f but the separator still cuts through the
    // interior of the edge
    //
    // This case is prevented by predicate C3
    //
    // return value:
    //  '0': the separator does not touch (n1,n2)
    //  '1': the separator intersects (n1,n2)
    //
	char cutBy_simplified(Feature * f, Feature * g, const Point2d& n1, const Point2d& n2);

	//print warning message, called by buildVor
	void printBuildVorWarning(short typeCount[5]);

	//
	// build edge segments of this box sharing with its neighbors in quadtree
	// for q balanced tree, Bd[i] has at most 2 elements
	//
	// store them in "vector<BdSeg> Bd[4]"
	// The index of Bd is the same as that of Children, i.e. 0=N, 1=E, 2=S, 3=W
	//
	int buildBdSegs(vector<BdSeg> Bd[4]);

    //
    // build edge segments of this box sharing with its neighboring box marked with ON
    // for q balanced tree, Bd[i] has at most 2 elements
    //
    // store them in "vector<BdSeg> Bd[4]"
    // The index of Bd is the same as that of Children, i.e. 0=N, 1=E, 2=S, 3=W
    //
    int buildOnBdSegs(vector<BdSeg> Bd[4]);

    //
    // An edge segments of this box, 0=N, 1=E, 2=S, 3=W
    //
    BdSeg getBdSeg(short index);

    //build vor curves by calling either buildVor_VF or buildVor_CKY
    void buildVor();

    //build vor curves for this box based on the vertex features
    void buildVor_VF();


    //build vor curves for this box
    //as described in the paper
    void buildVor_CKY();

	//build Voronoi curves with 2 features!
    void buildVor(Feature * f, Feature * g);

    //this is a more complex version of buildVor
    void buildVor_complex(Feature * f, Feature * g);

    //this is a more simplified version of buildVor
    void buildVor_simplified(Feature * f, Feature * g);

    //build Voronoi curves with 3 features!
    void buildVor(Feature * f, Feature * g, Feature * h);

    //build Voronoi curves with >3 features!
	void buildVorDegenerate(const list<Feature*>& insep);

	//check if a feature is wall or corner
	bool isWall(Feature * f);

	//
    // Voronoi vertex test (VVT)
    //
	// This function determines the types of features and calls VVT_ordered.
	// see VVT_ordered for detail.
	//
    // return:
	//
    // '0': failed, no intersection between the box and the bisection of w1/w2
    // 'f': failed, all intersections are closer to f/g (the f/g feature in the paper)
    // 'h': failed, all intersections are closer to h (the h feature in the paper)
    // '1': passed, there is a vertex in the box
    //
    char VVT(Feature * f1, Feature * f2, Feature * f3);

    //
    // Voronoi vertex test with ordered features (VVT)
    //
    // f&g are the same type (wall or corner) of feature
    // h may have the same or different type
    //
    // '0': failed, no intersection between the box and the bisection of w1/w2
    // 'f': failed, all intersections are closer to f/g (the f/g feature in the paper)
    // 'h': failed, all intersections are closer to h (the h feature in the paper)
    // '1': passed, there is a vertex in the box
    //
    char VVT_ordered(Feature * f, Feature * g, Feature * h);

    // compute the bisector of f1 and f2
    // @return
    // bo //bisector origin
    // bv //bisector vector, not normalized
    void getBisector(Feature * f1, Feature * f2, Point2d& bo, Vector2d& bv);


    // compute the bisector of w1 and w2
    // @return
    // bo //bisector origin
    // bv //bisector vector, not normalized
    void getBisector(Wall * w1, Wall * w2, Point2d& bo, Vector2d& bv);

    // compute the bisector of c1 and c2
    // @return
    // bo //bisector origin
    // bv //bisector vector, not normalized
    void getBisector(Corner * c1, Corner * c2, Point2d& bo, Vector2d& bv);

	//
	// compute the intersection between the bisector of (f1,f2) and the line segment connecting n1 and n2
    //
    // the return value is true if there is an intersection between the bisector of f1/f2 and the
    // segment connecting n1 and n2
    //
    // "cross" contains the intersection if the return is true
	//
	bool intersection(Feature * f1, Feature * f2, const BoxNode& n1, const BoxNode& n2, BoxNode& cross);

	//
	//compute the intersection between the bisector of w1&w2 and the segment (n1,n2)
	//
	//return true if there is an intersection. If so, "cross" stores the intersection
	//
	bool intersection(Wall * w1, Wall * w2, const BoxNode& n1, const BoxNode& n2, BoxNode& cross);

    //
    //compute the intersection between the bisector of c1&c2 and the segment (n1,n2)
    //
    //return true if there is an intersection. If so, "cross" stores the intersection
    //
    bool intersection(Corner * c1, Corner * c2, const BoxNode& n1, const BoxNode& n2, BoxNode& cross);


    //collect the intersections between the boundary of this box and the bisector of the features f and g
    void getBoundaryIntersections(Feature * f, Feature * g, list<BoxNode>& crossings);

    //collect all neighbors of this box
    //this box must be a leaf and quadtree must be balanced
    void getAllNeighthbors(list<Box*>& nei_list);

    //collect all neighbors of this box from each side: (N,E,S,W)
    //this box must be a leaf and quadtree must be balanced
    void getAllNeighthbors(list<Box*> nei_list[4]);

    //check if the box b and this box are neighbors
    //this box and box b must be leaves and quadtree must be balanced
    bool isNeighbor(Box * b);

protected:

    // build a node (intersection between seg and Vor) from features f, g
    bool buildVorNode(BdSeg& seg, Feature * f, Feature * g);

    // build a node (intersection between seg and Vor) from features f, g
    // and checked by feature h
    // Called by buildVor(f,g,h)
    bool buildVorNode(BdSeg& seg, Feature * f, Feature * g, Feature * h);

    // build a Voronoi curve from features f, g, h
    // Called by buildVor(f,g,h)
    void buildVorCurve(Feature * f, Feature * g, Feature * h);

    //build Voronoi vertex from 3 features
    Point2d buildVorVertex(Feature * f, Feature * g, Feature * h);

    //find the closest feature in the inseparable features (ISF)
    //to the end points in seg
    Feature * closestInISF(Feature * g, const Point2d& n1, const Point2d& n2);

private:

    //
    static bool isOverLimit(const Box* base, const Box* nextBox)
    {
        if ((nextBox->o[0] > base->o[0] - base->width / 2 && nextBox->o[0] < base->o[0] + base->width / 2)
            || (nextBox->o[1] > base->o[1] - base->height / 2 && nextBox->o[1] < base->o[1] + base->height / 2))
        {
            return false;
        }
        return true;
    }

};//class Box
