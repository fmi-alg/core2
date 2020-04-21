/* **************************************
   File: QuadTree.h

   Description: 

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#pragma once

#include <iostream>

#include "subdivision/Box.h"
#include "subdivision/status.h"
#include "sss/UnionFind.h"
#include "strategy/PriorityQueue.h"

extern int freeCount;
extern int stuckCount;
extern int mixCount;
extern int mixSmallCount;
extern std::vector<int> expansions;

class QuadTree
{
private:
  void insertNode(Box* b){
    switch(b->getBoxStatus()){
      case FREE:
        new Set(b);
        unionAdjacent(b);
        ++freeCount;
      break;
      case STUCK:
        ++stuckCount;
      break;
      case MIXED:
        ++mixCount;
        if(b->height < epsilon || b->width < epsilon)
          ++mixSmallCount;
        PQ->push(b);
      break;
      case UNKNOWN:
        std::cout << "UNKNOWN not handled?" << std::endl;
    }
  }

public:
  UnionFind* pSets;
  BoxQueue* PQ;
  Box* pRoot;
  double epsilon;
  int SearchType;

  QuadTree(Box* root, double e, int qType):pRoot(root), epsilon(e), SearchType(qType){
    switch(SearchType){
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
        PQ = new dijkstraQueue<VorCmp>();
      break;
      case 4:
        PQ = new dijkstraQueue<BiVorCmp>();
      break;
    }

    //PQ = new randQueue();

    pRoot->checkVoronoiFeatureSet();
    pRoot->findCleanrance3(pRoot->mB);

    pRoot->checkFeatureSet();
    insertNode(pRoot);
  }

  Box* getBox(Box* root, Point2d p){
    if(p.X() > root->mB.X() + root->width / 2  || p.X() < root->mB.X() - root->width / 2 ||
       p.Y() > root->mB.Y() + root->height / 2 || p.Y() < root->mB.Y() - root->height / 2){
      return 0;
    }

    Box* b = root;
    while (!b->isLeaf){
      double dx = p.X() - b->mB.X();
      double dy = p.Y() - b->mB.Y();
      if(dx <= 0 && dy >= 0){
        b = b->pChildren[0];
      }
      else if(dx >= 0 && dy >= 0){
        b = b->pChildren[1];
      }
      else if(dx >= 0 && dy <= 0){
        b = b->pChildren[2];
      }
      else if(dx <= 0 && dy <= 0){
        b = b->pChildren[3];
      }
    }
    return b;
  }

  Box* getBox(Point2d p){
    return getBox(pRoot, p);
  }

  bool expand(Box* b){
    if(!b->split(epsilon)){
      return false;
    }

    for(int i=0; i<4; ++i){
      b->pChildren[i]->checkVoronoiFeatureSet();
      b->pChildren[i]->findCleanrance3(b->pChildren[i]->mB);

      b->pChildren[i]->checkFeatureSet();
      insertNode(b->pChildren[i]);
    }

    return true;
  }

  bool expand(){
    while(!PQ->empty()){
      Box* b = PQ->extract();
      //b might not be a leaf since it could already be split in expand(Box* b), and PQ is not updated there
      if(b->isLeaf && b->split(epsilon)){
        assert(b->status == MIXED);
        for(int i=0; i<4; ++i){
          b->pChildren[i]->checkVoronoiFeatureSet();
          b->pChildren[i]->findCleanrance3(b->pChildren[i]->mB);

          b->pChildren[i]->checkFeatureSet();
          insertNode(b->pChildren[i]);
        }
        return true;
      }
    }
    return false;
  }

  bool isConnect(Box* a, Box* b){
    if(pSets->Find(a) == pSets->Find(b)){
      return true;
    }
    return false;
  }

  void unionAdjacent(Box* b){
    for(int i=0; i<4; ++i){
      BoxIter* iter = new BoxIter(b, i);
      Box* neighbor = iter->First();
      while(neighbor && neighbor != iter->End()){
        if(neighbor->status == FREE){
          pSets->Union(b, neighbor);
        }
        neighbor = iter->Next();
      }
    }
  }

  ~QuadTree(void){}
};
