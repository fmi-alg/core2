#pragma once

#include <stdlib.h>
#include <iostream>
using namespace std;

#define SMALLNUMBER 1e-10

inline double Area(const double a[2], const double b[2], const double c[2])
{
   return ( b[0] - a[0] ) * ( c[1] - a[1] ) -
		  ( c[0] - a[0] ) * ( b[1] - a[1] );
}

inline int AreaSign(const double a[2], const double b[2], const double c[2])
{
   double area=Area(a,b,c);
   if      ( area >  SMALLNUMBER ) return  1;
   else if ( area < -SMALLNUMBER ) return -1;
   else     return  0;
}

inline int Collinear(const double a[2], const double b[2], const double c[2])
{
  return AreaSign( a, b, c ) == 0;
}

inline bool AlmostEqual(const double a[2], const double b[2])
{
   return (fabs(a[0]-b[0])<SMALLNUMBER &&fabs(a[1]-b[1])<SMALLNUMBER);
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
  if(AlmostEqual(a,c)){ p[0]=a[0]; p[1]=a[1]; return 'v';}
  if(AlmostEqual(b,c)){ p[0]=b[0]; p[1]=b[1]; return 'v';}
  if(AlmostEqual(a,d)){ p[0]=a[0]; p[1]=a[1]; return 'v';}
  if(AlmostEqual(b,d)){ p[0]=b[0]; p[1]=b[1]; return 'v';}

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
  //if (fabs(denom)<SMALLNUMBER) denom=0;
  if (fabs(denom)<SMALLNUMBER) return  ParallelInt(a, b, c, d, p);


  if(AlmostEqual(a,c)){ p[0]=a[0]; p[1]=a[1]; return 'v';}
  if(AlmostEqual(b,c)){ p[0]=b[0]; p[1]=b[1]; return 'v';}
  if(AlmostEqual(a,d)){ p[0]=a[0]; p[1]=a[1]; return 'v';}
  if(AlmostEqual(b,d)){ p[0]=b[0]; p[1]=b[1]; return 'v';}

  //compute s
  num_s =    a[0] * ( d[1] - c[1] ) +
			 c[0] * ( a[1] - d[1] ) +
			 d[0] * ( c[1] - a[1] );


  s = num_s / denom;

  if( fabs(s)<SMALLNUMBER ) s=0;
  else if( fabs(1-s)<SMALLNUMBER ) s=1;


  //compute t
  num_t = -( a[0] * ( c[1] - b[1] ) +
			 b[0] * ( a[1] - c[1] ) +
			 c[0] * ( b[1] - a[1] ) );



  t = num_t / denom;

  if( fabs(t)<SMALLNUMBER ) t=0;
  else if( fabs(1-t)<SMALLNUMBER) t=1;

  //decide the code
//   if(s==0 || s==1 || t==0 || t==1)
//        code = 'v';
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

  //double check...
//   if(code=='v'){
//       double q[2];
//       q[0] = (c[0] + t*(d[0]-c[0]));
//       q[1] = (c[1] + t*(d[1]-c[1]));
//       if(AlmostEqual(p,q)==false){
//           //cout<<"SHOOT: "<<code<<" points:"<<p[0]<<","<<p[1]<<"  "<<q[0]<<","<<q[1]<<endl;
//           return '0';
//       }
//   }

  return code;
}
