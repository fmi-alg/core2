/*****************************************************************
 File: dt2.cc
 purpose:
        This is a CORE library version of O'Rourke's
        implementation of Graham's convex hull algorithm.

        Changes to O'Rourke's program:
        -- all printf and scanf are converted to std::cout and std::cin
        -- input numbers are assumed to be double instead of int
        -- input points are 2D points, dt2 can automatically evaluate
	   value of z by x^2 + y^2

 Usage:
        % dt2 [-l] < input_file >! output_file

        If the optional switch (-l) is not given, then dt2
        output a postscript file to draw a projection of the
        convex hull.  The switch modifies this behavior:
        -l      This forces dt2 to output the convex hull in a
                form suitable for display using LEDA's window sytem
 Author:
        Hui Jin (hj228@cs.nyu.edu), August 1999

 Modified:
        By Chen Li to incorprate LEDA input format (09/09/99).

 ******************************************************************
        O'Rourke's Original Comments:
 ******************************************************************
This code is described in "Computational Geometry in C" (Second Edition),
Chapter 5.  It is not written to be comprehensible without the 
explanation in that book.

Input: 2n integer coordinates for the points.
Output: The Delaunay triangulation, in postscript with embedded comments.

Compile: gcc -o dt2 dt2.c

Written by Joseph O'Rourke.
Last modified: July 1997
Questions to orourke@cs.smith.edu.
--------------------------------------------------------------------
This code is Copyright 1998 by Joseph O'Rourke.  It may be freely 
redistributed in its entirety provided that this copyright notice is 
not removed.
--------------------------------------------------------------------
*/
#define CORE_LEVEL   3
#include "CORE.h"

#include "macros.h"

/*Define Boolean type */
//typedef	enum { FALSE, TRUE }	bool;

/* Define vertex indices. */
#define X   0
#define Y   1
#define Z   2

/* Define structures for vertices, edges and faces */
typedef struct tVertexStructure tsVertex;
typedef tsVertex *tVertex;

typedef struct tEdgeStructure tsEdge;
typedef tsEdge *tEdge;

typedef struct tFaceStructure tsFace;
typedef tsFace *tFace;

struct tVertexStructure {
   double      v[3];
   int	    vnum;
   tEdge    duplicate;	        /* pointer to incident cone edge (or NULL) */
   bool     onhull;		/* T iff point on hull. */
   bool	    mark;		/* T iff point already processed. */
   tVertex  next, prev;
};

struct tEdgeStructure {
   tFace    adjface[2];
   tVertex  endpts[2];
   tFace    newface;            /* pointer to incident cone face. */
   bool     delete_bit;		/* T iff edge should be delete. */
   tEdge    next, prev;
};

struct tFaceStructure {
   tEdge    edge[3];
   tVertex  vertex[3];
   bool	    visible;	        /* T iff face visible from new point. */
   bool     lower;              /* T iff on the lower hull */
   tFace    next, prev;
};

/* Define flags */
#define ONHULL   	true
#define REMOVED  	true
#define VISIBLE  	true
#define PROCESSED	true
#define SAFE            1000000         /* Range of safe coord values. */

/* Global variable definitions */
tVertex vertices = NULL;
tEdge edges    	 = NULL;
tFace faces    	 = NULL;
bool debug = false;
bool check = false;
bool toLeda = false;
bool fromLeda = false;
const int HOMOGENIZING_FACTOR = 1;

/* Function declarations */
tVertex MakeNullVertex( void );
void    ReadVertices( void );
void    Print( void );
void    PrintToLeda();
void    SubVec( double a[3], double b[3], double c[3]);
void    DoubleTriangle( void );
void    ConstructHull( void );
bool	AddOne( tVertex p );
int     VolumeSign(tFace f, tVertex p);
double 	Volumei( tFace f, tVertex p );
tFace	MakeConeFace( tEdge e, tVertex p );
void    MakeCcw( tFace f, tEdge e, tVertex p );
tEdge   MakeNullEdge( void );
tFace   MakeNullFace( void );
tFace   MakeFace( tVertex v0, tVertex v1, tVertex v2, tFace f );
void    CleanUp( void );
void    CleanEdges( void );
void    CleanFaces( void );
void    CleanVertices( void );
bool	Collinear( tVertex a, tVertex b, tVertex c );
double	Normz( tFace f );
void    CheckEuler(int V, int E, int F );
void	PrintPoint( tVertex p );
void    Checks( void );
void	Consistency( void );
void	Convexity( void );
void	PrintOut( tVertex v );
void	PrintVertices( void );
void	PrintEdges( void );
void	PrintFaces( void );
void    LowerFaces( void );


/*-------------------------------------------------------------------*/
int main( int argc, char *argv[] )
{

  // Chen Li
  for (int i = 1; i < argc; i++) {
    char option = argv[i][1];
    switch (option) {
    case 'd':
      debug = true;
      check = true;
      std::cerr << "Debug and check mode\n";     
      break;

    case 'c':
      check = true;
      std::cerr << "Check mode\n";
      break;

    case 'l':
      toLeda = true;
      std::cerr << "Output to Leda\n";
      break;

    case 'L':
      fromLeda = true;
      std::cerr << "Input from Leda" << std::endl;
      break;

    default:
      std::cerr << "Usage: " << *argv  << "-d[ebug] c[heck]\n"
	   << "x y z coords of vertices from stdin\n";
      exit(1);
      break;
    } 
  }
  
  /*  
  if ( argc > 1 && argv[1][0] == '-' ) {
    if( argv[1][1] ==  'd' ) {
      debug = true;
      check = true;
      std::cerr << "Debug and check mode\n";
    }
    if( argv[1][1] == 'c' ) {
      check = true;
      std::cerr << "Check mode\n";
    }
    if( argv[1][1] == 'l' ) {
      toLeda = true;
      std::cerr << "Output to Leda\n";
    }
  }
  else if ( argc > 1 && argv[1][0] != '-' ) {
    std::cout << "Usage: " << *argv  << "-d[ebug] c[heck]\n" ;
    std::cout << "x y z coords of vertices from stdin\n";
    exit(1);
  }
  */

   ReadVertices();
   DoubleTriangle();
   ConstructHull();
   LowerFaces();
   if (toLeda)
	PrintToLeda();
   else
   	Print();
   return 0;
}

void    LowerFaces( void )
{
   tFace f = faces;
   /*int   z;*/
   int   Flower = 0;   /* Total number of lower faces. */

   do {
     /*z = Normz( f );
     if ( z < 0 ) {*/
     if ( Normz( f ) < 0 ) {
        Flower++;
        f->lower = true;
        /*printf("z=%10d; lower face indices: %d, %d, %d\n", z, */
        /*printf("lower face indices: %d, %d, %d\n",
           f->vertex[0]->vnum,
           f->vertex[1]->vnum,
           f->vertex[2]->vnum );*/
     }
     else f->lower = false;
     f = f->next;
   } while ( f != faces );
   /*printf("A total of %d lower faces identified.\n", Flower);*/
}

/*---------------------------------------------------------------------
MakeNullVertex: Makes a vertex, nulls out fields.
---------------------------------------------------------------------*/
tVertex	MakeNullVertex( void )
{
   tVertex  v;
   
   //NEW( v, tsVertex );
   v = new tsVertex;
   v->duplicate = NULL;
   v->onhull = !ONHULL;
   v->mark = !PROCESSED;
   ADD( vertices, v );

   return v;
}

/*---------------------------------------------------------------------
ReadVertices: Reads in the vertices, and links them into a circular
list with MakeNullVertex.  There is no need for the # of vertices to be
the first line: the function looks for EOF instead.  Sets the global
variable vertices via the ADD macro.
---------------------------------------------------------------------*/
void	ReadVertices( void )
{
   tVertex  v;
   //int      x, y, z;
   double      x, y, z;
   int	    vnum = 0;

   double r;

   while ( std::cin )  {
     if (fromLeda) {
       std::cin.get(); std::cin >> x;
       std::cin.get(); std::cin >> y;
       std::cin.get(); std::cin >> z;
       std::cin.get(); std::cin >> r;
       std::cin.get();
       // std::cin >> ch >> x >> ch >> y >> ch >> z >> ch >> r >> ch;
       x /= r;
       y /= r;
       // z ignored since this program only process 2D case;
     } else {
       std::cin >> x >> y;
     }
     if (std::cin) {
      v = MakeNullVertex();
      v->v[X] = x;
      v->v[Y] = y;
      z = x*x + y*y;
      v->v[Z] = z;
      v->vnum = vnum++;
     }
   }
   std::cerr << "# of vertices : " << vnum << std::endl;
}

/*---------------------------------------------------------------------
Print: Prints out the vertices and the faces.  Uses the vnum indices 
corresponding to the order in which the vertices were input.
Output is in PostScript format.
---------------------------------------------------------------------*/
void	Print( void )
{
   /* Pointers to vertices, edges, faces. */
   tVertex  v;
   tEdge    e;
   tFace    f;
   double xmin, ymin, xmax, ymax;
   //double a[3], b[3];  /* used to compute normal vector */
   /* Counters for Euler's formula. */
   int 	V = 0, E = 0 , F = 0;
   /* Note: lowercase==pointer, uppercase==counter. */

   /*-- find X min & max --*/
   v = vertices;
   xmin = xmax = v->v[X];
   do {
      if( v->v[X] > xmax ) xmax = v->v[X];
      else
	 if( v->v[X] < xmin ) xmin = v->v[X];
      v = v->next;
   } while ( v != vertices );
	
   /*-- find Y min & max --*/
   v = vertices;
   ymin = ymax = v->v[Y];
   do {
      if( v->v[Y] > ymax ) ymax = v->v[Y];
      else
	 if( v->v[Y] < ymin ) ymin = v->v[Y];
      v = v->next;
   } while ( v != vertices );
	
   double scale = 500 / ((xmax - xmin) >= (ymax - ymin) ? 
			 xmax - xmin : ymax - ymin);

   /* PostScript header */
   std::cout << "%%!PS\n";
   std::cout << "%%%%BoundingBox:  " << xmin << ymin  << xmax << ymax << std::endl;
   std::cout << ".00 .00 setlinewidth\n";
   std::cout << -xmin*scale+100 << " " << -ymin*scale+100  << " translate\n";
   /* The +72 shifts the figure one inch from the lower left corner */
   std::cout << scale << " " << scale << " scale " << std::endl;

   /* Vertices. */
   v = vertices;
   do {                                 
      if( v->mark ) V++;           
      v = v->next;
   } while ( v != vertices );
   std::cout << "\n%%%% Vertices:\tV = " << V << std::endl;
   std::cout << "%%%% index:\tx\ty\tz\n";
   do {                                 
      std::cout << "%%%% " << v->vnum << ":\t" << v->v[X] 
	   << "\t" << v->v[Y] << "\t" << v->v[Z] << std::endl;
      std::cout << "newpath\n";
      std::cout << v->v[X] << "\t" << v->v[Y] << " 2 0 360 arc\n";
      std::cout << "closepath stroke\n\n";
      v = v->next;
   } while ( v != vertices );
	
   /* Faces. */
   /* visible faces are printed as PS output */
   f = faces;
   do {
      ++F;                              
      f  = f ->next;
   } while ( f  != faces );
   std::cout << "\n%%%% Faces:\tF = " << F << std::endl;
   std::cout << "%%%% Visible faces only: \n";
   do {           
      /* Print face only if it is lower */
      if ( f-> lower )
      {
	 std::cout << "%%%% vnums:  " << 
		f->vertex[0]->vnum << " " <<  
		f->vertex[1]->vnum << " " <<   
		f->vertex[2]->vnum << " " << std::endl;
	 std::cout << "newpath\n";
	 std::cout << f->vertex[0]->v[X] << "\t" <<  f->vertex[0]->v[Y] << "\tmoveto\n"; 
	 std::cout << f->vertex[1]->v[X] << "\t" <<  f->vertex[1]->v[Y] << "\tlineto\n";
	 std::cout << f->vertex[2]->v[X] << "\t" <<  f->vertex[2]->v[Y] << "\tlineto\n", 
	 std::cout << "closepath stroke\n\n";
      }
      f = f->next;
   } while ( f != faces );

   /* prints a list of all faces */
   std::cout << "%%%% List of all faces: \n";
   std::cout << "%%%%\tv0\tv1\tv2\t(vertex indices)\n";
   do {
      std::cout << "%%%%\t" << f->vertex[0]->vnum << "\t"
	   << f->vertex[1]->vnum << "\t"
	   << f->vertex[2]->vnum << std::endl;
      f = f->next;
   } while ( f != faces );
	
   /* Edges. */	
   e = edges;
   do {
      E++;
      e = e->next;
   } while ( e != edges );
   std::cout << "\n%%%% Edges:\tE = " << E << std::endl;
   /* Edges not printed out (but easily added). */

   std::cout << "\nshowpage\n\n";
   std::cout << "%%EOF\n";

   check = true;
   CheckEuler( V, E, F );

}

/*---------------------------------------------------------------------
SubVec:  Computes a - b and puts it into c.
---------------------------------------------------------------------*/
void    SubVec( double a[3], double b[3], double c[3])
{
   int  i;

   for( i=0; i < 2; i++ )
      c[i] = a[i] - b[i];

}

/*---------------------------------------------------------------------
 DoubleTriangle builds the initial double triangle.  It first finds 3 
 noncollinear points and makes two faces out of them, in opposite order.
 It then finds a fourth point that is not coplanar with that face.  The  
 vertices are stored in the face structure in counterclockwise order so 
 that the volume between the face and the point is negative. Lastly, the
 3 newfaces to the fourth point are constructed and the data structures
 are cleaned up. 
---------------------------------------------------------------------*/
void    DoubleTriangle( void )
{
   tVertex  v0, v1, v2, v3;
   tFace    f0, f1 = NULL;
   int      vol;
	
	
   /* Find 3 non-Collinear points. */
   v0 = vertices;
   while ( Collinear( v0, v0->next, v0->next->next ) )
      if ( ( v0 = v0->next ) == vertices ) {
         std::cout << "DoubleTriangle:  All points are Collinear!\n";
	 exit(0);
      }
   v1 = v0->next;
   v2 = v1->next;
	
   /* Mark the vertices as processed. */
   v0->mark = PROCESSED;
   v1->mark = PROCESSED;
   v2->mark = PROCESSED;
   
   /* Create the two "twin" faces. */
   f0 = MakeFace( v0, v1, v2, f1 );
   f1 = MakeFace( v2, v1, v0, f0 );

   /* Link adjacent face fields. */
   f0->edge[0]->adjface[1] = f1;
   f0->edge[1]->adjface[1] = f1;
   f0->edge[2]->adjface[1] = f1;
   f1->edge[0]->adjface[1] = f0;
   f1->edge[1]->adjface[1] = f0;
   f1->edge[2]->adjface[1] = f0;
	
   /* Find a fourth, non-coplanar point to form tetrahedron. */
   v3 = v2->next;
   vol = VolumeSign( f0, v3 );
   while ( !vol )   {
      if ( ( v3 = v3->next ) == v0 ) {
         std::cout << "DoubleTriangle:  All points are coplanar!\n";
	 exit(0);
      }
      vol = VolumeSign( f0, v3 );
   }
	
   /* Insure that v3 will be the first added. */
   vertices = v3;
   if ( debug ) {
      std::cerr << "DoubleTriangle: finished. Head repositioned at v3.\n";
      PrintOut( vertices );
   }

	
}

  
/*---------------------------------------------------------------------
ConstructHull adds the vertices to the hull one at a time.  The hull
vertices are those in the list marked as onhull.
---------------------------------------------------------------------*/
void	ConstructHull( void )
{
   tVertex  v, vnext;
   //bool	    changed;	/* T if addition changes hull; not used. */

   v = vertices;
   do {
      vnext = v->next;
      if ( !v->mark ) {
         v->mark = PROCESSED;
	 //changed = AddOne( v );
	 CleanUp();

	 if ( check ) {
	    std::cerr << "ConstructHull: After Add of " << v->vnum << " & Cleanup:\n"; 
	    Checks();
	 }
	 if ( debug )
            PrintOut( v );
      }
      v = vnext;
   } while ( v != vertices );
}

/*---------------------------------------------------------------------
AddOne is passed a vertex.  It first determines all faces visible from 
that point.  If none are visible then the point is marked as not 
onhull.  Next is a loop over edges.  If both faces adjacent to an edge
are visible, then the edge is marked for deletion.  If just one of the
adjacent faces is visible then a new face is constructed.
---------------------------------------------------------------------*/
bool 	AddOne( tVertex p )
{
   tFace  f; 
   tEdge  e;
   int 	  vol;
   bool	  vis = false;

   if ( debug ) {
      std::cerr << "AddOne: starting to add v" << p->vnum << ".\n";
      PrintOut( vertices );
   }

   /* Mark faces visible from p. */
   f = faces;
   do {
      vol = VolumeSign( f, p );
      if (debug) 
	std::cerr << "faddr: " << f << "   paddr: " << p << "   Vol = " << vol << std::endl;
      if ( vol < 0 ) {
	 f->visible = VISIBLE;  
	 vis = true;                      
      }
      f = f->next;
   } while ( f != faces );

   /* If no faces are visible from p, then p is inside the hull. */
   if ( !vis ) {
      p->onhull = !ONHULL;  
      return false; 
   }

   /* Mark edges in interior of visible region for deletion.
      Erect a newface based on each border edge. */
   e = edges;
   do {
      tEdge temp;
      temp = e->next;
      if ( e->adjface[0]->visible && e->adjface[1]->visible )
	 /* e interior: mark for deletion. */
	 e->delete_bit = REMOVED;
      else if ( e->adjface[0]->visible || e->adjface[1]->visible ) 
	 /* e border: make a new face. */
	 e->newface = MakeConeFace( e, p );
      e = temp;
   } while ( e != edges );
   return true;
}

/*---------------------------------------------------------------------
VolumeSign returns the sign of the volume of the tetrahedron determined by f
and p.  VolumeSign is +1 iff p is on the negative side of f,
where the positive side is determined by the rh-rule.  So the volume 
is positive if the ccw normal to f points outside the tetrahedron.
The final fewer-multiplications form is due to Robert Fraczkiewicz.
---------------------------------------------------------------------*/
int  VolumeSign( tFace f, tVertex p )
{
   double  vol;
   int     voli = 0;
   double  ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;
   double  bxdx, bydy, bzdz, cxdx, cydy, czdz;

   ax = f->vertex[0]->v[X];
   ay = f->vertex[0]->v[Y];
   az = f->vertex[0]->v[Z];
   bx = f->vertex[1]->v[X];
   by = f->vertex[1]->v[Y];
   bz = f->vertex[1]->v[Z];
   cx = f->vertex[2]->v[X];
   cy = f->vertex[2]->v[Y];
   cz = f->vertex[2]->v[Z];
   dx = p->v[X];
   dy = p->v[Y];
   dz = p->v[Z];
   
   bxdx=bx-dx;
   bydy=by-dy;
   bzdz=bz-dz;
   cxdx=cx-dx;
   cydy=cy-dy;
   czdz=cz-dz;
   vol =   (az-dz) * (bxdx*cydy - bydy*cxdx)
         + (ay-dy) * (bzdz*cxdx - bxdx*czdz)
	 + (ax-dx) * (bydy*czdz - bzdz*cydy);

   if ( debug )
      std::cerr << "Face=" << f << "; Vertex=" << p->vnum << ": vol(int) = " 
	   << voli << ", vol(double) = " << vol << std::endl;

   /* The volume should be an integer. */
   if      ( vol > 0.5 )   return  1;
   else if ( vol < -0.5 )  return -1;
   else                    return  0;
}
/*---------------------------------------------------------------------*/
double  Volumei( tFace f, tVertex p )
{
   //int 	   vol;
   //int 	   ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;
   //int	   bxdx, bydy, bzdz, cxdx, cydy, czdz;
   double 	   vol;
   double 	   ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;
   double	   bxdx, bydy, bzdz, cxdx, cydy, czdz;
   double  vold;

   ax = f->vertex[0]->v[X];
   ay = f->vertex[0]->v[Y];
   az = f->vertex[0]->v[Z];
   bx = f->vertex[1]->v[X];
   by = f->vertex[1]->v[Y];
   bz = f->vertex[1]->v[Z];
   cx = f->vertex[2]->v[X];
   cy = f->vertex[2]->v[Y];
   cz = f->vertex[2]->v[Z];
   dx = p->v[X];
   dy = p->v[Y];
   dz = p->v[Z];
   
   bxdx=bx-dx;
   bydy=by-dy;
   bzdz=bz-dz;
   cxdx=cx-dx;
   cydy=cy-dy;
   czdz=cz-dz;
   vol =   (az-dz)*(bxdx*cydy-bydy*cxdx)
         + (ay-dy)*(bzdz*cxdx-bxdx*czdz)
	 + (ax-dx)*(bydy*czdz-bzdz*cydy);

   return vol;
}		

/*---------------------------------------------------------------------
Volumed is the same as VolumeSign but computed with doubles.  For 
protection against overflow.
---------------------------------------------------------------------*/
double 	Volumed( tFace f, tVertex p )
{
   double  vol;
   double  ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;
   double  bxdx, bydy, bzdz, cxdx, cydy, czdz;

   ax = f->vertex[0]->v[X];
   ay = f->vertex[0]->v[Y];
   az = f->vertex[0]->v[Z];
   bx = f->vertex[1]->v[X];
   by = f->vertex[1]->v[Y];
   bz = f->vertex[1]->v[Z];
   cx = f->vertex[2]->v[X];
   cy = f->vertex[2]->v[Y];
   cz = f->vertex[2]->v[Z];
   dx = p->v[X];
   dy = p->v[Y];
   dz = p->v[Z];
   
   bxdx=bx-dx;
   bydy=by-dy;
   bzdz=bz-dz;
   cxdx=cx-dx;
   cydy=cy-dy;
   czdz=cz-dz;
   vol = (az-dz)*(bxdx*cydy-bydy*cxdx)
         + (ay-dy)*(bzdz*cxdx-bxdx*czdz)
	 + (ax-dx)*(bydy*czdz-bzdz*cydy);

   return vol;
}

/*-------------------------------------------------------------------*/
void	PrintPoint( tVertex p )
{
   int	i;

   for ( i = 0; i < 3; i++ )
      std::cout << "\t" <<  p->v[i];
   putchar('\n');
}

/*---------------------------------------------------------------------
MakeConeFace makes a new face and two new edges between the 
edge and the point that are passed to it. It returns a pointer to
the new face.
---------------------------------------------------------------------*/
tFace	MakeConeFace( tEdge e, tVertex p )
{
   tEdge  new_edge[2];
   tFace  new_face;
   int 	  i, j;

   /* Make two new edges (if don't already exist). */
   for ( i=0; i < 2; ++i ) 
      /* If the edge exists, copy it into new_edge. */
      if ( !( new_edge[i] = e->endpts[i]->duplicate) ) {
	 /* Otherwise (duplicate is NULL), MakeNullEdge. */
	 new_edge[i] = MakeNullEdge();
	 new_edge[i]->endpts[0] = e->endpts[i];
	 new_edge[i]->endpts[1] = p;
	 e->endpts[i]->duplicate = new_edge[i];
      }

   /* Make the new face. */
   new_face = MakeNullFace();   
   new_face->edge[0] = e;
   new_face->edge[1] = new_edge[0];
   new_face->edge[2] = new_edge[1];
   MakeCcw( new_face, e, p ); 
        
   /* Set the adjacent face pointers. */
   for ( i=0; i < 2; ++i )
      for ( j=0; j < 2; ++j )  
	 /* Only one NULL link should be set to new_face. */
	 if ( !new_edge[i]->adjface[j] ) {
	    new_edge[i]->adjface[j] = new_face;
	    break;
	 }
        
   return new_face;
}

/*---------------------------------------------------------------------
MakeCcw puts the vertices in the face structure in counterclock wise 
order.  We want to store the vertices in the same 
order as in the visible face.  The third vertex is always p.
---------------------------------------------------------------------*/
void	MakeCcw( tFace f, tEdge e, tVertex p )
{
   tFace  fv;   /* The visible face adjacent to e */
   int    i;    /* Index of e->endpoint[0] in fv. */
   tEdge  s;	/* Temporary, for swapping */
      
   if  ( e->adjface[0]->visible )      
        fv = e->adjface[0];
   else fv = e->adjface[1];
       
   /* Set vertex[0] & [1] of f to have the same orientation
      as do the corresponding vertices of fv. */ 
   for ( i=0; fv->vertex[i] != e->endpts[0]; ++i )
      ;
   /* Orient f the same as fv. */
   if ( fv->vertex[ (i+1) % 3 ] != e->endpts[1] ) {
      f->vertex[0] = e->endpts[1];  
      f->vertex[1] = e->endpts[0];    
   }
   else {                               
      f->vertex[0] = e->endpts[0];   
      f->vertex[1] = e->endpts[1];      
      SWAP( s, f->edge[1], f->edge[2] );
   }
   /* This swap is tricky. e is edge[0]. edge[1] is based on endpt[0],
      edge[2] on endpt[1].  So if e is oriented "forwards," we
      need to move edge[1] to follow [0], because it precedes. */
   
   f->vertex[2] = p;
}
 
/*---------------------------------------------------------------------
MakeNullEdge creates a new cell and initializes all pointers to NULL
and sets all flags to off.  It returns a pointer to the empty cell.
---------------------------------------------------------------------*/
tEdge 	MakeNullEdge( void )
{
   tEdge  e;

   //NEW( e, tsEdge );
   e = new tsEdge;
   e->adjface[0] = e->adjface[1] = e->newface = NULL;
   e->endpts[0] = e->endpts[1] = NULL;
   e->delete_bit = !REMOVED;
   ADD( edges, e );
   return e;
}

/*--------------------------------------------------------------------
MakeNullFace creates a new face structure and initializes all of its
flags to NULL and sets all the flags to off.  It returns a pointer
to the empty cell.
---------------------------------------------------------------------*/
tFace 	MakeNullFace( void )
{
   tFace  f;
   int    i;

   //NEW( f, tsFace);
   f = new tsFace;
   for ( i=0; i < 3; ++i ) {
      f->edge[i] = NULL;
      f->vertex[i] = NULL;
   }
   f->visible = !VISIBLE;
   ADD( faces, f );
   return f;
}

/*---------------------------------------------------------------------
MakeFace creates a new face structure from three vertices (in ccw
order).  It returns a pointer to the face.
---------------------------------------------------------------------*/
tFace   MakeFace( tVertex v0, tVertex v1, tVertex v2, tFace fold )
{
   tFace  f;
   tEdge  e0, e1, e2;

   /* Create edges of the initial triangle. */
   if( !fold ) {
     e0 = MakeNullEdge();
     e1 = MakeNullEdge();
     e2 = MakeNullEdge();
   }
   else { /* Copy from fold, in reverse order. */
     e0 = fold->edge[2];
     e1 = fold->edge[1];
     e2 = fold->edge[0];
   }
   e0->endpts[0] = v0;              e0->endpts[1] = v1;
   e1->endpts[0] = v1;              e1->endpts[1] = v2;
   e2->endpts[0] = v2;              e2->endpts[1] = v0;
	
   /* Create face for triangle. */
   f = MakeNullFace();
   f->edge[0]   = e0;  f->edge[1]   = e1; f->edge[2]   = e2;
   f->vertex[0] = v0;  f->vertex[1] = v1; f->vertex[2] = v2;
	
   /* Link edges to face. */
   e0->adjface[0] = e1->adjface[0] = e2->adjface[0] = f;
	
   return f;
}

/*---------------------------------------------------------------------
CleanUp goes through each data structure list and clears all
flags and NULLs out some pointers.  The order of processing
(edges, faces, vertices) is important.
---------------------------------------------------------------------*/
void	CleanUp( void )
{
   CleanEdges();
   CleanFaces();
   CleanVertices();
}

/*---------------------------------------------------------------------
CleanEdges runs through the edge list and cleans up the structure.
If there is a newface then it will put that face in place of the 
visible face and NULL out newface. It also deletes so marked edges.
---------------------------------------------------------------------*/
void	CleanEdges( void )
{
   tEdge  e;	/* Primary index into edge list. */
   tEdge  t;	/* Temporary edge pointer. */
		
   /* Integrate the newface's into the data structure. */
   /* Check every edge. */
   e = edges;
   do {
      if ( e->newface ) { 
	 if ( e->adjface[0]->visible )
	    e->adjface[0] = e->newface; 
	 else	e->adjface[1] = e->newface;
	 e->newface = NULL;
      }
      e = e->next;
   } while ( e != edges );

   /* Delete any edges marked for deletion. */
   while ( edges && edges->delete_bit ) { 
      e = edges;
      DELETE( edges, e );
   }
   e = edges->next;
   do {
      if ( e->delete_bit ) {
	 t = e;
	 e = e->next;
	 DELETE( edges, t );
      }
      else e = e->next;
   } while ( e != edges );
}

/*---------------------------------------------------------------------
CleanFaces runs through the face list and deletes any face marked visible.
---------------------------------------------------------------------*/
void	CleanFaces( void )
{
   tFace  f;	/* Primary pointer into face list. */
   tFace  t;	/* Temporary pointer, for deleting. */
	

   while ( faces && faces->visible ) { 
      f = faces;
      DELETE( faces, f );
   }
   f = faces->next;
   do {
      if ( f->visible ) {
	 t = f;
	 f = f->next;
	 DELETE( faces, t );
      }
      else f = f->next;
   } while ( f != faces );
}

/*---------------------------------------------------------------------
CleanVertices runs through the vertex list and deletes the 
vertices that are marked as processed but are not incident to any 
undeleted edges. 
---------------------------------------------------------------------*/
void	CleanVertices( void )
{
   tEdge    e;
   tVertex  v, t;
	
   /* Mark all vertices incident to some undeleted edge as on the hull. */
   e = edges;
   do {
      e->endpts[0]->onhull = e->endpts[1]->onhull = ONHULL;
      e = e->next;
   } while (e != edges);
	
   /* Delete all vertices that have been processed but
      are not on the hull. */
   while ( vertices && vertices->mark && !vertices->onhull ) { 
      v = vertices;
      DELETE( vertices, v );
   }
   v = vertices->next;
   do {
      if ( v->mark && !v->onhull ) {    
	 t = v; 
	 v = v->next;
	 DELETE( vertices, t )
      }
      else v = v->next;
   } while ( v != vertices );
	
   /* Reset flags. */
   v = vertices;
   do {
      v->duplicate = NULL; 
      v->onhull = !ONHULL; 
      v = v->next;
   } while ( v != vertices );
}

/*---------------------------------------------------------------------
Collinear checks to see if the three points given are collinear,
by checking to see if each element of the cross product is zero.
---------------------------------------------------------------------*/
bool	Collinear( tVertex a, tVertex b, tVertex c )
{
   return 
         ( c->v[Z] - a->v[Z] ) * ( b->v[Y] - a->v[Y] ) -
         ( b->v[Z] - a->v[Z] ) * ( c->v[Y] - a->v[Y] ) == 0
      && ( b->v[Z] - a->v[Z] ) * ( c->v[X] - a->v[X] ) -
         ( b->v[X] - a->v[X] ) * ( c->v[Z] - a->v[Z] ) == 0
      && ( b->v[X] - a->v[X] ) * ( c->v[Y] - a->v[Y] ) -
         ( b->v[Y] - a->v[Y] ) * ( c->v[X] - a->v[X] ) == 0  ;
}

/*---------------------------------------------------------------------
Computes the z-coordinate of the vector normal to face f.
---------------------------------------------------------------------*/
double	Normz( tFace f )
{
   tVertex a, b, c;
   /*double ba0, ca1, ba1, ca0,z;*/

   a = f->vertex[0];
   b = f->vertex[1];
   c = f->vertex[2];

/*
   ba0 = ( b->v[X] - a->v[X] );
   ca1 = ( c->v[Y] - a->v[Y] );
   ba1 = ( b->v[Y] - a->v[Y] );
   ca0 = ( c->v[X] - a->v[X] );

   z = ba0 * ca1 - ba1 * ca0; 
   printf("Normz = %lf=%g\n", z,z);
   if      ( z > 0.0 )  return  1;
   else if ( z < 0.0 )  return -1;
   else                 return  0;
*/
   return 
      ( b->v[X] - a->v[X] ) * ( c->v[Y] - a->v[Y] ) -
      ( b->v[Y] - a->v[Y] ) * ( c->v[X] - a->v[X] );
}

/*---------------------------------------------------------------------
Consistency runs through the edge list and checks that all
adjacent faces have their endpoints in opposite order.  This verifies
that the vertices are in counterclockwise order.
---------------------------------------------------------------------*/
void	Consistency( void )
{
   register tEdge  e;
   register int    i, j;

   e = edges;

   do {
      /* find index of endpoint[0] in adjacent face[0] */
      for ( i = 0; e->adjface[0]->vertex[i] != e->endpts[0]; ++i )
	 ;
   
      /* find index of endpoint[0] in adjacent face[1] */
      for ( j = 0; e->adjface[1]->vertex[j] != e->endpts[0]; ++j )
	 ;

      /* check if the endpoints occur in opposite order */
      if ( !( e->adjface[0]->vertex[ (i+1) % 3 ] ==
	      e->adjface[1]->vertex[ (j+2) % 3 ] ||
	      e->adjface[0]->vertex[ (i+2) % 3 ] ==
	      e->adjface[1]->vertex[ (j+1) % 3 ] )  )
	 break;
      e = e->next;

   } while ( e != edges );

   if ( e != edges )
      std::cerr << "Checks: edges are NOT consistent.\n";
   else
      std::cerr << "Checks: edges consistent.\n";

}

/*---------------------------------------------------------------------
Convexity checks that the volume between every face and every
point is negative.  This shows that each point is inside every face
and therefore the hull is convex.
---------------------------------------------------------------------*/
void	Convexity( void )
{
   register tFace    f;
   register tVertex  v;
   int               vol;

   f = faces;
   
   do {
      v = vertices;
      do {
	 if ( v->mark ) {
	    vol = VolumeSign( f, v );
	    if ( vol < 0 )
	       break;
	 }
	 v = v->next;
      } while ( v != vertices );

      f = f->next;

   } while ( f != faces );

   if ( f != faces )
      std::cerr << "Checks: NOT convex.\n";
   else if ( check ) 
      std::cerr << "Checks: convex.\n";
}

/*---------------------------------------------------------------------
CheckEuler checks Euler's relation, as well as its implications when
all faces are known to be triangles.  Only prints positive information
when debug is true, but always prints negative information.
---------------------------------------------------------------------*/
void	CheckEuler( int V, int E, int F )
{
   if ( check )
      std::cerr << "Checks: V, E, F = " << V << " " << E << " " << F << ":\t";

   if ( (V - E + F) != 2 )
      std::cerr << "Checks: V-E+F != 2\n";
   else if ( check )
      std::cerr << "V-E+F = 2\t";


   if ( F != (2 * V - 4) )
      std::cerr << "Checks: F=" << F << " != 2V-4=" << 2*V-4 << "; V=" << V << std::endl;
   else if ( check ) 
      std::cerr << "F = 2V-4\t";
   
   if ( (2 * E) != (3 * F) )
      std::cerr << "Checks: 2E=" << 2*E << " != 3F=" << 3*F << "; E=" << E << ", F=" << F << std::endl;
   else if ( check ) 
      std::cerr << "2E = 3F\n";
}

/*-------------------------------------------------------------------*/
void	Checks( void )
{
   tVertex  v;
   tEdge    e;
   tFace    f;
   int 	   V = 0, E = 0 , F = 0;

   Consistency();
   Convexity();
   if ( (v = vertices) != NULL )
      do {
         if (v->mark) V++;
	 v = v->next;
      } while ( v != vertices );
   if ( (e = edges) != NULL )
      do {
         E++;
	 e = e->next;
      } while ( e != edges );
   if ( (f = faces) != NULL )
      do {
         F++;
	 f  = f ->next;
      } while ( f  != faces );
   CheckEuler( V, E, F );
}


/*===================================================================
These functions are used whenever the debug flag is set.
They print out the entire contents of each data structure.  
Printing is to standard error.  To grab the output in a file in the csh, 
use this:
	chull < i.file >&! o.file
=====================================================================*/
/*-------------------------------------------------------------------*/
void	PrintOut( tVertex v )
{
   std::cerr << "\nHead vertex " << v->vnum << " = " << v << " :\n";
   PrintVertices();
   PrintEdges();
   PrintFaces();
}

/*-------------------------------------------------------------------*/
void	PrintVertices( void )
{
   tVertex  temp;

   temp = vertices;
   std::cerr << "Vertex List\n";
   if (vertices) do {
      std::cerr << "  addr " << vertices << "\t";
      std::cerr << "  vnum " <<  vertices->vnum;
      std::cerr << "   (" << vertices->v[X] << "," 
		     << vertices->v[Y] << "," 
		     << vertices->v[Z] << ")";
      std::cerr << "   active:" <<  vertices->onhull;
      std::cerr << "   dup:" << vertices->duplicate;
      std::cerr << "   mark:" <<  vertices->mark << std::endl;
      vertices = vertices->next;
   } while ( vertices != temp );

}

/*-------------------------------------------------------------------*/
void	PrintEdges( void )
{
   tEdge  temp;
   int 	  i;
	
   temp = edges;
   std::cerr << "Edge List\n";
   if (edges) do {
      std::cerr << "  addr: " << edges << "\t";
      std::cerr << "adj: ";
      for (i=0; i<2; ++i) 
	 std::cerr << edges->adjface[i];
      std::cerr << "  endpts:";
      for (i=0; i<2; ++i) 
	 std::cerr <<  edges->endpts[i]->vnum;
      std::cerr << "  del:" << edges->delete_bit << std::endl;
      edges = edges->next; 
   } while (edges != temp );

}

/*-------------------------------------------------------------------*/
void	PrintFaces( void )
{
   int 	  i;
   tFace  temp;

   temp = faces;
   std::cerr << "Face List\n";
   if (faces) do {
      std::cerr << "  addr: " << faces << "\t";
      std::cerr << "  edges:";
      for( i=0; i<3; ++i )
	 std::cerr <<  faces->edge[i];
      std::cerr << "  vert:";
      for ( i=0; i<3; ++i)
	 std::cerr << faces->vertex[i]->vnum;
      std::cerr << "  vis: " << faces->visible << std::endl;
      faces= faces->next;
   } while ( faces != temp );
}

/*-------------------------------------------------------------------*/
void	PrintToLeda( void )
{
   tVertex  temp;

   temp = vertices;
   if (vertices) do {
      std::cout << "(" << vertices->v[X] << ", " 
	          << vertices->v[Y] << ", " 
		  << vertices->v[Z] << ", "
		  << HOMOGENIZING_FACTOR << ") \n";
      vertices = vertices->next;
   } while ( vertices != temp );

}

