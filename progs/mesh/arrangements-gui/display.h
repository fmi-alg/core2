#ifndef MK_DISPLAY_H_
#define MK_DISPLAY_H_

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include "glui.h"
#endif

#include <utility>
#include <vector>

#include "mk-defs.h"
#include "box.h"
#include "cxy.h"

using namespace std;

namespace display_funcs
{

//Postscript
//void renderScenePS();
//void treeTraversePS(SimplePSinC& PS, Box* b);
//void drawVorPS(SimplePSinC& PS, Box* b);
//void drawQuadPS(SimplePSinC& PS, Box* b);
//void drawWallsPS(SimplePSinC& PS, Box* b);

//GUI related functions
void updateVARinfo();
void updateSelectedBoxInfo();
void reset_move();

struct program_params_t
{
  program_params_t()
  {
      x_min = -3;
      x_max = 3;
      y_min = -3;
      y_max = 3;
      min_size = 0.0001;
      max_size = 0.01;

      max_ambiguous_box=20;
      max_gen = 15;

      b0=NULL;
      deltaX_Render=0;            // Translate display in x-direction
      deltaY_Render=0;            // Translate display in y-direction
      uscale_Render=1;            // Scale the display

      windowWidth=600;            // Window width
      windowHeight=600;           // Window height
      windowPosX = 20;           // X Position of Window
      windowPosY = 20;           // Y Position of Window

      showBoxBoundary = 1;  //draw box boundary
      showF=1;              //draw function F
      showG=1;              //draw function G
      showUnitCircle=1;         //draw the unit cirle
      showAxis=1;               //draw x&y axis
      showOffsetAxis=0;         //draw x=1&y=1 axis

      timeused=0;

      sel_circle=true;              //show the circle of selected box
      sel_features=true;            //show the features of selected box
      sel_wall_bisectors=false;     //show the wall bisectors of selected box (if possible)
      sel_corner_bisectors=false;   //show the corner bisectors of selected box (if possible)
      sel_parabola=false;           //show the parabola of wall/corner pairs in the features

      sel_x=sel_y=0;
  }

  string fxy_str;
  string gxy_str;

  int max_gen;
  unsigned int max_ambiguous_box; // This is the maximum number of ambiguous boxes to be printed

  //DT x_min, x_max, y_min, y_max;
  DT min_size;
  DT max_size;

  const Box * b0;
  vector<const Box *> n_it;
  vector<const Box *> outer_it;
  vector<const Box *> inner_it;
  vector<const Box *> amb;

  DT x_min, x_max, y_min, y_max;
  float scale;
  float gui_dXY[2];

  list<Box*> g_selected_PM;

  machine_double timeused;

  //
  float deltaX_Render;            // Translate display in x-direction
  float deltaY_Render;            // Translate display in y-direction
  float uscale_Render;            // Scale the display

  int windowWidth;            // Window width
  int windowHeight;           // Window height
  int windowPosX;             // X Position of Window
  int windowPosY;             // Y Position of Window

  int showBoxBoundary;        //draw box boundary
  int showF;                  //draw curve: F
  int showG;                  //draw curve: G
  int showUnitCircle;         //draw the unit cirle
  int showAxis;               //draw x&y axis
  int showOffsetAxis;         //draw x=1&y=1 axis

  int sel_circle;             //show the circle of selected box
  int sel_features;           //show the features of selected box
  int sel_wall_bisectors;     //show the wall bisectors of selected box (if possible)
  int sel_corner_bisectors;   //show the corner bisectors of selected box (if possible)
  int sel_parabola;           //show the parabola of wall/corner pairs in the features

  //last point user clicked
  machine_double sel_x;
  machine_double sel_y;

  //line segments
  vector<pair<cxy::Point *, cxy::Point *> > func_F_cxy_line_segs;
  vector<pair<cxy::Point *, cxy::Point *> > func_G_cxy_line_segs;
};

extern program_params_t PROG_PARAMS;

//create GLUI
void CreateGUI();

// Clear the background for display.
void ClearBackground();

// Call back when the window canvas is reshaped.
void ReshapeHandler(GLsizei w,GLsizei h);

// Called when a key is pressed in the context of the opengl window
// used to exit when escape is pressed.
void KeyHandler(const unsigned char key, const int x, const int y);

// The main display routine.
void DisplayHandler();

//initialize openGL states
bool InitGL();

//GL

void drawCircle(float radius);

//void renderScene(void);
void drawCircle( double Radius, int numPoints, const Point2d& o, double r, double g, double b);

void filledCircle( double radius, int numPoints, const Point2d& o, double r, double g, double b);

void drawQuad(const Box* b);

void drawQuad_selected(list<Box*> boxes);

void updateVARinfo();

//void Keyboard( unsigned char key, int x, int y );

void SpecialKey( int key, int x, int y );

void Mouse(int button, int state, int x, int y);

//draw the entire tree
void treeTraverse(Box* b);

//convex a list of BoxT<NT> to a list of Box
template <typename NT>
void ConvertList(const vector<const BoxT<NT> *> *input, vector<const Box *> *output)
{

  for (unsigned int i = 0; i < input->size(); ++i)
  {
    const BoxT<NT> *box = (*input)[i];
    output->push_back(new Box(
        0,
        IntervalT<NT>(
            box->x_range.getL().doubleValue(),
            box->x_range.getR().doubleValue()),
        IntervalT<NT>(
            box->y_range.getL().doubleValue(),
            box->y_range.getR().doubleValue())));
    //delete box;
  }
  // input->clear();
}


// Set the display parameters. This HAS to be called before
// the call to glutMainLoop.
template <typename NT>
void SetDisplayParams(const BoxT<NT> *B0,
                      const vector<const BoxT<NT> *> *non_intersect,  // boxes the curve intersects
                      const vector<const BoxT<NT> *> *outer_intersects,  
                      const vector<const BoxT<NT> *> *inner_intersects, 
                      const vector<const BoxT<NT> *> *ambiguous,
                      program_params_t & prog_params = PROG_PARAMS) // lines to draw.
{

  //PROG_PARAMS.b0 = B0;

  ConvertList<NT>(non_intersect, &prog_params.n_it);
  ConvertList<NT>(outer_intersects, &prog_params.outer_it);
  ConvertList<NT>(inner_intersects, &prog_params.inner_it);
  ConvertList<NT>(ambiguous, &prog_params.amb);

  // v1 , v2, v3, v4
  machine_double p1 = B0->x_range.getL().doubleValue(),
       p2 = B0->x_range.getR().doubleValue(),
       p3 = B0->y_range.getL().doubleValue(),
       p4 = B0->y_range.getR().doubleValue();

  prog_params.x_min = p1;
  prog_params.x_max = p2;
  prog_params.y_min = p3;
  prog_params.y_max = p4;
  prog_params.scale = 1.1;
  //prog_params.x_delta = 0.0;
  //prog_params.y_delta = 0.0;
}

}//end of namespace

#endif   // CXY2_DISPLAY_H_
