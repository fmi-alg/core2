/* **************************************
   File: QuadTree.h

   Description:

    The main routines here are:
        expand()  -- this keeps expanding mixed boxes
                until the priority queue is empty
                or a path is found

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#pragma once

#include <iostream>

#include "Box.h"
#include "UnionFind.h"
#include "PriorityQueue.h"

extern vector<Box*> allLeaf;
extern vector<Set*> allSet;

extern int freeCount;
extern int stuckCount;
extern int mixCount;
extern int mixSmallCount;

//extern FILE *fptr;

class QuadTree
{
private:
    void insertNode(Box* b)
    {
        switch (b->getStatus())
        {
        case Box::FREE:
            {
                Set* st = new Set(b);
                allSet.push_back(st);
            }
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

    QuadTree (Box* root, double e, int qType):
        pRoot(root), epsilon(e), QType(qType)
    {
        switch (QType)
        {
        case 1:
            PQ = new seqQueue();
            break;
        case 0:
            PQ = new randQueue();
            break;
        case 2:
            PQ = new dijkstraQueue<DistCmp>();
            break;
        case 3:
            PQ = new dijkstraQueue<DistPlusSizeCmp>();
            break;
        case 4:
            PQ = new dijkstraQueue<VorCmp>();
            break;
        }

        //PQ = new randQueue();

        pRoot->updateStatus();
        insertNode(pRoot);
    }

    //bfs search for alpha/beta
    Box* getBox (double x, double y, double a, int& ct) {
        std::queue<Box*> q;

        for (int i = 0; i < (int)allLeaf.size(); ++i) {
            if ( allLeaf[i]->contains(x, y, a) ) {
                if (allLeaf[i]->isFree()) {
                    return allLeaf[i];
                }
                if (allLeaf[i]->status == Box::STUCK) {
                    return 0;
                }
                q.push(allLeaf[i]);
            }
        }

        while (q.size()) {
            Box* b = q.front();
            q.pop();

            if (!b->isLeaf) {
                continue;
            }
            if (!b->contains(x, y, a)) {
                return 0; // STUCK
            }

            vector<Box*> cldrn;
            if (!expand(b, cldrn)) {
                return 0; // STUCK
            }
            ++ct;

            for (int i = 0; i < (int)cldrn.size(); ++i) {
                if ( cldrn[i]->contains(x, y, a) ) {
                    if (cldrn[i]->isFree()) {
                        return cldrn[i];
                    }
                    if (cldrn[i]->status == Box::STUCK) {
                        return 0;
                    }
                    q.push(cldrn[i]);
                }
            }

        }
        return 0;
    }

    bool expand (Box* b)
    {
        vector<Box*> cldrn;
        return expand(b, cldrn);
    }

    //expand and put children ptr in cldrn
    bool expand (Box* b, vector<Box*>& cldrn)
    {
        if (!b->split(epsilon, cldrn))
        {
            return false;
        }

        for (int i = 0; i < (int)cldrn.size(); ++i)
        {
            cldrn[i]->updateStatus();
            insertNode(cldrn[i]);
        }

        return true;
    }

    bool expand ()
    {
        while(!PQ->empty())
        {
            Box* b = PQ->extract();
            if (!b->isLeaf)
            {
                continue;
            }

            vector<Box*> cldrn;

            //b might not be a leaf since it could already be split in expand(Box* b), and PQ is not updated there
            if (b->split(epsilon, cldrn))
            {
                assert(b->status == Box::MIXED);

                for (int i = 0; i < (int)cldrn.size(); ++i)
                {
                    cldrn[i]->updateStatus();
                    insertNode(cldrn[i]);
                }
                return true;
            }
        }
        return false;
    }

    bool isConnected (Box* a, Box* b)
    {
        if (pSets->Find(a) == pSets->Find(b))
        {
            return true;
        }
        return false;
    }

    void unionAdjacent (Box* b)
    {
        for (int i = 0; i < 6; ++i)
        {
            for (vector<Box*>::iterator it = b->Nhbrs[i].begin(); it != b->Nhbrs[i].end(); ++it)
            {
                Box* neighbor = *it;
                if (neighbor->status == Box::FREE)
                {
                    pSets->Union(b, neighbor);
                }
            }
        }
    }

    ~QuadTree (void)
    {
    }
};
