#pragma once

#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "Wall.h"
#include "Corner.h"
#include <vector>
#include <list>

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
	
	void updateStatus()
	{
		if (status != UNKNOWN)
		{
			return;
		}
		
		double outerDomain = r0 + rB;
		double innerDomain = r0 > rB ? r0 - rB : 0;
		for (list<Corner*>::iterator it = corners.begin(); it != corners.end(); )
		{
			Corner* c = *it;
			if (c->distance(this->x, this->y) <= innerDomain)
			{
				status = STUCK;
				return;
			}

			// Chee & Tom: Feb 2016 Put this mixed decision afterwards
			// if this is greater than the outer domain or not in zone, erase it 
			if( c->distance(this->x, this->y) > outerDomain || !c->intersectZone(this) ) {
				it = corners.erase(it);
			}
			else {
				++it;
			}
		}

		for (list<Wall*>::iterator it = walls.begin(); it != walls.end(); )
		{
			Wall* w = *it;
			double distWall = w->distance(this->x, this->y);
			if (distWall < innerDomain)
			{
				status = STUCK;
				return;				
			} 
			
			if (distWall > outerDomain || !w->intersectZone(this) )
			{
				it = walls.erase(it);
			}
			else {
				++it;
			}
		}

		// Chee & Tom: Feb 2016 
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
	}


	// find the nearest feature, and check
	Status checkChildStatus(double x, double y)
	{
		//assert(walls.size());

		// warning maybe remove DOUBLE_MAX
		double mindistW = DOUBLE_MAX;
		Wall* nearestWall = NULL;

		list<Wall*>::iterator iterW = walls.begin();
		for (; iterW != walls.end(); ++iterW)
		{
			Wall* w = *iterW;
			double dist = w->distance(x, y);
			if (dist < mindistW)
			{
				mindistW = dist;
				nearestWall = *iterW;
			}
		}

		double mindistC = mindistW +1; //mindistC may not exist, so init to a bigger number
		Corner* nearestCorner = NULL;
		if (corners.size())
		{			
			list<Corner*>::iterator iterC = corners.begin();
			mindistC = (*iterC)->distance(x, y);
			nearestCorner = *iterC;
			++iterC;
			for (; iterC != corners.end(); ++iterC)
			{
				Corner* c = *iterC;
				double dist = c->distance(x, y);
				if (dist < mindistC)
				{
					mindistC = dist;
					nearestCorner = *iterC;
				}
			}
		}

		bool isFree = false;

		//nearest feature is a wall
		if (mindistW < mindistC)
		{
			if (nearestWall->isRight(x, y))
			{
				isFree = true;
			}
		}		
		//otherwise check the corner's convexity
		//if convex, out; if concave, in
		//note that if a wall and a corner are the same dist, 
		//only need to take care of the corner
		else
		{
			if (nearestCorner->isConvex())
			{
				isFree = true;
			}
		}

		if (isFree)
		{
			return FREE;
		}
		return STUCK;
	}

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
		}

		for (int i = 0; i < 4; ++i)
		{
			this->pChildren[i] = children[i];
			this->pChildren[i]->pParent = this;
			//add all of parent's walls and corners to each child,
			//will be filtered later in updatestatus()
			this->pChildren[i]->walls.insert(this->pChildren[i]->walls.begin(),
				this->walls.begin(), this->walls.end());
			this->pChildren[i]->corners.insert(
				this->pChildren[i]->corners.begin(),
				this->corners.begin(), this->corners.end());
		}
		this->isLeaf = false;

		return true;
	}

};//class Box
