#include "Corner.h"
#include "Wall.h"


bool Corner::isConvex()
{
	Corner* c = this->nextWall->dst;
	return !this->preWall->isRight(c->x, c->y);
}

bool Corner::intersectZone()
{
	return true;
}