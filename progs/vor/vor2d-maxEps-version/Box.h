//
// Box in Quadtree
//
// 	REMARKS: the use of FLT_MAX and 1e-5 and 1e-10 below need to be
// 		fixed to make this code acceptable to all CORE LEVELS.
//
//	Boxex are classified with the enum "Status" type with values
//	 IN: the VOR is inside the box but doesn't intersect the border of the box
//	 OUT: the VOR is outside
//	 ON: the VOR is cross the border
//	 TRICKY: tricky case (see def below)
//
// $Id: Box.h 1149 2012-06-25 06:13:37Z jmlien $
//

#pragma once

#define ACTUAL 0

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <math.h>
#include "Wall.h"
#include "Corner.h"
#include <vector>
#include <list>
#include <set>
#include <float.h>
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
    BoxNode(){ x=y=clearance=0; nearest_feature=NULL; }
    //list<Corner*> corners;
    //list<Wall*> walls;
    Feature* nearest_feature;
    double clearance;
    double x, y;
};

class VorSegment
{
public:
    double p[2], q[2];
};

class Box
{
private:

    //
	static bool isOverLimit(const Box* base, const Box* nextBox)
	{
		if ((nextBox->x > base->x - base->width / 2 && nextBox->x < base->x + base->width / 2)
			|| (nextBox->y > base->y - base->height / 2 && nextBox->y < base->y + base->height / 2))
		{
			return false;
		}
		return true;
	}

public:
	
	friend class BoxIter;
	friend class QuadTree;
	
	int depth;
	double x, y;
	double width, height;
	int priority;
	bool isLeaf;
	static double r0; //radius of robot
	double rB; //radius of box
	double cl_m; //clearance of the mid point of the box

	static int counter;	// time of expansion (used in BFS strategy)
	
	//Pointers to children, but when no children (i.e., leaf),
	//	the pointers are used as neighbor pointers
	// where
	//	0 = NW, 1 = EN, 2 = SE, 3 = WS (if pChildren are kids)
	//  0 = N, 1=E, 2=S, 3=3 (for neighbors)
	Box* pChildren[4]; 

	list<VorSegment> vor_segments;

	Box* pParent; //parent in quadtree
	enum Status { IN, OUT, ON, UNKNOWN };
	   // IN: the VOR is inside the box but doesn't intersect the border of the box
	   // OUT: the VOR is outside
	   // ON: the VOR is cross the border
	   // TRICKY: tricky case (see def below)
	Status status;

	//these are the features contained in this box
	list<Corner*> corners;
	list<Wall*> walls;


    //list<Corner*> node_corners;
    //list<Wall*> node_walls;

	//for shortest path
	double dist2Source;
	int heapId;
	Box* prev;
	bool visited;

	Box(double xx, double yy, double w, double h):
	    depth(1), x(xx), y(yy), width(w), height(h), isLeaf(true),
		pParent(0), status(UNKNOWN),
		dist2Source(-1), heapId(-1), prev(0), visited(false)
	{
		for (int i = 0; i < 4; ++i)
		{
			pChildren[i] = 0;
		}
		rB = sqrt(width*width + height*height)/2;
		priority = Box::counter; 
	}
	

	//
	// determine the status of the box: UNKNOWN, IN, OUT, ON
	//
	//Chee: void updateStatus(double maxEps)
	void updateStatus();

	//
	void distribute_features2box(Box * child);

	//
	void determine_clearance(BoxNode& node);

	/*
	//
	// find the nearest feature and the clearance
	// results are stored in "node"
	//
	void determine_clearance(BoxNode& node)
	{

	    double x=node.x;
	    double y=node.y;

	    //compute the closest wall
		Wall* nearestWall=NULL;
		double mindistW=FLT_MAX;
		for (list<Wall*>::iterator iterW = walls.begin(); iterW != walls.end(); ++iterW)
		{
			Wall* w = *iterW;
			double dist = w->distance_star(x, y); //w->distance_star(x, y); //w->distance(x, y);
			if( fabs(dist-mindistW)<1e-10 )
            {
                if( w->distance_sign(x,y)==0 && w->isRight(x,y) )
                    nearestWall = *iterW;
            }
			else if (dist < mindistW) //shorter distance
			{
			    mindistW = dist;
			    nearestWall = *iterW;
			}
		}

		//
		// compute a closest corner that is closer than  (mindistW +1) (?? why +1)
		//
		double mindistC = FLT_MAX; //mindistC may not exist, so init to a bigger number
		Corner* nearestCorner = NULL;
        for (list<Corner*>::iterator iterC = corners.begin(); iterC != corners.end(); ++iterC)
        {
            Corner* c = *iterC;
            double dist = c->distance(x, y);

            if( fabs(dist-mindistC)<1e-5 ) //if(dist == mindistC)
            {
                if( c->inZone_star(x,y) )
                {
                    //in the zone
                    nearestCorner = *iterC;
                }

            }
            else if (dist < mindistC)  //shorter distance
            {
                mindistC = dist;
                nearestCorner = *iterC;
            }
        }

		if (mindistW<=mindistC)
		{
		    node.clearance=mindistW;
		    if(nearestWall!=NULL){
		        node.nearest_feature=nearestWall;
		    }
		}
		else
		{
		    node.clearance=mindistC;
		    if(nearestCorner!=NULL){
		        node.nearest_feature=nearestCorner;
		    }
		}
	}
	*/


	void addCorner(Corner* c)
	{
		corners.push_back(c);
	}

	void addWall(Wall* w)
	{
		walls.push_back(w);
	}

	Status getStatus()
	{
		updateStatus();
		return status;
	}

	//  split(eps, maxEpsilon)
	//
	// 	--returns false if we fail to split for some reason
	//
	bool split (double epsilon, double maxEpsilon);
/*
	{
		if (this->height < epsilon || this->width < epsilon)
		{
			return false;
		}


        //if ((!this->isLeaf || this->status != IN)
	//	
	// Chee: "status != IN"
	//
	// 	is equiv. to
	//
	// 	"status == OUT || status == UNKNOWN || status == ON".
	//
	// 	But we weaken the ON part, by adding a maxEpsilon criterion:
	//
        if ( (!this->isLeaf	|| this->status == OUT || this->status == UNKNOWN)
	  	|| (this->status == ON	&& 
		    this->height < maxEpsilon	&& this->width < maxEpsilon
		   )
	   )
        {
            return false;
        }

		//record the time of this split event, will be used to set priority of children
		++Box::counter;

		Box* children[4];
		children[0] = new Box(x - width / 4, y + height / 4, width / 2, height / 2);
		children[1] = new Box(x + width / 4, y + height / 4, width / 2, height / 2);
		children[2] = new Box(x + width / 4, y - height / 4, width / 2, height / 2);
		children[3] = new Box(x - width / 4, y - height / 4, width / 2, height / 2);

		for (int i = 0; i < 4; ++i)
		{
		    if(children[i]==NULL)
		    {
		        cerr<<"! Error: Box::split: Not enough memory"<<endl;
		        return false;
		    }

			children[i]->depth = this->depth + 1;
			BoxNode node;
			node.x=children[i]->x;
			node.y=children[i]->y;
			determine_clearance(node);
			children[i]->cl_m=node.clearance;

			//distribute the feature
			distribute_features2box(children[i]);
		}

		//
		// for internal nodes, children [i] is the i-th child
		// for leaves, children [i] is the pointer to first node in i-th adj list
		//
		for (int i = 0; i < 4; ++i)
		{
			//find three other directions
			int prev = (i + 3) % 4;
			int next = (i + 1) % 4;
			int cross = (i + 2) % 4;

			//update easy cases
			children[i]->pChildren[i] = pChildren[i];
			children[i]->pChildren[next] = children[next];
			children[i]->pChildren[cross] = children[prev];

			//init box neighbor iterator for direction i
			BoxIter* iter = new BoxIter(this, i);
			Box* neighbor = iter->First();

			if (!neighbor)
			{
				continue;
			}

			// if neighbor are no smaller
			if (neighbor->depth <= this->depth)
			{
                //after split child 'next' should also point to
                //neighbor in direction i
				children[next]->pChildren[i] = neighbor;

                //if neighbor's cross direction point to this, it should
                //instead point to child 'next' after split
				if (neighbor->pChildren[cross] == this)
				{
					neighbor->pChildren[cross] = children[next];
				}
				continue;
			}

			Box* prevNeighbor = neighbor;

			//indicate if we go across the boundary between child 'i'
			//and 'next' the first time
			bool firstTimeCrossBetweenChildren = true;

			//if neighbor smaller
			while(neighbor != iter->End())
			{
                //within the strip of child next, neighbor's cross direction
                //should point to next
				if (!isOverLimit(children[next], neighbor))
				{
					neighbor->pChildren[cross] = children[next];
				} 

				//within the strip of child i, neighbor's cross
				//direction should point to i
				else if (!isOverLimit(children[i], neighbor))
				{
					neighbor->pChildren[cross] = children[i];

					//first time cross between child i and next,
					//should update next's i direction pointer
					if (firstTimeCrossBetweenChildren)
					{
						firstTimeCrossBetweenChildren = false;
						children[next]->pChildren[i] = prevNeighbor;
					}
				}
				else
				{
					assert(0);
				}
				prevNeighbor = neighbor;
				neighbor = iter->Next();
			}
		}


		//setup the children/parent relation
		for (int i = 0; i < 4; ++i)
		{
			this->pChildren[i] = children[i];
			this->pChildren[i]->pParent = this;
		}

		this->isLeaf = false;

		return true;
	}//split 
*/

	//
	// determine if the corners and walls in this class are separable
	// return true if separable
	// otherwise return false
	//
	int separable_features_count (list<Wall*>& walls, list<Corner*>& corners)
	{
        list<Feature*> insep_features;
        getInseparableFeatures(walls,corners,insep_features);

        //done
        return insep_features.size();
	}

    int separable_features_count()
    {
        return separable_features_count(walls,corners);
    }

	//
	// given a set of features (both corner and wall)
	// determine a set of inseparable features
    //
    // Note: after this call, pSet of features (Wall or Corner) will point to the root of the set
	//
	void getInseparableFeatures(list<Wall*>& walls, list<Corner*>& corners, list<Feature*>& insep)
	{
         static list<Set*> features;

         //previously used sets, remove and clean them
         if(features.empty()==false)
         {
             for(list<Set*>::iterator i=features.begin();i!=features.end();i++)
             {
                 Set * s=*i;
                 s->pFeature->pSet=NULL;
                 delete s;
             }//end for i
             features.clear();
         }

         //now start building the new sets
         typedef list<Wall*>::iterator WIT;
         typedef list<Corner*>::iterator CIT;

         //create a set for each wall
         for (WIT it = walls.begin(); it != walls.end(); it++){
             Set* set=new Set((Feature*)*it);
             assert(set);
             features.push_back(set);
         }//end for

         //create a set for each corner
         for (CIT it = corners.begin(); it != corners.end(); it++){
             Set* set=new Set((Feature*)*it);
             assert(set);
             features.push_back(set);
         }//end for

         UnionFind UF;

         //start to merge
         for (WIT it = walls.begin(); it != walls.end(); it++){
             Wall* w=*it;
             if(w->dst->pSet!=NULL) UF.Union(w,w->dst);
             if(w->src->pSet!=NULL) UF.Union(w,w->src);
         }//end for

         for (CIT it = corners.begin(); it != corners.end(); it++){
             Corner* c=*it;
             if(c->preWall)  if(c->preWall->pSet!=NULL) UF.Union(c,c->preWall);
             if(c->nextWall) if(c->nextWall->pSet!=NULL) UF.Union(c,c->nextWall);
         }//end for

         //now check the number of Sets
         set<Feature*> featureset;
         for (WIT it = walls.begin(); it != walls.end(); it++){
             featureset.insert(UF.Find(*it));
         }//end for

         for (CIT it = corners.begin(); it != corners.end(); it++){
             featureset.insert(UF.Find(*it));
         }//end for

         //remember the result
         insep.clear();
         insep.insert(insep.end(),featureset.begin(),featureset.end());
	}

	bool in(double qx, double qy)
	{
	    if( qx<x-width/2 || qx>x+width/2 ) return false;
	    if( qy<y-height/2 || qy>y+height/2) return false;
	    return true;
	}

	Box * find(double qx, double qy)
	{
	    if(isLeaf) return this;

	    for(int i=0;i<4;i++){
	        if(pChildren[i]->in(qx,qy))
	            return pChildren[i]->find(qx,qy);
	    }

	    return NULL;
	}

	//recursively obtain all leaves of this box
	void getLeaves(list<Box*>& leaves)
	{
	    if(isLeaf){
	        leaves.push_back(this);
	        return;
	    }

        for(int i=0;i<4;i++) pChildren[i]->getLeaves(leaves);
	}


	int feature_count(Feature* f, list<Feature*>& all_features)
	{
	    typedef list<Feature*>::iterator IT;
	    int count=0;
	    for(IT i=all_features.begin();i!=all_features.end();i++){
	        if( (*i)==f ) count++;
	    }
	    return count;
	}

	void buildVor ()
	{
	    if(status!=Box::ON) return;

	    UnionFind UF;

	    //this build sets of inseparable features
	    separable_features_count();

        //find actual features of the box nodes
        BoxNode UL, LL, UR, LR; //upper left, lower left, upper right, lower right
        UL.x=x-width/2; UL.y=y+height/2;
        LL.x=x-width/2; LL.y=y-height/2;
        UR.x=x+width/2; UR.y=y+height/2;
        LR.x=x+width/2; LR.y=y-height/2;

        determine_clearance(UL);
        determine_clearance(LL);
        determine_clearance(UR);
        determine_clearance(LR);

        Feature * ulf=(UL.nearest_feature==NULL)?NULL:UF.Find(UL.nearest_feature);
        Feature * llf=(LL.nearest_feature==NULL)?NULL:UF.Find(LL.nearest_feature);
        Feature * urf=(UR.nearest_feature==NULL)?NULL:UF.Find(UR.nearest_feature);
        Feature * lrf=(LR.nearest_feature==NULL)?NULL:UF.Find(LR.nearest_feature);

        if(lrf==NULL || llf==NULL || urf==NULL || lrf==NULL)
            return;

        list<BoxNode> mids[4]; //confusing name, this is for storing the intersections
                               //between Vor and each edge of the box (north, east, south, west)

        Feature * mid_features[4]={NULL,NULL,NULL,NULL}; //closest feature of the mid pt of north, east, south, west edge
        list<Feature *> all_features;
        all_features.push_back(ulf);
        all_features.push_back(llf);
        all_features.push_back(urf);
        all_features.push_back(lrf);

        //north edge (top)
        if(ulf!=urf && ulf!=NULL && urf!=NULL)
        {

#if ACTUAL

            BoxNode mid;
            mid=intersection(ulf,urf,UL,UR);
            mids[0].push_back(mid);

#else
            BoxNode mid;
            mid.x=x;
            mid.y=y+height/2;

            if(pChildren[0]==NULL)
                mids[0].push_back(mid);
            else{
                if(pChildren[0]->depth>depth){
                    determine_clearance(mid);
                    Feature * mf=(mid.nearest_feature==NULL)?NULL:UF.Find(mid.nearest_feature);
                    mid_features[0]=mf;
                    all_features.push_back(mf);
                    if(ulf==mf && mf!=urf){
                        mid.x=x+width/4;
                        mids[0].push_back(mid);
                    }
                    else if(ulf!=mf && mf==urf){
                        mid.x=x-width/4;
                        mids[0].push_back(mid);
                    }
                    else{
                        //cout<<"More complicated case"<<endl;
                        mid.x=x-width/4;
                        mids[0].push_back(mid);
                        mid.x=x+width/4;
                        mids[0].push_back(mid);
                    }
                }
                else{
                    mids[0].push_back(mid);
                }
            }
#endif
        }

        //east edge (right)
        if(urf!=lrf && urf!=NULL && lrf!=NULL)
        {
#if ACTUAL
            BoxNode mid;
            mid=intersection(urf,lrf,UR,LR);
            mids[1].push_back(mid);
#else
            BoxNode mid;
            mid.x=x+width/2;
            mid.y=y;
            if(pChildren[1]==NULL)
                mids[1].push_back(mid);
            else{
                if(pChildren[1]->depth>depth){
                    determine_clearance(mid);
                    Feature * mf=(mid.nearest_feature==NULL)?NULL:UF.Find(mid.nearest_feature);
                    mid_features[1]=mf;
                    all_features.push_back(mf);
                    if(urf==mf && mf!=lrf){
                        mid.y=y-height/4;
                        mids[1].push_back(mid);
                    }
                    else if(urf!=mf && mf==lrf){
                        mid.y=y+height/4;
                        mids[1].push_back(mid);
                    }
                    else{
                        //cout<<"More complicated case"<<endl;
                        mid.y=y+height/4;
                        mids[1].push_back(mid);
                        mid.y=y-height/4;
                        mids[1].push_back(mid);
                    }
                }
                else{
                    mids[1].push_back(mid);
                }
            }
#endif
        }

        //south edge (bottom)
        if(lrf!=llf && lrf!=NULL && llf!=NULL)
        {
#if ACTUAL
            BoxNode mid;
            mid=intersection(lrf,llf,LR,LL);
            mids[2].push_back(mid);
#else
            BoxNode mid;
            mid.x=x;
            mid.y=y-height/2;
            if(pChildren[2]==NULL)
                mids[2].push_back(mid);
            else{
                if(pChildren[2]->depth>depth){
                    determine_clearance(mid);
                    Feature * mf=(mid.nearest_feature==NULL)?NULL:UF.Find(mid.nearest_feature);
                    mid_features[2]=mf;
                    all_features.push_back(mf);
                    if(lrf==mf && mf!=llf){
                        mid.x=x-width/4;
                        mids[2].push_back(mid);
                    }
                    else if(lrf!=mf && mf==llf){
                        mid.x=x+width/4;
                        mids[2].push_back(mid);
                    }
                    else{
                        mid.x=x+width/4;
                        mids[2].push_back(mid);
                        mid.x=x-width/4;
                        mids[2].push_back(mid);
                    }
                }
                else{
                    mids[2].push_back(mid);
                }
            }
#endif
        }

        //west edge (left)
        if(llf!=ulf && llf!=NULL && ulf!=NULL)
        {
#if ACTUAL
            BoxNode mid;
            mid=intersection(llf,ulf,LL,UL);
            mids[3].push_back(mid);
#else
            BoxNode mid;
            mid.x=x-width/2;
            mid.y=y;
            if(pChildren[3]==NULL)
                mids[3].push_back(mid);
            else{
                if(pChildren[3]->depth>depth){
                    determine_clearance(mid);
                    Feature * mf=(mid.nearest_feature==NULL)?NULL:UF.Find(mid.nearest_feature);
                    mid_features[3]=mf;
                    all_features.push_back(mf);
                    if(llf==mf && mf!=ulf){
                        mid.y=y+height/4;
                        mids[3].push_back(mid);
                    }
                    else if(llf!=mf && mf==ulf){
                        mid.y=y-height/4;
                        mids[3].push_back(mid);
                    }
                    else{
                        mid.y=y-height/4;
                        mids[3].push_back(mid);
                        mid.y=y+height/4;
                        mids[3].push_back(mid);
                    }
                }
                else{
                    mids[3].push_back(mid);
                }
            }//end if
#endif
        }

        int degree=mids[0].size()+mids[1].size()+mids[2].size()+mids[3].size();

        if(degree==3)
        {
            for(int i=0;i<4;i++)
            {
                for(list<BoxNode>::iterator j=mids[i].begin();j!=mids[i].end();j++)
                {
                    VorSegment seg;
                    seg.p[0]=j->x;
                    seg.p[1]=j->y;
                    seg.q[0]=x;
                    seg.q[1]=y;
                    vor_segments.push_back(seg);
                }//end for j
            }//end for i
        }
        else if(degree==2)
        {
            list<BoxNode> nodes;
            for(int i=0;i<4;i++) nodes.insert(nodes.end(),mids[i].begin(),mids[i].end());
            VorSegment seg;
            seg.p[0]=nodes.front().x;
            seg.p[1]=nodes.front().y;
            seg.q[0]=nodes.back().x;
            seg.q[1]=nodes.back().y;
            vor_segments.push_back(seg);
        }
        else
        {
            //connect the points
            //check upper-left corner
            int ul_fc=feature_count(ulf,all_features);
//            if(mid_features[3]==NULL && llf==ulf) ul_fc--;
//            if(mid_features[0]==NULL && urf==ulf) ul_fc--;
//            if(mid_features[3]==ulf) ul_fc--;
//            if(mid_features[0]==ulf) ul_fc--;

            if( ul_fc==1 ){ //unique
                VorSegment seg;
                BoxNode n3=(mid_features[3]==ulf)?mids[3].front():mids[3].back();
                BoxNode n0=(mid_features[0]==ulf)?mids[0].back():mids[0].front();
                seg.p[0]=n3.x;
                seg.p[1]=n3.y;
                seg.q[0]=n0.x;
                seg.q[1]=n0.y;
                vor_segments.push_back(seg);
            }

            //connect the points
            //check upper-right corner
            int ur_fc=feature_count(urf,all_features);
//            if(mid_features[0]==NULL && ulf==urf) ur_fc--;
//            if(mid_features[1]==NULL && lrf==urf) ur_fc--;
//            if(mid_features[0]==urf) ur_fc--;
//            if(mid_features[1]==urf) ur_fc--;

            if( ur_fc==1 ){
                VorSegment seg;
                BoxNode n0=(mid_features[0]==urf)?mids[0].front():mids[0].back();
                BoxNode n1=(mid_features[1]==urf)?mids[1].back():mids[1].front();
                seg.p[0]=n0.x;
                seg.p[1]=n0.y;
                seg.q[0]=n1.x;
                seg.q[1]=n1.y;
                vor_segments.push_back(seg);
            }

            //connect the points
            //check lower right corner
            int lr_fc=feature_count(lrf,all_features);
//            if(mid_features[1]==NULL && urf==lrf) lr_fc--;
//            if(mid_features[2]==NULL && llf==lrf) lr_fc--;
//            if(mid_features[1]==lrf) lr_fc--;
//            if(mid_features[2]==lrf) lr_fc--;

            if( lr_fc==1 ){
                VorSegment seg;
                BoxNode n1=(mid_features[1]==lrf)?mids[1].front():mids[1].back();
                BoxNode n2=(mid_features[2]==lrf)?mids[2].back():mids[2].front();
                seg.p[0]=n1.x;
                seg.p[1]=n1.y;
                seg.q[0]=n2.x;
                seg.q[1]=n2.y;
                vor_segments.push_back(seg);
            }

            //connect the points
            //check lower lower-left corner
            int ll_fc=feature_count(llf,all_features);
//            if(mid_features[2]==NULL && lrf==llf) ll_fc--;
//            if(mid_features[3]==NULL && ulf==llf) ll_fc--;
//            if(mid_features[2]==llf) ll_fc--;
//            if(mid_features[3]==llf) ll_fc--;

            if( ll_fc==1 ){
                VorSegment seg;
                BoxNode n2=(mid_features[2]==llf)?mids[2].front():mids[2].back();
                BoxNode n3=(mid_features[3]==llf)?mids[3].back():mids[3].front();
                seg.p[0]=n2.x;
                seg.p[1]=n2.y;
                seg.q[0]=n3.x;
                seg.q[1]=n3.y;
                vor_segments.push_back(seg);
            }

            //check north edge
            if(ulf==urf){
                Feature * l_f=(mid_features[3]!=NULL)?mid_features[3]:llf;
                Feature * r_f=(mid_features[1]!=NULL)?mid_features[1]:lrf;
                if( ulf!=l_f &&  urf!=r_f){
                    VorSegment seg;
                    seg.q[0]=mids[3].back().x;
                    seg.q[1]=mids[3].back().y;
                    seg.p[0]=mids[1].front().x;
                    seg.p[1]=mids[1].front().y;
                    vor_segments.push_back(seg);
                }
            }

            //check east edge
            if(urf==lrf){
                Feature * t_f=(mid_features[0]!=NULL)?mid_features[0]:ulf; //top
                Feature * b_f=(mid_features[2]!=NULL)?mid_features[2]:llf; //bottom
                if( urf!=t_f &&  lrf!=b_f){
                    VorSegment seg;
                    seg.p[0]=mids[0].back().x;
                    seg.p[1]=mids[0].back().y;
                    seg.q[0]=mids[2].front().x;
                    seg.q[1]=mids[2].front().y;
                    vor_segments.push_back(seg);
                }
            }

            //check south edge
            if(lrf==llf){
                Feature * r_f=(mid_features[1]!=NULL)?mid_features[1]:urf; //right
                Feature * l_f=(mid_features[3]!=NULL)?mid_features[3]:ulf; //left
                if( lrf!=r_f &&  llf!=l_f){

                    if(mid_features[1]!=NULL || mid_features[3]!=NULL)
                    {
                        VorSegment seg;
                        seg.p[0]=mids[1].back().x;
                        seg.p[1]=mids[1].back().y;
                        seg.q[0]=mids[3].front().x;
                        seg.q[1]=mids[3].front().y;
                        vor_segments.push_back(seg);
                    }
                }
            }


            //check west edge
            if(llf==ulf){
                Feature * b_f=(mid_features[2]!=NULL)?mid_features[2]:lrf; //bottom
                Feature * t_f=(mid_features[0]!=NULL)?mid_features[0]:urf; //top
                if( llf!=b_f &&  ulf!=t_f){

                    if(mid_features[0]!=NULL || mid_features[2]!=NULL)
                    {
                        VorSegment seg;
                        seg.p[0]=mids[2].back().x;
                        seg.p[1]=mids[2].back().y;
                        seg.q[0]=mids[0].front().x;
                        seg.q[1]=mids[0].front().y;
                        vor_segments.push_back(seg);
                    }
                }
            }

//            if( ulf!=llf &&  urf!=lrf &&
//                    ulf==urf && llf==lrf)
//            {
//                VorSegment seg;
//                seg.p[0]=mids[1].front().x;
//                seg.p[1]=mids[1].front().y;
//                seg.q[0]=mids[3].front().x;
//                seg.q[1]=mids[3].front().y;
//                vor_segments.push_back(seg);
//            }
//
//            if( ulf==llf &&  urf==lrf &&
//                    ulf!=urf && llf!=lrf)
//            {
//                VorSegment seg;
//                seg.p[0]=mids[0].front().x;
//                seg.p[1]=mids[0].front().y;
//                seg.q[0]=mids[2].front().x;
//                seg.q[1]=mids[2].front().y;
//                vor_segments.push_back(seg);
//            }
        }//end if(sfc!=3)

	}

	//
	//compute the intersection between the bisector of (f1,f2) and the line segment connecting n1 and n2
	//
	BoxNode intersection(Feature * f1, Feature * f2, const BoxNode& n1, const BoxNode& n2)
	{
	    bool is_f1_wall=dynamic_cast<Wall*>(f1)!=NULL;
	    bool is_f2_wall=dynamic_cast<Wall*>(f2)!=NULL;

	    if( is_f1_wall && is_f2_wall) //two walls
	    {
	        Wall * w1=dynamic_cast<Wall*>(f1);
	        Wall * w2=dynamic_cast<Wall*>(f2);
	        //find bisector
	        pair<Corner*,Corner*> pairs[2];
	        //find left end-pt

	        //check w1
	        if( w1->src->x<w1->dst->x ){
	            pairs[0].first=w1->src;
	            pairs[1].first=w1->dst;
	        }
	        else if( w1->src->x>w1->dst->x ){
	            pairs[0].first=w1->dst;
                pairs[1].first=w1->src;
	        }
	        else{
	            if( w1->src->y<w1->dst->y ){
	                pairs[0].first=w1->src;
	                pairs[1].first=w1->dst;
	            }
	            else {
	                pairs[0].first=w1->dst;
	                pairs[1].first=w1->src;
	            }
	        }

	        //check w2
            if( w2->src->x<w2->dst->x ){
                pairs[0].second=w2->src;
                pairs[1].second=w2->dst;
            }
            else if( w2->src->x>w2->dst->x ){
                pairs[0].second=w2->dst;
                pairs[1].second=w2->src;
            }
            else{
                if( w2->src->y<w2->dst->y ){
                    pairs[0].second=w2->src;
                    pairs[1].second=w2->dst;
                }
                else {
                    pairs[0].second=w2->dst;
                    pairs[1].second=w2->src;
                }
            }

            //now determine the segment
            BoxNode e1, e2;
            e1.x=(pairs[0].first->x+pairs[0].second->x)/2;
            e1.y=(pairs[0].first->y+pairs[0].second->y)/2;
            e2.x=(pairs[1].first->x+pairs[1].second->x)/2;
            e2.y=(pairs[1].first->y+pairs[1].second->y)/2;

            double v_x=e2.x-e1.x;
            double v_y=e2.y-e1.y;
            e1.x=e1.x-v_x*1000;
            e1.y=e1.y-v_y*1000;
            e2.x=e2.x+v_x*1000;
            e2.y=e2.y+v_y*1000;

            //find intersection...
            double a[2]={e1.x,e1.y};
            double b[2]={e2.x,e2.y};
            double c[2]={n1.x,n1.y};
            double d[2]={n2.x,n2.y};
            double p[2];

            char code=SegSegInt(a,b,c,d,p);
            BoxNode result;

            if(code!='0'){
                result.x=p[0];
                result.y=p[1];
            }
            else{
                result.x=(n1.x+n2.x)/2;
                result.y=(n1.y+n2.y)/2;
            }

            return result;
	    }
	    else if( is_f1_wall==false && is_f2_wall==false) //two corners
	    {
	        Corner * c1=dynamic_cast<Corner*>(f1);
	        Corner * c2=dynamic_cast<Corner*>(f2);
	        BoxNode mid;
	        mid.x = (c1->x+c2->x)/2;
	        mid.y = (c1->y+c2->y)/2;
            double v_x=c1->x-c2->x;
            double v_y=c1->y-c2->y;
            double n_x=v_y*1000;
            double n_y=-v_x*1000;

            //find intersection...
            double a[2]={mid.x+n_x,mid.y+n_y};
            double b[2]={mid.x-n_x,mid.y-n_y};
            double c[2]={n1.x,n1.y};
            double d[2]={n2.x,n2.y};
            double p[2];

            char code=SegSegInt(a,b,c,d,p);
            BoxNode result;

            if(code!='0'){
                result.x=p[0];
                result.y=p[1];
            }
            else{
                result.x=(n1.x+n2.x)/2;
                result.y=(n1.y+n2.y)/2;
            }

            return result;
	    }
	    else{
	        //tricky case

            BoxNode result;
            result.x=(n1.x+n2.x)/2;
            result.y=(n1.y+n2.y)/2;

            return result;
	    }
	    //done
	}

};//class Box
