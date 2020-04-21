#include "definitions.h"
#include "PQlist.h"

PQlist *PQhash;
int PQhashsize;
int PQcount;
int PQmin;

int
compare(const Halfedge& h1, const Halfedge& h2)
  { if (h1.r->ystar > h2.r->ystar) return 1;
    if (h1.r->ystar < h2.r->ystar) return -1;
    if (h1.r->vertex.x() > h2.r->vertex.x()) return 1;
    if (h1.r->vertex.x() < h2.r->vertex.x()) return -1;
    return(0);
  }

inline int
operator > (const Halfedge& h1, const Halfedge& h2) 
	{ return (compare(h1, h2) > 0);
	}

void 
PQlist::PQinsert(Halfedge& he)
 {
PQlist last;
PQlist next = *this;

while (next.notNull() && (he > next.r->he))
	{last = next;  next = next.r->PQnext;}

PQlistRep *neww = new PQlistRep(he, next);
if (last.isNull()) r = neww;
	    else last.r -> PQnext.r = neww;

PQcount += 1;
}

void PQinsert(Halfedge& he, Vertex& v, MyFloat offset)
{
he.r -> vertex = v;
he.r -> ystar = v.y() + offset;
int bucket = PQbucket(he);
PQhash[bucket].PQinsert(he);
}

void
PQlist::PQdelete(Halfedge& he)
{
PQlist last = *this;

if (last.r->he == he)
     {
	PQlist next = last.r->PQnext;
	delete r;
	r = next.r;
     }
else {
	while (last.r -> PQnext.r->he != he) last = last.r -> PQnext;

	PQlist next = last.r->PQnext.r -> PQnext;
        delete last.r->PQnext.r;
	last.r -> PQnext = next;
     }
PQcount -= 1;
}


void PQdelete(Halfedge& he)
{
if((he.r ->  vertex.isNull())) return;
he.r -> vertex.setNull();  // give it a Null site

int bucket = PQbucket(he);
PQhash[bucket].PQdelete(he);
}

int PQbucket(Halfedge& he)
{
int bucket;

bucket = ((he.r->ystar - ymin)/deltay * (MyFloat)PQhashsize).intValue();
if (bucket<0) bucket = 0;
if (bucket>=PQhashsize) bucket = PQhashsize-1 ;
if (bucket < PQmin) PQmin = bucket;
return(bucket);
}



int PQempty()
{
	return(PQcount==0);
}



void PQ_min(Point *answer)
{
	while(PQhash[PQmin].isNull()) {PQmin += 1;};
	answer->x = PQhash[PQmin].r->he.r -> vertex.x();
	answer->y = PQhash[PQmin].r->he.r -> ystar;
}

Halfedge
PQlist::extractmin()
{
 	Halfedge curr = r->he;
	PQlist next = r -> PQnext;
        delete r;
	r = next.r;
	--PQcount;
	return(curr);
}
Halfedge PQextractmin()
{
	return PQhash[PQmin].extractmin();
}


void PQinitialize()
{
	PQcount = 0;
	PQmin = 0;
	PQhashsize = 4 * sqrt_nsites;
	PQhash = new PQlist[PQhashsize];
}

