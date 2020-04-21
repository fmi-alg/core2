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
      case 'q':
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
      case 'o': // zoom out
        DISPLAY_PARAMS_INSTANCE.scale /= 0.9;
        //DISPLAY_PARAMS_INSTANCE.x_delta = 0;
        //DISPLAY_PARAMS_INSTANCE.y_delta = 0;
        break;
      case 'k':
      case 'i': // zoom in
        DISPLAY_PARAMS_INSTANCE.scale *= 0.9;
        //DISPLAY_PARAMS_INSTANCE.x_delta = 0;
        //DISPLAY_PARAMS_INSTANCE.y_delta = 0;
        break;
      case 'r': // reset
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

// Chee(Mar'12): drawing a circle:
const float DEG2RAD = 3.14159/180;
void drawCircle(float radius) {
   glBegin(GL_LINE_LOOP);
   	for (int i=0; i < 360; i++) {
      	   float degInRad = i*DEG2RAD;
      	   glVertex2f(cos(degInRad)*radius,sin(degInRad)*radius);
   	}
   glEnd();
}//drawCircle

// The main display routine.
void DisplayHandler() {
  glClear(GL_COLOR_BUFFER_BIT);

  glLineWidth(1.5f); 		// thick line!
  glColor3f(1.0f,0.0f,0.0f);	// red
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

  glColor3f(0.0f,1.0f,0.0f);	//green
  it = DISPLAY_PARAMS_INSTANCE.outer_it.begin();
  //glLineWidth(5.0f);
  while (it != DISPLAY_PARAMS_INSTANCE.outer_it.end()) {
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


  glColor3f(0.0f,0.0f,0.0f);	//black
  it = DISPLAY_PARAMS_INSTANCE.inner_it.begin();
  //glLineWidth(5.0f);
  while (it != DISPLAY_PARAMS_INSTANCE.inner_it.end()) {
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

  glColor3f(0.0f,0.0f,1.0f);	//blue
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

  // Chee(Mar'12): Draw the X- and Y-axes!  
	  glLineWidth(2.0f); 		// thick line!
	  glColor3f(0.0f, 0.2f, 0.8f);	// bluish line
	  glBegin(GL_LINES);
	  vertex2fWrapper( DISPLAY_PARAMS_INSTANCE.x_min, 0.0f); // X-axis
	  vertex2fWrapper( DISPLAY_PARAMS_INSTANCE.x_max, 0.0f); // X-axis
	  vertex2fWrapper( 0.0f, DISPLAY_PARAMS_INSTANCE.y_min); // Y-axis
	  vertex2fWrapper( 0.0f, DISPLAY_PARAMS_INSTANCE.y_max); // Y-axis
	  glEnd();
	// Draw circle around origin:
	  drawCircle(1.0);

// Chee(Mar'12): Draw the line at unit distance from the axes, to inidicate scale!
	  glLineWidth(2.0f); 		// thick line!
	  glColor3f(0.0f, 0.8f, 0.8f);	// bluish-green 
	  glBegin(GL_LINES);
	  vertex2fWrapper( DISPLAY_PARAMS_INSTANCE.x_min, 1.0f); // X-axis
	  vertex2fWrapper( DISPLAY_PARAMS_INSTANCE.x_max, 1.0f); // X-axis
	  vertex2fWrapper( 1.0f, DISPLAY_PARAMS_INSTANCE.y_min); // Y-axis
	  vertex2fWrapper( 1.0f, DISPLAY_PARAMS_INSTANCE.y_max); // Y-axis
	  glEnd();

  glLineWidth(1.0f);

  glFlush();
}

} // namespace display_funcs.
