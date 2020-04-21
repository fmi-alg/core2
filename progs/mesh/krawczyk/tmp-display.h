#ifndef KRAWCZYK_DISPLAY_H_
#define KRAWCZYK_DISPLAY_H_

#include <GL/glut.h>
#include <utility>
#include <vector>

#include "krawczyk-defs.h"
#include "box.h"

using namespace std;

namespace display_funcs {

struct display_params_t {
  const BoxT<DoubleWrapper> *b0;
  vector<const BoxT<DoubleWrapper> *> n_it;
  vector<const BoxT<DoubleWrapper> *> it;
  vector<const BoxT<DoubleWrapper> *> amb;

  machine_double x_min, x_max, y_min, y_max;
  machine_double scale;
  machine_double x_delta, y_delta;
};

// Clear the background for display.
void ClearBackground();
// Call back when the window canvas is reshaped.
void ReshapeHandler(GLsizei w,GLsizei h);
// Called when a key is pressed in the context of the opengl window
// used to exit when escape is pressed.
void KeyHandler(const unsigned char key, const int x, const int y);
// The main display routine.
void DisplayHandler();

template <typename NT>
void ConvertList(const vector<const BoxT<NT> *> *input,
    vector<const BoxT<DoubleWrapper> *> *output) {
  for (unsigned int i = 0; i < input->size(); ++i) {
    const BoxT<NT> *box = (*input)[i];
    output->push_back(new BoxT<DoubleWrapper>(
        0,
        IntervalT<DoubleWrapper>(
            box->x_range.getL().doubleValue(),
            box->x_range.getR().doubleValue()),
        IntervalT<DoubleWrapper>(
            box->y_range.getL().doubleValue(),
            box->y_range.getR().doubleValue())));
    delete box;
  }
  // input->clear();
}

extern display_params_t DISPLAY_PARAMS_INSTANCE;

// Set the display parameters. This HAS to be called before
// the call to glutMainLoop.
template <typename NT>
void SetDisplayParams(const BoxT<NT> *B0,
                      const vector<const BoxT<NT> *> *non_intersect,  // boxes the curve intersects
                      const vector<const BoxT<NT> *> *intersects,  // boxes the curve does not intersect
                      const vector<const BoxT<NT> *> *ambiguous,
                      display_params_t &display_params = DISPLAY_PARAMS_INSTANCE) // lines to draw.
{
  //  display_params.b0 = B0;

  ConvertList<NT>(non_intersect, &display_params.n_it);
  ConvertList<NT>(intersects, &display_params.it);
  ConvertList<NT>(ambiguous, &display_params.amb);

  // v1 , v2, v3, v4
  machine_double p1 = B0->x_range.getL().doubleValue(),
       p2 = B0->x_range.getR().doubleValue(),
       p3 = B0->y_range.getL().doubleValue(),
       p4 = B0->y_range.getR().doubleValue();

  display_params.x_min = p1;
  display_params.x_max = p2;
  display_params.y_min = p3;
  display_params.y_max = p4;
  display_params.scale = 1.0;
  display_params.x_delta = 0.0;
  display_params.y_delta = 0.0;
}

}

#endif   // CXY2_DISPLAY_H_
