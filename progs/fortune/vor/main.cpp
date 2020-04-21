/* *****************************************************
 File: main.cpp

 	This is the Core Version of Fortune's implementation
	of his Voronoi diagram algorithm for a 2-D point set.

 Usage: The program (called voronoi here) reads from standard input and outputs
     	standard output. You can redirect input and output as follows:

		% ./voronoi < input_file > output_file

	Without any arguments, the default output is a Voronoi diagram
	represented by a text file with lines in one of the 4 forms:

	   s  [x] [y]		-- "site" with coordinates (x,y), an input point
	   v  [x] [y]		-- "voronoi vertex" with coordinates (x,y)
	   e  [u] [uL] [uR]	-- "voronoi edge" with ID=u, and with endpoints (i.e.,
	   			   	voronoi vertices) whose ID's are uL and uR.
	   l  [a] [b] [c]	-- "voronoi ray" with line equation aX+bY=c.

	The program takes several options:

		% voronoi [-option]

	where option is any combination taken from

	   d = debug	   -- similar to the default output, but more verbose
				(e.g., each site (s) will have an ID,
				each ray (l) will tell you the ID's of
				the two sites that it bisects, etc)
	   c = postscript  -- output a postscript file
	   t = triangulate -- compute Delaunay triangulation instead of Vor.Diag
	   s = sorted	   -- sort the points (to speed up computation)
	   p = plot	   -- disabled

	E.g.,
		% ./voronoi -tc < input_file > output_file.ps

		will output a postscript file representing the
		Delaunay triangulation of the input points.

 Author: Steve Fortune wrote the original code in C.
	1997: Tom Dube and Chee Yap adapted it for Real/Expr and C++.
 	2000: Celso Rodriguiz adapted it for Core Library.

 Core Library File
 ***************************************************** */

#include <ctime>
#include "getopt.h"
#define INCLUDEGLOBAL
#include "definitions.h"

using namespace std;

/* templates for functions defined in this file  */
extern "C" int scomp(const void *elem1, const void *elem2);
void readsites();
void sortsites();

// static data
static Site  *sites = NULL;

int main(int argc, char **argv) 
{	
   long msec;
   
   msec = clock();
   
   sorted = 0; triangulate = 0; plot = 0; debug = 0; postscript = 0;

   int c;
   while((c=getopt(argc,argv,"dpstfc")) != EOF)
      switch(c) {
   case 'd': debug = 1;
      break;
   case 's': sorted = 1;
      break;
   case 't': triangulate = 1;
      break;
   case 'p': plot = 1;
      break;
   case 'c': postscript = 1;
   };

   if(sorted) {
      cin >> nsites >> xmin >> xmax >> ymin >> ymax;
      readsites();
   } else {	
      readsites();
      sortsites();
   };
   
   siteidx = 0;
   geominit();
   if(plot | postscript) 
      plotinit();
   
   voronoi(triangulate);
   
if (!postscript) {

cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
cout << "Number of Voronoi edges = " << EdgeRep::nedges << endl;
cout << "Number of Voronoi vertices = " << Vertex::nvertices << endl;
cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
}

   if (plot | postscript) 
      plotclose();
   if (!postscript){
      msec = clock();
      cout << "The number of microseconds is " <<  msec << "\n";
   }
   return 0;
}

/* sort sites on y, then x  */
int scomp(const void * elem1, const void * elem2)
{
   Site * s1 = (Site *)(elem1);
   Site * s2 = (Site *)(elem2);
   if (s1 == NULL || s2 == NULL)
      return 0;
	if((*s1).y() < (*s2).y()) return(-1);
	if((*s1).y() > (*s2).y()) return(1);
	if((*s1).x() < (*s2).x()) return(-1);
	if((*s1).x() > (*s2).x()) return(1);
	return(0);
}

/* return a single in-storage site */

Site nullSite;

Site& nextsite()
{
   if(siteidx < nsites) return (sites[siteidx++]);
   else	return( nullSite);
}


void readsites()
{
   MyFloat x, y;
   
   nsites=0;
   sites = new Site[4096];
   
   cin >> x >> y;
   while (!cin.eof())
   {	// nsites should be handled inside of class Site
      sites[nsites] = Site(x, y, nsites);
      nsites += 1;
      if (nsites % 4000 == 0) 
         moreSites(&sites, nsites, 4096);
      
      cin >> x >> y;
   }
}


/*  sort, and compute xmin, xmax, ymin, ymax */
//  These xmin, etc, are only computed for sites, so for output we
//  also need to look at Voronoi vertices and Vor edges!
void sortsites()
{ 
   int i;

   qsort(sites, nsites, sizeof(Site), scomp);
   xmin=sites[0].x(); 
   xmax=sites[0].x();
   for(i=1; i<nsites; i+=1) {	
      if(sites[i].x() < xmin) xmin = sites[i].x();
      if(sites[i].x() > xmax) xmax = sites[i].x();
   };
   ymin = sites[0].y();
   ymax = sites[nsites-1].y();
}

