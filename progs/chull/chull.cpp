/*****************************************************************
 File: chull.ccp
 Purpose:
	To compute the convex hull of an input set of 3D points.
	It can run in level 1 or 3 of the Core Library.
	This is a CORE-ized version of O'Rourke's program
		(see comments below).	

	Changes to O'Rourke's program:
 	-- all printf and scanf are converted to std::cout and std::cin
 	-- input numbers are assumed to be double instead of int
 	-- input points are in homogeneous coordinates (x,y,z,w)

 Usage:
  	% chull [-l | -s] < input_file >! output_file

        If the optional switch (-l or -s) are not given,
        then chull output a postscript file to draw a projection
	of the convex hull.  The switches modifies this behavior:

	-l 	This forces chull to output the convex hull in a
		form suitable for display using LEDA's window sytem
	-s	This forcess chull to output a list of vertices,
		edges and faces in the convex hull.

 Caveat: Most of O'Rourke's original comments are intact, but
	some of them no longer make sense.

 Author:
	Hui Jin (hj228@cs.nyu.edu), August 1999.

 ******************************************************************
 	O'Rourke's Original Comments:
 ******************************************************************
This code is described in "Computational Geometry in C" (Second Edition),
Chapter 4.  It is not written to be comprehensible without the 
explanation in that book.

Input: 3n integer coordinates for the points.
Output: the 3D convex hull, in postscript with embedded comments
        showing the vertices and faces.

Compile: gcc -o chull chull.c

Written by Joseph O'Rourke, with contributions by 
  Kristy Anderson, John Kutcher, Catherine Schevon, Susan Weller.
Last modified: March 1998
Questions to orourke@cs.smith.edu.
--------------------------------------------------------------------
This code is Copyright 1998 by Joseph O'Rourke.  It may be freely 
redistributed in its entirety provided that this copyright notice is 
not removed.
--------------------------------------------------------------------
*/

#ifndef CORE_LEVEL
#define CORE_LEVEL 3
#endif
#include "CORE.h"

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
   int	    vNum;
   tEdge    duplicate;	        /* pointer to incident cone edge (or NULL) */
   bool     onhull;		/* T iff point on hull. */
   bool	    mark;		/* T iff point already processed. */
   tVertex  next, prev;
};

struct tEdgeStructure {
   int      eNum;
   tFace    adjface[2];
   tVertex  endpts[2];
   tFace    newface;            /* pointer to incident cone face. */
   bool     delete_bit;		/* T iff edge should be delete. */
   tEdge    next, prev;
};

struct tFaceStructure {
   int      fNum;
   tEdge    edge[3];
   tVertex  vertex[3];
   bool	    visible;	        /* T iff face visible from new point. */
   tFace    next, prev;
};

/* Define flags */
#define ONHULL   	true
#define REMOVED  	true
#define VISIBLE  	true
#define PROCESSED	true
#define SAFE		1000000		/* Range of safe coord values. */

/* Global variable definitions */
tVertex vertices = NULL;
tEdge edges    	 = NULL;
tFace faces    	 = NULL;
bool debug = false;
bool check = false;
bool toSimpleFormat = false;		// if true, output is easy to be read
bool toLeda = false;			// if true, output for Leda
int ecount = 0;
int fcount = 0;
const int HOMOGENIZING_FACTOR = 1;

/* Function declarations */
tVertex MakeNullVertex( void );
void    ReadVertices( void );
void    Print( void );
//void    SubVec( int a[3], int b[3], int c[3]);
void    SubVec( double a[3], double b[3], double c[3]);
void    DoubleTriangle( void );
void    ConstructHull( void );
bool	AddOne( tVertex p );
int     VolumeSign(tFace f, tVertex p);
int 	Volumei( tFace f, tVertex p );
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
void    CheckEuler(int V, int E, int F );
void	PrintPoint( tVertex p );
void    Checks( void );
void	Consistency( void );
void	Convexity( void );
void	PrintOut( tVertex v );
void	PrintVertices( void );
void	PrintEdges( void );
void	PrintFaces( void );
void	PrintVerticesToLeda( void );
void	PrintVerticesToSimpleFormat( void );
void	PrintEdgesToSimpleFormat( void );
void	PrintFacesToSimpleFormat( void );

#include "macros.h"

/*-------------------------------------------------------------------*/
int main( int argc, char *argv[] )
{

  if ( argc > 1 && argv[1][0] == '-' ) {
    if( argv[1][1] ==  'd' ) {
      debug = true;
      check = true;
      std::cerr << "Debug and check mode\n";
    }
    if( argv[1][1] == 'c' ) {
      check = true;
      std::cerr << "Check mode\n" ;
    }
    if( argv[1][1] == 'l' ) {
      toLeda = true;
      std::cerr << "Output for LEDA\n" ;
    }
    if( argv[1][1] == 's' ) {
      toSimpleFormat = true;
      std::cerr << "Output in simple format\n" ;
    }
  }
  else if ( argc > 1 && argv[1][0] != '-' ) {
    std::cout << "Usage:  %s -d[ebug] c[heck]\n" <<  *argv ;
    std::cout << "x y z coords of vertices from stdin\n" ;
    exit(1);
  }
  
   ReadVertices();
   DoubleTriangle();
   ConstructHull();
   if (toLeda) 	// output for Leda
   {
	PrintVerticesToLeda();
   } 
   else	if(toSimpleFormat)  // output in simple format
   {
   	PrintVerticesToSimpleFormat();
   	PrintEdgesToSimpleFormat();
   	PrintFacesToSimpleFormat();
   }
   else  //  output postscript file
   {
	Print();
   }
   return 0;
}

/*---------------------------------------------------------------------
MakeNullVertex: Makes a vertex, nulls out fields.
---------------------------------------------------------------------*/
tVertex	MakeNullVertex( void )
{
   tVertex  v;
   
   // NEW( v, tsVertex );
   v = new tsVertex ;
   v->vNum = -1;
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
   int	    vNum = 0;

    while( std::cin ) {
      std::cin >> x >> y >> z;
      v = MakeNullVertex();
      v->v[X] = x;
      v->v[Y] = y;
      v->v[Z] = z;
      v->vNum = vNum++;
      if ( ( fabs(x) > SAFE ) || ( fabs(y) > SAFE ) || ( fabs(z) > SAFE ) ) {
         std::cout << "Coordinate of vertex below might be too large: run with -c flag\n" ;
         PrintPoint(v);
      }
   }
}

/*---------------------------------------------------------------------
Print: Prints out the vertices and the faces.  Uses the vNum indices 
corresponding to the order in which the vertices were input.
Output is in PostScript format.
---------------------------------------------------------------------*/
void	Print( void )
{
   /* Pointers to vertices, edges, faces. */
   tVertex  v;
   tEdge    e;
   tFace    f;
   //int xmin, ymin, xmax, ymax;
   double xmin, ymin, xmax, ymax;
   //int a[3], b[3];  /* used to compute normal vector */
   double a[3], b[3];  /* used to compute normal vector */
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
	
   /* PostScript header */
   std::cout << "%%!PS\n" ;
   std::cout << "%%%%BoundingBox: " << xmin
	<< " " << ymin
	<< " " << xmax
	<< " " << ymax << std::endl;
   std::cout << ".00 .00 setlinewidth\n" ;
   double scale = 500.0 / ((ymax-ymin) >= (xmax - xmin) ? ymax - ymin: xmax - xmin);
   std::cout << -xmin*scale+72 << " " << -ymin*scale+120 << " translate\n" ;
   /* The +72 shifts the figure one inch from the lower left corner */
   std::cout << scale << " " << scale << " scale" << std::endl;


   /* Vertices. */
   v = vertices;
   do {                                 
      if( v->mark ) V++;           
      v = v->next;
   } while ( v != vertices );
   std::cout << "\n%%%% Vertices:\tV = " << V << std::endl ;
   std::cout << "%%%% index:\tx\ty\tz\n" ;
   do {                                 
	//     v->vNum, v->v[X], v->v[Y], v->v[Z] );
      std::cout << "%%%% " << v->vNum << ":\t"
		<< v->v[X] << "\t" 
		<< v->v[Y] << "\t" 
		<< v->v[Z] << std::endl ;
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
   std::cout << "%%%% Visible faces only: \n" ;
   do {           
      /* Print face only if it is visible: if normal vector >= 0 */
      SubVec( f->vertex[1]->v, f->vertex[0]->v, a );
      SubVec( f->vertex[2]->v, f->vertex[1]->v, b );	  
      if(( a[0] * b[1] - a[1] * b[0] ) >= 0 )
      {
	std::cout << "%%%% vNums:  " <<
		f->vertex[0]->vNum << " " << 
		f->vertex[1]->vNum << " " << 
		f->vertex[2]->vNum << std::endl;
	std::cout << "newpath\n" ;
	std::cout << f->vertex[0]->v[X] << "\t" 
		<< f->vertex[0]->v[Y] << "\tmoveto\n" ; 
	std::cout << f->vertex[1]->v[X] << "\t"
		<< f->vertex[1]->v[Y] << "\tlineto\n" ; 
	std::cout << f->vertex[2]->v[X] << "\t"
		<< f->vertex[2]->v[Y] << "\tlineto\n" ; 
	std::cout << "closepath stroke\n\n" ;
      }
      f = f->next;
   } while ( f != faces );

   /* prints a list of all faces */
   std::cout << "%%%% List of all faces: \n" ;
   std::cout << "%%%%\tv0\tv1\tv2\t(vertex indices)\n" ;
   do {
      std::cout << "%%%%\t" <<
	 	f->vertex[0]->vNum << "\t" <<
	 	f->vertex[1]->vNum << "\t" <<
	 	f->vertex[2]->vNum << std::endl;
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

   std::cout << "\nshowpage\n\n" ;

   check = true;
   CheckEuler( V, E, F );

}

/*---------------------------------------------------------------------
SubVec:  Computes a - b and puts it into c.
---------------------------------------------------------------------*/
/*
void    SubVec( int a[3], int b[3], int c[3])
{
   int  i;

   for( i=0; i < 2; i++ )
      c[i] = a[i] - b[i];

}
*/
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
	
   /* Find 3 noncollinear points. */
   v0 = vertices;
   while ( Collinear( v0, v0->next, v0->next->next ) )
      if ( ( v0 = v0->next ) == vertices ) {
         std::cout << "DoubleTriangle:  All points are Collinear!\n" ;
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
   f0->fNum = fcount++;
   f1->fNum = f0->fNum;

   /* Link adjacent face fields. */
   f0->edge[0]->adjface[1] = f1;
   f0->edge[1]->adjface[1] = f1;
   f0->edge[2]->adjface[1] = f1;
   f1->edge[0]->adjface[1] = f0;
   f1->edge[1]->adjface[1] = f0;
   f1->edge[2]->adjface[1] = f0;
	
   /* Find a fourth, noncoplanar point to form tetrahedron. */
   v3 = v2->next;
   vol = VolumeSign( f0, v3 );
   // while ( !vol )   {	// THIS IS A BUG!! Chee (July 2003)
   while (vol == 0) {
      if ( ( v3 = v3->next ) == v0 ) {
         std::cout << "DoubleTriangle:  All points are coplanar!\n";
	 exit(0);
      }
      vol = VolumeSign( f0, v3 );
   }
	
   /* Insure that v3 will be the first added. */
   vertices = v3;
   if ( debug ) {
      std::cerr << "DoubleTriangle: finished. Head repositioned at v3.\n" ;
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
	 //changed = AddOne( v );	// changed not used
	 AddOne( v );
	 CleanUp();

	 if ( check ) {
	    std::cerr << "ConstructHull: After Add of " <<
                v->vNum << " & Cleanup:\n" ; 
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
   tEdge  e, temp;
   int 	  vol;
   bool	  vis = false;

   if ( debug ) {
      std::cerr <<  "AddOne: starting to add v " << p->vNum << ".\n";
      PrintOut( vertices );
   }

   /* Mark faces visible from p. */
   f = faces;
   do {
      vol = VolumeSign( f, p );
      if (debug)
	  std::cerr << "faddr: " << f << "  paddr: " << p
	  << "   Vol = " << vol << std::endl ;
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
      temp = e->next;
      if ( e->adjface[0]->visible && e->adjface[1]->visible )
      {
	 /* e interior: mark for deletion. */
	 e->delete_bit = REMOVED;
      }
      else if ( e->adjface[0]->visible || e->adjface[1]->visible ) 
      {
       	 e->eNum = ecount++;
	 /* e border: make a new face. */
	 e->newface = MakeConeFace( e, p );
      }
      e = temp;
   } while ( e != edges );
   return true;
}

/*---------------------------------------------------------------------
VolumeSign returns the sign of the volume of the tetrahedron determined by f
and p.  VolumeSign is +1 iff p is on the negative side of f,
where the positive side is determined by the rh-rule.  So the volume 
is positive if the ccw normal to f points outside the tetrahedron.
The final fewer-multiplications form is due to Bob Williamson.
---------------------------------------------------------------------*/
int  VolumeSign( tFace f, tVertex p )
{
   double  vol;
   int     voli = 0;
   double  ax, ay, az, bx, by, bz, cx, cy, cz;

   ax = f->vertex[0]->v[X] - p->v[X];
   ay = f->vertex[0]->v[Y] - p->v[Y];
   az = f->vertex[0]->v[Z] - p->v[Z];
   bx = f->vertex[1]->v[X] - p->v[X];
   by = f->vertex[1]->v[Y] - p->v[Y];
   bz = f->vertex[1]->v[Z] - p->v[Z];
   cx = f->vertex[2]->v[X] - p->v[X];
   cy = f->vertex[2]->v[Y] - p->v[Y];
   cz = f->vertex[2]->v[Z] - p->v[Z];

   vol =   ax * (by*cz - bz*cy)
         + ay * (bz*cx - bx*cz)
         + az * (bx*cy - by*cx);

   if ( debug )
      std::cerr << "Face=" << f << "; Vertex=" << p->vNum
	   << ": vol(int) = " << voli
	   << ", vol(double) = " << vol << std::endl;

   /* The volume should be an integer. */
   if      ( vol >  0.5 )  return  1;
   else if ( vol < -0.5 )  return -1;
   else                    return  0;
}
/*---------------------------------------------------------------------*/
int  Volumei( tFace f, tVertex p )
{
   double  vol;
   int     voli = 0;
   double  ax, ay, az, bx, by, bz, cx, cy, cz;

   ax = f->vertex[0]->v[X] - p->v[X];
   ay = f->vertex[0]->v[Y] - p->v[Y];
   az = f->vertex[0]->v[Z] - p->v[Z];
   bx = f->vertex[1]->v[X] - p->v[X];
   by = f->vertex[1]->v[Y] - p->v[Y];
   bz = f->vertex[1]->v[Z] - p->v[Z];
   cx = f->vertex[2]->v[X] - p->v[X];
   cy = f->vertex[2]->v[Y] - p->v[Y];
   cz = f->vertex[2]->v[Z] - p->v[Z];

   vol =  (ax * (by*cz - bz*cy)
         + ay * (bz*cx - bx*cz)
         + az * (bx*cy - by*cx));

   if ( debug )
      std::cerr << "Face=" << f << "; Vertex=" << p->vNum
	   << ": vol(int) = " << voli
	   << ", vol(double) = " << vol << std::endl ;

   /* The volume should be an integer. */
   if      ( vol > 0.5 )   return  1;
   else if ( vol < -0.5 )  return -1;
   else                    return  0;
}


/*-------------------------------------------------------------------*/
void	PrintPoint( tVertex p )
{
   int	i;

   for ( i = 0; i < 3; i++ )
      std::cout << "\t" << p->v[i] ;
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
	 new_edge[i]->eNum = ecount++;
	 new_edge[i]->endpts[0] = e->endpts[i];
	 new_edge[i]->endpts[1] = p;
	 e->endpts[i]->duplicate = new_edge[i];
      }

   /* Make the new face. */
   new_face = MakeNullFace();   
   new_face->fNum = fcount++;
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

   // NEW( e, tsEdge );
   e = new tsEdge;
   e->eNum = -1;
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

   // NEW( f, tsFace);
   f = new tsFace;
   f->fNum = -1;
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
      std::cerr << "Checks: edges are NOT consistent.\n" ;
   else
      std::cerr << "Checks: edges consistent.\n" ;

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
      std::cerr << "Checks: NOT convex.\n" ;
   else if ( check ) 
      std::cerr << "Checks: convex.\n" ;
}

/*---------------------------------------------------------------------
CheckEuler checks Euler's relation, as well as its implications when
all faces are known to be triangles.  Only prints positive information
when debug is true, but always prints negative information.
---------------------------------------------------------------------*/
void	CheckEuler( int V, int E, int F )
{
   if ( check )
      std::cerr << "Checks: V, E, F = " << V << " " << E
		<< " " << F << ":\t";

   if ( (V - E + F) != 2 )
      std::cerr << "Checks: V-E+F != 2\n" ;
   else if ( check )
      std::cerr << "V-E+F = 2\t" ;


   if ( F != (2 * V - 4) )
      std::cerr << "Checks: F=" << F
		<< " != 2V-4=" << 2*V-4
		<< "; V=" << V << std::endl;
   else if ( check ) 
      std::cerr << "F = 2V-4\t" ;
   
   if ( (2 * E) != (3 * F) )
      std::cerr << "Checks: 2E=" << 2*E
		<< " != 3F=" << 3*F
		<< "; E=" << E 
		<< ", F=" << F << std::endl;
   else if ( check ) 
      std::cerr << "2E = 3F\n" ;
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
   std::cerr << "\nHead vertex " 
	<< v->vNum << " = " << v << " :\n";
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
      std::cerr << "  vNum " << vertices->vNum ;
      std::cerr << "   (" << vertices->v[X] << ","
      		<< vertices->v[Y] << ","
      		<< vertices->v[Z] << ")";
      std::cerr << "   active:" << vertices->onhull ;
      std::cerr << "   dup:" << vertices->duplicate ;
      std::cerr << "   mark:" << vertices->mark << std::endl;
      vertices = vertices->next;
   } while ( vertices != temp );

}

/*-------------------------------------------------------------------*/
void	PrintEdges( void )
{
   tEdge  temp;
   int 	  i;
	
   temp = edges;
   std::cerr << "Edge List\n" ;
   if (edges) do {
      std::cerr << "  addr: " << edges << "\t" ;
      std::cerr << "adj: " ;
      for (i=0; i<2; ++i) 
	 std::cerr << edges->adjface[i] ;
      std::cerr << "  endpts:" ;
      for (i=0; i<2; ++i) 
	 std::cerr << edges->endpts[i]->vNum ;
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
   std::cerr <<  "Face List\n";
   if (faces) do {
      std::cerr << "  addr: " << faces << "\t";
      std::cerr << "  edges:" ;
      for( i=0; i<3; ++i )
	 std::cerr << faces->edge[i] ;
      std::cerr << "  vert:" ;
      for ( i=0; i<3; ++i)
	 std::cerr << faces->vertex[i]->vNum ;
      std::cerr << "  vis: " << faces->visible << std::endl;
      faces= faces->next;
   } while ( faces != temp );

}

/*-------------------------------------------------------------------*/
void    PrintToSimpleFormat ( void )
{
	PrintVerticesToSimpleFormat();
	PrintEdgesToSimpleFormat();
	PrintEdgesToSimpleFormat();
}

/*-------------------------------------------------------------------*/
void    PrintVerticesToSimpleFormat( void )
{
   tVertex  temp;

   temp = vertices;
   std::cout << "Vertex List\n";
   if (vertices) do {
      std::cout << "v" << vertices->vNum ;
      std::cout << "   (" << vertices->v[X] << ","
                << vertices->v[Y] << ","
                << vertices->v[Z] << ")" << std::endl;
      vertices = vertices->next;
   } while ( vertices != temp );

}

/*-------------------------------------------------------------------*/
void    PrintEdgesToSimpleFormat ( void )
{
   tEdge  temp;
   int    i;
       
   temp = edges;
   std::cout << "\nEdge List\n" ;
   if (edges) do {
      std::cout << "e" << edges->eNum << "   (";
      for (i=0; i<2; ++i)
         std::cout << "v" << edges->endpts[i]->vNum << ",";
      std::cout << "f" << edges->adjface[0]->fNum << ",";
      std::cout << "f" << edges->adjface[1]->fNum << ")" << std::endl;
      edges = edges->next;
   } while (edges != temp );

}

/*-------------------------------------------------------------------*/
void    PrintFacesToSimpleFormat( void )
{
   int    i;
   tFace  temp;

   temp = faces;
   std::cout <<  "\nFace List\n";
   if (faces) do {
      std::cout << "f" << faces->fNum << "   (";
      for ( i=0; i<2; ++i)
         std::cout << "v" << faces->vertex[i]->vNum << ",";
      std::cout << "v" << faces->vertex[2]->vNum << ")" << std::endl;
      faces= faces->next;
   } while ( faces != temp );

}

/*-------------------------------------------------------------------*/
void    PrintVerticesToLeda( void )
{
   tVertex  temp;

   temp = vertices;
   if (vertices) do {
      std::cout << "(" << vertices->v[X] << ","
                << vertices->v[Y] << ","
                << vertices->v[Z] << ","
		<< HOMOGENIZING_FACTOR << ") \n";
      vertices = vertices->next;
   } while ( vertices != temp );

}

