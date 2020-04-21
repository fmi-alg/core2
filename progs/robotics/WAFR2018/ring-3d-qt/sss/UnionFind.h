#pragma once

#include <iostream>

#include "subdivision/ConfBox3d.h"

class Set {
 public:
  ConfBox3d* pBox;
  Set* pParent;  // parent in union find
  int rank;

  explicit Set(ConfBox3d* b):pBox(b), rank(1) {
    pParent = this;
    b->pSet = this;
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
    Set* root = pathCompress(b->pSet);
    return root->pBox;
  }

  bool isConnect(ConfBox3d* a, ConfBox3d* b) {
    if (Find(a) == Find(b)) {
      return true;
    }
    return false;
  }

  void Union(ConfBox3d*a, ConfBox3d* b) {
    Set* roota = Find(a)->pSet;
    Set* rootb = Find(b)->pSet;
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
