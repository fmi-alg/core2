/***********************************************
 * file: t.ccp
 *
 * Since CORE Library Version 1.6
 * $Id: t.cpp,v 1.1 2006/03/07 04:51:24 exact Exp $
 **********************************************/

#include "CORE/CORE.h"
#include "CORE/geometry2d.h"

using namespace std;

int main() {
	Point2d S[20];

	int N = 0;
	int n = readPoints(cin, S, 20, N);
	 	// N is the number of points to read
	 	// if N=0, then input file must specify
	 	// the number of points to read
	cout << "n= " << n << endl;
	for (int i=0; i<n; i++)
		cout << i << ": " << S[i] << endl;

	return 0;
}

