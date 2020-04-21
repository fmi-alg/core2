/*
 *  Corner.cpp
 *  
 *  Author: Ching-Hsiang (Tom) Hsu
 *  Email: chhsu@nyu.edu
 *  Modified: Ching-Hsiang (Tom) Hsu, Oct. 31, 2016
 */


#include "Corner.h"
#include "Wall.h"
#include "Box.h"
#include <iostream>

bool Corner::isConvex()
{
//	std::cout<<"isConvex 8"<<endl;
//	std::cout<<this->nextWall<<endl;
    Corner* c = this->nextWall->dst;
//	std::cout<<"isConvex 10"<<endl;
    return !this->preWall->isRight(c->x, c->y);
}


bool Corner::inZone(Box * box)
{
    if(box->in(this->x,this->y)) return true;

    double x=box->x;
    double y=box->y;

    double w2=box->width/2;  //half of width
    double h2=box->height/2; //half of height

    double corner1[2]={x-w2,y-h2};
    double corner2[2]={x+w2,y-h2};
    double corner3[2]={x+w2,y+h2};
    double corner4[2]={x-w2,y+h2};

    //check with the Zone of the previous wall
    short ps1=0, ps2=0, ps3=0, ps4=0;

    if(preWall!=NULL)
    {
        ps1=preWall->distance_sign(corner1[0],corner1[1]);
        ps2=preWall->distance_sign(corner2[0],corner2[1]);
        ps3=preWall->distance_sign(corner3[0],corner3[1]);
        ps4=preWall->distance_sign(corner4[0],corner4[1]);
    }

    //check with the Zone of the next wall
    short ns1=0, ns2=0, ns3=0, ns4=0;
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
        if( ( ps1==1 && ns1==-1) ) return true;
        if( ( ps2==1 && ns2==-1) ) return true;
        if( ( ps3==1 && ns3==-1) ) return true;
        if( ( ps4==1 && ns4==-1) ) return true;

        if( ( ps1==1 || ps2==1 || ps3==1 || ps4==1) && ( ns1==-1 || ns2==-1 || ns3==-1 || ns4==-1) )
        {
            //now we have a box crossing the zone from either above or below...
            //need to make sure the box is crossing from above.
            double dx_pre=preWall->dst->x-preWall->src->x;
            double dy_pre=preWall->dst->y-preWall->src->y;
            double dx_nex=nextWall->dst->x-nextWall->src->x;
            double dy_nex=nextWall->dst->y-nextWall->src->y;

            double nx_pre=dy_pre;
            double ny_pre=-dx_pre;
            double nx_nex=dy_nex;
            double ny_nex=-dx_nex;
            double vx=x-this->x;
            double vy=y-this->y;
            double dot_x=(nx_pre+nx_nex)*vx;
            double dot_y=(ny_pre+ny_nex)*vy;
            double dot=dot_x+dot_y;
            if(isConvex()==false) dot=-dot;

            if(dot>0){
                return true;
            }

            return false;
        }
        return false;
    }

    return false;
}

bool Corner::inZone_star(Box * box)
{
    if(isConvex()==false) return false;
    return inZone(box);
}

bool Corner::inZone(double x2, double y2)
{
    if(preWall!=NULL) if(preWall->distance_sign(x2,y2)!=1) return false;
    if(nextWall!=NULL) if(nextWall->distance_sign(x2,y2)!=-1 ) return false;
    return true;
}

bool Corner::inZone_star(double x2, double y2)
{
//	std::cout<<"inZone_star 116"<<endl;
    if(isConvex()==false) {
//		std::cout<<"inZone_star 119"<<endl;
        return false;
    }
//	std::cout<<"inZone_star 121"<<endl;
    return inZone(x2,y2);
}

