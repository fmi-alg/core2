/*****************************************************************************/
/*                                                                           */
/*           F I S T :  Fast, Industrial-Strength Triangulation              */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/* (C)                            Martin Held                                */
/* (C)            Universitaet Salzburg, Salzburg, Austria                   */
/*                                                                           */
/* This code is not in the public domain. All rights reserved! Please make   */
/* sure to read the full copyright statement contained in api_functions.cpp. */
/*                                                                           */
/*****************************************************************************/

#ifndef _BASIC_H_
#define _BASIC_H_

#ifndef  RAND
#define  RND_MAX 2147483647

#define UniformRandom(x) \
{\
    x = ((machine_double) random()) / RND_MAX; }

#define RandomInteger(var, N) \
 (\
   assert(N > 0), \
   var = random(), \
   var - (var / (N)) * (N))
      
#define InitRandom(seed) \
{\
    srandom(seed); }

#else 

#ifdef RAND_MAX
#define RND_MAX RAND_MAX
#else
#define RND_MAX 32767
#endif

#define UniformRandom(x) \
{\
    x = ((machine_double) rand()) / RND_MAX; }

#define RandomInteger(var, N) \
 (\
   assert(N > 0), \
   var = rand(), \
   var - (var / (N)) * (N))

#define InitRandom(seed) \
{\
    srand(seed); }

#endif


#define RandomVertex(a) \
{\
   UniformRandom((a).x); \
   UniformRandom((a).y); \
   UniformRandom((a).z); }

   

#define DetExp(u_x, u_y, u_z, v_x, v_y, v_z, w_x, w_y, w_z) \
 ((u_x) * ((v_y) * (w_z)  -  (v_z) * (w_y))  - \
  (u_y) * ((v_x) * (w_z)  -  (v_z) * (w_x))  + \
  (u_z) * ((v_x) * (w_y)  -  (v_y) * (w_x)))


#define Det3D(u, v, w) \
 ((u).x * ((v).y * (w).z  -  (v).z * (w).y)  -  \
  (u).y * ((v).x * (w).z  -  (v).z * (w).x)  +  \
  (u).z * ((v).x * (w).y  -  (v).y * (w).x))


#define Det2D(u, v, w) \
 (((u).x - (v).x) * ((v).y - (w).y) + ((v).y - (u).y) * ((v).x - (w).x))


#define Length2D(u)  \
 (((u).x * (u).x) + ((u).y * (u).y))


#define Length2(u)  \
 (((u).x * (u).x) + ((u).y * (u).y) + ((u).z * (u).z))


#define Length_l1(u)  \
 (Abs((u).x) + Abs((u).y) + Abs((u).z))


#define Length_l2(u)  \
 sqrt(((u).x * (u).x) + ((u).y * (u).y) + ((u).z * (u).z))


#define DotProduct(u, v)  \
 (((u).x * (v).x) + ((u).y * (v).y) + ((u).z * (v).z))


#define DotProduct2D(u, v)  \
 (((u).x * (v).x) + ((u).y * (v).y))

#define DotProduct2DTMD(u, v)  \
 ((TO_MDOUBLE((u).x) * TO_MDOUBLE((v).x)) + (TO_MDOUBLE((u).y) * TO_MDOUBLE((v).y)))

#define VectorProduct(p, q, r)  \
 {(r).x = (p).y * (q).z  -  (q).y * (p).z; \
  (r).y = (q).x * (p).z  -  (p).x * (q).z; \
  (r).z = (p).x * (q).y  -  (q).x * (p).y; } 


#define VectorAdd(p, q, r)  \
 {(r).x = (p).x + (q).x; \
  (r).y = (p).y + (q).y; \
  (r).z = (p).z + (q).z; } 

#define LinearComb(p, q, r, t)  \
 {(r).x = (p).x + (t) * ((q).x - (p).x); \
  (r).y = (p).y + (t) * ((q).y - (p).y); \
  (r).z = (p).z + (t) * ((q).z - (p).z); }

#define VectorSub(p, q, r)  \
 {(r).x = (p).x - (q).x; \
  (r).y = (p).y - (q).y; \
  (r).z = (p).z - (q).z; } 


#define VectorAdd2D(p, q, r)  \
 {(r).x = (p).x + (q).x; \
  (r).y = (p).y + (q).y; } 


#define VectorSub2D(p, q, r)  \
 {(r).x = (p).x - (q).x; \
  (r).y = (p).y - (q).y; } 


#define LinearComb2D(p, q, r, t)  \
 {(r).x = (p).x + (t) * ((q).x - (p).x); \
  (r).y = (p).y + (t) * ((q).y - (p).y); }

#define InvertVector(p) \
 {(p).x = -(p).x; \
  (p).y = -(p).y; \
  (p).z = -(p).z; }


#define MultScalar(scalar, u)  \
 {(u).x *= scalar; \
  (u).y *= scalar; \
  (u).z *= scalar; } 


#define DivScalar(scalar, u)  \
 {(u).x /= scalar; \
  (u).y /= scalar; \
  (u).z /= scalar; } 


#define InvertVector2D(p) \
 {(p).x = -(p).x; \
  (p).y = -(p).y; }


#define MultScalar2D(scalar, u)  \
 {(u).x *= scalar; \
  (u).y *= scalar; } 


#define DivScalar2D(scalar, u)  \
 {(u).x /= scalar; \
  (u).y /= scalar; } 


#define SignEps(var, x, eps)  \
(var = (x), \
 ((var <= eps) ? ((var < -eps) ? -1 : 0) : 1))


#define MinMax3(a, b, c, min, max) {\
  if ((a) < (b)) {\
                    if ((a) < (c)) {\
                                      min = (a); \
                                      if ((b) < (c))  max = (c); \
                                      else            max = (b); \
                    }\
                    else {          \
                                      min = (c); \
                                      max = (b); \
                    }\
  }\
  else {          \
                    if ((a) < (c)) {\
                                      min = (b); \
                                      max = (c); \
                    }  \
                    else {          \
                                      max = (a); \
                                      if ((b) < (c))  min = (b); \
                                      else            min = (c); \
                    }  \
  }\
}


#define MinMax4(a, b, c, d, min, max)  {\
  if ((a) < (b)) {\
                    if ((a) < (c)) {\
                                      min = (a); \
                                      if ((b) < (c))  max = (c); \
                                      else            max = (b); \
                    }  \
                    else {          \
                                      min = (c); \
                                      max = (b); \
                    } \
  }\
  else {         \
                   if ((a) < (c)) { \
                                      min = (b); \
                                      max = (c); \
                   }  \
                   else {           \
                                      max = (a); \
                                      if ((b) < (c))  min = (b); \
                                      else            min = (c); \
                   }\
  } \
 if      ((d) < min)   min = (d); \
 else if ((d) > max)   max = (d); \
}



#define ScaleX(data,xc)  ((xc - data->shift.x) * data->scale_factor)

#define ScaleY(data,yc)  ((yc - data->shift.y) * data->scale_factor)

#define UnscaleX(data,xc)  (data->shift.x + xc / data->scale_factor)

#define UnscaleY(data,yc)  (data->shift.y + yc / data->scale_factor)



#define SortTwoNumbers(a, b, c) {\
  if (a > b) { \
                 c = a; \
                 a = b; \
                 b = c; \
  } \
}



#define SortThreeNumbers(a, b, c, d)  {\
  if (a < b) {\
                if (a < c) {\
                              if (b > c) {\
                                            d = b; \
                                            b = c; \
                                            c = d; \
                              }\
                }\
                else {      \
                              d = a; \
                              a = c; \
                              c = b; \
                              b = d; \
                }\
  }\
  else {      \
                if (a < c) { \
                               d = a; \
                               a = b; \
                               b = d; \
                } \
                else {       \
                               if (b < c) {\
                                             d = a; \
                                             a = b; \
                                             b = c; \
                                             c = d; \
                               }\
                               else {      \
                                             d = a; \
                                             a = c; \
                                             c = d; \
                               }\
                }\
  }\
}


#define MinMax(a, b, min, max) {\
  if ((b) < (a)) {\
                    min = (b); \
                    max = (a); \
  } \
  else {          \
                    min = (a); \
                    max = (b); \
  } \
}
   
#endif

