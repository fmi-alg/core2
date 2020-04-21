/***********************************************************************
 * LAST UPDATE  $Id: drawCurve.cpp,v 1.6 2009/04/27 17:20:26 exact Exp $
 *
 * FILE NAME: drawcurve.cpp 
 * (adapted from drawpoly.cc)
 *
 * USAGE:
 * 		>> drawcurve [verbose] [< inputfile]
 *
 * 		where the optional argument [verbose], if given,
 * 		will cause some printout on the screen (mainly
 * 		for debugging). 
 *
 * 		To quit the display, type ESC.
 *
 * DESCRIPTION:
 *  --- Reads from standard input a list of 2D polygons or polylines
 *      and draws them.  Each polygon has its own color.
 *  --- The max and min values of input points are determined
 *      and the window is scaled to fit the entire set of polygons
 *  --- Uses OpenGL/GLUT library
 *
 * INPUT FORMAT:
 *  --- Each polygon has the format
 *      	p <number_of_vertices>
 *      	<r_value> <g_value> <b_value>
 *      	<x_coord> <y_coord>
 *      	<x_coord> <y_coord>
 *      	...
 *      	<x_coord> <y_coord>
 *      E.g. 
 *      	p   4                    # this specifies a quadrilateral
 *      	0.183     0.273   0.281  # the RGB color of the triangle
 *      	0           2            # first point
 *      	-0.26       2            # second point
 *      	1.5         2.07         # third point
 *      	.123e2      .123e-2      # fourth point (scientific notation!)
 *  --- The separate elements are separated by whitespaces
 *      (space, tabs or newlines).
 *  --- By definition, the polygon is closed
 *      (the first point is repeated at the end).
 *  --- Instead of polygon (with the character 'p'), you could 
 *      specify an open polygonal curve, using the character 'o'.
 *  --- We allow comment, which is any line
 *      that begins with `#' (perhaps preceded by whitespaces).
 *      Also, after each line in the above format.
 *  --- Note that scientific notation is allowed.
 *
 * BUGS/FEATURES:
 *      --The list of polygons is reversed 
 * TODO:
 *      zooming, panning,
 *      axes display
 *      labels and point display
 *      calls Core plot to redraw curve?
 *
 * HISTORY:
 *	Jan 28, 1999: original version by Chen Li, for
 *		the visualization project at NYU
 *	Mar  3, 1999:  modified by Chee and Ting-Jen to allow comment 
 *		lines (`#') and common scaling in x and y directions
 *		(so image is not distorted).
 *	Mar 28, 2004: adapted by Chee for Core Library display
 ***********************************************************************/

#include <iostream>
#include <limits.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
// #include <GL/glut.h>
// #include <gl/glut.h>
#include <GLUT/glut.h>
using namespace std;



#define MY_POLYGONS_LIST 1
const int SIZEOFBUFFER = 500;

// DBL_MAX not found in cygwin environment, so hack:
double DBL_MAX = (double)(1L << 30);

char buf[SIZEOFBUFFER];

class Vertex2D {
private:
  double x;
  double y;
public:
  Vertex2D() :x(0), y(0) {}
  Vertex2D(double x, double y) : x(x), y(y) {}
  void setX(double x) { this->x = x; }
  void setY(double y) { this->y = y; }
  double xval() const { return x; }
  double yval() const { return y; }
  void dump() const { cout << "   vertex (" << x << ", " << y << ")" << endl;}
};

class Polygon2D {
private:
  int n;	// number of points in polygon
  char c;	// c='p' for closed polygon, c='o' for open curve
  Vertex2D *p;
  double r, g, b; // colors
public:
  Polygon2D *next;

  Polygon2D() : n(0), c('p'), p(NULL), r(1.0), g(1.0), b(1.0), next(NULL)  {}
  Polygon2D(int n, char c, double r, double g, double b) {
    this->n = n;
    this->c = c;
    p = new Vertex2D[n];
    this->r = r; 
    this->g = g;
    this->b = b;
    next = NULL;
  }

  void setp(int i, const Vertex2D &v) { p[i] = v; }
  Vertex2D getp(int i) { return p[i]; }
  int nval() { return n; }
  char cval() { return c; }

// The following is a KLUDGE to ensure that the colors of polygons are not 
// too dark (since they are drawn against a dark background:

  double rval() { return (r > 0.5) ? r: 1. -r; }
  double gval() { return g; }
  double bval() { return (b>0.5) ? b : 1.-b; }

  void dump() const {
    cout << "--- POLYGON ---" << endl;
    cout << "n = " << n << endl;
    for (int i=0; i<n; i++) 
      p[i].dump();
  }
};

double xo = 0.0;
double yo = 0.0;
double xScale = 1;
double yScale = 1;
double vStretch = 1;
double hStretch = 1;

Polygon2D *myPolys = NULL;

char getNextChar() {
  char c = cin.peek();
  while (isspace(c) || c == '#') {
    if (c != '#') {
	cin.get(c);
    } else  {
	cin.getline(buf, SIZEOFBUFFER);
    }
    c = cin.peek();
  }
  return c;  // may be EOF char!
}

void readinPolys(bool verbose) {
  char c = 0;
  int n;

  c = getNextChar();  // c is a peek.

  // while (!cin.eof()) {
  //   if ((c != 'o') || (c != 'p')) break;
  //
  while ((c == 'o') || (c == 'p')) { 
	  // p='c' is a closed polygon
	  // c='o' for a open polygonal curve
    cin >> c;
    cin >> n; 
    getNextChar();
    double r, g, b;
    cin >> r >> g >> b;
    getNextChar();
    Polygon2D *poly = new Polygon2D(n, c, r, g, b);

    for (int i=0; i<n; i++) {
      double x, y;
      cin >> x >> y;
      getNextChar();
      poly->setp(i, Vertex2D(x, y));
    }
    poly->next = myPolys;
    myPolys = poly;
    c = getNextChar();
  }

  if (verbose) { 
    Polygon2D *tmp = myPolys;
    while (tmp) {
      tmp->dump();
      tmp = tmp->next;
    }
  }
  return;
}//readinPolys



static void Idle( void )
{
  /* update animation vars */
  glutPostRedisplay();
}

// Timer function - this should run the app at 50Hz 
// (which should be far more than needed, it's a static app
//  otherwise, the Idle() will run as fast as it can, and 
// hang a system that isn't v-synced on swapbuffer. 

static void TimerCB( int t ) { 
  Idle();
  glutTimerFunc( 20, TimerCB, t );
}

void polygon(Polygon2D *poly) {
//  'p' draws shaded polygons, so individual polygons may not show up:
  if (poly->cval() == 'p')
	  glBegin(GL_LINE_LOOP);	// for closed polygons
  else if (poly->cval() == 'o')
  	glBegin(GL_LINE_STRIP);	// for open polygons
  else {
	  cout << "ERROR" << endl;
	  exit(1);
  }
  glColor3f(poly->rval(), poly->gval(), poly->bval());
  for (int i=0; i<poly->nval(); i++)
  {
    glVertex2f((poly->getp(i).xval() - xo) * xScale * hStretch,
	       (poly->getp(i).yval() - yo) * yScale * vStretch);
  }
  
  glEnd();
}

void buildPolygonList() {
  glNewList(MY_POLYGONS_LIST, GL_COMPILE);
  glPushMatrix();

  // polygons go here
  // should be rewritten to fit the concrete application
  Polygon2D *tmp = myPolys;
  while (tmp) {
    polygon(tmp);
    tmp = tmp->next;
  }
  glPopMatrix();
  glEndList();
}

static void Display( void )
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glPushMatrix();
   /* draw stuff here */
   glCallList(MY_POLYGONS_LIST);

   glPopMatrix();

   glutSwapBuffers();
}


static void Reshape( int width, int height )
{
   glViewport( 0, 0, width, height );
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   gluOrtho2D(0.0, 100.0, 0.0, 100.0);
   //   glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 25.0 );
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   glTranslatef( 50.0, 50.0, 0.0);
}

// not used
static void Key( unsigned char key, int x, int y )
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
   glutPostRedisplay();
}

// not used
static void SpecialKey( int key, int x, int y )
{
   switch (key) {
      case GLUT_KEY_UP:
         break;
      case GLUT_KEY_DOWN:
         break;
      case GLUT_KEY_LEFT:
         break;
      case GLUT_KEY_RIGHT:
         break;
   }
   glutPostRedisplay();
}


static double Init( bool verbose )
{
  readinPolys(verbose);
  /* compute scaling factor here, based on the scale of your polygons */
  double xmin, xmax, ymin, ymax;
  double ratio;

  xmin = DBL_MAX; ymin = DBL_MAX;
  xmax = -DBL_MAX; ymax = -DBL_MAX;


  Polygon2D *tmp = myPolys;
  while (tmp) {
    for (int i=0; i < tmp->nval(); i++) {
      Vertex2D vi = tmp->getp(i);
      double vix = vi.xval();
      double viy = vi.yval();
      if (xmin > vix) xmin = vix;
      if (xmax < vix) xmax = vix;
      if (ymin > viy) ymin = viy;
      if (ymax < viy) ymax = viy;
	
    }
    tmp = tmp->next;
  }

  cout << "xmin = " << xmin << ", xmax = " << xmax << endl;
  cout << "ymin = " << ymin << ", ymax = " << ymax << endl;

  ratio = (xmax-xmin)/(ymax - ymin);
// Chee: compensate for very large or very small ratios:
  if (ratio>5) {
	  vStretch = ratio/5.;
  }
  if (ratio<0.2) {
	  hStretch = 1./(ratio*5.);
  }

  cout << "ratio = (xmax-xmin)/(ymax-ymin) = " << ratio << endl;

  xo = (xmin + xmax) / 2.0;
  yo = (ymin + ymax) / 2.0;


// modified by Chee and Ting-Jen
// double scale = ((xmax-xmin) < (ymax-ymin)) ? (ymax-ymin) : (xmax-xmin);
  xScale = 100 * 0.9 / (xmax-xmin);
  yScale = 100 * 0.9 / (ymax-ymin);
  cout << "xScale = " << xScale << endl;
  cout << "yScale = " << yScale << endl;

  return(ratio);
}//Init


int main( int argc, char *argv[] )
{
   double ratio;

   bool verbose=false;
   if (argc > 1) {
      verbose=true;
      cout << "Verbose Mode !" << endl;
   }

   glutInit( &argc, argv );

   ratio = Init(verbose);

   glutInitWindowSize( (int)(400.0*ratio), 400 );

   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );

   glutCreateWindow(argv[0]);

   /* setup call lists, etc */
   buildPolygonList();

   glutReshapeFunc( Reshape );
   glutKeyboardFunc( Key );
   glutSpecialFunc( SpecialKey );
   glutDisplayFunc( Display );
   //   glutIdleFunc( Idle );
   glutTimerFunc( 20, TimerCB, 0 );
   glutMainLoop();
}

