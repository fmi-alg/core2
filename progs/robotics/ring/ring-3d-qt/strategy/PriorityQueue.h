#pragma once

#include <queue>
#include <vector>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <iterator>
#include <math.h>
#include <unordered_set>

#include "subdivision/ConfBox3d.h"
#include "sss/Graph.h"
#include "sss/UnionFind.h"

using namespace std;

enum QTYPE { RANDOM, BFS, GREEDY, BIGREEDY, GREEDY_SIZE, BIGREEDY_SIZE, VORONOI, BIVORONOI };

extern int searchType;
extern bool moveAway;

extern Point3d start;
extern Point3d goal;
extern Point3d startRot;
extern Point3d goalRot;

class PathCmp {
 public:
  bool operator()(ConfBox3d* a, ConfBox3d* b) {
    return a->distDij > b->distDij;
  }
};

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

//won't work with std pq, as this comparison is not transitional!
class DistCmp
{
public:
  bool operator() (const ConfBox3d* a, const ConfBox3d* b){
    double distDiff = a->mB.distance(goal)-b->mB.distance(goal);
    return distDiff > 0;


    if(a->rot_width != -2 && b->rot_width != -2){
//      if(abs(distDiff) > 0) return distDiff;
//      else{
//        double dirA = Vector(a->rot_mB.X(), a->rot_mB.Y(), a->rot_mB.Z())*Vector(goalRot.X(), goalRot.Y(), goalRot.Z());
//        double dirB = Vector(b->rot_mB.X(), b->rot_mB.Y(), b->rot_mB.Z())*Vector(goalRot.X(), goalRot.Y(), goalRot.Z());
//        return dirA < dirB;
//      }

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
    else if(a->rot_width != -2) return true;
    else if(b->rot_width != -2) return false;
    else                        return distDiff > 0;
  }
};

//won't work with std pq, as this comparison is not transitional!
class BiDistCmp
{
public:
  bool operator() (const ConfBox3d* a, const ConfBox3d* b){
    Point3d pa, pb;
    if(a->bidirection > 0) pa = goal;
    else                   pa = start;
    if(b->bidirection > 0) pb = goal;
    else                   pb = start;

    double distDiff = a->mB.distance(pa)-b->mB.distance(pb);
    return distDiff > 0;
  }
};


//won't work with std pq, as this comparison is not transitional!
class DistSizeCmp
{
public:
  bool operator() (const ConfBox3d* a, const ConfBox3d* b){
    double distDiff = a->mB.distance(goal)*a->depth-b->mB.distance(goal)*b->depth;
    return distDiff > 0;
  }
};

//won't work with std pq, as this comparison is not transitional!
class BiDistSizeCmp
{
public:
  bool operator() (const ConfBox3d* a, const ConfBox3d* b){
    Point3d pa, pb;
    if(a->bidirection > 0) pa = goal;
    else                   pa = start;
    if(b->bidirection > 0) pb = goal;
    else                   pb = start;

    double distDiff = a->mB.distance(pa)*a->depth-b->mB.distance(pb)*b->depth;
    return distDiff > 0;
  }
};

//won't work with std pq, as this comparison is not transitional!
class VorCmp {
public:
  bool operator()(const ConfBox3d* a, const ConfBox3d* b) {
    double dista = a->mB.distance(goal);
    double distb = b->mB.distance(goal);
    double a_v = a->vorCorners.size() + a->vorEdges.size() + a->vorWalls.size();
    double b_v = b->vorCorners.size() + b->vorEdges.size() + b->vorWalls.size();

//    if(a_v == 2 && b_v == 2)
//      return dista > distb;
//    else if(a_v == 2)
//      return false;
//    else if(b_v == 2)
//      return true;
//    else
//      return a_v > b_v;

    return dista/a_v > distb/b_v;
    //return dista*dista/a_v > distb*distb/b_v;
    //return pow(dista, a_v) > pow(distb, b_v);
    //return dista/(a_v*a_v) > distb/(b_v*b_v);
    //return dista*a_v > distb*b_v;
    //return pow(dista, 1.0f/a_v) > pow(distb, 1.0f/b_v);

    // if this is true,
    // it means a's priority is lower than b's.
  }
};

//won't work with std pq, as this comparison is not transitional!
class BiVorCmp {
public:
  bool operator()(const ConfBox3d* a, const ConfBox3d* b) {

    Point3d pa, pb;
    if(a->bidirection > 0) pa = goal;
    else                   pa = start;
    if(b->bidirection > 0) pb = goal;
    else                   pb = start;

    double dista = a->mB.distance(pa);
    double distb = b->mB.distance(pb);
    double a_v = a->vorCorners.size() + a->vorEdges.size() + a->vorWalls.size();
    double b_v = b->vorCorners.size() + b->vorEdges.size() + b->vorWalls.size();

    //    if(a_v == 2 && b_v == 2)
    //      return dista > distb;
    //    else if(a_v == 2)
    //      return false;
    //    else if(b_v == 2)
    //      return true;
    //    else
    //      return a_v > b_v;

    return dista/a_v > distb/b_v;
  }
};

//won't work with std pq, as this comparison is not transitional!
class DistCmpSpecial
{
public:
  bool operator() (const ConfBox3d* a, const ConfBox3d* b){

    Point3d pa, pb;
    if(a->bidirection > 0) pa = goal;
    else                   pa = start;
    if(b->bidirection > 0) pb = goal;
    else                   pb = start;

    double distA = std::numeric_limits<double>::max();
    if(a->nearestC != NULL){
      distA = a->mB.distance(a->nearestC->p);
    }
    if(a->nearestE != NULL && distA > a->nearestE->seg.distance(a->mB)){
      distA = a->nearestE->seg.distance(a->mB);
    }
    if(a->nearestW != NULL && distA > a->nearestW->tri.distance(a->mB)){
      distA = a->nearestW->tri.distance(a->mB);
    }

    double distB = std::numeric_limits<double>::max();
    if(b->nearestC != NULL){
      distB = b->mB.distance(b->nearestC->p);
    }
    if(b->nearestE != NULL && distB > b->nearestE->seg.distance(b->mB)){
      distB = b->nearestE->seg.distance(b->mB);
    }
    if(b->nearestW != NULL && distB > b->nearestW->tri.distance(b->mB)){
      distB = b->nearestW->tri.distance(b->mB);
    }

    if(fabs(distA-distB) < 1e-10)
      return (a->mB.distance(pa)-b->mB.distance(pb) > 0);
    else
      return (distA-distB < 0);
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

    //fprintf(g_fptr, "box queue\n");

    if(b->getStatus() == MIXED)
      push(b);
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

 public:
 RandQueue(){
   //srand( Qseed );
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

template<typename Cmp>
class DijkstraQueue : public BoxQueue
{
 private:
  vector<ConfBox3d*> bv;

 public:
  DijkstraQueue(){}


  void push(ConfBox3d* b) {
    distHeap<Cmp>::insert(bv, b);

    if(searchType != VORONOI && searchType != BIVORONOI)
      distHeap<Cmp>::insert(bv, b);
    else{
      if(moveAway || (b->vorCorners.size()+b->vorEdges.size()+b->vorWalls.size() > 2)){
        distHeap<Cmp>::insert(bv, b);
      }
    }
  }

  ConfBox3d* extract() {
    ConfBox3d* b = distHeap<Cmp>::extractMin(bv);
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
  }
};
