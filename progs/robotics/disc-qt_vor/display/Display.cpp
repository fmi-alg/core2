// Header
#include "display/Display.h"

// Custom
#include "ShaderException.h"

// Standard Library
#include <iostream>

//Sleeping Library
#include <unistd.h>


// Global Variables in triangle-qt.cpp
extern QuadTree* QT;
extern std::string fileName;
extern Point2d alpha;
extern Point2d beta;
extern double epsilon;
extern double R0;
extern int SearchType;
extern double boxWidth;
extern double boxHeight;
extern Box* boxA;
extern Box* boxB;
extern bool noPath;
extern bool hideBoxBoundary;
extern bool hideBox;
extern bool showAnim;
extern bool pauseAnim;
extern bool replayAnim;
extern bool showTrace;
extern bool showFilledObstacles;

// usleep((99-animationSpeed)*animationSpeedScale);
extern int animationSpeed;         // control the speed on the slider
extern int animationSpeedScale;    // the scale is used when we are not rendering the boxes
extern int animationSpeedScaleBox; // the scaleBox is for rendering the boxes simultaneously

extern vector<Box*> PATH;
extern vector<Polygon2d> polygons;
extern std::vector<int> expansions;

// color coding variable ========================================
extern Color clr_totalFREE;
extern Color clr_partialFREE;
extern Color clr_MIXED;
extern Color clr_STUCK;
extern Color clr_EPS;
extern Color clr_UNKNOWN;
extern Color clr_start;
extern Color clr_goal;
extern Color clr_path;
extern Color clr_robot;
extern Color clr_obstacle;
extern Color clr_boundary;


bool doTriangulation(false);
vector<Triangle2d> triangles;

// Member Variables
static int numQuads = 0;
unsigned int inc(0);
unsigned int renderSteps(1);
bool step(false);






/*
 * CONSTRUCTOR
 *
 * Define data members
 */
Display::Display(QWidget* parent):
    QOpenGLWidget(parent)
{}

/*
 * DESTRUCTOR
 *
 * Destroy shader program
 */
Display::~Display()
{}

/*
 * INITIALIZE GL
 *
 * Called once to prepare display window for
 * rendering.
 * Creates and initializes shader program
 */
void Display::initializeGL() {
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    glClearDepth(1.0f);
}


/*
 * PAINT GL
 *
 * Regenerates (if necessary) and draws scene to
 * dispaly screen.
 */
void Display::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    renderScene();
    glFlush();
}


/*
 * RESIZE GL
 *
 * Sets size of viewport.
 * If 'Display' is the wooden frame of a painting,
 * the viewport is the canvas.
 */
void Display::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, 0, height); // set origin to bottom left corner
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Display::renderScene() {

  if(!hideBox){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    putBoxes();
    glDisable(GL_BLEND);
  }


  drawObstacles();

  Point2d start = alpha;
  Point2d goal = beta;

  if(!noPath){
    drawPath(start, goal);

    if(replayAnim){
      replayAnim = false;

      showAnim = true;
      pauseAnim = false;
      inc = 0;
    }

    if(showAnim){
      if(hideBox) usleep((99-animationSpeed)*animationSpeedScale);
      else        usleep((99-animationSpeed)*animationSpeedScaleBox);

      if(pauseAnim){
        drawRobot(PATH.at(PATH.size()-1-inc), clr_robot);
      }
      else {
        if(inc < PATH.size()){            //each regen, draw robot at next step in path
          Box* cur = PATH.at(PATH.size()-1-inc++);
          drawRobot(cur, clr_robot);
        }
        else if(inc==PATH.size()){   //draw robot in final box
          drawRobot(boxB, clr_goal);
          inc++;
        }
        else if(inc==PATH.size()+1){     //draw robot at end position
          drawRobot(beta, clr_goal);
          showAnim=false;
          inc=0;
        }
      }
      update();
    }
    else{
      if(showTrace){
        for(unsigned i=0;i<PATH.size();i+=5){
          Box* cur = PATH.at(i);
          drawRobot(cur, clr_robot);
        }
      }
    }
  }

  drawRobot(start, clr_start);
  drawRobot(goal, clr_goal);
}

//===========================================================//
//======            Functions to Draw Shapes           ======//
//===========================================================//
void Display::putBoxes(){
  Box* tmp;
  if(step){
    for(unsigned int i=0;  i<Box::pAllLeaf->size()&&i<expansions.at(renderSteps);i++){
      tmp=Box::pAllLeaf->at(i);
      drawBoxes(tmp,epsilon);
      numQuads++;
    }
  }//NOTE: This draws ALL quads, including non leaves - this is overkill, and wastes time and space.
  //Can we do this better?
  else{
    for(unsigned int i=0;i<Box::pAllLeaf->size();i++){
      tmp = Box::pAllLeaf->at(i);
      if(!tmp->isLeaf) continue;
      drawBoxes(tmp,epsilon);
      numQuads++;
    }
  }
}

/*
 * DRAW Boxes(Quads)
 */
void Display::drawBoxes(Box* b, double epsilon) {

    Color clr_box = clr_MIXED;

    switch(b->status) {
        case FREE:
            clr_box = clr_totalFREE;
            break;
        case STUCK:  //color is red
            clr_box = clr_STUCK;
            break;
        case MIXED: //color is yellow (if box is epsilon-large)
            if (b->height < epsilon || b->width < epsilon) { // color is gray (if box is epsilon-small)
                clr_box = clr_EPS;
            }
            break;
        case UNKNOWN:
            //* << "UNKNOWN box status\n";
        break;
    }
    // 1st Corner: lower left
    Point2d lower_left  = Point2d(b->mB.X() - b->width / 2, b->mB.Y() - b->height / 2);
    // 2nd Corner: lower right
    Point2d lower_right = Point2d(b->mB.X() + b->width / 2, b->mB.Y() - b->height / 2);
    // 3rd Corner: upper right
    Point2d upper_right = Point2d(b->mB.X() + b->width / 2, b->mB.Y() + b->height / 2);
    // 4th Corner: upper left
    Point2d upper_left  = Point2d(b->mB.X() - b->width / 2, b->mB.Y() + b->height / 2);

    glColor4fv(clr_box.rgba);
    glBegin(GL_POLYGON);
        glVertex2f( lower_left.X(),  lower_left.Y() );
        glVertex2f( lower_right.X(), lower_right.Y() );
        glVertex2f( upper_right.X(), upper_right.Y() );
        glVertex2f( upper_left.X(),  upper_left.Y() );
        glVertex2f( lower_left.X(),  lower_left.Y() );
    glEnd();

    // 1 2 3 4
    if (!hideBoxBoundary) {
        glColor4fv(Color(0,0,0,1).rgba);
        glLineWidth(2.0);
        glBegin(GL_LINE_STRIP);
            glVertex2f( lower_left.X(),  lower_left.Y() );
            glVertex2f( lower_right.X(), lower_right.Y() );
            glVertex2f( upper_right.X(), upper_right.Y() );
            glVertex2f( upper_left.X(),  upper_left.Y() );
            glVertex2f( lower_left.X(),  lower_left.Y() );
        glEnd();
        glLineWidth(1.0);
    }
}

/*
 * DRAW Obstacles
 */
void Display::drawObstacles() {
    Box* b = QT->pRoot;
    glLineWidth(3.0);
    for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end(); ++iter) {
        Wall* w = *iter;
        glColor4fv(clr_obstacle.rgba);
        glBegin(GL_LINE_STRIP);
            glVertex2f(w->src->p.X(), w->src->p.Y());
            glVertex2f(w->dst->p.X(), w->dst->p.Y());
        glEnd();
    }
    glLineWidth(1.0);

    if(showFilledObstacles){
        if(!doTriangulation){
            doTriangulation = true;
            triangles.clear();
            for(unsigned p=0;p<polygons.size();++p){
              if(polygons[p].orientation() > 0) continue;
              vector<Point2d> a;
              for(unsigned j=0;j<polygons[p].points().size()-1;++j){
                a.push_back(polygons[p].pointp(j));
              }
              // allocate an STL vector to hold the answer.
              vector<Point2d> result;
              //  Invoke the triangulator to triangulate this polygon.
              Polygon2d tmp;
              tmp.processTriangulate(a, result);
              // print out the results.
              int tcount = result.size()/3;

              for (int i=0; i<tcount; i++){
                  const Point2d &p1 = result[i*3+0];
                  const Point2d &p2 = result[i*3+1];
                  const Point2d &p3 = result[i*3+2];
                  Triangle2d tmp(p1, p2, p3);
                  triangles.push_back(tmp);
              }
            }
        }
        else{
            for(unsigned tri=0;tri<triangles.size();++tri){
                drawTriangle(triangles[tri], Color(0.6,0.6,0.6), true, false);
            }
        }
    }
}

/*
 * DRAW PATH
 */
void Display::drawPath(Point2d start, Point2d goal) {
    glColor4fv(clr_path.rgba);
    glLineWidth(3.0);
    glBegin(GL_LINE_STRIP);
      glVertex2f(goal.X(), goal.Y());
      for(unsigned i=0; i<PATH.size(); ++i){
        glVertex2f(PATH[i]->mB.X(), PATH[i]->mB.Y());
      }
      glVertex2f(start.X(), start.Y());
    glEnd();
    glLineWidth(1.0);
}

void Display::drawRobot(Box* b, Color c){
  //fill a circle using a triangle fan
  glColor4fv(c.rgba);
  glBegin(GL_TRIANGLE_FAN);
    //All triangles fan out starting with this point
    glVertex2f(b->mB.X(), b->mB.Y());
    for(int i=0; i<=361; ++i){
      glVertex2f(b->mB.X()+R0*cos(i*M_PI/180.0f), b->mB.Y()+R0*sin(i*M_PI/180.0f));
    }
  glEnd();
}

void Display::drawRobot(Point2d p, Color c){
  //fill a circle using a triangle fan
  glColor4fv(c.rgba);
  glBegin(GL_TRIANGLE_FAN);
    //All triangles fan out starting with this point
    glVertex2f(p.X(), p.Y());
    for(int i=0; i<=361; ++i){
      glVertex2f(p.X()+R0*cos(i*M_PI/180.0f), p.Y()+R0*sin(i*M_PI/180.0f));
    }
  glEnd();
}

void Display::drawTriangle(Triangle2d t, Color clr, bool draw_solid, bool draw_outline) {
    if (draw_solid) {
        glColor4fv(clr.rgba);
        glBegin(GL_TRIANGLES);
            glVertex2f( t.point1().X(), t.point1().Y() );
            glVertex2f( t.point2().X(), t.point2().Y() );
            glVertex2f( t.point3().X(), t.point3().Y() );
        glEnd();
    }
    if (draw_outline) {
        glColor4fv(Color(0,0,0,1).rgba);
        glLineWidth(1.0);
        glBegin(GL_LINE_STRIP);
          glVertex2f( t.point1().X(), t.point1().Y() );
          glVertex2f( t.point2().X(), t.point2().Y() );
          glVertex2f( t.point3().X(), t.point3().Y() );
          glVertex2f( t.point1().X(), t.point1().Y() );
        glEnd();
        glLineWidth(1.0);
    }
}


/*
 * DRAW Cube
 */
void Display::drawCube(double length) {
    glBegin (GL_LINE_LOOP);
        glVertex3f(length, length, 0);
        glVertex3f(-length,length,0);
        glVertex3f(-length,-length,0);
        glVertex3f(length,-length,0);
    glEnd();
    glBegin (GL_LINE_LOOP);
        glVertex3f(length, length, length);
        glVertex3f(-length, length, length);
        glVertex3f(-length, -length, length);
        glVertex3f(length, -length, length);
    glEnd();
    glBegin (GL_LINES);
        glVertex3f(length,length,0);
        glVertex3f(length,length,length);
        glVertex3f(-length,length,0);
        glVertex3f(-length,length,length);
        glVertex3f(-length,-length,0);
        glVertex3f(-length,-length,length);
        glVertex3f(length,-length,0);
        glVertex3f(length,-length,length);
    glEnd();
}

