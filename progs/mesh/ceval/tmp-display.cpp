#include <utility>
#include <vector>

#include "tmp-display.h"
#include "algorithm.h"

namespace display_funcs {

struct display_params_t {
  const Box *b0;
  const vector<const Box *> *n_it;
  const vector<const Box *> *it;
  const vector<const Box *> *amb;

  machine_double x_min, x_max, y_min, y_max;
  machine_double scale;
  machine_double x_delta, y_delta;
  machine_double accel;	// translation factor
  machine_double scalefactor; // zooming factor
} display_params;

void SetDisplayParams(const Box *B0,
                      const vector<const Box *> *non_intersect,
                      const vector<const Box *> *intersect,
                      const vector<const Box *> *ambiguous) {
  display_params.b0 = B0;
  display_params.n_it = non_intersect;
  display_params.it = intersect;
  display_params.amb = ambiguous;

  display_params.x_min = B0->min_.re().doubleValue();
  display_params.x_max = B0->max_.re().doubleValue();
  display_params.y_min = B0->min_.im().doubleValue();
  display_params.y_max = B0->max_.im().doubleValue();
  display_params.scale = 1.0;
  display_params.x_delta = 0.0;
  display_params.y_delta = 0.0;
  display_params.accel = 1.0;
  display_params.scalefactor = 1.0;
}

// Clear the background for display.
void ClearBackground() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  const machine_double scale = display_params.scale;
  const machine_double x_delta = display_params.x_delta;
  const machine_double y_delta = display_params.y_delta;
  // Chee(Feb2011) Made the center of the display invariant under zooming:
  gluOrtho2D((display_params.x_min)*scale + x_delta,
             (display_params.x_max)*scale + x_delta,
             (display_params.y_min)*scale + y_delta,
             (display_params.y_max)*scale + y_delta);
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
      case 27:             // ESCAPE key
        exit(0);
        break;
      case 'F':
	display_params.scalefactor *= 1.1;
	break;
      case 'f':
	display_params.scalefactor /= 1.1;
	break;
      case 'C':		// increase acceleration?
	display_params.accel *= 1.1;
	break;
      case 'c':		// decrease acceleration?
	display_params.accel /= 1.1;
	break;
      case 'w':
        display_params.y_delta+=0.5*display_params.accel;
        break;
      case 'a':
        display_params.x_delta-=0.5*display_params.accel;
        break;
      case 's':
        display_params.y_delta-=0.5*display_params.accel;
        break;
      case 'd':
        display_params.x_delta+=0.5*display_params.accel;
        break;
      case 'o':
      case 'k':
        display_params.scale /= 0.9*display_params.scalefactor;
        //display_params.x_delta = 0
        //display_params.y_delta = 0;
        break;
      case 'i':
        display_params.scale *= 0.9*display_params.scalefactor;
        //display_params.x_delta = 0;
        //display_params.y_delta = 0;
        break;
      case ' ':
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

  glColor3f(1.0f,0.0f,0.0f);	// red color boxes (excluded boxes)

  vector<const Box *>::const_iterator it = display_params.n_it->begin();

  // The old mapping for the sake of references
  // v1=a.getX(); v2=b.getX(); v3=a.getY(); v4=b.getY();
  while (it != display_params.n_it->end()) {
    const double &x_min = (*it)->min_.re();
    const double &x_max = (*it)->max_.re();
    const double &y_min = (*it)->min_.im();
    const double &y_max = (*it)->max_.im();

    glBegin(GL_LINES);
    vertex2fWrapper(x_min, y_min);
    vertex2fWrapper(x_min, y_max);

    vertex2fWrapper(x_min, y_max);
    vertex2fWrapper(x_max, y_max);

    vertex2fWrapper(x_max, y_max);
    vertex2fWrapper(x_max, y_min);

    vertex2fWrapper(x_max, y_min);
    vertex2fWrapper(x_min, y_min);
    glEnd();
    ++it;
  }

  glColor3f(0.0f,1.0f,0.0f);	//green (isolated a root)
  it = display_params.it->begin();
  while (it != display_params.it->end()) {
    const double &x_min = (*it)->min_.re();
    const double &x_max = (*it)->max_.re();
    const double &y_min = (*it)->min_.im();
    const double &y_max = (*it)->max_.im();

    glBegin(GL_LINES);
    vertex2fWrapper(x_min, y_min);
    vertex2fWrapper(x_min, y_max);

    vertex2fWrapper(x_min, y_max);
    vertex2fWrapper(x_max, y_max);

    vertex2fWrapper(x_max, y_max);
    vertex2fWrapper(x_max, y_min);
    vertex2fWrapper(x_max, y_min);
    vertex2fWrapper(x_min, y_min);
    glEnd();
    ++it;
  }

  glColor3f(0.0f,0.0f,1.0f);	//blue (ambiguous)
  it = display_params.amb->begin();
  while (it != display_params.amb->end()) {
    const double &x_min = (*it)->min_.re();
    const double &x_max = (*it)->max_.re();
    const double &y_min = (*it)->min_.im();
    const double &y_max = (*it)->max_.im();

    glBegin(GL_LINES);
    vertex2fWrapper(x_min, y_min);
    vertex2fWrapper(x_min, y_max);

    vertex2fWrapper(x_min, y_max);
    vertex2fWrapper(x_max, y_max);

    vertex2fWrapper(x_max, y_max);
    vertex2fWrapper(x_max, y_min);

    vertex2fWrapper(x_max, y_min);
    vertex2fWrapper(x_min, y_min);
    glEnd();
    ++it;
  }

  //double expand = 0.05 * display_params.scale;
  double expand = 0.4 * display_params.scale;
  glColor3f(1.0f,0.0f,1.0f);	//purple (ambiguous)
  it = display_params.amb->begin();
  while (it != display_params.amb->end()) {
    const double &x_min = (*it)->min_.re()-expand;
    const double &x_max = (*it)->max_.re()+expand;
    const double &y_min = (*it)->min_.im()-expand;
    const double &y_max = (*it)->max_.im()+expand;

    glBegin(GL_LINES);
    vertex2fWrapper(x_min, y_min);
    vertex2fWrapper(x_min, y_max);

    vertex2fWrapper(x_min, y_max);
    vertex2fWrapper(x_max, y_max);

    vertex2fWrapper(x_max, y_max);
    vertex2fWrapper(x_max, y_min);

    vertex2fWrapper(x_max, y_min);
    vertex2fWrapper(x_min, y_min);
    glEnd();
    ++it;
  }

  expand = 2 * display_params.scale;
  glColor3f(0.0f,0.0f,0.0f);	//black (ambiguous)
  it = display_params.amb->begin();
  while (it != display_params.amb->end()) {
    const double &x_min = (*it)->min_.re()-expand;
    const double &x_max = (*it)->max_.re()+expand;
    const double &y_min = (*it)->min_.im()-expand;
    const double &y_max = (*it)->max_.im()+expand;

    glBegin(GL_LINES);
    vertex2fWrapper(x_min, y_min);
    vertex2fWrapper(x_min, y_max);

    vertex2fWrapper(x_min, y_max);
    vertex2fWrapper(x_max, y_max);

    vertex2fWrapper(x_max, y_max);
    vertex2fWrapper(x_max, y_min);

    vertex2fWrapper(x_max, y_min);
    vertex2fWrapper(x_min, y_min);
    glEnd();
    ++it;
  }

  glFlush();
}

} // namespace display_funcs.
