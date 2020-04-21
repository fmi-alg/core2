#ifndef POSE
#define POSE

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

class Pose {
public:
    GLfloat x, y, theta;

    Pose(float xx, float yy, float tt):x(xx), y(yy), theta(tt){}
    Pose(float xx, float yy):x(xx), y(yy), theta(0){}
};

#endif // POSE

