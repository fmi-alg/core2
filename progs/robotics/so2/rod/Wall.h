/* **************************************
   File: Wall.h

   Description: A wall is a triangle defined by vertices (corners) a, b, c. 

   HISTORY: July, 2013: Surin Ahn

 ************************************** */

#pragma once
#include "math.h"

class Corner;
class Edge;

class Wall
{
public:
	Corner* a;
	Corner* b;
	Corner* c; 

	Wall (Corner* aa, Corner* bb, Corner* cc);	
	
	double distance (double x, double y , double z);	

	bool isRight(double x, double y, double z);

private:
	// Parameters for distance formula
	double t;
	double s;

	double dot (double m1[3], double m2[3]);

};
