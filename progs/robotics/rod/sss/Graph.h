#pragma once

#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "../subdivision/ConfBox3d.h"

using namespace std;

extern double beta[3];

class distCmp {
 public:
  bool operator()(ConfBox3d* a, ConfBox3d* b) {
    return a->dist2Source > b->dist2Source;
  }
};


// min heap
template <typename CmpFunctor>
class distHeap {
 private:
  static void siftDown(vector<ConfBox3d*>& bv, unsigned long i) {
    CmpFunctor cmp;
    unsigned int l = 2 * i + 1;
    unsigned int r = 2 * i + 2;
    unsigned long smallest = i;
    if (l < bv.size() && cmp(bv[i], bv[l])) {
      smallest = l;
    }
    if (r < bv.size() && cmp(bv[smallest], bv[r])) {
      smallest = r;
    }
    if (smallest != i) {
      ConfBox3d* tmp = bv[smallest];
      bv[smallest] = bv[i];
      bv[i] = tmp;
      bv[smallest]->heapId = smallest;
      bv[i]->heapId = i;

      siftDown(bv, smallest);
    }
  }

 public:
  static void makeHeap(vector<ConfBox3d*>& bv) {
    if (bv.size() <= 1) {
      return;
    }
    for (int i = 0; i < bv.size(); ++i) {
      bv[i]->heapId = i;
    }
    for (int i = (bv.size() -2) / 2; i >= 0; --i) {
      siftDown(bv, i);
    }
  }

  static void insert(vector<ConfBox3d*>& bv, ConfBox3d* b) {
    bv.push_back(b);
    int bid = bv.size() - 1;
    b->heapId = bid;
    decreaseKey(bv, b, b->dist2Source);
  }

  static void decreaseKey(vector<ConfBox3d*>& bv, ConfBox3d* b, double dist) {
    CmpFunctor cmp;
    assert(b->heapId >= 0);
    unsigned long bid = static_cast<unsigned long>(b->heapId);
    unsigned long pid = (bid - 1) / 2;
    assert(bv[bid] == b);
    assert(b->dist2Source >= dist);

    b->dist2Source = dist;
    while (bid > 0 && cmp(bv[pid], bv[bid])) {
      ConfBox3d* tmp = bv[bid];
      bv[bid] = bv[pid];
      bv[pid] = tmp;
      bv[bid]->heapId = bid;
      bv[pid]->heapId = pid;

      bid = pid;
      pid = (bid - 1) / 2;
    }
  }

  static ConfBox3d* extractMin(vector<ConfBox3d*>& bv) {
    ConfBox3d* minB = bv[0];
    bv[0] = bv.back();
    bv[0]->heapId = 0;
    minB->heapId = -1;
    bv.pop_back();
    siftDown(bv, 0);
    return minB;
  }
};

// won't work with std pq, as this comparison is not transitional!
class PQCmp3 {
 public:
  bool operator() (const ConfBox3d* a, const ConfBox3d* b) {
    // if depth diff bigger than 3, use depth as priority
    // if (abs(a->depth - b->depth) > 8)
    // {
    //   return a->depth > b->depth;
    // }
    // otherwise expand box closer to beta
    double distDiff =
      (a->x - beta[0]) * (a->x - beta[0]) +
      (a->y - beta[1]) * (a->y - beta[1]) +
      (a->z - beta[2]) * (a->z - beta[2]) -
      ((b->x - beta[0]) * (b->x - beta[0]) +
       (b->y - beta[1]) * (b->y - beta[1]) +
       (b->z - beta[2]) * (b->z - beta[2]));
    return distDiff > 0;
  }
};
