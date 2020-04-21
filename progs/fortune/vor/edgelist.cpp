
#include "definitions.h"

int     ELhashsize;
Halfedge *ELhash;

void ELinitialize()
{
	ELhashsize = 2 * sqrt_nsites;
	ELhash =  new Halfedge[ELhashsize];
	//ELleftend = Halfedge( Edge(), 0);
	//ELrightend = Halfedge(Edge(), 0);
	ELleftend.r -> ELright = ELrightend;
	ELrightend.r -> ELleft = ELleftend;
	ELhash[0] = ELleftend;
	ELhash[ELhashsize-1] = ELrightend;
}



void ELinsert(Halfedge lb, Halfedge neww)
{
	neww.r -> ELleft = lb;
	neww.r -> ELright = lb.ELright();
	(lb.r -> ELright).r -> ELleft = neww;
	lb.r -> ELright = neww;
}


Halfedge NullHalfedge;

/* Get entry from hash table, pruning any deleted nodes */
Halfedge ELgethash(int b)
{
 Halfedge he;

	if(b<0 || b>=ELhashsize) return NullHalfedge;
	he = ELhash[b]; 
	if (!he.isDeleted()) return (he);

/* Hash table points to deleted half edge.  Patch as necessary. */
	ELhash[b] = NullHalfedge;
	return  NullHalfedge;
}	

Halfedge ELleftbnd(Point *p)
{
int i, bucket;
 Halfedge he;

/* Use hash table to get close to desired halfedge */
 bucket = ((p->x - xmin)/deltax * (ELhashsize)).intValue();
 // double bucket2 = ((p->x - xmin)/deltax * ELhashsize);  
	if(bucket<0) bucket =0;
	if(bucket>=ELhashsize) bucket = ELhashsize - 1;
	he = ELgethash(bucket);
	if(he.isNull())
	{   for(i=1; 1 ; i += 1)
	    {	he=ELgethash(bucket-i);
	     	if (he.notNull()) break;
		he=ELgethash(bucket+i);
	     	if (he.notNull()) break;
	    }
	}
/* Now search linear list of halfedges for the corect one */
	if (he==ELleftend  || (he != ELrightend && right_of(he,p)))
	{do {he = he.ELright();} while (he!=ELrightend && right_of(he,p));
	 he = he.ELleft();
	}
	else 
	do {he = he.ELleft();} while (he!=ELleftend && !right_of(he,p));

/* Update hash table and reference counts */
	if(bucket > 0 && bucket <ELhashsize-1)
	{	
		ELhash[bucket] = he;
	};
	return (he);
}

	
/* This delete routine can't reclaim node, since pointers from hash
   table may be present.   */
void ELdelete(Halfedge he)
{
	(he.r -> ELleft).r -> ELright = he.ELright();
	(he.r -> ELright).r -> ELleft = he.ELleft();
	he.r -> deleted = 1;
}


Site&
Halfedge::leftreg(Site& bottomsite)
{
	if((r->ELedge).isNull()) return(bottomsite);
	return (r -> ELedge.reg(r->ELpm));
}

Site&
Halfedge::rightreg(Site& bottomsite)
{
	if((r->ELedge).isNull()) return(bottomsite);
	return (r -> ELedge.reg(1 - r->ELpm));
}


