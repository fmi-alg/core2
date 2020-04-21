/*
File: dt4.cc (modified by Chen Li on 09/15/99)

Not: This program is from O'Rourke's book. Some changes are made
in order to utilize CORE library:
	1. use double or long instead of float and int, so that
the critical variables can be automatically promoted.
	2. change printf and scanf to stream I/O operations.

Usage:
	dt4 < input_file > output_file
		
	Input: as above.
	Output: print to the standard output
		the delaunay triangles and the total
		number of faces found.
*/

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

#define	NMAX	1001

int main()
{
	double	x[NMAX],y[NMAX],z[NMAX];/* input points xy,z=x^2+y^2 */
	int	n;			/* number of input points */
	int	i, j, k, m;		/* indices of four points */
	double	xn, yn, zn;		/* outward vector normal to (i,j,k) */
	int	flag;			/* t if m above of (i,j,k) */
	int	F = 0;			/* # of lower faces */

	/* Input points and compute z = x^2 + y^2. */
	/* original code:
	scanf("%d", &n);
	for ( i = 0; i < n; i++ ) {
		scanf("%d %d", &x[i], &y[i]);
		z[i] = x[i] * x[i] + y[i] * y[i];
	}
	*/

	n = 0;
	while ((std::cin) && (n < NMAX)) {
	  std::cin >> x[n] >> y[n];
	  if (std::cin) {	
	    z[n] = x[n] * x[n] + y[n] * y[n];
	    n++;
	  }
	}
        std::cout << "n = " << n << std::endl;

	if (std::cin) { // not all have been read in
	  std::cerr << "ERROR: input array range overflow" << std::endl;
	  exit(1);
	}

	/* For each triple (i,j,k) */
	for ( i = 0; i < n - 2; i++ )
	for ( j = i + 1; j < n; j++ )
	for ( k = i + 1; k < n; k++ ) 
	if ( j != k ) {
		/* Compute normal to triangle (i,j,k). */
		xn = (y[j]-y[i])*(z[k]-z[i]) - (y[k]-y[i])*(z[j]-z[i]);
		yn = (x[k]-x[i])*(z[j]-z[i]) - (x[j]-x[i])*(z[k]-z[i]);
		zn = (x[j]-x[i])*(y[k]-y[i]) - (x[k]-x[i])*(y[j]-y[i]);

		/* Only examine faces on bottom of paraboloid: zn < 0. */
		flag = (zn < 0);
		if ( flag )
			/* For each other point m */
			for (m = 0; m < n; m++)
				/* Check if m above (i,j,k). */
				flag = flag && 
					((x[m]-x[i])*xn +
					 (y[m]-y[i])*yn +
					 (z[m]-z[i])*zn <= 0);
		if (flag) {
		  /*
	  	   printf("z=%10d; lower face indices: %d, %d, %d\n", 
		     zn, i, j, k);
		  */
		  std::cout << "z=" << zn <<"; lower face indices: " 
		       << i << ", " << j << ", " << k << std::endl;
		  F++;
		}
	}
	printf("A total of %d lower faces found.\n", F);
	return 0;
}

