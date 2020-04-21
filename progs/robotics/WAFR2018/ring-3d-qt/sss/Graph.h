#pragma once

#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

#include "subdivision/ConfBox3d.h"

using namespace std;

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
    decreaseKey(bv, b, b->distDij);
  }

  static void decreaseKey(vector<ConfBox3d*>& bv, ConfBox3d* b, double dist) {
    CmpFunctor cmp;
    assert(b->heapId >= 0);
    unsigned long bid = static_cast<unsigned long>(b->heapId);
    unsigned long pid = (bid - 1) / 2;
    assert(bv[bid] == b);
    assert(b->distDij >= dist);

    b->distDij = dist;
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
