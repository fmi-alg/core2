/*
 *  Color.h
 *  
 *  Author: Ching-Hsiang (Tom) Hsu
 *  Email: chhsu@nyu.edu
 *  Modified: Ching-Hsiang (Tom) Hsu, Oct. 31, 2016
 */

#ifndef COLOR
#define COLOR

#include "gl.h"

class Color {
public:
    GLfloat rgba[4];
    GLfloat rgb[3];

    Color(float r, float g, float b, float a){
        rgb[0] = rgba[0] = r;
        rgb[1] = rgba[1] = g;
        rgb[2] = rgba[2] = b;
        rgba[3] = a;
    }

    Color(float r, float g, float b){
        rgb[0] = rgba[0] = r;
        rgb[1] = rgba[1] = g;
        rgb[2] = rgba[2] = b;
        rgba[3] = 1.0f;
    }
};


#endif // COLOR

