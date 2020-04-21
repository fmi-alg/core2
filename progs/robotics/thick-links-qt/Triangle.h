/*
 *  Triangle.h
 *  
 *  Author: Ching-Hsiang (Tom) Hsu
 *  Email: chhsu@nyu.edu
 *  Modified: Ching-Hsiang (Tom) Hsu, Oct. 31, 2016
 */

#ifndef TRIANGLE
#define TRIANGLE

#include "Pose.h"

class Triangle {
public:
    Pose a, b, c;

    Triangle(Pose aa, Pose bb, Pose cc):a(aa), b(bb), c(cc){}
};

#endif // TRIANGLE

