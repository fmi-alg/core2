#pragma once
#include "math.h"

class Corner;

class Wall
{
public:
	Corner* src;
	Corner* dst;

	Wall(Corner* s, Corner* d);

	double distance(double x, double y);

	bool isRight(double x, double y);

};
