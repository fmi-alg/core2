/* **************************************
   File: display.cpp

   Description: 
	Display code factored out from cxy.cpp.


   HISTORY: 2008: Long Lin (Java Code for cxy paper)
	    Apr, 2009: Shuxing Lu (converted Java code to Core2)
	    Jun, 2010: Narayan Kamath (refactored code into independent units)

   Since Core Library  Version 2.0
   $Id: display.cpp,v 1.7 2011/02/09 22:16:07 exact Exp $
 ************************************** */

#include <utility>
#include <vector>

#include "display.h"

#include "box.h"
#include "point-inl.h"

namespace display_funcs {

struct display_params_t {
  const Box *b0;
  bool draw_boxes;
  const vector<Box *> *n_it;
  const vector<Box *> *it;
  const vector<Box *> *unresolved;
  const vector<pair<Point *, Point *> > *lines;

  machine_double x_min, x_max, y_min, y_max;
  machine_double scale;
  machine_double x_delta, y_delta;
} display_params;

void SetDisplayParams(const Box *B0,
                      const bool draw_boxes,
                      const vector<Box *> *non_intersect,
                      const vector<Box *> *intersect,
                      const vector<Box *> *unresolved,
                      const vector<pair<Point *, Point *> > *lines) {
  display_params.b0 = B0;
  display_params.draw_boxes = draw_boxes;
  display_params.n_it = non_intersect;
  display_params.it = intersect;
  display_params.unresolved = unresolved;
  display_params.lines = lines;

  const vector<Point *> *corners = B0->getCorners();
  // v1 , v2, v3, v4
  double p1 = corners->at(0)->getX(), p2 = corners->at(2)->getX(),
       p3 = corners->at(2)->getY(), p4 = corners->at(0)->getY();
  display_params.x_min = p1.doubleValue();
  display_params.x_max = p2.doubleValue();
  display_params.y_min = p3.doubleValue();
  display_params.y_max = p4.doubleValue();
  display_params.scale = 1.0;
  display_params.x_delta = 0.0;
  display_params.y_delta = 0.0;
}

// Clear the background for display.
void ClearBackground() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  const machine_double scale = display_params.scale;
  const machine_double x_delta = display_params.x_delta;
  const machine_double y_delta = display_params.y_delta;
  gluOrtho2D((display_params.x_min)*scale + x_delta,
             (display_params.x_max)*scale + x_delta,
             (display_params.y_min)*scale + y_delta,
             (display_params.y_max)*scale + y_delta);
  /*
  gluOrtho2D((display_params.x_min)*scale + x_delta,
             (display_params.x_max)*scale + x_delta,
             (display_params.y_min)*scale + y_delta,
             (display_params.y_max)*scale + y_delta);
  gluOrtho2D((display_params.x_min + x_delta)*scale,
             (display_params.x_max + x_delta)*scale,
             (display_params.y_min + y_delta)*scale,
             (display_params.y_max + y_delta)*scale);
*/
  glMatrixMode(GL_MODELVIEW);
  glClearColor(255.0/256.0,192.0/256.0,203.0/256.0,0.0);
}

// Call back when the window canvas is reshaped.
void ReshapeHandler(const GLsizei w,const GLsizei h) {
  glViewport(0,0,w,h);

  ClearBackground();
}

// Called when a key is pressed in the context of the opengl window
// used to exit when escape is pressed.
void KeyHandler(const unsigned char key, const int x, const int y) {
  switch (key)
  {
    case 27:    // ESCAPE key
    case 'q':	// quit
      exit(0);
      break;
    case 'w': // pan up
      display_params.y_delta+=0.5*display_params.scale;
      break;
    case 'a': // pan left
      display_params.x_delta-=0.5*display_params.scale;
      break;
    case 's': // pan down
      display_params.y_delta-=0.5*display_params.scale;
      break;
    case 'd': // pan right
      display_params.x_delta+=0.5*display_params.scale;
      break;
    case 'i':  // zoom in
      display_params.scale *= 0.9;
      //display_params.x_delta = 0;
      //display_params.y_delta = 0;
      break;
    case 'k':  // zoom out
    case 'o':  // zoom out
      display_params.scale /= 0.9;
      //display_params.x_delta = 0;
      //display_params.y_delta = 0;
      break;
    case ' ':   // space bar for reset
      display_params.scale = 1;
      display_params.x_delta = 0;
      display_params.y_delta = 0;
  }

  ClearBackground();
  DisplayHandler();
}

inline void vertex2fWrapper(const double &x, const double &y) {
  glVertex2f(x.doubleValue(), y.doubleValue());
}

// The main display routine.
void DisplayHandler() {
  glClear(GL_COLOR_BUFFER_BIT);

  if (display_params.draw_boxes) {
    //glColor3f(1.0f,0.0f,0.0f);

    vector<Box *>::const_iterator it = display_params.n_it->begin();
    // The old mapping for the sake of references
    // v1=a.getX(); v2=b.getX(); v3=a.getY(); v4=b.getY();
    while (it != display_params.n_it->end()) {
      const vector<Point *> &vs = *((*it)->getCorners());
      if ((*it)->getMark(Box::OUT_BOX_NEG)) {
    	  glColor3f(0.0f,1.0f,1.0f);
      } else {
    	  glColor3f(1.0f,0.0f,0.0f);
      }
      glBegin(GL_LINE_LOOP);
      // v1 - v4
      vertex2fWrapper(vs[0]->getX(), vs[0]->getY());
      // v1 - v3
      vertex2fWrapper(vs[0]->getX(), vs[2]->getY());
      // v2 - v3
      vertex2fWrapper(vs[2]->getX(), vs[2]->getY());
      // v2 - v4
      vertex2fWrapper(vs[2]->getX(), vs[0]->getY());
      glEnd();
      ++it;
    }

    glColor3f(0.0f,1.0f,0.0f);
    it = display_params.it->begin();
    while (it != display_params.it->end()) {
      const vector<Point *> &vs = *((*it)->getCorners());
      glBegin(GL_LINE_LOOP);
      // v1 - v4
      vertex2fWrapper(vs[0]->getX(), vs[0]->getY());
      // v1 - v3
      vertex2fWrapper(vs[0]->getX(), vs[2]->getY());
      // v2 - v3
      vertex2fWrapper(vs[2]->getX(), vs[2]->getY());
      // v2 - v4
      vertex2fWrapper(vs[2]->getX(), vs[0]->getY());
      glEnd();
      ++it;
    }

    glColor3f(0.0f,0.0f,1.0f);
    it = display_params.unresolved->begin();
    while (it != display_params.unresolved->end()) {
      const vector<Point *> &vs = *((*it)->getCorners());
      glBegin(GL_LINE_LOOP);
      // v1 - v4
      vertex2fWrapper(vs[0]->getX(), vs[0]->getY());
      // v1 - v3
      vertex2fWrapper(vs[0]->getX(), vs[2]->getY());
      // v2 - v3
      vertex2fWrapper(vs[2]->getX(), vs[2]->getY());
      // v2 - v4
      vertex2fWrapper(vs[2]->getX(), vs[0]->getY());
      glEnd();
      ++it;
    }
  }

  glColor3f(0.0,0.0,0.0);
  vector<pair<Point *, Point *> >::const_iterator it = display_params.lines->begin();
  glBegin(GL_LINES);
  while (it != display_params.lines->end()) {
    const pair<Point *, Point *> &line = *it;
    vertex2fWrapper(line.first->getX(), line.first->getY());
    vertex2fWrapper(line.second->getX(), line.second->getY());
    ++it;
  }
  glEnd();

  glFlush();
}

} // namespace display_funcs.
