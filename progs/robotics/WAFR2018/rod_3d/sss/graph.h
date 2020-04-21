/*
 * graph.h
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <math.h>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "subdivision/configuration_box3d.h"

// min heap
template <typename CmpFunctor>
class distHeap {
 private:
  static void siftDown(std::vector<ConfBox3d*>& bv, unsigned long i) {
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
      bv[smallest]->heap_id = smallest;
      bv[i]->heap_id = i;

      siftDown(bv, smallest);
    }
  }

 public:
  static void makeHeap(std::vector<ConfBox3d*>& bv) {
    if (bv.size() <= 1) {
      return;
    }
    for (int i = 0; i < bv.size(); ++i) {
      bv[i]->heap_id = i;
    }
    for (int i = (bv.size() - 2) / 2; i >= 0; --i) {
      siftDown(bv, i);
    }
  }

  static void insert(std::vector<ConfBox3d*>& bv, ConfBox3d* b) {
    bv.push_back(b);
    int bid = bv.size() - 1;
    b->heap_id = bid;
    decreaseKey(bv, b, b->dist_Dijkstra);
  }

  static void decreaseKey(std::vector<ConfBox3d*>& bv, ConfBox3d* b,
                          double dist) {
    CmpFunctor cmp;
    assert(b->heap_id >= 0);
    unsigned long bid = static_cast<unsigned long>(b->heap_id);
    unsigned long pid = (bid - 1) / 2;
    assert(bv[bid] == b);
    assert(b->dist_Dijkstra >= dist);

    b->dist_Dijkstra = dist;
    while (bid > 0 && cmp(bv[pid], bv[bid])) {
      ConfBox3d* tmp = bv[bid];
      bv[bid] = bv[pid];
      bv[pid] = tmp;
      bv[bid]->heap_id = bid;
      bv[pid]->heap_id = pid;

      bid = pid;
      pid = (bid - 1) / 2;
    }
  }

  static ConfBox3d* extractMin(std::vector<ConfBox3d*>& bv) {
    ConfBox3d* minB = bv[0];
    bv[0] = bv.back();
    bv[0]->heap_id = 0;
    minB->heap_id = -1;
    bv.pop_back();
    siftDown(bv, 0);
    return minB;
  }
};

#endif  // GRAPH_H
