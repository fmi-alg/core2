/* ************************************************
   File: delaunay.cpp

   Purpose:  Simple Delaunay Triangulation of N points on a circle.
   		
		This program is adapted from O'Rourke's book
		"Computational Geometry in C" (Cambridge Univ.Press, 1994).

		At CORE_LEVEL 3, these points lie EXACTLY on a circle.
		Hence every possible triangle will be considered a Delaunay
		triangle.  With N points, you must see {N \choose 3} triangles.

   Usage: 

        % delaunay  [N]

	If the optional argument N is not given, you will be prompted.

   Author: Chee Yap

   Since Core Library Version 1.5
   $Id: delaunay.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */

#include "CORE/CORE.h"

int main(int argc, char** argv)
{
  double x[1000],y[1000],z[1000];/* input points x y,z=x^2+y^2 */
  int    n;                      /* number of input points */
  double xn, yn, zn;             /* outward vector normal to (i,j,k) */
  int    flag;                   /* true if m above (i,j,k) */
  int    F = 0;                  /* # of lower faces */
  // define the rotation angle to generate points
  double sintheta = 5;  sintheta /= 13;
  double costheta = 12; costheta /= 13;

  // check if argument N is specified in command line
printf("debug: argc = %d \n", argc);

  if (argc >= 2) {
      n = atoi(argv[1]);
  } else {
      printf("Please input the number of points on the circle: ");
      scanf("%d", &n);
  }

  x[0] = 65;  y[0] = 0; z[0] = x[0] * x[0] + y[0] * y[0];
  for (int i = 1; i < n; i++ ) {
    x[i] = x[i-1]*costheta - y[i-1]*sintheta; // compute x-coordinate
    y[i] = x[i-1]*sintheta + y[i-1]*costheta; // compute y-coordinate
    z[i] = x[i] * x[i] + y[i] * y[i];         // compute z-coordinate
  }
  for (int i = 0; i < n - 2; i++ )
    for (int j = i + 1; j < n; j++ )
      for (int k = i + 1; k < n; k++ ) 
        if ( j != k ) {
          // For each triple (i,j,k), compute normal to triangle (i,j,k). 
          xn = (y[j]-y[i])*(z[k]-z[i]) - (y[k]-y[i])*(z[j]-z[i]);
          yn = (x[k]-x[i])*(z[j]-z[i]) - (x[j]-x[i])*(z[k]-z[i]);
          zn = (x[j]-x[i])*(y[k]-y[i]) - (x[k]-x[i])*(y[j]-y[i]);
          if ( (flag = (zn < 0) )) // Only examine faces on bottom of paraboloid
            for (int m = 0; m < n; m++)
              /* For each other point m, check if m is above (i,j,k). */
              flag = flag && 
                ((x[m]-x[i])*xn + (y[m]-y[i])*yn + (z[m]-z[i])*zn <= 0);
          if (flag) {
            printf("lower face indices: %d, %d, %d\n", i, j, k);
            F++;
          }
        }
  printf("A total of %d lower faces found.\n", F);
  int CorrectNo = (n*(n-1)*(n-2))/6;
  if (F == CorrectNo) 
    printf("CORRECT!  The number of lower faces is equal to {%d choose 3} = %d\n", 
		    n, F);
  else
    printf("ERROR!  The number of lower faces is not equal to {%d choose 3} = %d\n", 
		    n, CorrectNo);

  return 0;
}//main	

