#ifndef TRIANGLE
#define TRIANGLE

#include "Pose.h"

class Triangle {
public:
    Pose a, b, c;

    Triangle(Pose aa, Pose bb, Pose cc):a(aa), b(bb), c(cc){}
};

#endif // TRIANGLE

