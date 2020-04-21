#pragma once
#include "Box.h"
#include <queue>
#include <vector>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <iterator>
#include "Graph.h"
#include <math.h>

using namespace std;

class PQCmp
{
public:
    bool operator() (const Box* a, const Box* b)
    {
        //use depth for now
        //if (a->depth > b->depth)
        if (a->width < b->width)
        {
            return true;
        }
        //if same depth, expand box created earlier first
        else if (a->width == b->width)
        {
            return a->priority > b->priority;
        }
        return false;
    }
};


class BoxQueue
{
private:

public:

    BoxQueue(void)
    {
    }

    virtual void push(Box* b) = 0;

    virtual Box* extract() = 0;

    virtual bool empty() = 0;

    ~BoxQueue(void)
    {
    }
};

class seqQueue : public BoxQueue
{
private:
    priority_queue<Box*, vector<Box*>, PQCmp> PQ;
public:
    void push(Box* b)
    {
        PQ.push(b);
    }

    Box* extract()
    {
        Box* r = PQ.top();
        PQ.pop();
        return r;
    }

    bool empty()
    {
        return PQ.empty();
    }
};

class randQueue : public BoxQueue
{
private:
    vector<Box*> L;
    vector<bool> dirty;
    int dirtyCt;

public:
    randQueue(): dirtyCt(0)
    {}

    void push(Box* b)
    {
        L.push_back(b);
        dirty.push_back(false);
    }

    Box* extract()
    {
        int i;
        do
        {
            i = rand() % L.size();
        }
        while (dirty[i]);

        Box* r = L[i];
        dirty[i] = true;
        ++dirtyCt;
        if ((unsigned) dirtyCt >= dirty.size() / 2)
        {
            vector<Box*> L2;
            vector<bool> dirty2;
            for (i = 0; (unsigned)i < L.size(); ++i)
            {
                if (!dirty[i])
                {
                    L2.push_back(L[i]);
                    dirty2.push_back(false);
                }
            }
            L.swap(L2);
            dirty.swap(dirty2);
            dirtyCt = 0;
        }
        return r;
    }

    bool empty()
    {
        return L.empty();
    }

};

template<typename Cmp>
class dijkstraQueue : public BoxQueue
{
private:
    vector<Box*> bv;
    distHeap<Cmp> dist_heap;

public:

    void push(Box* b)
    {
        dist_heap.insert(bv, b);
    }

    Box* extract()
    {
        Box* current = dist_heap.extractMin(bv);
        return current;
    }

    bool empty()
    {
        return bv.empty();
    }
};
