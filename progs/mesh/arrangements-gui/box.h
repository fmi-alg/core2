/*
 * box.h
 * box structure
 * July 15, 2011
 */

#pragma once
#ifndef MK_BOX_H_
#define MK_BOX_H_

#include <vector>
#include <list>

#include "CORE/CORE.h"
#include "CORE/IntervalT.h"

//#include "mk-defs.h"
//
#include "Point.h"
#include "Vector.h"

template <typename NT> class BoxT
{

public:

  typedef Point<NT,2> Point2d;
  typedef Vector<NT,2> Vector2d;

  BoxT() : generation_id(0), isLeaf(true), depth(1), pParent(NULL), priority(0), rB(0)
  {
      for (int i = 0; i < 4; ++i)
        {
            pChildren[i] = 0;
        }
  }

  BoxT(unsigned int gid,
       const IntervalT<NT> &x_range,
       const IntervalT<NT> &y_range) :
        generation_id(gid),
        x_range(x_range),
        y_range(y_range),
        isLeaf(true), depth(1), pParent(NULL), priority(0), rB(0)
  {
      for (int i = 0; i < 4; ++i)
        {
            pChildren[i] = 0;
        }

        NT w=width();
        NT h=height();
        rB = sqrt((w*w+h*h).doubleValue())/2;
  }

  //copy constructor
  BoxT( const BoxT<NT>& other ):
      generation_id(other.generation_id),
      x_range(other.x_range),
      y_range(other.y_range),
      isLeaf(other.isLeaf), depth(other.depth), pParent(other.pParent), priority(other.priority), rB(other.rB)
  {
      for (int i = 0; i < 4; ++i)
      {
          pChildren[i] = other.pChildren[i];
      }
  }

  ~BoxT() { }

  /*
  void Split(vector<const BoxT<NT> *> *output) const
  {
    const NT &x_start = x_range.getL();
    const NT &x_end = x_range.getR();
    const NT &x_mid = x_range.mid();
    const NT &y_start = y_range.getL();
    const NT &y_end = y_range.getR();
    const NT &y_mid = y_range.mid();

    const unsigned int gen_id = generation_id + 1;

    output->push_back(new BoxT<NT>(gen_id,
        IntervalT<NT>(x_start, x_mid), IntervalT<NT>(y_start, y_mid)));
    output->push_back(new BoxT<NT>(gen_id,
        IntervalT<NT>(x_mid, x_end), IntervalT<NT>(y_start, y_mid)));
    output->push_back(new BoxT<NT>(gen_id,
        IntervalT<NT>(x_start, x_mid), IntervalT<NT>(y_mid, y_end)));
    output->push_back(new BoxT<NT>(gen_id,
        IntervalT<NT>(x_mid, x_end), IntervalT<NT>(y_mid, y_end)));
  }
  */

  bool Split(NT epsilon);


  BoxT * Dilate(const NT factor) const
  {
    IntervalT<NT> new_x = x_range.Expand(factor);
    IntervalT<NT> new_y = y_range.Expand(factor);

    return (new BoxT(0, new_x, new_y));
  }

  bool in(const Point2d& p)
  {
      NT w2=width()/2;
      NT h2=height()/2;

      Point2d o=center();

      if( p[0]<o[0]-w2 || p[0]>o[0]+w2 ) return false;
      if( p[1]<o[1]-h2 || p[1]>o[1]+h2) return false;

      return true;
  }

  BoxT<NT> * find(const Point2d& p)
  {
      if(isLeaf) return this;

      for(int i=0;i<4;i++){
          if(pChildren[i]->in(p))
              return pChildren[i]->find(p);
      }

      return NULL;
  }

  //recursively obtain all leaves of this box
  void getLeaves(list<BoxT<NT>*>& leaves)
  {
      if(isLeaf){
          leaves.push_back(this);
          return;
      }

      for(int i=0;i<4;i++) pChildren[i]->getLeaves(leaves);
  }


  //
  static bool isOverLimit(const BoxT<NT>* base, const BoxT<NT>* nextBox)
  {

      Point2d bo=base->center();
      Point2d no=nextBox->center();

      if ((no[0] > bo[0] - base->width() / 2 && no[0] < bo[0] + base->width() / 2)
          || (no[1] > bo[1] - base->height() / 2 && no[1] < bo[1] + base->height() / 2))
      {
          return false;
      }
      return true;
  }


  //center
  Point2d center() const { return Point2d(x_mid(), y_mid()); }
  NT x_mid() const { return x_range.mid(); }
  NT y_mid() const { return y_range.mid(); }

  //compute the corners (vertices) of this box from its center and dimensions
  // UL: upper-left vertex, UR: upper-right vertex
  // LR: lower-right vertex, LL: lower-left vertex
  void getCorners(Point2d& UL, Point2d& UR, Point2d& LR, Point2d& LL )
  {
      NT w2=width()/2;
      NT h2=height()/2;

      Point2d o=center();

      NT x_l=o[0]-w2;
      NT x_r=o[0]+w2;
      NT y_l=o[1]-h2;
      NT y_u=o[1]+h2;

      UL.set(x_l, y_u);
      LL.set(x_l, y_l);
      UR.set(x_r, y_u);
      LR.set(x_r, y_l);
  }


  //dimensions
  NT width() const { return x_range.width(); }
  NT height() const { return y_range.width(); }


  const unsigned int generation_id;
  const IntervalT<NT> x_range;
  const IntervalT<NT> y_range;

  //
  // these data should be moved to private
  //
  int depth;

  int priority;

  //true if this box is a leaf
  bool isLeaf;

  //radius of box
  double rB;

  //Pointers to children, but when no children (i.e., leaf),
  //  the pointers are used as neighbor pointers
  // where
  //  0 = NW, 1 = EN, 2 = SE, 3 = WS (if pChildren are kids)
  //  0 = N, 1=E, 2=S, 3=3 (for neighbors)
  BoxT<NT>* pChildren[4];

  BoxT<NT>* pParent; //parent in quadtree

  // time of expansion (??)
  static int counter;
};

template <typename NT> int BoxT<NT>::counter=0;

//template <typename NT>
//ostream& operator<<(ostream& out, BoxT<NT>& box)
//{
//    out<<"Box=("<<box.x_range.getL()<<", "<<box.x_range.getR()<<"), ("
//       <<box.y_range.getL()<<", "<<box.y_range.getR()<<")";
//
//    return out;
//}

//Box iterator
template <typename NT>
class BoxIter
{
public:

    typedef BoxT<NT> Box;

    BoxIter(const Box* bb, int direc):b(bb), direction(direc), neighbor(0)
    {
        prev = (direc + 3) % 4;
        next = (direc + 1) % 4;
        cross = (direc + 2) % 4;
    }

    Box* First()
    {
        Box* n = b->pChildren[direction];
        if (n==NULL)
        {
            return 0;
        }

        // if neighbor are no smaller
        if (n->depth <= b->depth)
        {
            return n;
        }

        if(Box::isOverLimit(b, n))
        {
            cout<<"dir = "<<direction<<" b="<<(*b)<<" n="<<(*n)<<" Pointer b="<<b<<" n="<<n<<endl;
        }

        assert(!Box::isOverLimit(b, n));


        while (true)
        {
            if (n->pChildren[next]==NULL || Box::isOverLimit(b, n->pChildren[next]))
            {
                break;
            }
            n = n->pChildren[next];
        }

        //int t = 0;
        while (true)
        {
            if (n->pChildren[cross] == b)
            {
                break;
            }
            n = n->pChildren[cross];
        }
        neighbor = n;
        return neighbor;
    }

    Box* Next()
    {
        if (!neighbor)
        {
            return 0;
        }

        neighbor = neighbor->pChildren[prev];
        return neighbor;
    }


    Box* End()
    {
        if (b->pChildren[direction])
        {
            return b->pChildren[direction]->pChildren[prev];
        }
        return 0;
    }

private:

    const BoxT<NT>* b;
    int direction;
    int prev;
    int next;
    int cross;
    Box* neighbor;

}; //class BoxIter


template<typename NT>
bool BoxT<NT>::Split(NT epsilon)
{
    if (this->height() < epsilon || this->width() < epsilon)
    {
        return false;
    }

    //record the time of this split event, will be used to set priority of children
    ++BoxT<NT>::counter;
    BoxT<NT>* children[4];

    const NT &x_start = x_range.getL();
    const NT &x_end = x_range.getR();
    const NT &x_mid = x_range.mid();
    const NT &y_start = y_range.getL();
    const NT &y_end = y_range.getR();
    const NT &y_mid = y_range.mid();

    const unsigned int gen_id = generation_id + 1;

    children[0] = new BoxT<NT>(gen_id,IntervalT<NT>(x_start, x_mid), IntervalT<NT>(y_mid, y_end));
    children[1] = new BoxT<NT>(gen_id,IntervalT<NT>(x_mid, x_end), IntervalT<NT>(y_mid, y_end));
    children[2] = new BoxT<NT>(gen_id,IntervalT<NT>(x_mid, x_end), IntervalT<NT>(y_start, y_mid));
    children[3] = new BoxT<NT>(gen_id,IntervalT<NT>(x_start, x_mid), IntervalT<NT>(y_start, y_mid));


//    cout<<"---------> parent="<<*this<<" ("<<this<<" )"<<endl;
//    for(int n=0;n<4;n++)
//    {
//        if(pChildren[n]==NULL) continue;
//        cout<<"--------->\tneighbor["<<n<<"]="<<*pChildren[n]<<" ("<<pChildren[n]<<")"<<endl;
//    }

    for (int i = 0; i < 4; ++i)
    {
        if(children[i]==NULL)
        {
            cerr<<"! Error: Box::split: Not enough memory"<<endl;
            return false;
        }

        children[i]->depth = this->depth + 1;

        //distribute the feature
        /*
        BoxNode node;
        node.pos=children[i]->o;
        determine_clearance(node);
        children[i]->cl_m=node.clearance;
        distribute_features2box(children[i]);
        */
    }

    //
    // for internal nodes, children [i] is the i-th child
    // for leaves, children [i] is the pointer to first node in i-th adj list
    //
    for (int i = 0; i < 4; ++i)
    {
        //find three other directions
        int prev = (i + 3) % 4;
        int next = (i + 1) % 4;
        int cross = (i + 2) % 4;

        //update easy cases
        children[i]->pChildren[i] = pChildren[i];
        children[i]->pChildren[next] = children[next];
        children[i]->pChildren[cross] = children[prev];

        //init box neighbor iterator for direction i
        BoxIter<NT> iter(this, i);
        BoxT<NT>* neighbor = iter.First();

        if (!neighbor)
        {
            continue;
        }

        // if neighbor are no smaller
        if (neighbor->depth <= this->depth)
        {
            //after split child 'next' should also point to
            //neighbor in direction i
            children[next]->pChildren[i] = neighbor;

            //if neighbor's cross direction point to this, it should
            //instead point to child 'next' after split
            if (neighbor->pChildren[cross] == this)
            {
                neighbor->pChildren[cross] = children[next];
            }
            continue;
        }

        BoxT<NT>* prevNeighbor = neighbor;

        //indicate if we go across the boundary between child 'i'
        //and 'next' the first time
        bool firstTimeCrossBetweenChildren = true;

        //if neighbor smaller
        while(neighbor != iter.End())
        {
            //within the strip of child next, neighbor's cross direction
            //should point to next
            if (!isOverLimit(children[next], neighbor))
            {
                neighbor->pChildren[cross] = children[next];
            }

            //within the strip of child i, neighbor's cross
            //direction should point to i
            else if (!isOverLimit(children[i], neighbor))
            {
                neighbor->pChildren[cross] = children[i];

                //first time cross between child i and next,
                //should update next's i direction pointer
                if (firstTimeCrossBetweenChildren)
                {
                    firstTimeCrossBetweenChildren = false;
                    children[next]->pChildren[i] = prevNeighbor;
                }
            }
            else
            {
                assert(0);
            }
            prevNeighbor = neighbor;
            neighbor = iter.Next();
        }
    }


    //setup the children/parent relation
    for (int i = 0; i < 4; ++i)
    {
        this->pChildren[i] = children[i];
        this->pChildren[i]->pParent = this;

//        cout<<"pChildren["<<i<<"]="<<*pChildren[i]<<endl;
//
//        for(int n=0;n<4;n++)
//        {
//            if(children[i]->pChildren[n]==NULL) continue;
//            cout<<"\tneighbor["<<n<<"]="<<*children[i]->pChildren[n]<<" ("<<children[i]->pChildren[n]<<")"<<endl;
////            if(children[i]->pChildren[n]->depth<children[i]-depth)
////            {
////                assert(!isOverLimit(children[i]->pChildren[n], children[i]));
////            }
////            else{
////                assert(!isOverLimit(children[i], children[i]->pChildren[n]));
////            }
//
//        }
    }

    this->isLeaf = false;

    return true;
}

template <typename NT>
inline std::ostream& operator<<(std::ostream& o, const BoxT<NT>& B) {
	o << B.x_range << " * "<< B.y_range;
	return o;	
}

#endif /* BOX_H_ */
