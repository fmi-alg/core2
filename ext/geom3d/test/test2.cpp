#include "CORE.h"

#include "geometry3d.h"
#include <iostream.h>

void polygon3d_test() {
  cout << "\n\n############# Test Polygon3d ###############\n";
  Triangle3d T1( Point3d(3.0, 0.0, 0.0), Point3d(0.0, 1.0, 0.0), 
                  Point3d(3.0, 3.0, 0.0) ); 
  Triangle3d T2( Point3d(3.0, 0.0, 0.0), Point3d(0.0, 1.0, 0.0), 
                 Point3d(3.0, 1.0, 2.0) );
  cout << T2 << endl;
  cout << T2.toPlane() << endl;
  cout << T1.toPlane() << endl;

/*
  GeomObj* pInterObj = T1.intersection( T2.toPlane() );
*/

double result = T1.toPlane().apply( T2.V1()) ;
double result2 = T2.toPlane().apply( T1.V2());
if ( result == result2 )
   cout << "TRUE" << endl;
else 
   cout << "FALSE" << endl;

if (result2 == 0)
   cout << "CORRECT" << endl;
else
   if (result2 - "0.0000000001" < 0)
   	cout << "WRONG: Less than 0.00000000001" << endl;
   else
   	cout << "WRONG: Greater than 0.00000000001" << endl;

cout << "T1.toPlane().apply(T2.V1()) = " << result << endl;
cout << setprecision(50)
	<< "T2.toPlane().apply( T1.V2()) = "
	<< result2.approx(1000, CORE_INFTY) << endl;

cout << "T2.toPlane().contains( Segment3d( T1.V1(), T1.V2() ))? " ;

if (T2.toPlane().contains( Segment3d( T1.V1(), T1.V2() )))
    cout << "YES" << endl;
else
    cout << "NO" << endl;


/*
  Segment3d ansSeg( Point3d(3.0, 0.0, 0.0), Point3d(0.0, 1.0, 0.0) );
  if( *(Segment3d *)pInterObj != ansSeg ) 
    cout << "ERROR! --intersection( Triangle3d & )" << endl;
  else
    cout << "CORRECT! --intersection( Triangle3d & )" << endl;
*/

}



int main(int argc, char* argv[]) {
//  segment3d_test();
//  plane3d_test();
  polygon3d_test();
  cout << "Test finished" << endl;
}
