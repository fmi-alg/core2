#include <iostream>
#include "Box.h"
#include <assert.h>

using namespace std;

BoxIter::BoxIter(const Box* bb, int direc):b(bb), direction(direc)
{
	neighborVec.clear();
	index = 1;

	Box* n = b->pChildren[direction];
	if (!n)
	{
		neighborVec.push_back(0);
	}

	else if (n->isLeaf) 
	{
		neighborVec.push_back(n);
	}
	
	// n is an internal node, and we need to recursively add the relevant leaves to neighborVec	
	else
	{		
		storeNeighbors(n);
	}
	neighborVec.push_back(new Box(0,0,0,0));
}

double Box::r0 = 0;
int Box::counter = 0;

int BoxIter::size()
{
	return neighborVec.size();
}

int BoxIter::getIndex()
{
	return index;
}

Box* BoxIter::First()
{
	return neighborVec[0];
}

Box* BoxIter::End()
{
	return neighborVec[neighborVec.size()-1];
}

Box* BoxIter::Next()
{
	Box* next = neighborVec[index];
	index += 1;
	return next;
}

// recursively store neighbors in neighborVec
void BoxIter::storeNeighbors (Box* n)
{
	switch (direction)
	{
		case 0:
			if (n->isLeaf)
			{ 
				neighborVec.push_back(n);
			}
			else 
			{
				storeNeighbors(n->pChildren[4]);
				storeNeighbors(n->pChildren[5]);
				storeNeighbors(n->pChildren[6]);
				storeNeighbors(n->pChildren[7]);
			}
			break;
		case 1:
			if (n->isLeaf) neighborVec.push_back(n);
			else 
			{
				storeNeighbors(n->pChildren[0]);
				storeNeighbors(n->pChildren[1]);
				storeNeighbors(n->pChildren[2]);
				storeNeighbors(n->pChildren[3]);
			}
			break;
		case 2:
			if (n->isLeaf) neighborVec.push_back(n);
			else 
			{
				storeNeighbors(n->pChildren[1]);
				storeNeighbors(n->pChildren[2]);
				storeNeighbors(n->pChildren[5]);
				storeNeighbors(n->pChildren[6]);
			}
			break;
		case 3:
			if (n->isLeaf) neighborVec.push_back(n);
			else 
			{
				storeNeighbors(n->pChildren[0]);
				storeNeighbors(n->pChildren[3]);
				storeNeighbors(n->pChildren[4]);
				storeNeighbors(n->pChildren[7]);
			}
			break;
		case 4:
			if (n->isLeaf) neighborVec.push_back(n);
			else 
			{
				storeNeighbors(n->pChildren[0]);
				storeNeighbors(n->pChildren[1]);
				storeNeighbors(n->pChildren[4]);
				storeNeighbors(n->pChildren[5]);
			}
			break;
		case 5:
			if (n->isLeaf) neighborVec.push_back(n);
			else 
			{
				storeNeighbors(n->pChildren[2]);
				storeNeighbors(n->pChildren[3]);
				storeNeighbors(n->pChildren[6]);
				storeNeighbors(n->pChildren[7]);
			}
	}
}
