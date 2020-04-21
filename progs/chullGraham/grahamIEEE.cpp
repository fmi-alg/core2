/*****************************************************************
 File: graham1.ccp

 purpose:
 	This is just the Level 1 version of graham.cpp

 Author:
        Hui Jin (hj228@cs.nyu.edu), August 1999

	CORE Library: $Id: grahamIEEE.cpp,v 1.1 2006/03/07 04:51:23 exact Exp $

 ******************************************************************
        O'Rourke's Original Comments:
 ******************************************************************
This code is described in "Computational Geometry in C" (Second Edition),
Chapter 3.  It is not written to be comprehensible without the
explanation in that book.

Input: 2n integer coordinates of points in the plane. 
Output: the convex hull, cw, in PostScript; other output precedes the PS.

NB: The original array storing the points is overwritten.

Compile: gcc -o graham graham.c macros.h

Written by Joseph O'Rourke.
Last modified: October 1997
Questions to orourke@cs.smith.edu.
--------------------------------------------------------------------
This code is Copyright 1998 by Joseph O'Rourke.  It may be freely
redistributed in its entirety provided that this copyright notice is
not removed.
--------------------------------------------------------------------
*/

#include   "macros.h"

#define CORE_LEVEL 1
#include "CORE/CORE.h"

#define EXIT_FAILURE   1
#define X   0
#define Y   1
//typedef enum { FALSE, TRUE }   bool;

#define DIM 2               /* Dimension of points */
//typedef int tPointi[DIM];   /* Type integer point */
typedef double tPointi[DIM];   /* Type integer point */


/*----------Point(s) Structure-------------*/
typedef struct tPointStructure tsPoint;
typedef tsPoint *tPoint;
struct tPointStructure {
   int     vnum;
   tPointi v;
   bool    delete_bit;
};
/* Global variables */
#define PMAX    3000               /* Max # of points */
typedef tsPoint tPointArray[PMAX];
static tPointArray P;
int n = 0;                         /* Actual # of points */
int ndelete_bit = 0;                   /* Number delete_bitd */
bool toLeda = false;
const int HOMOGENIZING_FACTOR = 1;

/*----------Stack Structure-------------*/
typedef struct tStackCell tsStack; /* Used on in NEW() */
typedef tsStack *tStack;
struct tStackCell {
   tPoint   p;
   tStack   next;
};


/*----------Function Prototypes-------------*/
tStack	Pop( tStack s );
void    PrintStack( tStack t );
void    PrintToLeda( tStack t );
tStack  Push( tPoint p, tStack top );
tStack  Graham( void );
void    Squash( void );
void	Copy( int i, int j );
void    PrintPostscript( tStack t );
extern "C" int     Compare( const void *tp1, const void *tp2 );
void    FindLowest( void );
void	Swap( int i, int j );

//int     AreaSign( tPointi a, tPointi b, tPointi c );
double     AreaSign( tPointi a, tPointi b, tPointi c );
bool    Left( tPointi a, tPointi b, tPointi c );
int     ReadPoints( void );
void    PrintPoints( void );


/*----------Main Program-------------*/

int main(int argc, char *argv[])
{
   tStack   top;

   if (argc > 1 && argv[1][0] == '-' ) {
     if( argv[1][1] == 'l' ) {
       toLeda = true;
       //std::cerr << "Output for LEDA\n" ;
     }
   }

    
   n = ReadPoints();
// std::cout << "before qsort\n";  PrintPoints();
   FindLowest();
   //PrintPoints();
   qsort(
      &P[1],             /* pointer to 1st elem */
      n-1,               /* number of elems */
      sizeof( tsPoint ), /* size of each elem */
      Compare            /* -1,0,+1 compare function */
   );
   //std::cerr << "After sorting,  ndelete_bit = " << ndelete_bit << ":\n";
// std::cout << "after qsort\n";  PrintPoints();
   if (ndelete_bit > 0) {
      Squash();
      //std::cerr << "After squashing:\n";
   }

   top = Graham();
   //std::cerr << "Hull:\n";
   //PrintStack( top );
   if (toLeda)
	PrintToLeda( top );
   else
   	PrintPostscript( top );
   return 0;
}

/*---------------------------------------------------------------------
FindLowest finds the rightmost lowest point and swaps with 0-th.
The lowest point has the min y-coord, and amongst those, the
max x-coord: so it is rightmost among the lowest.
---------------------------------------------------------------------*/
void   FindLowest( void )
{
   int i;
   int m = 0;   /* Index of lowest so far. */

   for ( i = 1; i < n; i++ )
      if ( (P[i].v[Y] <  P[m].v[Y]) ||
          ((P[i].v[Y] == P[m].v[Y]) && (P[i].v[X] > P[m].v[X])) ) 
         m = i;
   //std::cerr << "Swapping " << m << " with 0\n";
   Swap(0,m); /* Swap P[0] and P[m] */
}

void	Swap( int i, int j )
{
   //int temp;
   double temp;
   int temp1;
   bool temp2;
   /* Uses swap macro. */

   SWAP( temp1, P[i].vnum,   P[j].vnum );
   SWAP( temp, P[i].v[X],   P[j].v[X] );
   SWAP( temp, P[i].v[Y],   P[j].v[Y] );
   SWAP( temp2, P[i].delete_bit, P[j].delete_bit );

}
/*---------------------------------------------------------------------
Compare: returns -1,0,+1 if p1 < p2, =, or > respectively;
here "<" means smaller angle.  Follows the conventions of qsort.
---------------------------------------------------------------------*/
int   Compare( const void *tpi, const void *tpj )
{
   //int a;             /* area */
   double a;             /* area */
   //int x, y;          /* projections of ri & rj in 1st quadrant */
   double x, y;          /* projections of ri & rj in 1st quadrant */
   tPoint pi, pj;
   pi = (tPoint)tpi;
   pj = (tPoint)tpj;

   a = AreaSign( P[0].v, pi->v, pj->v );
   if (a > 0)
      return -1;
   else if (a < 0)
      return 1;
   else { /* Collinear with P[0] */
      x =  fabs( pi->v[X] -  P[0].v[X] ) - fabs( pj->v[X] -  P[0].v[X] );
      y =  fabs( pi->v[Y] -  P[0].v[Y] ) - fabs( pj->v[Y] -  P[0].v[Y] );

      ndelete_bit++;
      if ( (x < 0) || (y < 0) ) {
         pi->delete_bit = true;
         return -1;
      }
      else if ( (x > 0) || (y > 0) ) {
         pj->delete_bit = true;
         return 1;
      }
      else { /* points are coincident */
         if (pi->vnum > pj->vnum)
             pj->delete_bit = true;
         else 
             pi->delete_bit = true;
         return 0;
      }
   }
}


/*---------------------------------------------------------------------
Pops off top elment of stack s, frees up the cell, and returns new top.
---------------------------------------------------------------------*/
tStack   Pop( tStack s )
{
   tStack top;

   top = s->next;
   FREE( s );
   return top;
}

/*---------------------------------------------------------------------
Get a new cell, fill it with p, and push it onto the stack.
Return pointer to new stack top.
---------------------------------------------------------------------*/
tStack   Push( tPoint p, tStack top )
{
   tStack   s;

   /* Get new cell and fill it with point. */
   //NEW( s, tsStack );
   s = new tsStack;
   s->p = p;
   s->next = top;
   return s;
}
/*---------------------------------------------------------------------
---------------------------------------------------------------------*/
void   PrintStack( tStack t )
{
   if (!t) std::cerr << "Empty stack\n";
   while (t) { 
      std::cerr << "vnum=" << t->p->vnum 
	   << "\tx=" << t->p->v[X]
	   << "\ty=" << t->p->v[Y]  << "\n"; 
      t = t->next;
   }
}

/*---------------------------------------------------------------------
Performs the Graham scan on an array of angularly sorted points P.
---------------------------------------------------------------------*/
tStack   Graham()
{
   tStack   top;
   int i;
   tPoint p1, p2;  /* Top two points on stack. */

   /* Initialize stack. */
   top = NULL;
   top = Push ( &P[0], top );
   top = Push ( &P[1], top );

   /* Bottom two elements will never be removed. */
   i = 2;

   while ( i < n ) {
      //std::cerr << "Stack at top of while loop, i=" << i
	   //<< ", vnum=" << P[i].vnum << ":\n";
      //PrintStack( top );
      if( !top->next) {
	//std::cerr << "Error\n";
	exit(EXIT_FAILURE);
      }
      p1 = top->next->p;
      p2 = top->p;
      if ( Left( p1->v , p2->v, P[i].v ) ) {
         top = Push ( &P[i], top );
         i++;
      } else    
         top = Pop( top );
      //std::cerr << "Stack at bot of while loop, i=" << i 
	   //<< ", vnum=" << P[i].vnum << ":\n";
      //PrintStack( top );
      //putchar('\n');
   }

   return top;

}

/*---------------------------------------------------------------------
Squash removes all elements from P marked delete_bit.
---------------------------------------------------------------------*/
void   Squash( void )
{
   int i, j;
   i = 0; j = 0;
   /*printf("Squash: n=%d\n",n);*/
   while ( i < n ) {
      /*printf("i=%d,j=%d\n",i,j);*/
      if ( !P[i].delete_bit ) { /* if not marked for deletion */
         Copy( i, j ); /* Copy P[i] to P[j]. */
         j++;
      }
      /* else do nothing: delete_bit by skipping. */
      i++;
   }
   n = j;
   //std::cerr << "After Squash: n=" << n << "\n";
   //PrintPoints();
}

void	Copy( int i, int j )
{
   P[j].v[X] = P[i].v[X];
   P[j].v[Y] = P[i].v[Y];
   P[j].vnum = P[i].vnum;
   P[j].delete_bit = P[i].delete_bit;
}


/*---------------------------------------------------------------------
Returns twice the signed area of the triangle determined by a,b,c.
The area is positive if a,b,c are oriented ccw, negative if cw,
and zero if the points are collinear.
---------------------------------------------------------------------*/
//int     Area2( tPointi a, tPointi b, tPointi c )
double     Area2( tPointi a, tPointi b, tPointi c )
{
   return
          (b[X] - a[X]) * (c[Y] - a[Y]) -
          (c[X] - a[X]) * (b[Y] - a[Y]);
}

/*---------------------------------------------------------------------
Returns true iff c is strictly to the left of the directed
line through a to b.
---------------------------------------------------------------------*/
bool   Left( tPointi a, tPointi b, tPointi c )
{ 
   return  Area2( a, b, c ) > 0;
}

/*---------------------------------------------------------------------
Reads in the coordinates of the points from stdin,
puts them into P, and returns n, the number of vertices.
Initializes other fields of point structure.
	NOTE:	Limited commenting in input files allowed -- before
		the start of data, lines beginning with # is allowed
		and are discarded.
---------------------------------------------------------------------*/
int    ReadPoints( void )
{
   int n = 0;
   char buf[256];

   // Removal initial comment lines (line must begin with #)
   while (std::cin.peek() == '#') 
	   std::cin.getline(buf, 256);

   while ( (n < PMAX) && (std::cin) ) {
      std::cin >> P[n].v[0] >> P[n].v[1];
 
      P[n].vnum = n;
      P[n].delete_bit = false;
      /*printf("vnum=%3d, x=%4d, y=%4d, delete_bit=%d\n", 
	 P[n].vnum, P[n].v[X], P[n].v[Y], P[n].delete_bit);*/
      ++n;
   }
   if (n < PMAX)
	;
      //std::cerr << "n = " << n << " vertices read\n";
   else {
      std::cerr << "Error in ReadPoints:  too many points; max is "
	   << PMAX << "\n";
      exit(EXIT_FAILURE);
   }

   return n;
}

void   PrintPoints( void )
{
   int   i;

   std::cerr << "Points:\n";
   for( i = 0; i < n; i++ )
      std::cerr << "vnum=" << P[i].vnum << " x=" << P[i].v[X]
	   << "y=" << P[i].v[Y] << " delete_bit=" << P[i].delete_bit << "\n"; 
 
}

void  PrintToLeda ( tStack t)
{
   while (t) { 
      std::cout << "(" << t->p->v[X] << ", " << t->p->v[Y]  
	   << ", " << 0 << ", " << HOMOGENIZING_FACTOR << ")\n"; 
      t = t->next;
   }
}

void   PrintPostscript( tStack t)
{
   int   i;
   //int xmin, ymin, xmax, ymax;
   double xmin, ymin, xmax, ymax;

   xmin = xmax = P[0].v[X];
   ymin = ymax = P[0].v[Y];
   for (i = 1; i < n; i++) {
      if      ( P[i].v[X] > xmax ) xmax = P[i].v[X];
      else if ( P[i].v[X] < xmin ) xmin = P[i].v[X];
      if      ( P[i].v[Y] > ymax ) ymax = P[i].v[Y];
      else if ( P[i].v[Y] < ymin ) ymin = P[i].v[Y];
   }

   /* PostScript header */
   std::cout << "%%!PS\n";
   std::cout << "%%%%Creator: graham.c \n";
   std::cout << "%%%%BoundingBox: " << xmin << " " << ymin << " "
	<< xmax << " " << ymax << std::endl;
   std::cout << "%%%%EndComments\n";
   std::cout << ".00 .00 setlinewidth\n";
   double scale = 500.0 / ((ymax-ymin) >= (xmax - xmin) ? ymax - ymin: xmax - xmin);
   double radius = 3/scale;
   std::cout << -xmin*scale+72 << " " << -ymin*scale+120 << " translate\n" ;
   /* The +72 shifts the figure one inch from the lower left corner */
   std::cout << scale << " " << scale << " scale" << std::endl;

   /* Draw the points as little circles. */
   std::cout << "newpath\n";
   std::cout << "\n%%Points:\n";
   for (i = 0; i < n; i++)
      std::cout << P[i].v[X] << "\t" << P[i].v[Y] 
	   << "\t" << radius << "  0  360\tarc\tstroke\n"; 
   std::cout << "closepath\n";

   /* Draw the polygon. */
   std::cout << "\n%%Hull:\n";
   std::cout << "newpath\n";
   std::cout << t->p->v[X] << "\t" << t->p->v[Y] << "\tmoveto\n"; 
   while (t) {
      std::cout << t->p->v[X] << "\t" << t->p->v[Y] << "\tlineto\n"; 
      t = t->next;
   }
   std::cout << "closepath stroke\n";
   std::cout << "showpage\n%%%%EOF\n";
}
//int     AreaSign( tPointi a, tPointi b, tPointi c )
double     AreaSign( tPointi a, tPointi b, tPointi c )
{
    double area2;

    area2 = ( b[0] - a[0] ) * (double)( c[1] - a[1] ) -
            ( c[0] - a[0] ) * (double)( b[1] - a[1] );

    /* The area should be an integer. */

    // if      ( area2 >  0.5 ) return  1;
    // else if ( area2 < -0.5 ) return -1;
    // else                     return  0;
    
    /* The comment (that area is integer)
     * is wrong for the Core Version of this code, and causes
     * error in Level 3!
     * Anyway, we prefer to just return the area, not its sign.
     * (Chee, 9/28/2003) */
    
    return(area2);
}
