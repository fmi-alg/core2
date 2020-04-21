/*
 * union_find.h
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <iostream>

#include "subdivision/configuration_box3d.h"

class Set {
 public:
  ConfBox3d* pBox;
  Set* pParent;  // parent in union find
  int rank;

  explicit Set(ConfBox3d* b) : pBox(b), rank(1) {
    pParent = this;
    b->p_set = this;
  }
};

class UnionFind {
 private:
  Set* pathCompress(Set* set) {
    if (set->pParent == set) {
      return set;
    }

    set->pParent = pathCompress(set->pParent);
    return set->pParent;
  }

 public:
  ConfBox3d* Find(ConfBox3d* b) {
    Set* root = pathCompress(b->p_set);
    return root->pBox;
  }

  bool isConnect(ConfBox3d* a, ConfBox3d* b) {
    if (Find(a) == Find(b)) {
      return true;
    }
    return false;
  }

  void Union(ConfBox3d* a, ConfBox3d* b) {
    Set* roota = Find(a)->p_set;
    Set* rootb = Find(b)->p_set;
    if (roota == rootb) {
      return;
    }
    if (roota->rank > rootb->rank) {
      rootb->pParent = roota;
    } else if (roota->rank == rootb->rank) {
      rootb->pParent = roota;
      ++roota->rank;
    } else {
      roota->pParent = rootb;
    }
  }
};

#endif  // UNION_FIND_H
