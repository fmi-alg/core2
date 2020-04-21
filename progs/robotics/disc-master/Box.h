#pragma once

#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <list>
#include "ControlWindow.h"
#include "Corner.h"
#include "Wall.h"

class Box;
class Set;
class Wall;

extern ControlWindow* controlWin;
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

public:
	friend class BoxIter;
	friend class QuadTree;
	int depth;
	double x, y;		// midpoint of box
	double width, height;
	int priority;
	bool isLeaf;
	static double r0;
	double rB;

	static int counter;	// time of expansion (used in BFS strategy)
	
	//Pointers to children, but when no children (i.e., leaf),
	//	the pointers are used as neighbor pointers
	// where
	//	0 = NW, 1 = EN, 2 = SE, 3 = WS
	Box* pChildren[4]; 

	Box* pParent; //parent in quadtree
	enum Status { FREE, STUCK, MIXED, UNKNOWN };
	Status status;
	Set* pSet;
	list<Corner*> corners;
	list<Wall*> walls;

	//for shortest path
	double dist2Source;
	int heapId;
	Box* prev;
	bool visited;

	Box(double xx, double yy, double w, double h):
	    	depth(1), x(xx), y(yy), width(w), height(h), isLeaf(true), 
		pParent(0), status(UNKNOWN),
		pSet(0), dist2Source(-1), heapId(-1), prev(0), visited(false)
	{
		for (int i = 0; i < 4; ++i)
		{
			pChildren[i] = 0;
		}
		rB = sqrt(width*width + height*height)/2;
        priority = Box::counter;
	}
    
    ~Box() {}
	
	void updateStatus()
	{
		if (status != UNKNOWN)
		{
			return;
		}
		
		double outerDomain = r0 + rB;
		double innerDomain = r0 > rB ? r0 - rB : 0;
        double mindistC = 1e30;
        Corner *nearestC = NULL;
		for (list<Corner*>::iterator it = corners.begin(); it != corners.end(); )
		{
			Corner* c = *it;
            double dist = c->distance(this->x, this->y);
            if(mindistC > dist){
                mindistC = dist;
                nearestC = *it;
            }
            if (dist <= innerDomain)
            {
                status = STUCK;
				return;
			}

            // Chee & Tom: Feb 21 2016 Put this mixed decision afterwards
            // if this is greater than the outer domain or not intersect the zone, erase it
            if(dist > outerDomain || !c->intersectZone(this) ) {
                it = corners.erase(it);
            }
            else
            {
                status = MIXED;
                ++it;
            }
		}

        double mindistW = 1e30;
        Wall *nearestW = NULL;
		for (list<Wall*>::iterator it = walls.begin(); it != walls.end(); )
		{
			Wall* w = *it;
            double dist = w->distance(this->x, this->y);
            if(mindistW > dist){
                mindistW = dist;
                nearestW = *it;
            }
            if (dist < innerDomain)
			{
                status = STUCK;
				return;				
            }

            // Chee & Tom: Feb 21 2016 Put this mixed decision afterwards
            // if this is greater than the outer domain or not intersect the zone, erase it
            if (dist > outerDomain || !w->intersectZone(this) )
            {
                it = walls.erase(it);
            }
            else
            {
                status = MIXED;
                ++it;
            }
		}

        // Chee & Tom: Feb 23 2016
        // empty set
        // 1 or 2 feature
        // 		if (rB < sep(m_B, f))
        // 			check status because it is far enough
		if (corners.size() == 0 && walls.size() == 0)
		{
			if (!pParent)
			{
                status = MIXED;
			} 
			else
			{
				status = pParent->checkChildStatus(this->x, this->y);
			}			
		}
        else if (corners.size() > 0 && walls.size() == 0){
            if (mindistC > rB && !(nearestC->isConvex())){
                status = STUCK;
            }
            else
            {
                status = MIXED;
            }
        }
        else if (corners.size() == 0 && walls.size() > 0){
            if (mindistW > rB && !(nearestW->isRight(this->x, this->y))){
                status = STUCK;
            }
            else
            {
                status = MIXED;
            }
        }
        else if(corners.size() > 0 && walls.size() > 0) {
            if (mindistW < mindistC) {
                if (mindistW > rB && !(nearestW->isRight(this->x, this->y))){
                    status = STUCK;
                }
                else
                {
                    status = MIXED;
                }
            }
            else {
                if (mindistC > rB && !(nearestC->isConvex())){
                    status = STUCK;
                }
                else
                {
                    status = MIXED;
                }
            }
        }
	}


	//find the nearest feature, and check
	Status checkChildStatus(double x, double y)
	{
		//assert(walls.size());

        bool dupW = false;
        int f = -1;

        double mindistW = 1e30;
        Wall* nearestWall = NULL;
        Wall* nearestWall2 = NULL;
        list<Wall*>::iterator iterW = walls.begin();
        for (int i=0; iterW != walls.end(); ++iterW)
		{
			Wall* w = *iterW;
			double dist = w->distance(x, y);
			if (dist < mindistW)
			{
				mindistW = dist;
				nearestWall = *iterW;
                f = i;
			}
            ++i;
		}
        iterW = walls.begin();
        for (int i=0; iterW != walls.end(); ++iterW)
        {
            Wall* w = *iterW;
            double dist = w->distance(x, y);
            if (fabs(dist-mindistW) < 1e-10 && i != f)
            {
                nearestWall2 = *iterW;
                dupW = true;
            }
            ++i;
        }

        bool dupC = false;
        f = -1;

        double mindistC = 1e30; //mindistC may not exist, so init to a bigger number
		Corner* nearestCorner = NULL;
        Corner* nearestCorner2 = NULL;
		if (corners.size())
		{			
			list<Corner*>::iterator iterC = corners.begin();
            for (int i=0; iterC != corners.end(); ++iterC)
			{
				Corner* c = *iterC;
				double dist = c->distance(x, y);
				if (dist < mindistC)
				{
					mindistC = dist;
					nearestCorner = *iterC;
                    f = i;
				}
                ++i;
			}

            corners.begin();
            for (int i=0; iterC != corners.end(); ++iterC)
            {
                Corner* c = *iterC;
                double dist = c->distance(x, y);
                if (fabs(dist-mindistC) < 1e-10 && i != f)
                {
                    nearestCorner2 = *iterC;
                    dupC = true;
                }
                ++i;
            }
		}

		//nearest feature is a wall
		if (mindistW < mindistC)
		{
            //if (dupW)
            //{
            //    if (nearestWall->isRight(x, y) || nearestWall2->isRight(x, y))
            //    {
            //        return FREE;
            //    }
            //}
            //else
            //{
                if (nearestWall->isRight(x, y))
                {
                    return FREE;
                }
            //}
		}		
		//otherwise check the corner's convexity
		//if convex, out; if concave, in
		//note that if a wall and a corner are the same dist, 
		//only need to take care of the corner
		else
		{
            //if (dupC)
            //{
            //    if (nearestCorner->isConvex() || nearestCorner2->isConvex())
            //    {
            //        return FREE;
            //    }
            //}
            //else{
                if (nearestCorner->isConvex())
                {
                    return FREE;
                }
            //}
		}

        *controlWin<<"Box at "<<this->x<<" "<<this->y<<" is stuck\n";
        *controlWin<<"MinDistW is "<<mindistW<<" and mindistC is "<<mindistC<<"\n";

        return STUCK;
    }//checkChildStatus

	void addCorner(Corner* c)
	{
		corners.push_back(c);
	}

	void addWall(Wall* w)
	{
		walls.push_back(w);
	}

	bool isFree()
	{
		if (status == FREE)
		{
			return true;
		}
		return false;
	}

    bool in(double qx, double qy)
    {
        if( qx<x-width/2 || qx>x+width/2 ) return false;
        if( qy<y-height/2 || qy>y+height/2) return false;
        return true;
    }

	Status getStatus()
	{
		updateStatus();
		return status;
	}

	// split(eps)
	// 	returns false if we fail to split for some reason
	//
	bool split(double epsilon)
	{
		if (this->height < epsilon || this->width < epsilon)
		{
			return false;
		}

		if (!this->isLeaf || this->status != MIXED)
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
			children[i]->depth = this->depth + 1;
		}

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
			//assert( abs(abs(neighbor->x - this->x)
			//	- (neighbor->width/2 + this->width/2)) < 1e-8 
			// ||  abs(abs(neighbor->y - this->y)
			// 	- (neighbor->height/2 + this->height/2)) < 1e-8);
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
            
            delete iter;
		}

		for (int i = 0; i < 4; ++i)
		{
			this->pChildren[i] = children[i];
			this->pChildren[i]->pParent = this;
			//add all of parent's walls and corners to each child,
			//will be filtered later in updatestatus()
			this->pChildren[i]->walls.insert(this->pChildren[i]->walls.begin(),				 this->walls.begin(), this->walls.end());
			this->pChildren[i]->corners.insert(
				this->pChildren[i]->corners.begin(),
				this->corners.begin(), this->corners.end());
		}
		this->isLeaf = false;
        
		return true;
	}

};//class Box
