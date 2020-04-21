#include "definitions.h"
#ifdef PLOT
#include "plot.h"
#endif

using namespace std;

void  clip_line(Edge& e, int plotflag);

MyFloat pxmin, pxmax, pymin, pymax, cradius;

// yap
// need to update xmin and xmax, ymin and ymax
void updateBoundingBox(const MyFloat& x, const MyFloat& y)
{
  if (xmin > x) xmin = x;
  if (xmax < x) xmax = x;
  if (ymin > y) ymin = y;
  if (ymax < y) ymax = y;
}

void OutPoint(const MyFloat& x, const MyFloat& y)
{ 
   cout << ((MyFloat)RES * x).intValue() << "\t" 
	<< ((MyFloat)RES * y).intValue() << "\t";
   updateBoundingBox(x, y);
}

void out_bisector(Edge& e)
{
#ifdef PLOT
   if(triangulate & plot &!debug)
      line(e.r->reg[0].x(), e.r->reg[0].y(),
      e.r->reg[1].x(), e.r->reg[1].y());
#endif
   // yap
   if(triangulate & postscript &!debug) {
      OutPoint(e.r->reg[0].x(), e.r->reg[0].y());
      OutPoint(e.r->reg[1].x(), e.r->reg[1].y());
      cout << " edge\n";
   }
   double a = e.r->a;
   double b = e.r->b;
   double c = e.r->c;
   if(!triangulate & !plot &!debug &!postscript)
      cout << "l " << a << " " << b << " " << c << "\n";
   if(debug) {
      cout << "line(" << e.r->edgenbr << ") " << a << "x+" << b <<
      "y=" << c << ", bisecting " << 
      e.r->reg[le].id() << " " << e.r->reg[re].id() << "\n";
   }
}


// output endpoint (ep)
void out_ep(Edge& e)
{
   if((!triangulate) & plot) 	// put (..) around ! to avoid warning
      clip_line(e,1);
   if((!triangulate) & postscript) 	// put (..) around ! to avoid warning
      clip_line(e,0);
   if((!triangulate) & (!plot) & (!postscript))	// put (..) around ! 
   {	cout << "e " << e.r->edgenbr << " " ;
   
   if (e.r->ep[le].isNull()) cout << "-1 ";
   else cout << e.r->ep[le].id() << " ";
   if (e.r->ep[re].isNull()) cout << "-1\n";
   else cout << e.r->ep[re].id() << "\n";

   if(debug){
	cout << "edge(" << e.r->edgenbr << "), (?) ";
	if (e.r->ep[le].isNull()) cout << "left endpoint id = -1 ";
	else cout << "left endpoint id = " << e.r->ep[le].id();
	if (e.r->ep[re].isNull()) cout << "right endpoint id = -1 ";
	else cout << "right endpoint id = " << e.r->ep[re].id();
	cout << endl;
   }
   };
}

void
Vertex::output()
{
   double xd = x();
   double yd = y();
   if(!triangulate && !plot && !debug && !postscript)
      cout << "v " << xd << " " << yd << "\n";
   if(debug)
      cout << "vertex(" << id() << ") at " << xd << " " << yd << "\n";
}


void
Site::output()
{
   double xd = x();
   double yd = y();
#ifdef PLOT
   if(!triangulate && plot && !debug)	// changed & to && to avoid warning
      circle (xd, yd, cradius);
#endif
   if(!triangulate && postscript && !debug)  // changed & to && to avoid warning
   { OutPoint(x(),y());
	  cout << " vertex\n";
   }
   if(!triangulate && !plot && !debug && !postscript)// changed & to && 
      cout << "s " << xd << " " << yd << "\n";
   if(debug)
      cout << "site (" << id() << ") at " << xd
      << " " << yd << "\n";
}


void out_triple(const Site& s1, const Site& s2, const Site& s3)
{
   if(triangulate & !plot &!debug & !postscript)
      cout << s1.id() << " " <<  s2.id() << " " <<
      s3.id() << "\n";
   if(debug)
      cout << "circle through left=" << s1.id() << " right="
      << s2.id() << " bottom=" <<  s3.id() << "\n";
}



void plotinit()
{
 MyFloat dx,dy,d;
   
   dy = ymax - ymin;
   dx = xmax - xmin;
   d = ( dx > dy ? dx : dy) * (MyFloat)1.1;
   pxmin = xmin - (d-dx)/(MyFloat)2.0;
   pxmax = xmax + (d-dx)/(MyFloat)2.0;
   pymin = ymin - (d-dy)/(MyFloat)2.0;
   pymax = ymax + (d-dy)/(MyFloat)2.0;
   cradius = (pxmax - pxmin)/(MyFloat)350.0;
#ifdef PLOT 
   if (plot) {
      openpl();
      range(pxmin, pymin, pxmax, pymax);
   }
#endif
   // yap
   if (postscript) {
      cout << "%!PS-Adobe-2.0\n";
      cout << "%% draw vertex\n"
         << "/vertex {\n"
         << "2 0 360 arc fill\n"
         << "} def\n\n";
      cout << "%% draw edge \n"
         << "/edge {\n"
         << "/y2 exch def\n"
         << "/x2 exch def\n"
         << "moveto\n"
         << "x2 y2 lineto stroke\n"
         << "} def\n\n";
      
      cout << XTRANS << "\t" << YTRANS << "\t translate\n"
/*
	   << "newpath\n"
	   << ((MyFloat)RES *pxmin).intValue() << "\t" 
	   << ((MyFloat)RES *pymin).intValue() << "\t moveto\n"; 
      cout << ((MyFloat)RES *pxmin).intValue() << "\t" 
	   << ((MyFloat)RES *pymax).intValue() << "\t lineto\n";
      cout << ((MyFloat)RES *pxmax).intValue() << "\t" 
	   << ((MyFloat)RES *pymax).intValue() << "\t lineto\n";
      cout << ((MyFloat)RES *pxmax).intValue() << "\t" 
	   << ((MyFloat)RES *pymin).intValue() << "\t lineto\n";
      cout << "closepath\n"
*/
         << "2 setlinewidth\n"
         << "stroke \n"
         << "1 setlinewidth\n\n";
   } // yap
}


void  clip_line(Edge&  e, int plotflag)
{
   Site s1, s2;
   MyFloat x1,x2,y1,y2;
   
   if(e.r -> a == (MyFloat)1.0 && e.r ->b >= (MyFloat)0.0)
   {	s1 = e.r -> ep[1];
   s2 = e.r -> ep[0];
   }
   else 
   {	s1 = e.r -> ep[0];
   s2 = e.r -> ep[1];
   };
   
   if(e.r -> a == (MyFloat)1.0)
   {
      y1 = pymin;
      if (s1.notNull() && s1.y() > pymin)
         y1 = s1.y();
      if(y1>pymax) return;
      x1 = e.r -> c - e.r -> b * y1;
      y2 = pymax;
      if (s2.notNull() && s2.y() < pymax) 
         y2 = s2.y();
      if(y2<pymin) return;
      x2 = e.r -> c - e.r -> b * y2;
      if (((x1> pxmax) & (x2>pxmax)) | ((x1<pxmin)&(x2<pxmin))) return;
      if(x1> pxmax)
      {	x1 = pxmax; y1 = (e.r -> c - x1)/e.r -> b;};
      if(x1<pxmin)
      {	x1 = pxmin; y1 = (e.r -> c - x1)/e.r -> b;};
      if(x2>pxmax)
      {	x2 = pxmax; y2 = (e.r -> c - x2)/e.r -> b;};
      if(x2<pxmin)
      {	x2 = pxmin; y2 = (e.r -> c - x2)/e.r -> b;};
   }
   else
   {
      x1 = pxmin;
      if (s1.notNull() && s1.x() > pxmin) 
         x1 = s1.x();
      if(x1>pxmax) return;
      y1 = e.r -> c - e.r -> a * x1;
      x2 = pxmax;
      if (s2.notNull() && s2.x() < pxmax) 
         x2 = s2.x();
      if(x2<pxmin) return;
      y2 = e.r -> c - e.r -> a * x2;
      if (((y1> pymax) & (y2>pymax)) | ((y1<pymin)&(y2<pymin))) return;
      if(y1> pymax)
      {	y1 = pymax; x1 = (e.r -> c - y1)/e.r -> a;};
      if(y1<pymin)
      {	y1 = pymin; x1 = (e.r -> c - y1)/e.r -> a;};
      if(y2>pymax)
      {	y2 = pymax; x2 = (e.r -> c - y2)/e.r -> a;};
      if(y2<pymin)
      {	y2 = pymin; x2 = (e.r -> c - y2)/e.r -> a;};
   };
   
#ifdef PLOT
   if (plotflag)
      line(x1,y1,x2,y2);
#endif
   if (postscript) {
      cout << ((MyFloat)RES *x1).intValue() << "\t" 
	   << ((MyFloat)RES *y1).intValue() << "\t" 
	   << ((MyFloat)RES *x2).intValue() << "\t" 
	   << ((MyFloat)RES *y2).intValue() << "\t" << " edge\n";
      updateBoundingBox(x1,y1);
      updateBoundingBox(x2,y2);
   }
}

void plotclose()
{
   MyFloat dx,dy,d;
   
   dy = ymax - ymin;
   dx = xmax - xmin;
   d = ( dx > dy ? dx : dy) * (MyFloat)1.1;
   pxmin = xmin;// - (d-dx)/(MyFloat)2.0;
   pxmax = xmax;// + (d-dx)/(MyFloat)2.0;
   pymin = ymin;// - (d-dy)/(MyFloat)2.0;
   pymax = ymax;// + (d-dy)/(MyFloat)2.0;
#ifdef PLOT 
   if (plot) {
      openpl();
      range(pxmin, pymin, pxmax, pymax);
   }
#endif
#ifdef PLOT
   if (plot) closepl();
#endif
   if (postscript){
      cout << "newpath\n"
	   << (RES*pxmin).intValue() << "\t" 
	   << (RES*pymin).intValue() << "\t moveto\n"; 
      cout << (RES*pxmin).intValue() << "\t" 
	   << (RES*pymax).intValue() << "\t lineto\n";
      cout << (RES*pxmax).intValue() << "\t" 
	   << (RES*pymax).intValue() << "\t lineto\n";
      cout << (RES*pxmax).intValue() << "\t" 
	   << (RES*pymin).intValue() << "\t lineto\n";
      cout << "closepath\n";
      cout << "stroke\n";
     cout << "\n showpage\n";
   }
}
