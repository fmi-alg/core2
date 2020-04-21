#pragma once

#include <stdlib.h>
#include <iostream>
using namespace std;

inline double Area(const double a[2], const double b[2], const double c[2])
{
   return ( b[0] - a[0] ) * ( c[1] - a[1] ) -
		  ( c[0] - a[0] ) * ( b[1] - a[1] );
}

inline int AreaSign(const double a[2], const double b[2], const double c[2])
{
   double area=Area(a,b,c);
   if      ( area >  0 ) return  1;
   else if ( area < 0 ) return -1;
   else     return  0;
}

inline int Collinear(const double a[2], const double b[2], const double c[2])
{
  return AreaSign( a, b, c ) == 0;
}

inline bool Equal(const double a[2], const double b[2])
{
   return (a[0]==b[0] && a[1]==b[1]);
}


/*---------------------------------------------------------------------
Returns TRUE iff point c lies on the closed segement ab.
Assumes it is already known that abc are collinear.
---------------------------------------------------------------------*/
inline bool Between(const double a[2], const double b[2], const double c[2])
{
  // If ab not vertical, check betweenness on x; else on y.
  if ( fabs(a[0]-b[0])>fabs(a[1]-b[1]) )
	 return ((a[0] <= c[0]) && (c[0] <= b[0])) ||
			((a[0] >= c[0]) && (c[0] >= b[0]));
  else
	 return ((a[1] <= c[1]) && (c[1] <= b[1])) ||
			((a[1] >= c[1]) && (c[1] >= b[1]));
}


//
inline char ParallelInt
( const double a[2], const double b[2], const double c[2], const double d[2], double p[2])
{
  if(!Collinear(a, b, c)) return '0';

  //check if they overlap..
  if(Between(a,b,c)) return 'e';
  if(Between(a,b,d)) return 'e';
  if(Between(c,d,a)) return 'e';
  if(Between(c,d,b)) return 'e';

  //they don't overlap but the end points may..
  //check if the end points overlap
  if(Equal(a,c)){ p[0]=a[0]; p[1]=a[1]; return 'v';}
  if(Equal(b,c)){ p[0]=b[0]; p[1]=b[1]; return 'v';}
  if(Equal(a,d)){ p[0]=a[0]; p[1]=a[1]; return 'v';}
  if(Equal(b,d)){ p[0]=b[0]; p[1]=b[1]; return 'v';}

  return '0';
}

/*---------------------------------------------------------------------
SegSegInt: Finds the point of intersection p between two closed
segments ab and cd.  Returns p and a char with the following meaning:
  'e': The segments collinearly overlap, sharing a point.
  'v': An endpoint (vertex) of one segment is on the other segment,
	   but 'e' doesn't hold.
  '1': The segments intersect properly (i.e., they share a point and
	   neither 'v' nor 'e' holds).
  '0': The segments do not intersect (i.e., they share no points).
Note that two collinear segments that share just one point, an endpoint
of each, returns 'e' rather than 'v' as one might expect.
---------------------------------------------------------------------*/
inline char SegSegInt( const double a[2], const double b[2], const double c[2], const double d[2], double p[2] )
{
  double  s, t;        // The two parameters of the parametric eqns.
  double  num_s, num_t, denom;   // Numerator and denoninator of equations.
  char    code = '?';    // Return char characterizing intersection.

  denom = a[0] * ( d[1] - c[1] ) +
		  b[0] * ( c[1] - d[1] ) +
		  d[0] * ( b[1] - a[1] ) +
		  c[0] * ( a[1] - b[1] );

  // If denom is zero, then segments are parallel: handle separately.
  if (denom==0) return  ParallelInt(a, b, c, d, p);


  if(Equal(a,c)){ p[0]=a[0]; p[1]=a[1]; return 'v';}
  if(Equal(b,c)){ p[0]=b[0]; p[1]=b[1]; return 'v';}
  if(Equal(a,d)){ p[0]=a[0]; p[1]=a[1]; return 'v';}
  if(Equal(b,d)){ p[0]=b[0]; p[1]=b[1]; return 'v';}

  //compute s
  num_s =    a[0] * ( d[1] - c[1] ) +
			 c[0] * ( a[1] - d[1] ) +
			 d[0] * ( c[1] - a[1] );


  s = num_s / denom;


  //compute t
  num_t = -( a[0] * ( c[1] - b[1] ) +
			 b[0] * ( a[1] - c[1] ) +
			 c[0] * ( b[1] - a[1] ) );



  t = num_t/denom;

  //decide the code
  if( (0.0<s) && (s< 1.0) && (0.0< t) && (t< 1.0) )
	   code = '1';
  else if ( (0.0>s) || (s>1.0) || (0.0>t) || (t>1.0) )
	   return '0';
  else
	  code= 'v';

  if(code!='v'){
	  p[0] = (a[0] + s*(b[0]-a[0]));
	  p[1] = (a[1] + s*(b[1]-a[1]));
  }
  else{
	 if(s==0){ p[0]=a[0]; p[1]=a[1]; }
	 else if(s==1){p[0]=b[0]; p[1]=b[1]; }
	 else if(t==0){p[0]=c[0]; p[1]=c[1]; }
	 else if(t==1){p[0]=d[0]; p[1]=d[1]; }
	 else{
		 cout<<"s="<<s<<" t="<<t<<endl;
		 cout<<"a="<<a[0]<<","<<a[1]<<endl;
		 cout<<"b="<<b[0]<<","<<b[1]<<endl;
		 cout<<"c="<<c[0]<<","<<c[1]<<endl;
		 cout<<"d="<<d[0]<<","<<d[1]<<endl;
		 assert(false);
	 }
  }

  return code;
}
