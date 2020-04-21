#include "definitions.h"
#include "PQlist.h"

int triangulate, sorted, plot, debug, postscript;
MyFloat xmin, xmax, ymin, ymax, deltax, deltay;
int	nsites;
int	siteidx;
int	sqrt_nsites;
const Halfedge & ELleftend = Halfedge(Edge(), 0);
const Halfedge & ELrightend = Halfedge(Edge(), 0);

PQlist::PQlist(Halfedge& he, PQlist& next)
	{ r = new PQlistRep(he, next); }


PQlist PQlist::PQnext()
 { return r->PQnext; }

Halfedge PQlist::he()
 { return r->he; }

Halfedge:: Halfedge(const Edge& e, int pm) { r = new HalfedgeRep(e, pm); ref(); }

void Halfedge::ref() { if (r != (HalfedgeRep *)NULL) r->ELrefcnt++; }

void Halfedge::kill() { if (r != (HalfedgeRep *)NULL)
		if (--(r->ELrefcnt) <= 0) delete r;
		    }

const Halfedge & Halfedge::ELleft() const { return r->ELleft; }

const Halfedge &Halfedge::ELright() const { return r->ELright; }

int Halfedge::isDeleted() { if (isNull()) return 0;
			  return (r->deleted);
			}
bool operator == (const Halfedge& h1, const Halfedge& h2)
  { return (h1.r == h2.r); }

bool operator != (const Halfedge& h1, const Halfedge& h2)
  { return (h1.r != h2.r); }

bool operator < (const Point& p1, const Point& p2)
  { return (compare(p1, p2) < 0);  }
