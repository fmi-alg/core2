/* **************************************
   File: QuadTree.h

   Description: 

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#pragma once
#include <iostream>
#include "Box.h"
#include "UnionFind.h"
#include "PriorityQueue.h"

extern int debug_counter;

extern int freeCount;
extern int stuckCount;
extern int mixCount;
extern int mixSmallCount;

class QuadTree
{
private:
	void insertNode(Box* b)
	{
		switch (b->getStatus())
		{
		case Box::FREE:
			new Set(b);
			unionAdjacent(b);
			++freeCount;
			break;
		case Box::STUCK:
			++stuckCount;
			break;
		case Box::MIXED:
			++mixCount;
			if (b->height < epsilon || b->width < epsilon)
				++mixSmallCount;
			PQ->push(b);
			break;
		case Box::UNKNOWN:
			std::cout << "UNKNOWN not handled?" << std::endl;
		}
	}

public:
	UnionFind* pSets;
	BoxQueue* PQ;
	Box* pRoot;
	double epsilon;
	int QType;
	int seed;

	QuadTree(Box* root, double e, int qType, int s):
	    pRoot(root), epsilon(e), QType(qType), seed(s)
	{
		switch (QType)
		{
		case 1:
			PQ = new seqQueue();
			break;
		case 0:
			PQ = new randQueue(s);
			break;
		case 2:
			PQ = new dijkstraQueue();
			break;
		}
		
		//PQ = new randQueue();

		pRoot->updateStatus();
		insertNode(pRoot);
	}

	Box* getBox(Box* root, double x, double y)
	{
		if (x > root->x + root->width / 2 || x < root->x - root->width / 2
			|| y > root->y + root->height / 2 || y < root->y - root->height / 2)
		{
			return 0;
		}

		Box* b = root;
		while (!b->isLeaf)
		{
			double dx = x - b->x;
			double dy = y - b->y;
			if (dx <= 0 && dy >= 0)
			{
				b = b->pChildren[0];
			}
			else if (dx >= 0 && dy >= 0)
			{
				b = b->pChildren[1];
			}
			else if (dx >= 0 && dy <= 0)
			{
				b = b->pChildren[2];
			}
			else if (dx <= 0 && dy <= 0)
			{
				b = b->pChildren[3];
			}
		}
		return b;
	}

	Box* getBox(double x, double y)
	{
		return getBox(pRoot, x, y);
	}

	bool expand (Box* b)
	{
		if (!b->split(epsilon))
		{
			return false;
		}
	
debug_counter++;
fprintf(stderr, "debug %d\n", debug_counter);

		for (int i = 0; i < 4; ++i)
		{
			b->pChildren[i]->updateStatus();
			insertNode(b->pChildren[i]);
		}

		return true;
	}

	bool expand ()
	{
		while(!PQ->empty())
		{
			Box* b = PQ->extract();
			//b might not be a leaf since it could already be split in expand(Box* b), and PQ is not updated there
			if (b->isLeaf && b->split(epsilon))
			{
				assert(b->status == Box::MIXED);
				for (int i = 0; i < 4; ++i)
				{
					b->pChildren[i]->updateStatus();
					insertNode(b->pChildren[i]);
				}			
				return true;
			}			
		}
		return false;
	}

	bool isConnect(Box* a, Box* b)
	{
		if (pSets->Find(a) == pSets->Find(b))
		{
			return true;
		}
		return false;
	}

	void unionAdjacent(Box* b)
	{
		for (int i = 0; i < 4; ++i)
		{
			BoxIter* iter = new BoxIter(b, i);
			Box* neighbor = iter->First();
			while(neighbor && neighbor != iter->End())
			{
				if (neighbor->status == Box::FREE)
				{
					pSets->Union(b, neighbor);
				}
				neighbor = iter->Next();
			}
		}
	}

	~QuadTree(void)
	{
	}
};
