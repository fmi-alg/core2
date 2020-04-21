/* *****************************************************
 File: main_lattice.cpp

 	Variant of main.cpp, a
	Core Library Version of Fortune's implementation of his algorithm.

 ***************************************************** */


#include <ctime>
// #include "getopt.h"
extern "C" int scomp(const void *elem1, const void *elem2);
void readsites();
void sortsites();

// static data
static Site  *sites = NULL;
static int L = 10;
static int N = 32;

int main(int argc, char **argv) 
{	
   long msec;
   
   msec = clock();
   
   sorted = 0; triangulate = 0; plot = 0; debug = 0; postscript = 1;

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

   if (argc == 3) {
	N = atoi(argv[1]);
	L = atoi(argv[2]);
   } else {
	std::cout << "Usage: voronoi <N> <L>" << std::endl;
	exit(1);
   }
   if(sorted) {
      std::cin >> nsites >> xmin >> xmax >> ymin >> ymax;
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
   
   if (plot | postscript) 
      plotclose();
   if (!postscript){
      msec = clock();
      std::cout << "The number of microseconds is " <<  msec << "\n";
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
   nsites=0;
   sites =  new Site[N * N];

   // generate a M * M uniform grid with L-bit coordinates
   BigInt RANGE = 1 ; // BigFloat::exp2(L-1);
   RANGE = RANGE << (L - 2);
   BigInt BASE = RANGE << 1;
   BigInt max_step = (RANGE / N);
   BigInt step = abs(randomize(max_step));

   std::cout << "%% BASE: " << BASE << ", step: " << step << std::endl;
   BigInt tx = 0, ty = 0;
   for (int i = 0; i < N; i++) {
     MyFloat xx = BASE + tx;
     for (int j = 0; j < N; j++) {
       MyFloat yy = BASE + ty;
       sites[nsites] = Site(xx, yy, nsites);
	std::cout <<"xx, yy = " << xx << ", " << yy << std::endl;
       nsites += 1;
       ty += step;
     }
     tx += step;
   }
   /*
   std::cin >> x >> y;
   while (!std::cin.eof())
   {	// nsites should be handled inside of class Site
      MyFloat xx = x;
      MyFloat yy = y;
      sites[nsites] = Site(xx, yy, nsites);
      nsites += 1;
      if (nsites % 4000 == 0) 
         moreSites(&sites, nsites, 4096);
      
      std::cin >> x >> y;
   }
   */
}


/*  sort, and compute xmin, xmax, ymin, ymax */
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

