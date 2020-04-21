#pragma once

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "Wall.h"  
#include "Edge.h"
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
	vector<Box*> neighborVec;
	int direction;
	int index;
	void storeNeighbors (Box* n);
public:
	
	BoxIter(const Box* bb, int direc);
	
	int size();
	int getIndex();

	Box* First();

	Box* Next();

	Box* End();
}; //class BoxIter

class Box
{
private:
	static bool isOverLimit(const Box* base, const Box* nextBox)
	{
		if ( ((nextBox->x > base->x - base->width / 2 && nextBox->x < base->x + base->width / 2)
			&& (nextBox->y > base->y - base->width / 2 && nextBox->y < base->y + base->width / 2))
			|| ((nextBox->x > base->x - base->width / 2 && nextBox->x < base->x + base->width / 2)
			&& (nextBox->z > base->z - base->width / 2 && nextBox->z < base->z + base->width / 2)) 
			|| ((nextBox->y > base->y - base->width / 2 && nextBox->y < base->y + base->width / 2)
			&& (nextBox->z > base->z - base->width / 2 && nextBox->z < base->z + base->width / 2)) )
		{
			return false;
		}
		return true;
	}

public:
	friend class BoxIter;
	friend class QuadTree;
	int depth;
	double x, y, z;
	double width;
	int priority;
	bool isLeaf;
	static double r0;
	double rB;

	static int counter;	// time of expansion (used in BFS strategy)
	
	//Pointers to children, but when no children (i.e., leaf),
	//	the pointers are used as neighbor pointers
	// where
	//	0 = TNW, 1 = TEN, 2 = TSE, 3 = TWS  [T = Top]
	//      4 = BNW, 5 = BEN, 6 = BSE, 7 = BWS  [B = Bottom]
	Box* pChildren[8]; 

	Box* pParent; //parent in octree
	enum Status { FREE, STUCK, MIXED, UNKNOWN };
	Status status;
	Set* pSet;
	list<Corner*> corners;
	list<Edge*> Edges;
	list<Wall*> Walls;

	//for shortest path
	double dist2Source;
	int heapId;
	Box* prev;
	bool visited;

	Box(double xx, double yy, double zz, double w):
	    	depth(1), x(xx), y(yy), z(zz), width(w), isLeaf(true), 
		pParent(0), status(UNKNOWN),
		pSet(0), dist2Source(-1), heapId(-1), prev(0), visited(false)
	{
		for (int i = 0; i < 8; ++i)
		{
			pChildren[i] = 0;
		}
		rB = (width * sqrt(3))/2; 
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
			if (c->distance(this->x, this->y, this->z) <= innerDomain)
			{
				status = STUCK;
				return;
			}
			else if( c->distance(this->x, this->y, this->z) <= outerDomain ) 
			{
				status = MIXED;
				++it;
			}
			else {
				it = corners.erase(it);
			}
		}

		for (list<Edge*>::iterator it = Edges.begin(); it != Edges.end(); )
		{
			Edge* e = *it;
			double distEdge = e->distance(this->x, this->y, this->z);
			if (distEdge < innerDomain)
			{
				status = STUCK;
				return;				
			} 
			else if (distEdge <= outerDomain)
			{				
				status = MIXED;
				++it;
			}
			else
			{
				it = Edges.erase(it);
			}
		}

		for (list<Wall*>::iterator it = Walls.begin(); it != Walls.end(); )
		{
			Wall* w = *it;
			double distWall = w->distance(this->x, this->y, this->z);
			if (distWall < innerDomain)
			{
				status = STUCK;
				return;				
			} 
			else if (distWall <= outerDomain)
			{
				status = MIXED;
				++it;
			}
			else
			{
				it = Walls.erase(it);
			}
		}


		if (corners.size() == 0 && Edges.size() == 0 && Walls.size() == 0)
		{
			if (!pParent)
			{
				status = FREE;
			} 
			else
			{
				status = pParent->checkChildStatus(this->x, this->y, this->z);
			}			
		}
		
	}


	//find the nearest feature, and check
	Status checkChildStatus(double x, double y, double z)
	{
		Wall* nearestWall;
		list<Wall*>::iterator iterW = Walls.begin();
		double mindistW = (*iterW)->distance(x, y, z);
		nearestWall = *iterW;
		++iterW;
		for (; iterW != Walls.end(); ++iterW) 
		{
			Wall* w = *iterW;
			double dist = w->distance(x, y, z);
			if (dist < mindistW)
			{
				mindistW = dist;
				nearestWall = *iterW;
			}
		}		
		
		//assert(Edges.size());
		
		Edge* nearestEdge;
		list<Edge*>::iterator iterE = Edges.begin();
		double mindistE = (*iterE)->distance(x, y, z);
		nearestEdge = *iterE;
		++iterE;
		for (; iterE != Edges.end(); ++iterE) 
		{
			Edge* e = *iterE;
			double dist = e->distance(x, y, z);
			if (dist < mindistE)
			{
				mindistE = dist;
				nearestEdge = *iterE;
			}
		}

		double mindistC = mindistE +1; //mindistC may not exist, so init to a bigger number
		Corner* nearestCorner = NULL;
		if (corners.size())
		{			
			list<Corner*>::iterator iterC = corners.begin();
			mindistC = (*iterC)->distance(x, y, z);
			nearestCorner = *iterC;
			++iterC;
			for (; iterC != corners.end(); ++iterC)
			{
				Corner* c = *iterC;
				double dist = c->distance(x, y, z);
				if (dist < mindistC)
				{
					mindistC = dist;
					nearestCorner = *iterC;
				}
			}
		}

		bool isFree = false;

		// if the nearest feature is a wall...
		// check the orientation of m(B) with respect to the wall
		if (mindistW < mindistE && mindistW < mindistC)
		{
			if (nearestWall->isRight(x, y, z))
			{
				isFree = true;
			}			
	
		}

		// else, the nearest feature is either an edge or corner, and the box must be free (since all obstacles are convex)
		else isFree = true;

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

	void addEdge(Edge* e)
	{
		Edges.push_back(e);
	}

	void addWall(Wall* w)
	{
		Walls.push_back(w);
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
		if (this->width < epsilon)
		{
			return false;
		}

		if (!this->isLeaf || this->status != MIXED)
		{
			return false; 
		}

		//record the time of this split event, will be used to set priority of children
		++Box::counter;

		Box* children[8];

		children[0] = new Box(x - width / 4, y + width / 4, z - width / 4, width / 2);
		children[1] = new Box(x + width / 4, y + width / 4, z - width / 4, width / 2);
		children[2] = new Box(x + width / 4, y + width / 4, z + width / 4, width / 2);
		children[3] = new Box(x - width / 4, y + width / 4, z + width / 4, width / 2);
		
		children[4] = new Box(x - width / 4, y - width / 4, z - width / 4, width / 2);
		children[5] = new Box(x + width / 4, y - width / 4, z - width / 4, width / 2);
		children[6] = new Box(x + width / 4, y - width / 4, z + width / 4, width / 2);
		children[7] = new Box(x - width / 4, y - width / 4, z + width / 4, width / 2);

		for (int i = 0; i < 8; ++i)
		{
			children[i]->depth = this->depth + 1;
			
			// add pointers to siblings
			switch (i)
			{
				case 0:
					children[i]->pChildren[1] = children[4];
					children[i]->pChildren[3] = children[1];
					children[i]->pChildren[4] = children[3];
					break;
				case 1:
					children[i]->pChildren[1] = children[5];
					children[i]->pChildren[2] = children[0];
					children[i]->pChildren[4] = children[3];
					break;
				case 2:
					children[i]->pChildren[1] = children[6];
					children[i]->pChildren[2] = children[3];
					children[i]->pChildren[5] = children[1];
					break;
				case 3:
					children[i]->pChildren[1] = children[7];
					children[i]->pChildren[3] = children[2];
					children[i]->pChildren[5] = children[0];
					break;
				case 4:
					children[i]->pChildren[0] = children[0];
					children[i]->pChildren[3] = children[5];
					children[i]->pChildren[4] = children[7];
					break;
				case 5:
					children[i]->pChildren[0] = children[1];
					children[i]->pChildren[2] = children[4];
					children[i]->pChildren[4] = children[6];
					break;
				case 6:
					children[i]->pChildren[0] = children[2];
					children[i]->pChildren[2] = children[7];
					children[i]->pChildren[5] = children[5];
					break;
				case 7: 
					children[i]->pChildren[0] = children[5];
					children[i]->pChildren[3] = children[6];
					children[i]->pChildren[5] = children[4];
			}
		}

		for (int i = 0; i < 6; ++i)
		{
			//init box neighbor iterator for direction i
			BoxIter* iter = new BoxIter(this, i);
			Box* neighbor = iter->First();

			if (!neighbor)
			{ 
				continue;
			}
			switch (i)
			{
				case 0:
					while (neighbor != iter->End())
					{ 
						if (!isOverLimit(children[0], neighbor) || !isOverLimit(neighbor, children[0]))
						{
							if (children[0]->depth >= neighbor->depth) // if neighbor is larger...
							{ 
								children[0]->pChildren[i] = neighbor;
								if (children[0]->depth == neighbor->depth) neighbor->pChildren[1] = children[0];
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (temp->depth != children[0]->depth) temp = temp->pParent;
								children[0]->pChildren[i] = temp;
								neighbor->pChildren[1] = children[0];
							}
						}
						if (!isOverLimit(children[1], neighbor) || !isOverLimit(neighbor, children[1]))
						{
							if (children[1]->depth >= neighbor->depth)
							{ 
								children[1]->pChildren[i] = neighbor;
								if (children[1]->depth == neighbor->depth) neighbor->pChildren[1] = children[1]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[1]->depth)) temp = temp->pParent;
								children[1]->pChildren[i] = temp;
								neighbor->pChildren[1] = children[1]; 
							}
						}
						if (!isOverLimit(children[2], neighbor) || !isOverLimit(neighbor, children[2]))
						{
							if (children[2]->depth >= neighbor->depth)
							{ 
								children[2]->pChildren[i] = neighbor;
								if (children[2]->depth == neighbor->depth) neighbor->pChildren[1] = children[2]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[2]->depth)) temp = temp->pParent;
								children[2]->pChildren[i] = temp;
								neighbor->pChildren[1] = children[2]; 
							}
						}
						if (!isOverLimit(children[3], neighbor) || !isOverLimit(neighbor, children[3]))
						{
							if (children[3]->depth >= neighbor->depth)
							{ 
								children[3]->pChildren[i] = neighbor;
								if (children[3]->depth == neighbor->depth) neighbor->pChildren[1] = children[3]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[3]->depth)) temp = temp->pParent;
								children[3]->pChildren[i] = temp;
								neighbor->pChildren[1] = children[3]; 
							}
						}

						neighbor = iter->Next();
					}
					break;
				case 1:
					while (neighbor != iter->End())
					{
						if (!isOverLimit(children[4], neighbor) || !isOverLimit(neighbor, children[4]))
						{
							if (children[4]->depth >= neighbor->depth)
							{ 
								children[4]->pChildren[i] = neighbor;
								if (children[4]->depth == neighbor->depth) neighbor->pChildren[0] = children[4]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[4]->depth)) temp = temp->pParent;
								children[4]->pChildren[i] = temp;
								neighbor->pChildren[0] = children[4]; 
							}
						}
						if (!isOverLimit(children[5], neighbor) || !isOverLimit(neighbor, children[5]))
						{
							if (children[5]->depth >= neighbor->depth)
							{ 
								children[5]->pChildren[i] = neighbor;
								if (children[5]->depth == neighbor->depth) neighbor->pChildren[0] = children[5]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[5]->depth)) temp = temp->pParent;
								children[5]->pChildren[i] = temp;
								neighbor->pChildren[0] = children[5]; 
							}
						}
						if (!isOverLimit(children[6], neighbor) || !isOverLimit(neighbor, children[6]))
						{
							if (children[6]->depth >= neighbor->depth)
							{ 
								children[6]->pChildren[i] = neighbor;
								if (children[6]->depth == neighbor->depth) neighbor->pChildren[0] = children[6]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[6]->depth)) temp = temp->pParent;
								children[6]->pChildren[i] = temp;
								neighbor->pChildren[0] = children[6]; 
							}
						}
						if (!isOverLimit(children[7], neighbor) || !isOverLimit(neighbor, children[7]))
						{
							if (children[7]->depth >= neighbor->depth)
							{ 
								children[7]->pChildren[i] = neighbor;
								if (children[7]->depth == neighbor->depth) neighbor->pChildren[0] = children[7]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[7]->depth)) temp = temp->pParent;
								children[7]->pChildren[i] = temp;
								neighbor->pChildren[0] = children[7]; 
							}
						}

						neighbor = iter->Next();
					}
					break;
				case 2:
					while (neighbor != iter->End())
					{
						if (!isOverLimit(children[0], neighbor) || !isOverLimit(neighbor, children[0]))
						{
							if (children[0]->depth >= neighbor->depth)
							{ 
								children[0]->pChildren[i] = neighbor;
								if (children[0]->depth == neighbor->depth) neighbor->pChildren[3] = children[0]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[0]->depth)) temp = temp->pParent;
								children[0]->pChildren[i] = temp;
								neighbor->pChildren[3] = children[0]; 
							}
						}
						if (!isOverLimit(children[3], neighbor) || !isOverLimit(neighbor, children[3]))
						{
							if (children[3]->depth >= neighbor->depth)
							{ 
								children[3]->pChildren[i] = neighbor;
								if (children[3]->depth == neighbor->depth) neighbor->pChildren[3] = children[3]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[3]->depth)) temp = temp->pParent;
								children[3]->pChildren[i] = temp;
								neighbor->pChildren[3] = children[3]; 
							}
						}
						if (!isOverLimit(children[4], neighbor) || !isOverLimit(neighbor, children[4]))
						{
							if (children[4]->depth >= neighbor->depth)
							{ 
								children[4]->pChildren[i] = neighbor;
								if (children[4]->depth == neighbor->depth) neighbor->pChildren[3] = children[4]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[4]->depth)) temp = temp->pParent;
								children[4]->pChildren[i] = temp;
								neighbor->pChildren[3] = children[4]; 
							}
						}
						if (!isOverLimit(children[7], neighbor) || !isOverLimit(neighbor, children[7]))
						{
							if (children[7]->depth >= neighbor->depth)
							{ 
								children[7]->pChildren[i] = neighbor;
								if (children[7]->depth == neighbor->depth) neighbor->pChildren[3] = children[7]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[7]->depth)) temp = temp->pParent;
								children[7]->pChildren[i] = temp;
								neighbor->pChildren[3] = children[7]; 
							}
						}
						neighbor = iter->Next();
					}
					break;
				case 3:
					while (neighbor != iter->End())
					{
						if (!isOverLimit(children[1], neighbor) || !isOverLimit(neighbor, children[1]))
						{
							if (children[1]->depth >= neighbor->depth)
							{ 
								children[1]->pChildren[i] = neighbor;
								if (children[1]->depth == neighbor->depth) neighbor->pChildren[2] = children[1]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[1]->depth)) temp = temp->pParent;
								children[1]->pChildren[i] = temp;
								neighbor->pChildren[2] = children[1]; 
							}
						}
						if (!isOverLimit(children[2], neighbor) || !isOverLimit(neighbor, children[2]))
						{
							if (children[2]->depth >= neighbor->depth)
							{ 
								children[2]->pChildren[i] = neighbor;
								if (children[2]->depth == neighbor->depth) neighbor->pChildren[2] = children[2]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[2]->depth)) temp = temp->pParent;
								children[2]->pChildren[i] = temp;
								neighbor->pChildren[2] = children[2]; 
							}
						}
						if (!isOverLimit(children[5], neighbor) || !isOverLimit(neighbor, children[5]))
						{
							if (children[5]->depth >= neighbor->depth)
							{ 
								children[5]->pChildren[i] = neighbor;
								if (children[5]->depth == neighbor->depth) neighbor->pChildren[2] = children[5]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[5]->depth)) temp = temp->pParent;
								children[5]->pChildren[i] = temp;
								neighbor->pChildren[2] = children[5]; 
							}
						}
						if (!isOverLimit(children[6], neighbor) || !isOverLimit(neighbor, children[6]))
						{
							if (children[6]->depth >= neighbor->depth)
							{ 
								children[6]->pChildren[i] = neighbor;
								if (children[6]->depth == neighbor->depth) neighbor->pChildren[2] = children[6]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[6]->depth)) temp = temp->pParent;
								children[6]->pChildren[i] = temp;
								neighbor->pChildren[2] = children[6]; 
							}
						}
						neighbor = iter->Next();						
					}					
					break;
				case 4:
					while (neighbor != iter->End())
					{
						if (!isOverLimit(children[2], neighbor) || !isOverLimit(neighbor, children[2]))
						{
							if (children[2]->depth >= neighbor->depth)
							{ 
								children[2]->pChildren[i] = neighbor;
								if (children[2]->depth == neighbor->depth) neighbor->pChildren[5] = children[2]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[2]->depth)) temp = temp->pParent;
								children[2]->pChildren[i] = temp;
								neighbor->pChildren[5] = children[2]; 
							}
						}
						if (!isOverLimit(children[3], neighbor) || !isOverLimit(neighbor, children[3]))
						{
							if (children[3]->depth >= neighbor->depth)
							{ 
								children[3]->pChildren[i] = neighbor;
								if (children[3]->depth == neighbor->depth) neighbor->pChildren[5] = children[3]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[3]->depth)) temp = temp->pParent;
								children[3]->pChildren[i] = temp;
								neighbor->pChildren[5] = children[3]; 
							}
						}
						if (!isOverLimit(children[6], neighbor) || !isOverLimit(neighbor, children[6]))
						{
							if (children[6]->depth >= neighbor->depth)
							{ 
								children[6]->pChildren[i] = neighbor;
								if (children[6]->depth == neighbor->depth) neighbor->pChildren[5] = children[6]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[6]->depth)) temp = temp->pParent;
								children[6]->pChildren[i] = temp;
								neighbor->pChildren[5] = children[6]; 
							}
						}
						if (!isOverLimit(children[7], neighbor) || !isOverLimit(neighbor, children[7]))
						{
							if (children[7]->depth >= neighbor->depth)
							{ 
								children[7]->pChildren[i] = neighbor;
								if (children[7]->depth == neighbor->depth) neighbor->pChildren[5] = children[7]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[7]->depth)) temp = temp->pParent;
								children[7]->pChildren[i] = temp;
								neighbor->pChildren[5] = children[7]; 
							}
						}

						neighbor = iter->Next();
					}
					break;
				case 5:
					while (neighbor != iter->End())
					{
						if (!isOverLimit(children[0], neighbor) || !isOverLimit(neighbor, children[0]))
						{
							if (children[0]->depth >= neighbor->depth)
							{ 
								children[0]->pChildren[i] = neighbor;
								if (children[0]->depth == neighbor->depth) neighbor->pChildren[4] = children[0]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[0]->depth)) temp = temp->pParent;
								children[0]->pChildren[i] = temp;
								neighbor->pChildren[4] = children[0]; 
							}
						}
						if (!isOverLimit(children[1], neighbor) || !isOverLimit(neighbor, children[1]))
						{
							if (children[1]->depth >= neighbor->depth)
							{ 
								children[1]->pChildren[i] = neighbor;
								if (children[1]->depth == neighbor->depth) neighbor->pChildren[4] = children[1]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[1]->depth)) temp = temp->pParent;
								children[1]->pChildren[i] = temp;
								neighbor->pChildren[4] = children[1]; 
							}
						}
						if (!isOverLimit(children[4], neighbor) || !isOverLimit(neighbor, children[4]))
						{
							if (children[4]->depth >= neighbor->depth)
							{ 
								children[4]->pChildren[i] = neighbor;
								if (children[4]->depth == neighbor->depth) neighbor->pChildren[4] = children[4]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[4]->depth)) temp = temp->pParent;
								children[4]->pChildren[i] = temp;
								neighbor->pChildren[4] = children[4]; 
							}
						}
						if (!isOverLimit(children[5], neighbor) || !isOverLimit(neighbor, children[5]))
						{
							if (children[5]->depth >= neighbor->depth)
							{ 
								children[5]->pChildren[i] = neighbor;
								if (children[5]->depth == neighbor->depth) neighbor->pChildren[4] = children[5]; 
							}
							else // neighbor is smaller, so we need to point to its ancestor of equal depth
							{
								Box* temp = neighbor;
								while (!(temp->depth == children[5]->depth)) temp = temp->pParent;
								children[5]->pChildren[i] = temp;
								neighbor->pChildren[4] = children[5]; 
							}
						}
						neighbor = iter->Next();
					}
					break;
			}
		}

		for (int i = 0; i < 8; ++i)
		{
			this->pChildren[i] = children[i];
			this->pChildren[i]->pParent = this;
			//add all of parent's Walls, Edges, and corners to each child,
			//will be filtered later in updatestatus()
			this->pChildren[i]->Walls.insert(this->pChildren[i]->Walls.begin(), this->Walls.begin(), this->Walls.end()); 
			this->pChildren[i]->Edges.insert(this->pChildren[i]->Edges.begin(), this->Edges.begin(), this->Edges.end());
			this->pChildren[i]->corners.insert(
				this->pChildren[i]->corners.begin(),
				this->corners.begin(), this->corners.end() );

		}
		this->isLeaf = false;

		return true;
	}

};//class Box
