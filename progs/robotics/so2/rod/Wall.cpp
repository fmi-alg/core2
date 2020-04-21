/* **************************************
   File: Wall.cpp

   Description: 

   HISTORY: July, 2013: Surin Ahn

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#include "Corner.h"
#include "Edge.h"
#include "Wall.h"
#include <iostream>

using namespace std;

Wall::Wall(Corner* aa, Corner* bb, Corner* cc): a(aa), b(bb), c(cc)
{
}

// distance from point to triangle (face)
double Wall::distance(double x, double y, double z)
{
	double B[3] = {a->x, a->y, a->z}; // B = TRI(1,:);
	double E0[3] = {b->x - B[0], b->y - B[1], b->z - B[2]}; // E0 = TRI(2,:) - B
	double E1[3] = {c->x - B[0], c->y - B[1], c->z - B[2]}; // E1 = TRI(3,:) - B

	double D[3] = {B[0]-x, B[1]-y, B[2]-z}; // D = B - P;
	double a = dot(E0,E0);
	double b = dot(E0,E1);
	double c = dot(E1,E1);
	double d = dot(E0,D);
	double e = dot(E1,D);
	double f = dot(D,D);

	double det = a*c - b*b; 
	s   = b*e - c*d;
	t   = b*d - a*e;

	double sqrDistance;
	double numer;
	double denom;
	double tmp0;
	double tmp1;

	if ((s+t) <= det)
	{
	  if (s < 0)
	  {
	    if (t < 0)
    	    {
	      //region4
	      if (d < 0)
	      {
		t = 0;
		if (-d >= a)
	        {
		  s = 1;
		  sqrDistance = a + 2*d + f; 
		}
		else
		{
		  s = -d/a;
		  sqrDistance = d*s + f;
		}
	      }
	      else
	      {
		s = 0;
		if (e >= 0)
		{
		  t = 0;
		  sqrDistance = f; 
		}
		else
		{
		  if (-e >= c)
		  {
		    t = 1;
		    sqrDistance = c + 2*e + f; 
		  }
		  else
		  {
		    t = -e/c;
		    sqrDistance = e*t + f;
		  }
		}
	      } // end of region 4
	    }
	    else
	    {
	      // region 3
	      s = 0;
	      if (e >= 0)
	      {
		t = 0;
		sqrDistance = f; 
	      }
	      else {
		if (-e >= c)
		{
		  t = 1;
		  sqrDistance = c + 2*e + f; 
	        }
		else {
		  t = -e/c;
		  sqrDistance = e*t + f;
		}
	      }
	    } // end of region 3 
          }
	  else {
	    if (t < 0) {
	      // region 5
	      t = 0;
	      if (d >= 0) {
		s = 0;
		sqrDistance = f;
	      }
	      else {
		if (-d >= a) {
		  s = 1;
		  sqrDistance = a + 2*d + f;
		}
		else {
		  s = -d/a;
		  sqrDistance = d*s + f;
		}
	      }
	    }
	    else {
	      // region 0
	      double invDet = 1/det;
	      s = s*invDet;
	      t = t*invDet;
	      sqrDistance = s*(a*s + b*t + 2*d) + t*(b*s + c*t + 2*e) + f;
	    }
	  }
	}
	else {
	  if (s < 0) {
	    // region 2
	    tmp0 = b + d;
	    tmp1 = c + e;
	    if (tmp1 > tmp0) {
	      numer = tmp1 - tmp0;
	      denom = a - 2*b + c;
	      if (numer >= denom) {
		s = 1;
		t = 0;
		sqrDistance = a + 2*d + f; 
	      }
	      else {
		s = numer/denom;
		t = 1-s;
		sqrDistance = s*(a*s + b*t + 2*d) + t*(b*s + c*t + 2*e) + f;
	      }
	    }
	    else {
	      s = 0;
	      if (tmp1 <= 0) {
		t = 1;
		sqrDistance = c + 2*e + f;
	      }
	      else {
		if (e >= 0) {
		  t = 0;
		  sqrDistance = f;
		}
		else {
		  t = -e/c;
		  sqrDistance = e*t + f;
		}
	      }
	    } // end of region 2
	  }
	  else {
	    if (t < 0) {
	      //region6 
	      tmp0 = b + e;
	      tmp1 = a + d;
	      if (tmp1 > tmp0) {
		numer = tmp1 - tmp0;
		denom = a-2*b+c;
		if (numer >= denom) {
		  t = 1;
		  s = 0;
		  sqrDistance = c + 2*e + f;
		}
		else {
		  t = numer/denom;
		  s = 1 - t;
		  sqrDistance = s*(a*s + b*t + 2*d) + t*(b*s + c*t + 2*e) + f;
		}
	      }
	      else {
		t = 0;
		if (tmp1 <= 0) {
		    s = 1;
		    sqrDistance = a + 2*d + f;
		}
		else {
		  if (d >= 0) {
		      s = 0;
		      sqrDistance = f;
		  }
		  else {
		      s = -d/a;
		      sqrDistance = d*s + f;
		  }
		}
	      }
	      // end region 6
	    }
	    else {
	      // region 1
	      numer = c + e - b - d;
	      if (numer <= 0) {
		s = 0;
		t = 1;
		sqrDistance = c + 2*e + f;
	      }
	      else {
		denom = a - 2*b + c;
		if (numer >= denom) {
		  s = 1;
		  t = 0;
		  sqrDistance = a + 2*d + f;
		}
		else {
		  s = numer/denom;
		  t = 1-s;
		  sqrDistance = s*(a*s + b*t + 2*d) + t*(b*s + c*t + 2*e) + f;
		}
	      } // end of region 1
	    }
	  }
	}

	// account for numerical round-off error
	if (sqrDistance < 0) sqrDistance = 0;

	double dist = sqrt(sqrDistance);
	return dist;
}

// returns the dot product of two 1x3 matrices
double Wall::dot(double m1[3], double m2[3])
{
	return m1[0]*m2[0] + m1[1]*m2[1] + m1[2]*m2[2];
}

bool Wall::isRight(double qx, double qy, double qz)
{
	double a0 [3] = {a->x, a->y, a->z};	
	double b0 [3] = {b->x, b->y, b->z};
	double c0 [3] = {c->x, c->y, c->z};

	// Subtract a from b, c, q so we get a 3x3 matrix
	// (p stands for "prime")
	
	double bp [3] = {b0[0]-a0[0], b0[1]-a0[1], b0[2]-a0[2]};
	double cp [3] = {c0[0]-a0[0], c0[0]-a0[0], c0[2]-a0[2]};
	double qp [3] = {qx-a0[0], qy-a0[1], qz-a0[2]};
	
	// Calculate the determinant of the 3x3 matrix whose columns are given by b', c', and q'
	double det = (bp[0]*cp[1]*qp[2]) + (cp[0]*qp[1]*bp[2]) + (qp[0]*bp[1]*cp[2]) - (qp[0]*cp[1]*bp[2]) - (cp[0]*bp[1]*qp[2]) - (bp[0]*qp[1]*cp[2]);
	return det < 0;	  
}
