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
		if (a->depth > b->depth)
		{
			return true;
		}
		//if same depth, expand box created earlier first
		else if (a->depth == b->depth)
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

	virtual int size() = 0;

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

	int size()
	{
		return PQ.size();
	}
};

class randQueue : public BoxQueue
{
private:
	list<Box*> L;
	int Qseed;

public:
	randQueue(int s): Qseed(s) {
		//srand( time(0) );
		srand( Qseed ); 
	}

	void push(Box* b)
	{
		L.push_back(b);
	}

	Box* extract()
	{
		int i = rand() % L.size();
		list<Box*>::iterator iter = L.begin();
		advance(iter, i);
		Box* r = *iter;
		L.erase(iter);
		return r;
	}

	bool empty()
	{
		return L.empty();
	}

	int size()
	{
		return L.size();
	}

};

class dijkstraQueue : public BoxQueue
{
private:
	vector<Box*> bv;

public:

	void push(Box* b)
	{
		distHeap<PQCmp3>::insert(bv, b);
	}

	Box* extract()
	{
		Box* current = distHeap<PQCmp3>::extractMin(bv);
		return current;
	}

	bool empty()
	{
		return bv.empty();
	}

	int size()
	{
		return bv.size();
	}
};