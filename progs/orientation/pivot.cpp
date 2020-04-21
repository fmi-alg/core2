/* *****************************************************************
 * file: pivot.cpp
 * Purpose:
 *    To test different pivot strategy in evaluating
 *    the signed area predicate in 2D.  If p,q,r are points, then
 *
 *         area(p, q, r) = det(A)
 *
 *              where A is the 2x2 matrix whose rows are q-p and r-p.
 *
 *    We also consider the accuracy of the orientation predicate,
 *
 *         orient(p, q, r) = sign(area(p,q,r))
 *
 * Usage:
 *      > pivot [infile] [outfile] [strategy]
 *
 *      where the default arguments are
 *            [infile]="in/data1.txt", which contains 100 points
 *            [outfile]="out/data1.txt" will hold the results of test
 *            [strategy]=1 is an integer indicating pivot choice heuristic
 *
 *      Pivot Choice Heuristics: 
 *            0 = unspecified (or, pivot on random point)
 *            i = pivot on i-th point (i=1,2,3)
 *            4 = Fortune's heuristic (pick point whose x value is median)
 *            5 = pivot on point opposite the longest edge
 *            6 = pivot on point with largest angle
 *
 * Author: Chee Yap
 *         Adapted from code by Seung-Hyun Yun (shyun@3map.snu.ac.kr)
 * Date: July 2004
 * Since Core Library Version 1.7 
 * *****************************************************************/

#include "pivot.h"
using namespace std;

// N=number of points in input file
#define N 100

// Files are global
char infile[]="in/data1.txt";
char outfile[]="out/data1.txt";
ifstream in;
ofstream out;

// MAIN ROUTINE
int main( int argc, char **argv )
{
   register int i;
   
   if (argc>1) in.open( argv[1] );
   else in.open(infile); // Input file has N points (= 2N numbers)

   if (argc>2) out.open( argv[2] );
   else out.open(outfile); // Results of test written here

   int strategy;
   if (argc>3) strategy = atoi( argv[3] );
   else strategy = 1;   // 0=first point, 1=fortune, 2=max length, 3=max angle

   // read input points from file
   Point *Pts = new Point[N];
   for ( i = 0; i < N; i++ ) {
      in >> Pts[i].x;
      in >> Pts[i].y;
   }

   // ACTUAL TEST ROUTINE
   pivotest( Pts, strategy );

   // CLEAN UP
   cout<< "done" << endl;
   delete [] Pts;
   out.close();
   return 0;
}

// Test the performance of a single strategy
void pivotest( Point *Pts, int strategy )
{
   register int i,j;
   Point p, q, r;
   double approx, exact;
   int countRightArea=0, countWrongArea=0;
   int countRightSign=0, countWrongSign=0;

   for (i=0; i<N; i++) { // N is global
      j = pivotChoice( Pts[i], Pts[i+1], Pts[i+2], strategy);
      approx = area_approx( Pts[i], Pts[i+1], Pts[i+2], j);
      exact = area_exact( Pts[i], Pts[i+1], Pts[i+2]);
      if (approx == exact) countRightArea++;
      else countWrongArea++;
      if ((approx * exact > 0)||
            ((approx == exact)&& (approx ==0))) countRightSign++;
      else countWrongSign++;
   }
   out << "PIVOTING RESULTS" << endl;
   out << "    Input file: " << infile << endl;
   out << "=======================================================" << endl;
   out << " count of areas:  right = " << countRightArea
         << ", wrong = " << countWrongArea << endl;
   out << " count of signs:  right = " << countRightSign
         << ", wrong = " << countWrongSign << endl;
}

// Test the performance of a single strategy
void pivotest2( Point *Pts, int strategy1, int strategy2 )
{
}

// This chooses the pivot based on some strategy:
//   --it is inlined because it is meant to be 
//     called mainly by the area_approx program
inline int pivotChoice( Point p, Point q, Point r, int strategy ){
   switch (strategy) {
      case 0: // Take the any point? Not really used
         return 1;
         break;
      case 1: // Take the first point
         return 1;
         break;
      case 2: // Take the second point
         return 2;
         break;
      case 3: // Take the third point
         return 3;
         break;
      case 4: // Fortune's.  Return the index of the median x-coordinate
         return median_index(p.x, q.x, r.x);
         break; 
      case 5: // longest side
         return max_index( p.get_norm_square(),
                           q.get_norm_square(),
                           r.get_norm_square());
         break; 
      case 6: // largest angle
         return max_index( get_angle( q-p, r-p ),
                           get_angle( p-q, r-q ),
                           get_angle( p-r, q-r ));
         break; 
      default:
         ;
   }
   cout << "ERROR in pivotChoice" << endl;
   return -1;    // should not reach this!
}

// computes signed area using specified pivot strategy
double area_approx( Point p, Point q, Point r, int pivot_idx )
{
   int i = pivotChoice(p, q, r, pivot_idx);
   switch( i )
   {
   case 1: // p as pivot
      return (( q.x - p.x ) * ( r.y - p.y ) - ( q.y - p.y ) * ( r.x - p.x ));
      break;
   case 2: // q as pivot
      return (( r.x - q.x ) * ( p.y - q.y ) - ( r.y - q.y ) * ( p.x - q.x ));
      break;
   case 3: // r as pivot
      return (( p.x - r.x ) * ( q.y - r.y ) - ( p.y - r.y ) * ( q.x - r.x ));
      break;
   default:
      ;
   }
   return 0;
}

// computes the exact area, converted to machine double
double area_exact( Point p, Point q, Point r )
{
   return (( Expr(q.x) - Expr(p.x) ) * ( Expr(r.y) - Expr(p.y) )
        -  ( Expr(q.y) - Expr(p.y) ) * ( Expr(r.x) - Expr(p.x) )).doubleValue();
}

// computes the angle(p0, origin, p1).
double get_angle( Point p0, Point p1 )
{
   double cs;
   p0.normalize();
   p1.normalize();

   cs = p0 * p1;
   cs = acos( cs );

   return cs;
}


/*
 * ORIGINAL CODE FROM Yun Seung Hyun
 
void signed_area( Point *Pts, int size, int strategy, ofstream& out )
{
   register i, j, k;
   Point p, q, r;

   double   *absolute_error;
   double   *relative_error;

   Expr   average_absolute_error,
         average_relative_error,
         max_absolute_error = Expr( 0 ),
         max_relative_error = Expr( 0 );

   double    sum_absolute_error = 0,
         sum_relative_error = 0;
         

   int pivot_idx, 
      count_correct_value = 0, 
      count_correct_sign = 0,
      count_inverted_sign = 0,
      count_total = N * ( N - 1 ) * ( N - 2 ) / 6,
      idx = 0;

   absolute_error = new double[count_total];
   relative_error = new double[count_total];

   Expr exact_area, approx_area;

   for ( i = 0; i < size; i++ )
   for ( j = i + 1; j < size; j++ )
   for ( k = j + 1; k < size; k++ )
   {
      p = Pts[i];
      q = Pts[j];
      r = Pts[k];
      double tmp;

      // get pivot method.
      if ( strategy == 1 )
         pivot_idx = get_pivot_idx_I( p, q, r );
      if ( strategy == 2 )
         pivot_idx = get_pivot_idx_II( p, q, r );
      if ( strategy == 3 )
         pivot_idx = get_pivot_idx_III( p, q, r );

      // get approximated area using machine level.
      tmp = signed_area_approx( p, q, r, pivot_idx );

      // get exact area using core lib.
      exact_area = signed_area_exact( p, q, r );
      
      // to compare approximated area with exact area.
      approx_area = Expr( tmp );

      // get error and sum
      
      if ( exact_area != 0 )
      {
         absolute_error[idx] = ExprAbs( approx_area-exact_area ).doubleValue();
         relative_error[idx] = ( ExprAbs( approx_area - exact_area ) / ExprAbs( exact_area ) ).doubleValue();
         sum_absolute_error = sum_absolute_error + absolute_error[idx];
         sum_relative_error = sum_relative_error + relative_error[idx];
      }
      else
      {
         relative_error[idx] = 0;
         absolute_error[idx] = ExprAbs( approx_area - exact_area ).doubleValue();
         sum_absolute_error = sum_absolute_error + absolute_error[idx];
         sum_relative_error = sum_relative_error + relative_error[idx];
      }
      
      
      // get maximum error.
      if ( absolute_error[idx] > max_absolute_error )
         max_absolute_error = absolute_error[idx];
      if ( relative_error[idx] > max_relative_error )
         max_relative_error = relative_error[idx];

      // count the followings...
      if ( absolute_error[idx] == 0 )
         count_correct_value++;

      if ( ExprSign( approx_area ) == ExprSign( exact_area ) )
         count_correct_sign++;
      if ( ExprSign( approx_area ) != ExprSign( exact_area ) )
         count_inverted_sign++;

      idx++;
   }

   average_absolute_error = sum_absolute_error / count_total;
   average_relative_error = sum_relative_error / count_total;


   double sum_absolute = 0.0;
   double sum_relative = 0.0;

   for ( i = 0; i < count_total; i++ )
   {
      Expr difference_absolute = absolute_error[i] - average_absolute_error;
      Expr difference_relative = relative_error[i] - average_relative_error;
      difference_absolute = difference_absolute * difference_absolute;
      difference_relative = difference_relative * difference_relative;
      sum_absolute = sum_absolute + difference_absolute.doubleValue();
      sum_relative = sum_relative + difference_relative.doubleValue();
   }

   Expr deviation_absolute, deviation_relative;
   out << setprecision(15);

   deviation_absolute = sqrt( sum_absolute / count_total );
   deviation_relative = sqrt( sum_relative / count_total );

   out << "=======================================================" << endl;
   out << "Result" << endl;
   out << "1. The number of time( approx_area == exact_area ) is " 
      << count_correct_value << endl;
   out << "2. The maximum value of |approx_area-exact_area| is " 
      << max_absolute_error << endl;
   out << "3. The average value of |approx_area - exact_area| is " 
      << average_absolute_error << endl;
   out << "4. The standard deviation of |approx_area - exact_area| is " 
      << deviation_absolute << endl;
   out << "5. The maximum value of |approx_area - exact_area| / |exact_area| is " 
      << max_relative_error << endl;
   out << "6. The average value of |approx_area - exact_area| / |exact_area| is " 
      << average_relative_error << endl;
   out << "7. The standard deviation of |approx_area - exact_area| / |exact_area| is " 
      << deviation_relative << endl;
   out << "8. The number of correct sign is " 
      << count_correct_sign << endl;
   out << "9. The number of inverted sign is " 
      << count_inverted_sign << endl;
   out << "=======================================================" << endl;

   delete [] absolute_error;
   delete [] relative_error;

}
int ExprSign( Expr e )
{
   if ( e == 0 )
      return 0;
   if ( e > 0 )
      return 1;
   return -1;
}

int get_pivot_idx_I( Point p, Point q, Point r )
{
   register i;
   int pivot_idx = 1;
   double min, tmp;
   min = rho( p, q, r, 1 );
   for ( i = 2; i < 4; i++ )
   {
      tmp = rho( p, q, r, i );
      if ( tmp < min )
      {
         pivot_idx = i;
         min = tmp;
      }
   }
   return pivot_idx;
}
int get_pivot_idx_II( Point p, Point q, Point r )
{
   double P, Q, R;
   P = get_angle( q-p, r-p );
   Q = get_angle( p-q, r-q );
   R = get_angle( p-r, q-r );
   if ( P > Q )
   {
      if ( P > R )
         return 1; // P > Q,R
      else 
         return 3; // R > P > Q
   }
   else
   {
      if ( Q > R )
         return 2; // Q > P,R
      else
         return 3; // R > Q > P;
   }
   return 1;
}
int get_pivot_idx_III( Point p, Point q, Point r )
{
   double P, Q, R;
   P = get_angle( q-p, r-p );
   Q = get_angle( p-q, r-q );
   R = get_angle( p-r, q-r );
   if ( P < Q )
   {
      if ( P < R )
         return 1; // P < Q,R
      else 
         return 3; // R < P < Q
   }
   else
   {
      if ( Q < R )
         return 2; // Q < P,R
      else
         return 3; // R < Q < P;
   }
   return 1;
}
double rho( double a, double b )
{
   double tmp0 = a / b;
   double tmp1 = b / a;
   tmp0 = fabs( tmp0 );
   tmp1 = fabs( tmp1 );
   double ret = MIN( tmp0, tmp1 );
   return ret;
}

double rho( Point p0, Point p1 )
{
   double ret = MAX( rho( p0.x, p1.x ), rho( p0.y, p1.y ) );
   return ret;
}

double rho( Point p, Point q, Point r, int pivot_idx )
{
   double ret;
   switch( pivot_idx )
   {
   case 1: // rho_p
      ret = MAX( rho( p, q ), rho( p, r ) );
      break;
   case 2: // rho_q
      ret = MAX( rho( q, r ), rho( q, p ) );
      break;
   case 3: // rho_r
      ret = MAX( rho( r, p ), rho( r, q ) );
      break;
   }
   return ret;
}

Expr ExprAbs( Expr e )
{
   Expr ret = e;
   if ( e < 0 )
      ret = -ret;
   return ret;
}

*/  


