#pragma once
#include "Box.h"
#include <iostream>


class Set
{
public:
	Box* pBox;
	Set* pParent; //parent in union find
	int rank;

	Set(Box* b):pBox(b), rank(1)
	{
		pParent = this;
		b->pSet = this;
	}
};

class UnionFind
{
private:
	Set* pathCompress(Set* set)
	{
		if (set->pParent == set)
		{
			return set;
		}

		set->pParent = pathCompress(set->pParent);
		return set->pParent;
	}

public:
	Box* Find(Box* b)
	{
		Set* root = pathCompress(b->pSet);
		return root->pBox;
	}

	void Union(Box*a, Box* b)
	{
		Set* roota = Find(a)->pSet;
		Set* rootb = Find(b)->pSet;
		if (roota == rootb)
		{
			return;
		} 
		if (roota->rank > rootb->rank)
		{
			rootb->pParent = roota;
		}
		else if (roota->rank == rootb->rank)
		{
			rootb->pParent = roota;
			++roota->rank;
		}
		else
		{
			roota->pParent = rootb;
		}
	}
};
