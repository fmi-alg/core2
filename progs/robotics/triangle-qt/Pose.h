#ifndef POSE
#define POSE

#include "gl.h"

class Pose {
public:
    GLfloat x, y, theta;

    Pose(float xx, float yy, float tt):x(xx), y(yy), theta(tt){}
    Pose(float xx, float yy):x(xx), y(yy), theta(0){}
};

#endif // POSE

