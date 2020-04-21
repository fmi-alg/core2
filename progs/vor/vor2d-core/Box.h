
//
// Box in Quadtree
//
// $Id: Box.h 978 2012-06-05 12:10:36Z jmlien $
//

#pragma once

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

extern bool c1; //c1 predicate (true is the new version and false is the old version)
extern bool c2; //c2 predicate

#define SMALLNUMBER 1e-10

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
	                                      // IN: the VOR is inside the box but does not intersect the border of the box
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
	void updateStatus()
	{
		if (status != UNKNOWN)
		{
			return;
		}

		int total_feature_size=corners.size()+walls.size();

		int sfc=separable_features_count();
		bool separable=(sfc>1);
		if(total_feature_size<6)
		    total_feature_size=sfc;

		//C_0
		if(total_feature_size>3){
		    status = IN; //need more split
		    return;
		}

		if(total_feature_size<=1){
		    status = OUT;
		    return;
		}

		//C_1
		if(c1){ //new definition
		    if(cl_m<rB)
		    {
		        //then the feature is either inseparable
		        if( separable==false ){
		            status = OUT;
		        }
		        else //or has segments that may split the cell
		        {
		            //we only need to look at walls
		            if(corners.empty() && walls.size()==2)
		            {
		                //overlapping wall
		                if(walls.front()->src==walls.back()->dst && walls.front()->dst==walls.back()->src)
		                    status = OUT;
		                else
		                {
		                    //check if walls are facing the opposing directions
		                    Wall * w1=walls.front();
		                    Wall * w2=walls.back();

		                    //check if the lines are opposing...
		                    bool r1=w1->isRight(w2->src->x,w2->src->y);
		                    bool r2=w1->isRight(w2->dst->x,w2->dst->y);
		                    bool r3=w2->isRight(w1->src->x,w1->src->y);
		                    bool r4=w2->isRight(w1->dst->x,w1->dst->y);

		                    if(!r1 && !r2 && !r3 && !r4){ //opposing
		                        status = OUT;
		                    }
		                    else //facing each other
		                        status = ON;
		                }
		            }
		            else
		                status = IN; //need more split
		        }
		        return;
		    }
		}
		else {//old definition
            if( (cl_m<rB) && separable ){
                status = IN; //need more split
                return;
            }
		}


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

        //determine the status: either OUT or ON or Tricky
        set<Feature*>   feature_groups;

        if(UL.nearest_feature)
            feature_groups.insert(UnionFind().Find(UL.nearest_feature));

        if(LL.nearest_feature)
            feature_groups.insert(UnionFind().Find(LL.nearest_feature));

        if(UR.nearest_feature)
            feature_groups.insert(UnionFind().Find(UR.nearest_feature));

        if(LR.nearest_feature)
            feature_groups.insert(UnionFind().Find(LR.nearest_feature));


        //C_2
        if(c2) //if c2 predicate flag is on
        {
            //check if the condition is special or not
            if(feature_groups.size()==1 && total_feature_size>1)
            {
                Wall * wall=dynamic_cast<Wall*>(*feature_groups.begin());

                if( wall!=NULL && corners.empty()==false && (cl_m<rB*rB) )
                {
                    status = IN; //need more split
                    return;
                }
            }
        }

        if(separable==false || feature_groups.size()<=1 )
        {
            status = OUT;
            return;
        }

        status = ON;

        return;
	}

	//
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
        for (WIT iterW=walls.begin(); iterW != walls.end(); ++iterW)
        {
            Wall* w = *iterW;
            double dist = w->distance(x, y); //w->distance_star(x, y); //w->distance(x, y);

            if (dist < cl2r) //within the distance range
            {
                {
                    bool zone=w->inZone_star(child); //true; //w->inZone(child); //w->inZone_star(child);
                    if(zone)
                        child->walls.push_back(w);
                }
            }//end if
        }//end for

        //compute the separation to corners
        for (CIT iterC=corners.begin(); iterC != corners.end(); ++iterC)
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
                        child->corners.push_back(c);
                }
            }//end if
        }//end for

//        return;

        //check the closest features of the box corners
        {

            BoxNode mid;
            mid.x=x;
            mid.y=y;
            determine_clearance(mid);

            list<Feature*> features;
            features.insert(features.end(),child->corners.begin(), child->corners.end());
            features.insert(features.end(),child->walls.begin(), child->walls.end());
            if( std::find(features.begin(), features.end(), mid.nearest_feature)==features.end() )
            {
                //some closest feature is lost....
                //this means the box is not in the zone of the closest feature....
                child->corners.clear();
                child->walls.clear();
                return;
            }
        }
	}

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
			    //nearestWall=NULL;

			    //if(w->isRight(x,y)){ //on the right size of the wall
			        nearestWall = *iterW;
			    //}
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
                if( c->isConvex() )
                {
                    //check if in the zone
                    if(c->preWall!=NULL) if(c->preWall->distance_sign(x,y)!=1) continue;
                    if(c->nextWall!=NULL) if(c->nextWall->distance_sign(x,y)!=-1 ) continue;

                    //in the zone
                    nearestCorner = *iterC;
                }

            }
            else if (dist < mindistC)  //shorter distance
            {
                mindistC = dist;
                //nearestCorner=NULL;

                //if(c->isConvex()) //reflect vertex has no Vor (outside the polygon)
                {
                    //check if (x,y) is in the zone of this convex vertex
                    //if( c->preWall->distance_sign(x,y)==1 && c->nextWall->distance_sign(x,y)==-1 )
                    {
                        nearestCorner = *iterC;
                    }
                }
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

	//  split (eps)
	// 	returns false if we fail to split for some reason
	//
	bool split(double epsilon)
	{
		if (this->height < epsilon || this->width < epsilon)
		{
			return false;
		}

		if (!this->isLeaf || this->status != IN)
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
	} 

	//
	// determine if the corners and walls in this class are separable
	// return true if separable
	// otherwise return false
	//
	int separable_features_count(list<Wall*>& walls, list<Corner*>& corners)
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

         for (WIT it = walls.begin(); it != walls.end(); it++){
             Set* set=new Set((Feature*)*it);
             assert(set);
             features.push_back(set);
         }//end for

         for (CIT it = corners.begin(); it != corners.end(); it++){
             Set* set=new Set((Feature*)*it);
             assert(set);
             features.push_back(set);
         }//end for

         UnionFind UF;

         //merge
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


	void buildVor()
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

        list<BoxNode> mids[4];

        //north edge
        if(ulf!=urf && ulf!=NULL && urf!=NULL)
        {
//            BoxNode mid;
//            mid=intersection(ulf,urf,UL,UR);
//            mids[0].push_back(mid);

            BoxNode mid;
            mid.x=x;
            mid.y=y+height/2;

            if(pChildren[0]==NULL)
                mids[0].push_back(mid);
            else{
                if(pChildren[0]->depth>depth){
                    determine_clearance(mid);
                    Feature * mf=(mid.nearest_feature==NULL)?NULL:UF.Find(mid.nearest_feature);
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
        }

        if(urf!=lrf && urf!=NULL && lrf!=NULL)
        {
//            BoxNode mid;
//            mid=intersection(urf,lrf,UR,LR);
//            mids[1].push_back(mid);

            BoxNode mid;
            mid.x=x+width/2;
            mid.y=y;
            if(pChildren[1]==NULL)
                mids[1].push_back(mid);
            else{
                if(pChildren[1]->depth>depth){
                    determine_clearance(mid);
                    Feature * mf=(mid.nearest_feature==NULL)?NULL:UF.Find(mid.nearest_feature);
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
        }

        if(lrf!=llf && lrf!=NULL && llf!=NULL)
        {
//            BoxNode mid;
//            mid=intersection(lrf,llf,LR,LL);
//            mids[2].push_back(mid);

            BoxNode mid;
            mid.x=x;
            mid.y=y-height/2;
            if(pChildren[2]==NULL)
                mids[2].push_back(mid);
            else{
                if(pChildren[2]->depth>depth){
                    determine_clearance(mid);
                    Feature * mf=(mid.nearest_feature==NULL)?NULL:UF.Find(mid.nearest_feature);
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

        }

        if(llf!=ulf && llf!=NULL && ulf!=NULL)
        {
//            BoxNode mid;
//            mid=intersection(llf,ulf,LL,UL);
//            mids[3].push_back(mid);

            BoxNode mid;
            mid.x=x-width/2;
            mid.y=y;
            if(pChildren[3]==NULL)
                mids[3].push_back(mid);
            else{
                if(pChildren[3]->depth>depth){
                    determine_clearance(mid);
                    Feature * mf=(mid.nearest_feature==NULL)?NULL:UF.Find(mid.nearest_feature);
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
        else
        {
            //connect the points
            if( ulf!=urf &&  ulf!=llf && ulf!=NULL){
                VorSegment seg;
                seg.p[0]=mids[0].front().x;
                seg.p[1]=mids[0].front().y;
                seg.q[0]=mids[3].back().x;
                seg.q[1]=mids[3].back().y;
                vor_segments.push_back(seg);
            }

            //connect the points
            if( ulf!=urf &&  urf!=lrf && urf!=NULL){
                VorSegment seg;
                seg.p[0]=mids[0].back().x;
                seg.p[1]=mids[0].back().y;
                seg.q[0]=mids[1].front().x;
                seg.q[1]=mids[1].front().y;
                vor_segments.push_back(seg);
            }

            //connect the points
            if( lrf!=urf &&  lrf!=llf && lrf!=NULL ){
                VorSegment seg;
                seg.p[0]=mids[1].back().x;
                seg.p[1]=mids[1].back().y;
                seg.q[0]=mids[2].front().x;
                seg.q[1]=mids[2].front().y;
                vor_segments.push_back(seg);
            }

            //connect the points
            if( llf!=lrf &&  llf!=ulf && llf!=NULL){
                VorSegment seg;
                seg.p[0]=mids[2].back().x;
                seg.p[1]=mids[2].back().y;
                seg.q[0]=mids[3].front().x;
                seg.q[1]=mids[3].front().y;
                vor_segments.push_back(seg);
            }

            if( ulf!=llf &&  urf!=lrf &&
                    ulf==urf && llf==lrf)
            {
                VorSegment seg;
                seg.p[0]=mids[1].front().x;
                seg.p[1]=mids[1].front().y;
                seg.q[0]=mids[3].front().x;
                seg.q[1]=mids[3].front().y;
                vor_segments.push_back(seg);
            }

            if( ulf==llf &&  urf==lrf &&
                    ulf!=urf && llf!=lrf)
            {
                VorSegment seg;
                seg.p[0]=mids[0].front().x;
                seg.p[1]=mids[0].front().y;
                seg.q[0]=mids[2].front().x;
                seg.q[1]=mids[2].front().y;
                vor_segments.push_back(seg);
            }
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

	inline double Area(const double a[2], const double b[2], const double c[2])
	{
	    return ( b[0] - a[0] ) * ( c[1] - a[1] ) -
	           ( c[0] - a[0] ) * ( b[1] - a[1] );
	}

	inline int AreaSign(const double a[2], const double b[2], const double c[2])
	{
	    double area=Area(a,b,c);
	    if      ( area >  SMALLNUMBER ) return  1;
	    else if ( area < -SMALLNUMBER ) return -1;
	    else     return  0;
	}

	inline int Collinear(const double a[2], const double b[2], const double c[2])
	{
	   return AreaSign( a, b, c ) == 0;
	}

	inline bool AlmostEqual(const double a[2], const double b[2])
	{
	    return (fabs(a[0]-b[0])<SMALLNUMBER &&fabs(a[1]-b[1])<SMALLNUMBER);
	}


	/*---------------------------------------------------------------------
	Returns TRUE iff point c lies on the closed segement ab.
	Assumes it is already known that abc are collinear.
	---------------------------------------------------------------------*/
	inline bool Between(const double a[2], const double b[2], const double c[2])
	{
	   // If ab not vertical, check betweenness on x; else on y.
	   if ( fabs(a[0]-b[0])>fabs(a[1]-b[1]) )
	      return ((a[0] <= c[0]) && (c[0] <= b[0])) ||
	             ((a[0] >= c[0]) && (c[0] >= b[0]));
	   else
	      return ((a[1] <= c[1]) && (c[1] <= b[1])) ||
	             ((a[1] >= c[1]) && (c[1] >= b[1]));
	}


	//
	inline char ParallelInt
	( const double a[2], const double b[2], const double c[2], const double d[2], double p[2])
	{
	   if(!Collinear(a, b, c)) return '0';

	   //check if they overlap..
	   if(Between(a,b,c)) return 'e';
	   if(Between(a,b,d)) return 'e';
	   if(Between(c,d,a)) return 'e';
	   if(Between(c,d,b)) return 'e';

	   //they don't overlap but the end points may..
	   //check if the end points overlap
	   if(AlmostEqual(a,c)){ p[0]=a[0]; p[1]=a[1]; return 'v';}
	   if(AlmostEqual(b,c)){ p[0]=b[0]; p[1]=b[1]; return 'v';}
	   if(AlmostEqual(a,d)){ p[0]=a[0]; p[1]=a[1]; return 'v';}
	   if(AlmostEqual(b,d)){ p[0]=b[0]; p[1]=b[1]; return 'v';}

	   return '0';
	}

	/*---------------------------------------------------------------------
	SegSegInt: Finds the point of intersection p between two closed
	segments ab and cd.  Returns p and a char with the following meaning:
	   'e': The segments collinearly overlap, sharing a point.
	   'v': An endpoint (vertex) of one segment is on the other segment,
	        but 'e' doesn't hold.
	   '1': The segments intersect properly (i.e., they share a point and
	        neither 'v' nor 'e' holds).
	   '0': The segments do not intersect (i.e., they share no points).
	Note that two collinear segments that share just one point, an endpoint
	of each, returns 'e' rather than 'v' as one might expect.
	---------------------------------------------------------------------*/
	inline char SegSegInt( const double a[2], const double b[2], const double c[2], const double d[2], double p[2] )
	{
	   double  s, t;        // The two parameters of the parametric eqns.
	   double  num_s, num_t, denom;   // Numerator and denoninator of equations.
	   char    code = '?';    // Return char characterizing intersection.
	   const double TINYNUMBER=SMALLNUMBER;

	   denom = a[0] * ( d[1] - c[1] ) +
	           b[0] * ( c[1] - d[1] ) +
	           d[0] * ( b[1] - a[1] ) +
	           c[0] * ( a[1] - b[1] );

	   // If denom is zero, then segments are parallel: handle separately.
	   //if (fabs(denom)<SMALLNUMBER) denom=0;
	   if (fabs(denom)<TINYNUMBER) return  ParallelInt(a, b, c, d, p);


	   if(AlmostEqual(a,c)){ p[0]=a[0]; p[1]=a[1]; return 'v';}
	   if(AlmostEqual(b,c)){ p[0]=b[0]; p[1]=b[1]; return 'v';}
	   if(AlmostEqual(a,d)){ p[0]=a[0]; p[1]=a[1]; return 'v';}
	   if(AlmostEqual(b,d)){ p[0]=b[0]; p[1]=b[1]; return 'v';}

	   //compute s
	   num_s =    a[0] * ( d[1] - c[1] ) +
	              c[0] * ( a[1] - d[1] ) +
	              d[0] * ( c[1] - a[1] );


	   s = num_s / denom;

	   if( fabs(s)<TINYNUMBER ) s=0;
	   else if( fabs(1-s)<TINYNUMBER ) s=1;


	   //compute t
	   num_t = -( a[0] * ( c[1] - b[1] ) +
	              b[0] * ( a[1] - c[1] ) +
	              c[0] * ( b[1] - a[1] ) );



	   t = num_t / denom;

	   if( fabs(t)<TINYNUMBER ) t=0;
	   else if( fabs(1-t)<TINYNUMBER) t=1;

	   //decide the code
	//   if(s==0 || s==1 || t==0 || t==1)
	//        code = 'v';
	   if( (0.0<s) && (s< 1.0) && (0.0< t) && (t< 1.0) )
	        code = '1';
	   else if ( (0.0>s) || (s>1.0) || (0.0>t) || (t>1.0) )
	        return '0';
	   else
	       code= 'v';

	   if(code!='v'){
	       p[0] = (a[0] + s*(b[0]-a[0]));
	       p[1] = (a[1] + s*(b[1]-a[1]));
	   }
	   else{
	      if(s==0){ p[0]=a[0]; p[1]=a[1]; }
	      else if(s==1){p[0]=b[0]; p[1]=b[1]; }
	      else if(t==0){p[0]=c[0]; p[1]=c[1]; }
	      else if(t==1){p[0]=d[0]; p[1]=d[1]; }
	      else{
	          cout<<"s="<<s<<" t="<<t<<endl;
	          cout<<"a="<<a[0]<<","<<a[1]<<endl;
	          cout<<"b="<<b[0]<<","<<b[1]<<endl;
	          cout<<"c="<<c[0]<<","<<c[1]<<endl;
	          cout<<"d="<<d[0]<<","<<d[1]<<endl;
	          assert(false);
	      }
	   }

	   //double check...
	//   if(code=='v'){
	//       double q[2];
	//       q[0] = (c[0] + t*(d[0]-c[0]));
	//       q[1] = (c[1] + t*(d[1]-c[1]));
	//       if(AlmostEqual(p,q)==false){
	//           //cout<<"SHOOT: "<<code<<" points:"<<p[0]<<","<<p[1]<<"  "<<q[0]<<","<<q[1]<<endl;
	//           return '0';
	//       }
	//   }

	   return code;
	}


};//class Box
