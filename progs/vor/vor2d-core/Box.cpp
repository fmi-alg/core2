#include "Box.h"
#include <assert.h>


BoxIter::BoxIter(const Box* bb, int direc):b(bb), direction(direc), neighbor(0)
{
	prev = (direc + 3) % 4;
	next = (direc + 1) % 4;
	cross = (direc + 2) % 4;
}

double Box::r0 = 0;
int Box::counter = 0;

Box* BoxIter::First()
{
	Box* n = b->pChildren[direction];
	if (!n)
	{
		return 0;
	}

	// if neighbor are no smaller
	if (n->depth <= b->depth)
	{
		return n;
	}

	assert(!Box::isOverLimit(b, n));
	
	while (true)
	{
		if (!n->pChildren[next] || Box::isOverLimit(b, n->pChildren[next]))
		{
			break;
		}
		n = n->pChildren[next];
	}

	//int t = 0;
	while (true)
	{
		if (n->pChildren[cross] == b)
		{
			break;
		}
		n = n->pChildren[cross];
	}
	neighbor = n;
	return neighbor;
}

Box* BoxIter::End()
{
	if (b->pChildren[direction])
	{
		return b->pChildren[direction]->pChildren[prev];
	}
	return 0;
}

Box* BoxIter::Next()
{
	if (!neighbor)
	{
		return 0;
	}

	neighbor = neighbor->pChildren[prev];
	return neighbor;
}
