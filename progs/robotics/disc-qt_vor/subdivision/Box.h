#pragma once

#include "CORE/geom2d/point2d.h"
#include "geometry/Corner.h"
#include "geometry/Wall.h"
#include "subdivision/Status.h"
#include "display/ControlWindow.h"

#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <list>

class Box;
class Set;
class Wall;

extern std::vector<int> expansions;

using namespace std;

class BoxIter
{
private:
  const Box* b;
  int direction;
  int prev;
  int next;
  int cross;
  Box* neighbor;

public:
  BoxIter(const Box* bb, int direc);

  Box* First();
  Box* Next();
  Box* End();

}; //class BoxIter

class Box
{
private:
  static bool isOverLimit(const Box* base, const Box* nextBox){
    if((nextBox->mB.X() > base->mB.X() - base->width/2  && nextBox->mB.X() < base->mB.X() + base->width/2) ||
       (nextBox->mB.Y() > base->mB.Y() - base->height/2 && nextBox->mB.Y() < base->mB.Y() + base->height/2)){
      return false;
    }
    return true;
  }

public:
  Point2d mB;
  static double r0;
  double rB;
  double width, height;

  int depth;
  int priority;
  bool isLeaf;

  static int counter;	// time of expansion (used in BFS strategy)

  friend class BoxIter;
  friend class QuadTree;

  //Pointers to children, but when no children (i.e., leaf),
  //	the pointers are used as neighbor pointers
  // where
  //	0 = NW, 1 = EN, 2 = SE, 3 = WS
  Box* pChildren[4];
  Box* pParent;      //parent in quadtree

  Status status;
  int d_classify_condition; // for debug

  Set* pSet;
  list<Corner*> corners;
  list<Wall*> walls;

  list<Corner*> corners_v;
  list<Wall*> walls_v;
  Corner* nearestC;
  Wall* nearestW;

  //for shortest path
  double dist2Source;
  int heapId;
  Box* prev;
  bool visited;
  bool inQueue;

  //for bidirectional search
  int bidirection;

  //for generate path
  bool inSource;
  double distDij;

  static vector<Box*>* pAllLeaf;

  Box(Point2d p, double w, double h):
    mB(p), width(w), height(h), depth(1), isLeaf(true),
    pParent(0), status(UNKNOWN),
    pSet(0), dist2Source(-1), heapId(-1), prev(0), visited(false), inQueue(false),
    bidirection(0),
    inSource(false), distDij(-1){
      for(int i = 0; i < 4; ++i){
        pChildren[i] = 0;
      }
      rB = sqrt(width*width + height*height)/2;
      priority = Box::counter;
      d_classify_condition = -1;
  }
    
  ~Box() {}

  // Sep(mB, f) <= r0 + rB
  void checkFeatureSet(){
    if (status != UNKNOWN) return;

    double outerDomain = r0 + rB;
    double innerDomain = r0 > rB ? r0 - rB : 0;

    double mindistC = std::numeric_limits<double>::max();
    Corner *nearestC = NULL;
    if(corners.size()){
      for(list<Corner*>::iterator it = corners.begin(); it != corners.end();){
        Corner* c = *it;
        double dist = c->distance(this->mB);

        // Chee & Tom: Feb 21 2016 Put this mixed decision afterwards
        // if this is greater than the outer domain or not intersect the zone, erase it
        if(dist > outerDomain || !c->intersectZone(this)){
          it = corners.erase(it);
        }
        else{
          if(mindistC > dist){
              mindistC = dist;
              nearestC = *it;
          }
          if (dist <= innerDomain){
              status = STUCK;
              d_classify_condition = 0;
              return;
          }

          status = MIXED;
          ++it;
        }
      }
    }

    double mindistW = std::numeric_limits<double>::max();
    Wall *nearestW = NULL;
    Wall *nearestW2 = NULL;
    if(walls.size()){
      for(list<Wall*>::iterator it = walls.begin(); it != walls.end();){
        Wall* w = *it;
        double dist = w->seg.distance(this->mB);

        // Chee & Tom: Feb 21 2016 Put this mixed decision afterwards
        // if this is greater than the outer domain or not intersect the zone, erase it
        if(dist > outerDomain || !w->intersectZone(this)){
          it = walls.erase(it);
        }
        else{
          if(mindistW > dist){
            mindistW = dist;
            nearestW = *it;
          }
          if(dist < innerDomain){ // equal sign ???????
            status = STUCK;
            d_classify_condition = 1;
            return;
          }

          status = MIXED;
          ++it;
        }
      }
    }

    bool dupW = false;
    if(nearestW != NULL){
      for(list<Wall*>::iterator it = walls.begin(); it != walls.end(); ++it){
        Wall* w = *it;
        if(w->src->p == w->dst->p){
            dupW = true;
            nearestW2 = w;
        }
      }
    }

    // Chee & Tom: Feb 23 2016
    // empty set
    // 1 or 2 feature
    // if(rB < sep(m_B, f))
    //  check status because it is far enough
    if(corners.size() == 0 && walls.size() == 0){
      if (!pParent){
        status = MIXED;
      }
      else{
        status = pParent->findCleanrance(this->mB);
        d_classify_condition = 2;
      }
    }
    else{
      status = MIXED;
    }
  } // checkFeatureSet

  // Sep(mB, f) <= 2rB + clearance(mB)
  void checkVoronoiFeatureSet(){
    double sep = 2*rB+findCleanrance2(this->mB);

    if(corners_v.size()){
      for(list<Corner*>::iterator it = corners_v.begin(); it != corners_v.end();){
        Corner* c = *it;
        double dist = c->distance(this->mB);

        if(dist > sep){
          it = corners_v.erase(it);
        }
        else{
          ++it;
        }
      }
    }

    if(walls_v.size()){
      for(list<Wall*>::iterator it = walls_v.begin(); it != walls_v.end();){
        Wall* w = *it;
        double dist = w->seg.distance(this->mB);

        if(dist > sep){
          it = walls_v.erase(it);
        }
        else{
          ++it;
        }
      }
    }
  } // checkVoronoiFeatureSet


  //find the nearest feature of mB, and use it to update the Box status
  Status findCleanrance(Point2d p){
    bool dupW = false;
    int f = -1;

    double mindistW = std::numeric_limits<double>::max();
    Wall* nearestWall = NULL;
    Wall* nearestWall2 = NULL;
    if(walls.size()){
      list<Wall*>::iterator iterW = walls.begin();
      for(int i=0; iterW != walls.end(); ++iterW){
        Wall* w = *iterW;

        double dist = w->seg.distance(p);
        if(dist < mindistW){
          mindistW = dist;
          nearestWall = *iterW;
          f = i;
        }
        ++i;
      }
      iterW = walls.begin();
      for (int i=0; iterW != walls.end(); ++iterW){
        Wall* w = *iterW;
        double dist = w->seg.distance(p);
        if(fabs(dist-mindistW) < 1e-10 && i != f){
          nearestWall2 = *iterW;
          dupW = true;
        }
        ++i;
      }
    }

    bool dupC = false;
    f = -1;

    double mindistC = std::numeric_limits<double>::max(); // mindistC may not exist, so init to a bigger number
    Corner* nearestCorner = NULL;
    Corner* nearestCorner2 = NULL;
    if(corners.size()){
      list<Corner*>::iterator iterC = corners.begin();
      for (int i=0; iterC != corners.end(); ++iterC){
        Corner* c = *iterC;
        double dist = c->distance(p);
        if (dist < mindistC){
          mindistC = dist;
          nearestCorner = *iterC;
          f = i;
        }
        ++i;
      }

      corners.begin();
      for (int i=0; iterC != corners.end(); ++iterC){
        Corner* c = *iterC;
        double dist = c->distance(p);
        if (fabs(dist-mindistC) < 1e-10 && i != f){
          nearestCorner2 = *iterC;
          dupC = true;
        }
        ++i;
      }
    }

    //nearest feature is a wall
    if(mindistW < mindistC){
      if(dupW){
        if((nearestWall->seg.orientation(p) > 0) ||
           (nearestWall2->seg.orientation(p) > 0)){
          return FREE;
        }
      }
      else{
        if((nearestWall->seg.orientation(p) > 0)){
          return FREE;
        }
      }
    }
    //otherwise check the corner's convexity
    //if convex, out; if concave, in
    //note that if a wall and a corner are the same dist,
    //only need to take care of the corner
    else{
      if(dupC){
        if(nearestCorner->isConvex() || nearestCorner2->isConvex()){
          return FREE;
        }
      }
      else{
        if(nearestCorner->isConvex()){
          return FREE;
        }
      }
    }

    return STUCK;
  } // findCleanrance

  //find the nearest feature of mB, and return the distance
  double findCleanrance2(Point2d p){
    double mindistW = std::numeric_limits<double>::max();
    Wall* nearestWall = NULL;
    if(walls.size()){
      for(list<Wall*>::iterator iterW = walls.begin(); iterW != walls.end(); ++iterW){
        Wall* w = *iterW;
        double dist = w->seg.distance(p);
        if(dist < mindistW){
          mindistW = dist;
          nearestWall = *iterW;
        }
      }
    }

    double mindistC = std::numeric_limits<double>::max(); // mindistC may not exist, so init to a bigger number
    Corner* nearestCorner = NULL;
    if(corners.size()){
      for(list<Corner*>::iterator iterC = corners.begin(); iterC != corners.end(); ++iterC){
        Corner* c = *iterC;
        double dist = c->distance(p);
        if (dist < mindistC){
          mindistC = dist;
          nearestCorner = *iterC;
        }
      }
    }

    if(mindistW < mindistC) return mindistW;
    else                    return mindistC;
  } // findCleanrance2

  //find the nearest voronoi feature of mB, and return the distance
  void findCleanrance3(Point2d p){
    double mindistW = std::numeric_limits<double>::max();
    nearestW = NULL;
    if(walls_v.size()){
      for(list<Wall*>::iterator iterW = walls_v.begin(); iterW != walls_v.end(); ++iterW){
        Wall* w = *iterW;
        double dist = w->seg.distance(p);
        if(dist < mindistW){
          mindistW = dist;
          nearestW = *iterW;
        }
      }
    }

    double mindistC = std::numeric_limits<double>::max(); // mindistC may not exist, so init to a bigger number
    nearestC = NULL;
    if(corners_v.size()){
      for(list<Corner*>::iterator iterC = corners_v.begin(); iterC != corners_v.end(); ++iterC){
        Corner* c = *iterC;
        double dist = c->distance(p);
        if (dist < mindistC){
          mindistC = dist;
          nearestC = *iterC;
        }
      }
    }
  } // findCleanrance3

  void addCorner(Corner* c){
    corners.push_back(c);
    corners_v.push_back(c);
  }

  void addWall(Wall* w){
    walls.push_back(w);
    walls_v.push_back(w);
  }

  bool isFree(){
    if (status == FREE){
      return true;
    }
    return false;
  }

  bool in(Point2d p){
    if(p.X() < mB.X()-width/2  || p.X() > mB.X()+width/2) return false;
    if(p.Y() < mB.Y()-height/2 || p.Y() > mB.Y()+height/2) return false;
    return true;
  }

  Status getBoxStatus(){
    checkFeatureSet();
    return status;
  }

  // split(eps)
  // 	returns false if we fail to split for some reason
  //
  bool split(double epsilon){
    if(this->height < epsilon || this->width < epsilon){
      return false;
    }

    if(!this->isLeaf || this->status != MIXED){
      return false;
    }

    //record the time of this split event, will be used to set priority of children
    ++Box::counter;

    Box* children[4];
    children[0] = new Box(Point2d(mB.X() - width/4, mB.Y() + height/4), width/2, height/2);
    children[1] = new Box(Point2d(mB.X() + width/4, mB.Y() + height/4), width/2, height/2);
    children[2] = new Box(Point2d(mB.X() + width/4, mB.Y() - height/4), width/2, height/2);
    children[3] = new Box(Point2d(mB.X() - width/4, mB.Y() - height/4), width/2, height/2);
    for(int i=0; i<4; ++i){
      children[i]->depth = this->depth + 1;
      children[i]->bidirection = this->bidirection;
    }

    for(int i=0; i<4; ++i){
      //find three other directions
      int prev = (i + 3) % 4;
      int next = (i + 1) % 4;
      int cross = (i + 2) % 4;

      //update easy cases
      children[i]->pChildren[i] = pChildren[i];
      children[i]->pChildren[next] = children[next];
      children[i]->pChildren[cross] = children[prev];

      //init box neighbor iterator for direction i
      BoxIter* iter = new BoxIter(this, i);
      Box* neighbor = iter->First();

      if(!neighbor){
        continue;
      }

      // if neighbor are not smaller
      if(neighbor->depth <= this->depth){
        //after split child 'next' should also point to
        //neighbor in direction i
        children[next]->pChildren[i] = neighbor;

        //if neighbor's cross direction point to this, it should
        //instead point to child 'next' after split
        if(neighbor->pChildren[cross] == this){
          neighbor->pChildren[cross] = children[next];
        }
        continue;
      }

      Box* prevNeighbor = neighbor;
      //indicate if we go across the boundary between child 'i'
      //and 'next' the first time
      bool firstTimeCrossBetweenChildren = true;

      //if neighbor smaller
      while(neighbor != iter->End()){
        //assert( abs(abs(neighbor->x - this->x)
        //	- (neighbor->width/2 + this->width/2)) < 1e-8
        // ||  abs(abs(neighbor->y - this->y)
        // 	- (neighbor->height/2 + this->height/2)) < 1e-8);
        //within the strip of child next, neighbor's cross direction
        //should point to next
        if(!isOverLimit(children[next], neighbor)){
          neighbor->pChildren[cross] = children[next];
        }
        //within the strip of child i, neighbor's cross
        //direction should point to i
        else if(!isOverLimit(children[i], neighbor)){
          neighbor->pChildren[cross] = children[i];

          //first time cross between child i and next,
          //should update next's i direction pointer
          if(firstTimeCrossBetweenChildren){
            firstTimeCrossBetweenChildren = false;
            children[next]->pChildren[i] = prevNeighbor;
          }
        }
        else{
          assert(0);
        }
        prevNeighbor = neighbor;
        neighbor = iter->Next();
      }

      delete iter;
    } // end while loop

    for(int i=0; i<4; ++i){
      this->pChildren[i] = children[i];
      this->pChildren[i]->pParent = this;
      //add all of parent's walls and corners to each child,
      //will be filtered later in updatestatus()
      this->pChildren[i]->walls.insert(this->pChildren[i]->walls.begin(), this->walls.begin(), this->walls.end());
      this->pChildren[i]->corners.insert(this->pChildren[i]->corners.begin(), this->corners.begin(), this->corners.end());

      this->pChildren[i]->walls_v.insert(this->pChildren[i]->walls_v.begin(), this->walls_v.begin(), this->walls_v.end());
      this->pChildren[i]->corners_v.insert(this->pChildren[i]->corners_v.begin(), this->corners_v.begin(), this->corners_v.end());
    }
    this->isLeaf = false;

    for (int i = 0; i < 4; ++i) {
        pAllLeaf->push_back(children[i]);
    }
    expansions.push_back(pAllLeaf->size());

    return true;
  } // split

}; //class Box
