#include <vector>
#include <algorithm>
#include <math.h>

#include "subdivision/Box.h"
#include "subdivision/QuadTree.h"
#include "subdivision/Status.h"

using namespace std;

extern Point2d alpha;
extern Point2d beta;

#ifndef _GRAPH_H_
#define _GRAPH_H_

class distCmp
{
public:
  bool operator()(Box* a, Box* b){
    return a->distDij > b->distDij;
  }
};


// min heap
template <typename CmpFunctor>
class distHeap
{
private:
  static void siftDown(vector<Box*>& bv, int i){
    CmpFunctor cmp;
    unsigned int l = 2*i + 1;
    unsigned int r = 2*i + 2;
    int smallest;
    if(l < bv.size() && cmp(bv[i], bv[l])){
      smallest = l;
    }
    else{
      smallest = i;
    }
    if(r < bv.size() && cmp(bv[smallest], bv[r])){
      smallest = r;
    }
    if(smallest != i){
      Box* tmp = bv[smallest];
      bv[smallest] = bv[i];
      bv[i] = tmp;
      bv[smallest]->heapId = smallest;
      bv[i]->heapId = i;

      siftDown(bv, smallest);
    }
  }

public:
  static void makeHeap(vector<Box*>& bv){
    if (bv.size() <= 1){
      return;
    }
    for (int i = 0; i < bv.size(); ++i){
      bv[i]->heapId = i;
    }
    for (int i = (bv.size() -2) / 2; i >= 0; --i){
      siftDown(bv, i);
    }
  }

  static void insert(vector<Box*>& bv, Box* b){
    CmpFunctor cmp;
    bv.push_back(b);
    int bid = bv.size() - 1;
    b->heapId = bid;
    int pid = (bid - 1) / 2;
    while (bid > 0 && cmp(bv[pid], bv[bid])){
      Box* tmp = bv[bid];
      bv[bid] = bv[pid];
      bv[pid] = tmp;
      bv[bid]->heapId = bid;
      bv[pid]->heapId = pid;

      bid = pid;
      pid = (bid - 1) / 2;
    }
  }

  static void decreaseKey(vector<Box*>& bv, Box* b, double dist){
    CmpFunctor cmp;
    assert(bv[b->heapId] == b);
    assert(b->distDij > dist);

    b->distDij = dist;
    int bid = b->heapId;
    int pid = (bid - 1) / 2;
    while (bid > 0 && cmp(bv[pid], bv[bid])){
      Box* tmp = bv[bid];
      bv[bid] = bv[pid];
      bv[pid] = tmp;
      bv[bid]->heapId = bid;
      bv[pid]->heapId = pid;

      bid = pid;
      pid = (bid - 1) / 2;
    }
  }

  static Box* extractMin(vector<Box*>& bv){
    Box* minB = bv[0];
    bv[0] = bv.back();
    bv[0]->heapId = 0;
    minB->heapId = -1;
    bv.pop_back();
    siftDown(bv, 0);
    return minB;
  }
};

//won't work with std pq, as this comparison is not transitional!
class DistCmp
{
public:
  bool operator() (const Box* a, const Box* b){
    //if depth diff bigger than 3, use depth as priority
    //if (abs(a->depth - b->depth) > 8)
    //{
    //	return a->depth > b->depth;
    //}
    //otherwise expand box closer to beta
    double distDiff = a->mB.distance(beta)-b->mB.distance(beta);
    return distDiff > 0;
  }
};

//won't work with std pq, as this comparison is not transitional!
class DistSizeCmp
{
public:
  bool operator() (const Box* a, const Box* b){
    //if depth diff bigger than 3, use depth as priority
    //if (abs(a->depth - b->depth) > 8)
    //{
    //	return a->depth > b->depth;
    //}
    //otherwise expand box closer to beta
    double distDiff = a->mB.distance(beta)*a->depth-b->mB.distance(beta)*b->depth;
    return distDiff > 0;
  }
};

//won't work with std pq, as this comparison is not transitional!
class VorCmp {
public:
  bool operator()(const Box* a, const Box* b) {
    double dista = a->mB.distance(beta);
    double distb = b->mB.distance(beta);
    double a_v = a->corners_v.size() + a->walls_v.size();
    double b_v = b->corners_v.size() + b->walls_v.size();

//    if(a_v == 2 && b_v == 2)
//      return dista > distb;
//    else if(a_v == 2)
//      return 1;
//    else if(b_v == 2)
//      return -1;
//    else
//      return dista/a_v > distb/b_v;

    return dista/a_v > distb/b_v;
  }
};

//won't work with std pq, as this comparison is not transitional!
class BiVorCmp {
public:
  bool operator()(const Box* a, const Box* b) {

    Point2d pa, pb;
    if(a->bidirection > 0) pa = beta;
    else                   pa = alpha;
    if(b->bidirection > 0) pb = beta;
    else                   pb = alpha;

    double dista = a->mB.distance(pa);
    double distb = b->mB.distance(pb);
    double a_v = a->corners_v.size() + a->walls_v.size();
    double b_v = b->corners_v.size() + b->walls_v.size();

//    if(a_v == 2 && b_v == 2)
//      return dista > distb;
//    else if(a_v == 2)
//      return 1;
//    else if(b_v == 2)
//      return -1;
//    else
//      return dista/a_v > distb/b_v;

    //fprintf(stderr, "dist a %f %d  dist b %f %d\n", dista, a->bidirection, distb, b->bidirection);

    return dista/(a_v) > distb/(b_v);
  }
};

//won't work with std pq, as this comparison is not transitional!
class DistCmpSpecial
{
public:
  bool operator() (const Box* a, const Box* b){

    Point2d pa, pb;
    if(a->bidirection > 0) pa = beta;
    else                   pa = alpha;
    if(b->bidirection > 0) pb = beta;
    else                   pb = alpha;

    double distA = std::numeric_limits<double>::max();
    if(a->nearestC != NULL){
      distA = a->mB.distance(a->nearestC->p);
    }
    if(a->nearestW != NULL && distA > a->nearestW->seg.distance(a->mB)){
      distA = a->nearestW->seg.distance(a->mB);
    }

    double distB = std::numeric_limits<double>::max();
    if(b->nearestC != NULL){
      distB = b->mB.distance(b->nearestC->p);
    }
    if(b->nearestW != NULL && distB > b->nearestW->seg.distance(b->mB)){
      distB = b->nearestW->seg.distance(b->mB);
    }

    if(fabs(distA-distB) < 1e-10)
      return (a->mB.distance(pa)-b->mB.distance(pb) > 0);
    else
      return (distA-distB < 0);
  }
};

class Graph
{
public:
};

#endif //_GRAPH_H_
