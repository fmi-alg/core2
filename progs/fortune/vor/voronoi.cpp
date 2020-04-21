#include "definitions.h"


/* implicit parameters: nsites, sqrt_nsites, xmin, xmax, ymin, ymax,
   deltax, deltay (can all be estimates).
   Performance suffers if they are wrong; better to make nsites,
   deltax, and deltay too big than too small.  (?) */

void voronoi(int triangulate)
{
 Site  bot, top, temp;
 Vertex p;
 Vertex v;
 Point newintstar;
int pm;
 Halfedge lbnd, rbnd, llbnd, rrbnd, bisector;
 Edge e;


PQinitialize();
Site bottomsite = nextsite();
bottomsite.output();
ELinitialize();

Site newsite = nextsite();
while(1)
{
	if(!PQempty()) PQ_min(&newintstar);

	if (newsite.notNull() &&
		 (PQempty() || newsite.point() < newintstar))
	{/* new site is smallest */
		newsite.output();
		lbnd = ELleftbnd(&(newsite.point()));
		rbnd = lbnd.ELright();
		bot = lbnd.rightreg(bottomsite);
		e = bisect(bot, newsite);
		bisector = Halfedge(e, le); //le=0 (see definitions.h)
		ELinsert(lbnd, bisector);
		p = intersect(lbnd, bisector);
		if (p.notNull())
		{	
			PQdelete(lbnd);
			PQinsert(lbnd, p, dist(p.point(),newsite.point()));
		};
		lbnd = bisector;
		bisector = Halfedge(e, re); //re=1 (see definitions.h)
		ELinsert(lbnd, bisector);
		p = intersect(bisector, rbnd);
		if (p.notNull())
		{
		 	PQinsert(bisector, p, dist(p.point(),newsite.point()));	
		}
		newsite = nextsite();	
	}
	else if (!PQempty()) 
	/* intersection is smallest */
        {
	 	lbnd = PQextractmin();
		llbnd = lbnd.ELleft();
		rbnd = lbnd.ELright();
		rrbnd = rbnd.ELright();
		bot = lbnd.leftreg(bottomsite);
		top = rbnd.rightreg(bottomsite);
		out_triple(bot, top, lbnd.rightreg(bottomsite));
		v = lbnd.r->vertex;
		v.makevertex();
		endpoint(lbnd.r->ELedge,lbnd.r->ELpm,v);
		endpoint(rbnd.r->ELedge,rbnd.r->ELpm,v);
		ELdelete(lbnd); 
		PQdelete(rbnd);
		ELdelete(rbnd); 
		pm = le;
		if (bot.y() > top.y())
		{	temp = bot; bot = top; top = temp; pm = re;}
		e = bisect(bot, top);
		bisector = Halfedge(e, pm);
		ELinsert(llbnd, bisector);
		endpoint(e, re-pm, v);
		p = intersect(llbnd, bisector);
		if (!p.isNull())
		{	PQdelete(llbnd);
			PQinsert(llbnd, p, dist(p.point(),bot.point()));
		};
		p = intersect(bisector, rrbnd);
		if (!p.isNull())
		{	PQinsert(bisector, p, dist(p.point(),bot.point()));
		};
	}
	else break;
};

for (Halfedge & h = const_cast<Halfedge &>(ELleftend.ELright()); 
     h != ELrightend; 
     h=const_cast<Halfedge &>(h.ELright()))	{	
   e = h.r -> ELedge;
   out_ep(e);
};
}
