#pragma once

#include "subdivision/Box.h"
#include "sss/Graph.h"

#include <queue>
#include <vector>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <iterator>
#include <math.h>

using namespace std;

extern int SearchType;
extern bool moveAway;

enum PriorityQ{
  RANDOM, BFS, GREEDY, VORONOI, BIVORONOI
};

class BFSCmp
{
public:
  bool operator() (const Box* a, const Box* b){
    //use depth for now
    if (a->depth > b->depth){
      return true;
    }
    //if same depth, expand box created earlier first
    else if (a->depth == b->depth){
      return a->priority > b->priority;
    }
    return false;
  }
};


class BoxQueue
{
private:

public:
  BoxQueue(void){}

  virtual void push(Box* b) = 0;

  virtual Box* extract() = 0;

  virtual bool empty() = 0;

  virtual int size() = 0;

  ~BoxQueue(void){}
};

class seqQueue : public BoxQueue
{
private:
  priority_queue<Box*, vector<Box*>, BFSCmp> PQ;
public:
  void push(Box* b){
    PQ.push(b);
  }

  Box* extract(){
    Box* r = PQ.top();
    PQ.pop();
    return r;
  }

  bool empty(){
    return PQ.empty();
  }

  int size(){
    return PQ.size();
  }
};

class randQueue : public BoxQueue
{
private:
  list<Box*> L;

public:
  randQueue(){}

  void push(Box* b){
    L.push_back(b);
  }

  Box* extract(){
    int i = rand() % L.size();
    list<Box*>::iterator iter = L.begin();
    advance(iter, i);
    Box* r = *iter;
    L.erase(iter);
    return r;
  }

  bool empty(){
    return L.empty();
  }

  int size(){
    return L.size();
  }
};

template<typename Cmp>
class dijkstraQueue : public BoxQueue
{
private:
  vector<Box*> bv;

public:
  void push(Box* b){
    if(SearchType != PriorityQ::VORONOI && SearchType != PriorityQ::BIVORONOI)
      distHeap<Cmp>::insert(bv, b);
    else{
      if(moveAway || (b->corners_v.size()+b->walls_v.size() > 1)){
        distHeap<Cmp>::insert(bv, b);
      }
    }
  }

  Box* extract(){
    Box* current = distHeap<Cmp>::extractMin(bv);
    return current;
  }

  bool empty(){
    return bv.empty();
  }

  int size(){
    return bv.size();
  }
};
