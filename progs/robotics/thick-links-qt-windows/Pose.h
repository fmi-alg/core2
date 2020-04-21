#ifndef POSE
#define POSE

#include "GL/gl.h"

class Pose {
public:
    GLfloat x, y, t1, t2;

    Pose(float xx, float yy, float tt1, float tt2):x(xx), y(yy), t1(tt1), t2(tt2){}
    Pose(float xx, float yy):x(xx), y(yy), t1(0), t2(0){}
    Pose():x(0), y(0), t1(0), t2(0){}
};

#endif // POSE

