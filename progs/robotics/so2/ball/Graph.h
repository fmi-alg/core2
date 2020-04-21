#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include "Box.h"
#include "Octree.h"

using namespace std;

extern double beta[3];

class distCmp
{
public:
	bool operator()(Box* a, Box* b)
	{
		return a->dist2Source > b->dist2Source;
	}
};


// min heap
template <typename CmpFunctor>
class distHeap
{
private:
	static void siftDown(vector<Box*>& bv, int i)
	{
		CmpFunctor cmp;
		unsigned int l = 2*i + 1;
		unsigned int r = 2*i + 2;
		int smallest;
		if (l < bv.size() && cmp(bv[i], bv[l]))
		{
			smallest = l;
		}
		else
		{
			smallest = i;
		}
		if (r < bv.size() && cmp(bv[smallest], bv[r]))
		{
			smallest = r;
		}
		if (smallest != i)
		{
			Box* tmp = bv[smallest];
			bv[smallest] = bv[i];
			bv[i] = tmp;
			bv[smallest]->heapId = smallest;
			bv[i]->heapId = i;

			siftDown(bv, smallest);
		}

	}

public:
	static void makeHeap(vector<Box*>& bv)
	{
		if (bv.size() <= 1)
		{
			return;
		}
		for (int i = 0; i < bv.size(); ++i)
		{
			bv[i]->heapId = i;
		}
		for (int i = (bv.size() -2) / 2; i >= 0; --i)
		{
			siftDown(bv, i);
		}
	}

	static void insert(vector<Box*>& bv, Box* b)
	{
		CmpFunctor cmp;
		bv.push_back(b);
		int bid = bv.size() - 1;
		b->heapId = bid;
		int pid = (bid - 1) / 2;
		while (bid > 0 && cmp(bv[pid], bv[bid]))
		{
			Box* tmp = bv[bid];
			bv[bid] = bv[pid];
			bv[pid] = tmp;
			bv[bid]->heapId = bid;
			bv[pid]->heapId = pid;

			bid = pid;
			pid = (bid - 1) / 2;
		}
	}

	static void decreaseKey(vector<Box*>& bv, Box* b, double dist)
	{
		CmpFunctor cmp;
		assert(bv[b->heapId] == b);
		assert(b->dist2Source > dist);

		b->dist2Source = dist;
		int bid = b->heapId;
		int pid = (bid - 1) / 2;
		while (bid > 0 && cmp(bv[pid], bv[bid]))
		{
			Box* tmp = bv[bid];
			bv[bid] = bv[pid];
			bv[pid] = tmp;
			bv[bid]->heapId = bid;
			bv[pid]->heapId = pid;

			bid = pid;
			pid = (bid - 1) / 2;
		}
	}

	static Box* extractMin(vector<Box*>& bv)
	{
		Box* minB = bv[0];
		bv[0] = bv.back();
		bv[0]->heapId = 0;
		minB->heapId = -1;
		bv.pop_back();
		siftDown(bv, 0);
		return minB;
	}


};

//won't work with std pq, as this comparison is not transitional!
class PQCmp3
{
public:
	bool operator() (const Box* a, const Box* b)
	{
		//if depth diff bigger than 3, use depth as priority
		//if (abs(a->depth - b->depth) > 8)
		//{
		//	return a->depth > b->depth;
		//}
		//otherwise expand box closer to beta
		double distDiff = (a->x - beta[0])*(a->x - beta[0]) + (a->y - beta[1])*(a->y - beta[1]) + (a->z - beta[2])*(a->z - beta[2])
			- ((b->x - beta[0])*(b->x - beta[0]) + (b->y - beta[1])*(b->y - beta[1]) + (b->z - beta[2])*(b->z - beta[2]));
		return distDiff > 0;	
	}
};

class Graph
{
public:
	static vector<Box*> dijkstraShortestPath(Box* a, Box* b)
	{
		a->dist2Source = 0;
		vector<Box*> bv;
		distHeap<distCmp>::insert(bv, a);
		while(bv.size())
		{
			Box* current = distHeap<distCmp>::extractMin(bv);
			current->visited = true;
			if (current == b)
			{			
				break;
			}

			for (int i = 0; i < 6; ++i)
			{
				BoxIter* iter = new BoxIter(current, i);
				Box* neighbor = iter->First();

				while (neighbor && neighbor != iter->End())
				{
					if (!neighbor->visited && neighbor->status == Box::FREE)
					{
						double dist2pre = sqrt( (current->x - neighbor->x)*(current->x - neighbor->x) + (current->y - neighbor->y)*(current->y - neighbor->y) + (current->z - neighbor->z)*(current->z - neighbor->z) );
						double dist2src = dist2pre + current->dist2Source;

						if (neighbor->dist2Source == -1)
						{
							neighbor->prev = current;
							neighbor->dist2Source = dist2src;
							distHeap<distCmp>::insert(bv, neighbor);
						}
						else
						{
							if (neighbor->dist2Source > dist2src)
							{
								neighbor->prev = current;
								distHeap<distCmp>::decreaseKey(bv, neighbor, dist2src);
							}
						}
					}
					neighbor = iter->Next();
				}
			}
		}

		vector<Box*> path;
		path.push_back(b);
		while (path.back()->prev)
		{
			path.push_back(path.back()->prev);
		}
		return path;
	}

};
