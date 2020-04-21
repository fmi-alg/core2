#pragma once
#include "Box.h"


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
//		cout << "UnionFind.h-24\n";
//		cout << set->pBox->xi[0]<<"\n";
//		cout << set->pParent->pBox->xi[0]<<"\n";
//		if(!set || !set->pParent){
//			return set;
//		}
		if (set->pParent == set)
		{
			return set;
		}
//		cout << "UnionFind.h-29\n";
		set->pParent = pathCompress(set->pParent);
//		cout << "UnionFind.h-31\n";
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
//		cout << "UnionFind.h-42\n";
		Set* roota = Find(a)->pSet;
//		cout << "UnionFind.h-44\n";
		Set* rootb = Find(b)->pSet;
//		cout << "UnionFind.h-46\n";
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
