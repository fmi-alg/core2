#include "definitions.h"

/*
Freelist::Freelist(int size)
{
head = (Freenode *) NULL;
nodesize = size;
}

void *
Freelist::getfree()
{
void *t;
if (head == (Freenode *) NULL)  t = new char[nodesize];
			else {  t = (void *)head;
				head = head -> nextfree;
			     }
return t;
}


void
Freelist::makefree(void* curr)
{
Freenode *newhead = (Freenode *)curr;
newhead -> nextfree = head;
head = newhead;
}

*/
void moreSites(Site **s, unsigned current, unsigned additional)
 { std::cerr << "Sorry, moreSites is unimplemented\n";
   exit(0);
 }

/*
Freelist SiteRep::flist(sizeof(SiteRep));
Freelist EdgeRep::flist(sizeof(EdgeRep));
Freelist HalfedgeRep::flist(sizeof(HalfedgeRep));*/
