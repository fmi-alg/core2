/*
 * color.h
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef COLOR
#define COLOR

#include "gl.h"

class Color {
 public:
  GLfloat rgba[4];

  Color(float r, float g, float b, float a) {
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
  }

  Color(float r, float g, float b) {
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = 1.0f;
  }
};

#endif  // COLOR
