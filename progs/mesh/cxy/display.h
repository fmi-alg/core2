/* **************************************
   File: display.h

   Description: 
	Display code factored out from cxy.cpp.

   HISTORY: 2008: Long Lin (Java Code for cxy paper)
	    Apr, 2009: Shuxing Lu (converted Java code to Core2)
	    Jun, 2010: Narayan Kamath (refactored code into independent units)

   Since Core Library  Version 2.0
   $Id: display.h,v 1.3 2010/06/15 22:24:58 exact Exp $
 ************************************** */

#ifndef CXY2_DISPLAY_H_
#define CXY2_DISPLAY_H_

#include <GL/glut.h>
#include <utility>
#include <vector>

#include "cxy-defs.h"

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
                      const bool draw_boxes,
                      const vector<Box *> *non_intersect,  // boxes the curve intersects
                      const vector<Box *> *intersects,  // boxes the curve does not intersect
                      const vector<Box *> *unresolved,
                      const vector<pair<Point *, Point *> > *lines); // lines to draw.

}

#endif   // CXY2_DISPLAY_H_
