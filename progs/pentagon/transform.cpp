

#include "CORE.h"

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2)          \
            dest[0]=v1[0]-v2[0]; \
            dest[1]=v1[1]-v2[1]; \
            dest[2]=v1[2]-v2[2]; 

#define ADD(dest,v1,v2)          \
            dest[0]=v1[0]+v2[0]; \
            dest[1]=v1[1]+v2[1]; \
            dest[2]=v1[2]+v2[2]; 

void transform( double in[3], double *out )
{
 // random transform matrix and translation vector
#if CORE_LEVEL >= 2
    double offset[3] = {"1.2", "3.4", "5.6" };
    double T0[3] = {"1.234", "7.777", "1.111" };
    double T1[3] = {"4.0", "5.0", "3.0" };
    double T2[3] = {"1.11", "8.88", "9.99" };
#else
    double offset[3] = {1.2, 3.4, 5.6 };
    double T0[3] = {1.234, 7.777, 1.111 };
    double T1[3] = {4.0, 5.0, 3.0 };
    double T2[3] = {1.11, 8.88, 9.99};
#endif

    double point[3];
    ADD(point, in, offset);
    out[0] = DOT(T0, point);
    out[1] = DOT(T1, point);
    out[2] = DOT(T2, point);
}    

