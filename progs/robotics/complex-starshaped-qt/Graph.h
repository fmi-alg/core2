#pragma once

#include <vector>
#include <algorithm>
#include <math.h>
#include "Box.h"
#include "QuadTree.h"
#include <queue>
#include <iostream>

using namespace std;

extern double beta[3];

// Greedy search strategy uses this for computing the final path from alpha to beta:
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
    CmpFunctor cmp;

    void siftDown(vector<Box*>& bv, int i)
    {
        //CmpFunctor cmp;
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
    void makeHeap(vector<Box*>& bv)
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

    void insert(vector<Box*>& bv, Box* b)
    {
        //CmpFunctor cmp;
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

    void decreaseKey(vector<Box*>& bv, Box* b, double dist)
    {
        //CmpFunctor cmp;
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

    Box* extractMin(vector<Box*>& bv)
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
class VorCmp
{
public:
    bool operator() (const Box* a, const Box* b)
    {
        //double distDiff = (a->x - beta[0])*(a->x - beta[0]) + (a->y - beta[1])*(a->y - beta[1])
        //	- ((b->x - beta[0])*(b->x - beta[0]) + (b->y - beta[1])*(b->y - beta[1]));
        //return distDiff > 0;

        ////use c*d(B) + 1 / w(B) as the measure, instead of just d(B)
        //double distDiff = sqrt((a->x - beta[0])*(a->x - beta[0]) + (a->y - beta[1])*(a->y - beta[1]))
        //	- sqrt(((b->x - beta[0])*(b->x - beta[0]) + (b->y - beta[1])*(b->y - beta[1])));
        //double wDiff = 1 / a->width - 1 / b->width;
        //double c = 0.0001;
        //return c * distDiff + wDiff > 0;

        //if #phi(B)=1 then the priority is zero.   We really do not want to expand such boxes.
        //If #phi(B) is big, then we like to expand such boxes because they tend to lie on the Voronoi edges.
        double dista = sqrt((a->x - beta[0])*(a->x - beta[0]) + (a->y - beta[1])*(a->y - beta[1]));
        double distb = sqrt(((b->x - beta[0])*(b->x - beta[0]) + (b->y - beta[1])*(b->y - beta[1])));
        return (a->vorCorners.size() + a->vorWalls.size() - 1) / dista < (b->vorCorners.size() + b->vorWalls.size() - 1) / distb;
    }
};

class DistCmp
{
public:
    bool operator() (const Box* a, const Box* b)
    {
        double distDiff = (a->x - beta[0])*(a->x - beta[0]) + (a->y - beta[1])*(a->y - beta[1])
            - ((b->x - beta[0])*(b->x - beta[0]) + (b->y - beta[1])*(b->y - beta[1]));
        return distDiff > 0;
    }
};

class DistPlusSizeCmp
{
public:
    bool operator() (const Box* a, const Box* b)
    {
        //use c*d(B) + 1 / w(B) as the measure, instead of just d(B)
        double distDiff = sqrt((a->x - beta[0])*(a->x - beta[0]) + (a->y - beta[1])*(a->y - beta[1]))
            - sqrt(((b->x - beta[0])*(b->x - beta[0]) + (b->y - beta[1])*(b->y - beta[1])));
        double wDiff = 1 / a->width - 1 / b->width;
        double c = 0.0001;
        return c * distDiff + wDiff > 0;
    }
};

class Graph
{
private:
    distHeap<distCmp> dist_heap;

public:
        // returns a vector of boxes representing a path from a to b:
    vector<Box*> dijkstraShortestPath(Box* a, Box* b)
    {
        a->dist2Source = 0;
        vector<Box*> bv;
        dist_heap.insert(bv, a);
        while(bv.size())
        {
            Box* current = dist_heap.extractMin(bv);
            current->visited = true;
            if (current == b)
            {
                break;
            }
            for (int i = 0; i < 6; ++i)
            {
                for (vector<Box*>::iterator it = current->Nhbrs[i].begin(); it != current->Nhbrs[i].end(); ++it)
                {
                    Box* neighbor = *it;
                    if (!neighbor->visited && neighbor->status == Box::FREE)
                    {
                        double dist2pre = sqrt( (current->x - neighbor->x)*(current->x - neighbor->x) + (current->y - neighbor->y)*(current->y - neighbor->y) );
                        double dist2src = dist2pre + current->dist2Source;

                        if (neighbor->dist2Source == -1)
                        {
                            neighbor->prev = current;
                            neighbor->dist2Source = dist2src;
                            dist_heap.insert(bv, neighbor);
                        }
                        else
                        {
                            if (neighbor->dist2Source > dist2src)
                            {
                                neighbor->prev = current;
                                dist_heap.decreaseKey(bv, neighbor, dist2src);
                            }
                        }
                    }
                }
            }
        }

        vector<Box*> path;
        path.push_back(b);
        while (path.back()->prev) {
            path.push_back(path.back()->prev);
        }
        path.push_back(a);
        reverse(path.begin(), path.end());

        return path;
    }

    static int bfsPath(Box* a, Box* b)
    {
        std::queue<Box*> q;
        q.push(a);
        bool foundB = false;
        while(q.size())
        {
            Box* current = q.front();
            q.pop();
            if (current == b)
            {
                foundB = true;
                break;
            }
            for (int i = 0; i < 6; ++i)
            {
                for (vector<Box*>::iterator it = current->Nhbrs[i].begin(); it != current->Nhbrs[i].end(); ++it)
                {
                    Box* neighbor = *it;
                    if (!neighbor->visited && neighbor->status == Box::FREE)
                    {
                        q.push(neighbor);
                    }
                }
            }
            current->visited = true;
        }

        std::cout<< "bfs found? " << foundB << endl;

        return 0;
    }

};
