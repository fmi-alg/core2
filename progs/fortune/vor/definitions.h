/*   MyFloat     */

#include <math.h>  // must include this BEFORE BigFloat

#ifndef CORE_LEVEL
	#define CORE_LEVEL 3
#endif
#include "CORE.h"

typedef double MyFloat;	
		// MyFloat is the number type assumed in
		// the program.  At CORE_LEVEL 3, this means MyFloat is
		// really Expr.  At CORE_LEVEL 1, MyFloat is machine double.

#ifndef NULL
#define NULL 0
#endif
#define DELETED -2
#define le 0
#define re 1

// yap: resolution of postscript output
const int RES = 250;
// yap: translation of postscript output
const int XTRANS = 10;
const int YTRANS = 15;

#define NEED_INPUT
//typedef float MyFloat;

/*   Freenode   */
struct	Freenode_struct	{ struct	Freenode_struct	*nextfree;
		        };
typedef struct Freenode_struct Freenode;

/*   Freelist   */
/*
class  Freelist	{ Freenode	*head;
	  	  int			nodesize;
public:
		Freelist(int size);
		void *getfree();
		void makefree(void* curr);
		};
*/


/*   Point       */
class Point {
public:
  MyFloat x;
  MyFloat y;

  Point()   {}
  Point(const MyFloat& xx, const MyFloat& yy) { x = xx; y = yy; }
  friend int compare(const Point& p1, const Point& p2);
 };

bool operator < (const Point& p1, const Point& p2);

/*   Site       */
class Site;  // forward reference
class Vertex;  // forward reference

/* structure used both for sites and for vertices */
class SiteRep {
	Point	coord;
        int	sitenbr;
        int	refcnt;

//	static Freelist flist;
//	void* operator new (unsigned size) {return flist.getfree();}
//	void operator delete (void *ptr)   {flist.makefree(ptr);}
public:
	SiteRep(const MyFloat& x, const MyFloat& y, int id)
		 : coord(x, y) { sitenbr = id; refcnt = 0;}

	friend class Site;
	friend class Vertex;
};

class Site {
	SiteRep *r;

	void kill() { if (r != (SiteRep *)NULL) 
			 if (--(r->refcnt) <= 0) delete r;
		    }
	void ref() { if (r != (SiteRep *)NULL) r->refcnt++; }
public:

	Site()  { r = (SiteRep *)NULL; }
	Site(MyFloat& x, MyFloat& y, int id) { r = new SiteRep(x,y,id);
				       ref();
				     }
	Site(const Site& s) { r = s.r;
			ref();
			}
   void setSite(MyFloat x, MyFloat y, int id) {
      r = new SiteRep(x, y, id);
      ref();
   }
   void operator = (const Site & s) {
      (const_cast<Site &>(s)).ref();
      kill();
      r = s.r;
   }
	virtual ~Site() { kill();}

	int isNull()  { return (r == (SiteRep *)NULL); }
	int notNull() { return (r != (SiteRep *)NULL); }
   void setNull() { r = NULL; }

	MyFloat& x()  const {return r->coord.x; }
	MyFloat& y()  const {return r->coord.y; }
	Point& point() const { return r->coord; }
	int id() const { return r->sitenbr; }

	void output();
   void print (std::ostream &o) const {
      o << "(" << x() << ", " << y() << ")" << std::endl;
   }

	friend bool operator == (const Site& s1, const Site& s2);

	friend class Vertex;
};

class Vertex : public Site {
public:
	static int nvertices;
	Vertex()  {r = (SiteRep *)NULL; }
	Vertex(MyFloat& x, MyFloat& y) { r = new SiteRep(x,y,0);
					 ref();
				       }
	Vertex(const Vertex& v) { r = v.r;
			ref();
			}
   void operator = (const Vertex & s) {
      (const_cast<Vertex &>(s)).ref();
      kill();
      r = s.r;
   }
   void operator = (Site & s) { s.ref();
				kill();
				r = s.r;
				}
	void makevertex();
	void output();
};



/*   Edge       */
class Edge;  // forward reference

class EdgeRep	{
//	static Freelist flist;
//	void* operator new (unsigned size) {return flist.getfree();}
//	void operator delete (void *ptr)   {flist.makefree(ptr);}
public:
			  MyFloat	a,b,c; // equation of line is aX+bY=c
			  Vertex 	ep[2];
			  Site		reg[2];
			  int		edgenbr;
			  int		refcnt;

			  static int	nedges;

	EdgeRep(Site& s1, Site& s2);

	friend class Edge;
	friend Vertex intersect(Edge& e1, Edge& e2);

};

class Edge {
public:			EdgeRep* r;
	void ref()  { if (r != (EdgeRep *)NULL) r->refcnt++; }
	void kill() { if (r != (EdgeRep *)NULL)
		 	if ((--r->refcnt) < 1) delete r;
		    }

	Edge()	{ r = (EdgeRep *)NULL; }
	Edge(Site& s1, Site& s2) { r = new EdgeRep(s1, s2); ref();}
	Edge(const Edge& e) { r = e.r; ref(); }
	~Edge() {kill(); }
	void operator = (const Edge& e) { (const_cast<Edge &>(e)).ref(); kill(); r = e.r; }

	int isNull() { return (r == (EdgeRep *)NULL); }
	int notNull() { return (r != (EdgeRep *)NULL); }

	Site& reg(int lr) { return r->reg[lr]; }

};



/*    Halfedge      */
class HalfedgeRep;  // forward reference;


class Halfedge {
	void ref();
	void kill();
public:
	HalfedgeRep* r;

	Halfedge() { r = (HalfedgeRep *)NULL; }
	Halfedge(const Edge& e, int pm);
	Halfedge(const Halfedge& he) { r = he.r;  ref(); }
	~Halfedge() { kill(); }

	void operator = (const Halfedge& he) { (const_cast<Halfedge &>(he)).ref(); kill(); r = he.r; }

	int isNull()  { return (r == (HalfedgeRep *)NULL); }
	int notNull() { return (r != (HalfedgeRep *)NULL); }
        int isDeleted();

	friend bool operator == (const Halfedge& h1, const Halfedge& h2);
	friend bool operator != (const Halfedge& h1, const Halfedge& h2);

	const Halfedge & ELleft() const;
	const Halfedge & ELright() const;
	Site& leftreg(Site& bottomsite);
	Site& rightreg(Site& bottomsite);
};


class HalfedgeRep {
//	static Freelist flist;
//	void* operator new (unsigned size) {return flist.getfree();}
//	void operator delete (void *ptr)   {flist.makefree(ptr);}
public:
			 Halfedge	ELleft, ELright;
			 Edge	ELedge;
			 int	ELrefcnt;
			 char	ELpm;
			 Vertex	vertex;
			 MyFloat ystar;
			 int    deleted;

			HalfedgeRep(const Edge& e, int pm) 
				{ 
              ELedge = const_cast<Edge &>(e);
				  ELpm = pm;
				  ELrefcnt = 0;
				  deleted = 0;
				}


			friend class Halfedge;
			};

bool operator == (const Halfedge& h1, const Halfedge& h2);

bool operator != (const Halfedge& h1, const Halfedge& h2);

/* the following functions are in edgelist.c */
void ELinitialize();
void ELinsert(Halfedge lb, Halfedge neww);
Halfedge ELgethash(int b);
Halfedge ELleftbnd(Point *p);
void ELdelete(Halfedge he);

/* the following functions ae in geometry.c */
void geominit();
Edge bisect(Site& s1, Site& s2);
Vertex intersect(Halfedge el1, Halfedge el2);
int right_of(Halfedge el, Point *p);
void  endpoint(Edge& e, int lr, Vertex& v);
MyFloat dist(const Point& s, const Point& e);

/* the following functions are in heap.c */
void PQinsert(Halfedge& he, Vertex& v, MyFloat offset);
void PQdelete(Halfedge& he);
int PQbucket(Halfedge& he);
int PQempty();
void PQ_min(Point *answer);
Halfedge PQextractmin();
void PQinitialize();

/* The following is defined in main.cc */
Site& nextsite();

/* The following functions are in memory.c */
void moreSites(Site **s, unsigned current, unsigned additional);


/* The following functions are in output.c */
void out_bisector(Edge& e);
void out_ep(Edge& e);
void out_triple(const Site& s1, const Site& s2, const Site& s3);
void plotinit();
void plotclose();

/* The following function is in voronoi.c */
void voronoi(int triangulate);

/* global data */
/*
#ifdef INCLUDEGLOBAL
int triangulate, sorted, plot, debug, postscript;
MyFloat xmin, xmax, ymin, ymax, deltax, deltay;
int	nsites;
int	siteidx;
int	sqrt_nsites;
const Halfedge & ELleftend = Halfedge(Edge(), 0);
const Halfedge & ELrightend = Halfedge(Edge(), 0);
#else
extern int triangulate, sorted, plot, debug, postscript;
extern MyFloat xmin, xmax, ymin, ymax, deltax, deltay;
extern std::size_t nsites;
extern int	siteidx;
extern int	sqrt_nsites;
extern const Halfedge & ELleftend;
extern const Halfedge & ELrightend;
#endif
*/

extern int triangulate, sorted, plot, debug, postscript;
extern MyFloat xmin, xmax, ymin, ymax, deltax, deltay;
extern int nsites;
extern int	siteidx;
extern int	sqrt_nsites;
extern const Halfedge & ELleftend;
extern const Halfedge & ELrightend;
