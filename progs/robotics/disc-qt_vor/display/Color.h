#ifndef COLOR
#define COLOR

#include "gl.h"

class Color {
public:
    GLfloat rgba[4];

    Color(float r, float g, float b, float a){
        rgba[0] = r;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = a;
    }

    Color(float r, float g, float b){
        rgba[0] = r;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = 1.0f;
    }
};


#endif // COLOR

