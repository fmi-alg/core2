#ifndef CEVAL_DISPLAY_H_
#define CEVAL_DISPLAY_H_

#include <GL/glut.h>
#include <utility>
#include <vector>

#include "ceval-defs.h"


class Point;
class Box;
using namespace std;

namespace display_funcs {

// Clear the background for display.
void ClearBackground();
// Call back when the window canvas is reshaped.
void ReshapeHandler(GLsizei w,GLsizei h);
// Called when a key is pressed in the context of the opengl window
// used to exit when escape is pressed.
void KeyHandler(const unsigned char key, const int x, const int y);
// The main display routine.
void DisplayHandler();
// Set the display parameters. This HAS to be called before
// the call to glutMainLoop.
void SetDisplayParams(const Box *B0,
                      const vector<const Box *> *non_intersect,  // boxes the curve intersects
                      const vector<const Box *> *intersects,  // boxes the curve does not intersect
                      const vector<const Box *> *ambiguous); // lines to draw.

}

#endif   // CEVAL_DISPLAY_H_
