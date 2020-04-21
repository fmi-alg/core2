#include "Corner.h"
#include "Wall.h"
#include "Box.h"

bool Corner::isConvex()
{
    if(isDangling()) return true;

	Corner* c = this->nextWall->dst;
	return !this->preWall->isRight(c->x, c->y);
}


bool Corner::inZone(Box * box)
{
    double x=box->x;
    double y=box->y;
    double w2=box->width/2;  //half of width
    double h2=box->height/2; //half of height

    double corner1[2]={x-w2,y-h2};
    double corner2[2]={x+w2,y-h2};
    double corner3[2]={x+w2,y+h2};
    double corner4[2]={x-w2,y+h2};

    //check with the Zone of the previous wall
    short ps1, ps2, ps3, ps4;

    if(preWall!=NULL)
    {
        ps1=preWall->distance_sign(corner1[0],corner1[1]);
        ps2=preWall->distance_sign(corner2[0],corner2[1]);
        ps3=preWall->distance_sign(corner3[0],corner3[1]);
        ps4=preWall->distance_sign(corner4[0],corner4[1]);
    }

    //check with the Zone of the next wall
    short ns1, ns2, ns3, ns4;
    if(nextWall!=NULL)
    {
        ns1=nextWall->distance_sign(corner1[0],corner1[1]);
        ns2=nextWall->distance_sign(corner2[0],corner2[1]);
        ns3=nextWall->distance_sign(corner3[0],corner3[1]);
        ns4=nextWall->distance_sign(corner4[0],corner4[1]);
    }

    if(preWall!=NULL && nextWall==NULL)
    {
        if( ps1==1 || ps2==1 || ps3==1 || ps4==1)
            return true;
        else
            return false;
    }
    else if(preWall==NULL && nextWall!=NULL)
    {
        if(ns1==-1 || ns2==-1 || ns3==-1 || ns4==-1)
            return true;
        else
            return false;
    }
    else
    {
        if( ( ps1==1 || ps2==1 || ps3==1 || ps4==1) && ( ns1==-1 || ns2==-1 || ns3==-1 || ns4==-1) )
            return true;
    }
    return false;	// Chee: moved this "return false" statement from inside
    			// the previous clause.   This is just to avoid the following 
			// compiler warning:
			// Corner.cpp:99: warning: control reaches end of non-void function
			//
}

bool Corner::inZone_star(Box * box)
{
    if(isConvex()==false) return false;
    return inZone(box);
}
