#include <utility>
#include <vector>

#include "tmp-display.h"


namespace display_funcs {

display_params_t DISPLAY_PARAMS_INSTANCE;

// Clear the background for display.
void ClearBackground() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  const machine_double scale = DISPLAY_PARAMS_INSTANCE.scale;
  const machine_double x_delta = DISPLAY_PARAMS_INSTANCE.x_delta;
  const machine_double y_delta = DISPLAY_PARAMS_INSTANCE.y_delta;
  gluOrtho2D((DISPLAY_PARAMS_INSTANCE.x_min)*scale + x_delta,
             (DISPLAY_PARAMS_INSTANCE.x_max)*scale + x_delta,
             (DISPLAY_PARAMS_INSTANCE.y_min)*scale + y_delta,
             (DISPLAY_PARAMS_INSTANCE.y_max)*scale + y_delta);
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
      case 'w':
        DISPLAY_PARAMS_INSTANCE.y_delta+=0.5*DISPLAY_PARAMS_INSTANCE.scale;
        break;
      case 'a':
        DISPLAY_PARAMS_INSTANCE.x_delta-=0.5*DISPLAY_PARAMS_INSTANCE.scale;
        break;
      case 's':
        DISPLAY_PARAMS_INSTANCE.y_delta-=0.5*DISPLAY_PARAMS_INSTANCE.scale;
        break;
      case 'd':
        DISPLAY_PARAMS_INSTANCE.x_delta+=0.5*DISPLAY_PARAMS_INSTANCE.scale;
        break;
      case 'i':  // ZOOM IN
        DISPLAY_PARAMS_INSTANCE.scale *= 0.9;
        //DISPLAY_PARAMS_INSTANCE.x_delta = 0;
        //DISPLAY_PARAMS_INSTANCE.y_delta = 0;
        break;
      case 'k':
      case 'o':  // ZOOM OUT
        DISPLAY_PARAMS_INSTANCE.scale /= 0.9;
        //DISPLAY_PARAMS_INSTANCE.x_delta = 0;
        //DISPLAY_PARAMS_INSTANCE.y_delta = 0;
        break;
      case 'r':    // RESET!
      case ' ':
        DISPLAY_PARAMS_INSTANCE.scale = 1;
        DISPLAY_PARAMS_INSTANCE.x_delta = 0;
        DISPLAY_PARAMS_INSTANCE.y_delta = 0;
    }

    ClearBackground();
    DisplayHandler();
}

inline void vertex2fWrapper(const double &x, const double &y) {
  glVertex2f(x, y);
}

// The main display routine.
void DisplayHandler() {
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1.0f,0.0f,0.0f);

  vector<const BoxT<DoubleWrapper> *>::const_iterator it = DISPLAY_PARAMS_INSTANCE.n_it.begin();


  // The old mapping for the sake of references
  // v1=a.getX(); v2=b.getX(); v3=a.getY(); v4=b.getY();
  while (it != DISPLAY_PARAMS_INSTANCE.n_it.end()) {
    const double &x_min = (*it)->x_range.getL().doubleValue();
    const double &x_max = (*it)->x_range.getR().doubleValue();
    const double &y_min = (*it)->y_range.getL().doubleValue();
    const double &y_max = (*it)->y_range.getR().doubleValue();

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

  glColor3f(0.0f,1.0f,0.0f);
  it = DISPLAY_PARAMS_INSTANCE.it.begin();
  //glLineWidth(5.0f);
  while (it != DISPLAY_PARAMS_INSTANCE.it.end()) {
    const double &x_min = (*it)->x_range.getL().doubleValue();
    const double &x_max = (*it)->x_range.getR().doubleValue();
    const double &y_min = (*it)->y_range.getL().doubleValue();
    const double &y_max = (*it)->y_range.getR().doubleValue();
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

  glColor3f(0.0f,0.0f,1.0f);
  it = DISPLAY_PARAMS_INSTANCE.amb.begin();
  while (it != DISPLAY_PARAMS_INSTANCE.amb.end()) {
    const double &x_min = (*it)->x_range.getL().doubleValue();
    const double &x_max = (*it)->x_range.getR().doubleValue();
    const double &y_min = (*it)->y_range.getL().doubleValue();
    const double &y_max = (*it)->y_range.getR().doubleValue();
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
  glLineWidth(1.0f);

  glFlush();
}

} // namespace display_funcs.
