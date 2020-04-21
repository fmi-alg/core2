#include "definitions.h"


int EdgeRep::nedges = 0;
int Vertex::nvertices = 0;

void geominit()
{
 double sn;  // This is just used for finding a square root

	sn = nsites+4;
	sqrt_nsites = (sqrt(sn)).intValue();
	deltay = ymax - ymin;
	deltax = xmax - xmin;
}

int compare(const Point& p1, const Point& p2)
  { if (p1.y < p2.y) return -1;
    if (p1.y > p2.y) return 1;
    if (p1.x < p2.x) return -1;
    if (p1.x > p2.x) return 1;
    return 0;
  }


EdgeRep::EdgeRep(Site& s1, Site& s2)
 { MyFloat dx,dy,adx,ady;

   reg[0]=s1;
   reg[1]=s2;
   edgenbr=nedges++;
   refcnt = 0;

   dx = s2.x() - s1.x();
   dy = s2.y() - s1.y();
   if (dx > (MyFloat)0) adx = dx;
                   else adx = -dx;
   if (dy > (MyFloat)0) ady = dy;
                   else ady = -dy;
   c = s1.x() * dx + s1.y() * dy + (dx*dx + dy*dy) * (MyFloat)0.5;

   if (adx>ady) {
			a = 1.0;
			b = dy/dx;
			c /= dx;
		}
	else	{
			b = 1.0;
			a = dx/dy;
			c /= dy;
		};
 }


Edge bisect(Site& s1, Site& s2)
{
 Edge newedge(s1, s2);

 out_bisector(newedge);
 return(newedge);
}

int isSmall(MyFloat& f)
  {
// CHen Li: use exact comparison
    // if (f > (MyFloat)1.0e-10) return 0;
    // if (f < (MyFloat)-1.0e-10) return 0;
    return (f == 0);
  }

Vertex intersect(Halfedge el1, Halfedge el2)
{
  Edge e1, e2, e;
  Halfedge el;
  MyFloat d, xint, yint;
  int right_of_site;
  Vertex v;	// a null vertex until we assign it something else

	e1 = el1.r -> ELedge;
	e2 = el2.r -> ELedge;
	if(e1.isNull() || e2.isNull()) return v;
	if (e1.r->reg[1] == e2.r->reg[1]) return v;

	d = e1.r->a * e2.r->b - e1.r->b * e2.r->a;
	if (isSmall(d)) return v;

	xint = (e1.r->c*e2.r->b - e2.r->c*e1.r->b)/d;
	yint = (e2.r->c*e1.r->a - e1.r->c*e2.r->a)/d;

	if( e1.r->reg[1].point() < e2.r->reg[1].point())
	{	el = el1; e = e1;}
	else
	{	el = el2; e = e2;};
	right_of_site = xint >= e.r -> reg[1].x();
	if ((right_of_site && el.r -> ELpm == le) ||
	   (!right_of_site && el.r -> ELpm == re)) return v;

	Vertex v2(xint, yint);
	return(v2);
}

/* returns 1 if p is to right of halfedge e */
int right_of(Halfedge el, Point *p)
{
 Edge e;
 Site topsite;
int right_of_site, above, fast;
MyFloat dxp, dyp, dxs, t1, t2, t3, yl;

e = el.r -> ELedge;
topsite = e.r -> reg[1];
right_of_site = p -> x > topsite.x();
if(right_of_site && el.r -> ELpm == le) return(1);
if(!right_of_site && el.r -> ELpm == re) return (0);

if (e.r->a == (MyFloat)1.0)
{	dyp = p->y - topsite.y();
	dxp = p->x - topsite.x();
	fast = 0;
	if (((!right_of_site) & (e.r->b<(MyFloat)0.0)) | (right_of_site&(e.r->b>=(MyFloat)0.0)) )
	{	above = dyp>= e.r->b*dxp;	
		fast = above;
	}
	else 
	{	above = p->x + p->y*e.r->b > e.r-> c;
		if(e.r->b<(MyFloat)0.0) above = !above;
		if (!above) fast = 1;
	};
	if (!fast)
	{	dxs = topsite.x() - e.r->reg[0].x();
		above = e.r->b * (dxp*dxp - dyp*dyp) <
		        dxs*dyp*((MyFloat)(1.0)+(MyFloat)(2.0)*dxp/dxs + e.r->b*e.r->b);
		if(e.r->b<(MyFloat)(0.0)) above = !above;
	};
}
else  /*e->b==1.0 */
{	yl = e.r->c - e.r->a*p->x;
	t1 = p->y - yl;
	t2 = p->x - topsite.x();
	t3 = yl - topsite.y();
	above = t1*t1 > t2*t2 + t3*t3;
};
return (el.r->ELpm==le ? above : !above);
}


void endpoint(Edge& e, int lr, Vertex& s)
{
e.r -> ep[lr] = s;
if(e.r -> ep[re-lr].isNull()) return;
out_ep(e);
}


MyFloat dist(const Point& s, const Point& t)
{
double dx,dy;  // Again, these are used for finding a Square root
	dx = (MyFloat)(s.x) - (MyFloat)(t.x);
	dy = (MyFloat)(s.y) - (MyFloat)(t.y);
	return((MyFloat)sqrt(dx*dx + dy*dy));
}


void
Vertex::makevertex()
{
r -> sitenbr = nvertices++;
output();
}

bool operator == (const Site& s1, const Site& s2)
  { return (s1.r == s2.r);
  }
