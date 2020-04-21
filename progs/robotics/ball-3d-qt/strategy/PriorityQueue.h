#pragma once
#include "./subdivision/ConfBox3d.h"
#include <queue>
#include <vector>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <iterator>
#include "./sss/Graph.h"
#include <math.h>
#include <unordered_set>
#include "./sss/UnionFind.h"

using namespace std;

class PQCmp {
 public:
  bool operator() (const ConfBox3d* a, const ConfBox3d* b) {
    //use depth for now
    if (a->depth > b->depth) {
      return true;
    }
    //if same depth, expand box created earlier first
    if (a->depth == b->depth) {
      return a->boxId > b->boxId;
    }
    return false;
  }
};


class BoxQueue {
 public:

  BoxQueue(void) { }

  virtual void push(ConfBox3d* b) = 0;

  virtual ConfBox3d* extract() = 0;

  virtual bool empty() = 0;

  virtual int size() = 0;

  virtual void clear() = 0;

  virtual void process(ConfBox3d* b) {
    if (b->getStatus() == MIXED) {
      push(b);
    }
  }

  ~BoxQueue(void) { }
};

class SeqQueue : public BoxQueue {
 private:
  priority_queue<ConfBox3d*, vector<ConfBox3d*>, PQCmp> PQ;
 public:
  void push(ConfBox3d* b) {
    PQ.push(b);
  }

  ConfBox3d* extract() {
    ConfBox3d* r = PQ.top();
    PQ.pop();
    return r;
  }

  bool empty() {
    return PQ.empty();
  }

  int size() {
    return PQ.size();
  }

  void clear() {
    PQ = priority_queue<ConfBox3d*, vector<ConfBox3d*>, PQCmp>();
  }
};

class RandQueue : public BoxQueue
{
 private:
  list<ConfBox3d*> L;
  unsigned int Qseed;

 public:
 RandQueue(unsigned int s): Qseed(s) {
   srand( Qseed );
 }

  void push(ConfBox3d* b) {
    L.push_back(b);
  }

  ConfBox3d* extract() {
    unsigned long i = static_cast<unsigned long>(rand()) % L.size();
    list<ConfBox3d*>::iterator iter = L.begin();
    advance(iter, i);
    ConfBox3d* r = *iter;
    L.erase(iter);
    return r;
  }

  bool empty() {
    return L.empty();
  }

  int size() {
    return L.size();
  }

  void clear() {
    L.clear();
  }

};

class DijkstraQueue : public BoxQueue
{
 private:
  vector<ConfBox3d*> bv;
  double alpha[3];
  double beta[3];
  unordered_set<ConfBox3d*> sourceSet;
  unordered_set<ConfBox3d*> fringe;

  bool hasNeighborInSourceSet(ConfBox3d* b) {
    for (int i = 0; i < (int)b->neighbors.size(); i++) {
      ConfBox3d* n = b->neighbors[i];
      if (sourceSet.find(n) != sourceSet.end()) {
          /* b->prev = n; */
        return true;
      }
    }
    return false;
  }

  void recursiveAddToSourceSet(ConfBox3d* b) {
    sourceSet.insert(b);
    for (int i = 0; i < (int)b->neighbors.size(); i++) {
      ConfBox3d* n = b->neighbors[i];
      if (n->getStatus() == FREE) {
        if (sourceSet.find(n) == sourceSet.end()) {
          /* n->prev = b; */
          recursiveAddToSourceSet(n);
        }
      } else if (n->getStatus() == MIXED) {
        if (fringe.find(n) == fringe.end()) {
          fringe.insert(n);
          push(n);
        }
      }
    }
  }

 public:

  DijkstraQueue(double alpha[3], double beta[3]) {
    this->alpha[0] = alpha[0];
    this->alpha[1] = alpha[1];
    this->alpha[2] = alpha[2];

    this->beta[0] = beta[0];
    this->beta[1] = beta[1];
    this->beta[2] = beta[2];
  }


  void push(ConfBox3d* b) {
    distHeap<PQCmp3>::insert(bv, b);
  }

  ConfBox3d* extract() {
    ConfBox3d* b = distHeap<PQCmp3>::extractMin(bv);
    return b;
  }

  bool empty() {
    return bv.empty();
  }

  int size() {
    return bv.size();
  }

  void clear() {
    bv.clear();
  }

  void process(ConfBox3d* b) {
    if (b->getStatus() == FREE) {
      if ((sourceSet.empty() && b->getBox(alpha[0], alpha[1], alpha[2]) == b) ||
          hasNeighborInSourceSet(b)) {
        recursiveAddToSourceSet(b);
      }
    }
  }
};
