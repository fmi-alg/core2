#pragma once
#include "Feature.h"

//
// Set structure with basic union-find operations
//

class Set
{
public:
	Feature* pFeature;
	Set* pParent; //parent in union find
	int rank;

	Set(Feature* b): pFeature(b), rank(1)
	{
		pParent = this;
		b->pSet = this;
	}
};

class UnionFind
{

private:

	Set* pathCompress(Set* set) //find root
	{
		if (set->pParent == set) //root
		{
			return set;
		}

		set->pParent = pathCompress(set->pParent); //set parent as root (path compress)
		return set->pParent;
	}

public:

	Feature* Find(Feature* b)
	{
		Set* root = pathCompress(b->pSet);
		return root->pFeature;
	}

	void Union(Feature*a, Feature* b)
	{

		//find
		Set* roota = Find(a)->pSet;
		Set* rootb = Find(b)->pSet;

		//union
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
