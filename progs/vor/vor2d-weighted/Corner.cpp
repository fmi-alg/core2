#include "Corner.h"
#include "Wall.h"
#include "Box.h"

bool Corner::isConvex()
{
    if(isDangling()) return true;

	Corner* c = this->nextWall->dst;
	return !this->preWall->isRight(c->pos);
}

bool Corner::inZone(Box * box)
{
    if(box->in(this->pos)) return true;

    //center o
    const Point2d& o=box->o;
    double w2=box->width/2;  //half of width
    double h2=box->height/2; //half of height

    Point2d corner1(o[0]-w2,o[1]-h2);
    Point2d corner2(o[0]+w2,o[1]-h2);
    Point2d corner3(o[0]+w2,o[1]+h2);
    Point2d corner4(o[0]-w2,o[1]+h2);

    //check with the Zone of the previous wall
    short ps1=0, ps2=0, ps3=0, ps4=0;

    if(preWall!=NULL)
    {
        ps1=preWall->distance_sign(corner1);
        ps2=preWall->distance_sign(corner2);
        ps3=preWall->distance_sign(corner3);
        ps4=preWall->distance_sign(corner4);
    }

    //check with the Zone of the next wall
    short ns1=0, ns2=0, ns3=0, ns4=0;
    if(nextWall!=NULL)
    {
        ns1=nextWall->distance_sign(corner1);
        ns2=nextWall->distance_sign(corner2);
        ns3=nextWall->distance_sign(corner3);
        ns4=nextWall->distance_sign(corner4);
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
            Vector2d v_pre=preWall->dst->pos-preWall->src->pos;
            Vector2d v_nex=nextWall->dst->pos-nextWall->src->pos;
            Vector2d n_pre(v_pre[1],-v_pre[0]);
            Vector2d n_nex(v_pre[1],-v_pre[0]);
            Vector2d v=box->o-pos;
            double dot=(n_nex+n_pre)*v;

//            double dx_pre=preWall->dst->x-preWall->src->x;
//            double dy_pre=preWall->dst->y-preWall->src->y;
//            double dx_nex=nextWall->dst->x-nextWall->src->x;
//            double dy_nex=nextWall->dst->y-nextWall->src->y;

//            double nx_pre=dy_pre;
//            double ny_pre=-dx_pre;
//            double nx_nex=dy_nex;
//            double ny_nex=-dx_nex;

//            double vx=x-this->x;
//            double vy=y-this->y;
//            double dot_x=(nx_pre+nx_nex)*vx;
//            double dot_y=(ny_pre+ny_nex)*vy;
//            double dot=dot_x+dot_y;

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

bool Corner::inZone(const Point2d& p)
{
    if(preWall!=NULL) if(preWall->distance_sign(p)!=1) return false;
    if(nextWall!=NULL) if(nextWall->distance_sign(p)!=-1 ) return false;
    return true;
}

bool Corner::inZone_star(const Point2d& p)
{
    if(isConvex()==false) return false;
    return inZone(p);
}

