#include "Box.h"
#include <assert.h>
#include <algorithm>

extern int vor_build_option; //TODO: defined in vor2d.cpp, find a way to remove this

#define CKY_SIMPLIFIED 0

//-----------------------------------------------------------------------------
//
// class BoxIter
//
// Box iterator
//
//-----------------------------------------------------------------------------

BoxIter::BoxIter(const Box* bb, int direc):b(bb), direction(direc), neighbor(0)
{
	prev = (direc + 3) % 4;
	next = (direc + 1) % 4;
	cross = (direc + 2) % 4;
}

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

//-----------------------------------------------------------------------------
//
// class Box
//
// keep these as options
//
// (1) pseudo ( cl(m)>r_B && inseparable )
// (2) internal medial axis
//
//-----------------------------------------------------------------------------

int Box::counter = 0;

//
// Methods for accessing data in this box
//
void Box::getCorners(Point2d& UL, Point2d& UR, Point2d& LR, Point2d& LL )
{
    double w2=width/2;
    double h2=height/2;
    double x_l=o[0]-w2;
    double x_r=o[0]+w2;
    double y_l=o[1]-h2;
    double y_u=o[1]+h2;

    UL.set(x_l, y_u);
    LL.set(x_l, y_l);
    UR.set(x_r, y_u);
    LR.set(x_r, y_l);
}

void Box::getCorners(BoxNode& UL, BoxNode& UR, BoxNode& LR, BoxNode& LL )
{
    getCorners(UL.pos,UR.pos,LR.pos,LL.pos);
}

//
// determine the status of the box: UNKNOWN, IN, OUT, ON
//

void Box::updateStatus()
{
    if (status != UNKNOWN)
    {
        return;
    }

    int total_feature_size=corners.size()+walls.size();

    int sfc=separable_features_count();
    bool separable=(sfc>1);
    if(total_feature_size<=6)
        total_feature_size=sfc;

    //C_0
    if(total_feature_size>3){
        status = IN; //need more split
        return;
    }

    if(total_feature_size<=1){
        status = OUT;
        return;
    }

    //C_1
    if(c1){ //new definition
        if(cl_m<rB)
        {
            //then the feature is either inseparable
            if( separable==false ){
                status = OUT;
            }
            else //or has segments that may split the cell
            {
                //we only need to look at walls
                if(corners.empty() && walls.size()==2)
                {
                    //overlapping wall
                    if(walls.front()->src==walls.back()->dst && walls.front()->dst==walls.back()->src)
                        status = OUT;
                    else
                    {
                        //check if walls are facing the opposing directions
                        Wall * w1=walls.front();
                        Wall * w2=walls.back();

                        //check if the lines are opposing...
                        bool r1=w1->isRight(w2->src->pos);
                        bool r2=w1->isRight(w2->dst->pos);
                        bool r3=w2->isRight(w1->src->pos);
                        bool r4=w2->isRight(w1->dst->pos);

                        if(!r1 && !r2 && !r3 && !r4){ //opposing
                            status = OUT;
                        }
                        else //facing each other
                            status = ON;
                    }
                }
                else
                    status = IN; //need more split
            }
            return;
        }
    }
    else {//old definition
        if( (cl_m<rB) && separable ){
            status = IN; //need more split
            return;
        }
    }


    //find actual features of the box nodes
    BoxNode UL, LL, UR, LR; //upper left, lower left, upper right, lower right
    getCorners(UL,UR,LR,LL);

    determine_clearance(UL);
    determine_clearance(LL);
    determine_clearance(UR);
    determine_clearance(LR);

    //determine the status: either OUT or ON or Tricky
    set<Feature*>   feature_groups;

    if(UL.nearest_feature)
        feature_groups.insert(UnionFind().Find(UL.nearest_feature));

    if(LL.nearest_feature)
        feature_groups.insert(UnionFind().Find(LL.nearest_feature));

    if(UR.nearest_feature)
        feature_groups.insert(UnionFind().Find(UR.nearest_feature));

    if(LR.nearest_feature)
        feature_groups.insert(UnionFind().Find(LR.nearest_feature));


    //C_2
    if(c2) //if c2 predicate flag is on
    {
        //check if the condition is special or not
        if(feature_groups.size()==1 && total_feature_size>1)
        {
            Wall * wall=dynamic_cast<Wall*>(*feature_groups.begin());

            if( wall!=NULL && corners.empty()==false && (cl_m<rB*rB) )
            {
                status = IN; //need more split
                return;
            }
        }
    }

    if(separable==false || feature_groups.size()<=1 )
    {
        status = OUT;
        return;
    }

    status = ON;

    return;
}

//
void Box::distribute_features2box(Box * child)
{
    typedef list<Wall*>::iterator   WIT;
    typedef list<Corner*>::iterator CIT;

    //center x,y
    //double x=child->x;
    //double y=child->y;
    const Point2d& c_o=child->o;

    //clearance+2*radius of the box
    double cl2r=child->rB*2+child->cl_m; //clearance + 2*rB

    //
    //compute the separation to walls
    for (WIT iterW=walls.begin(); iterW != walls.end(); ++iterW)
    {
        Wall* w = *iterW;
        double dist = w->distance(c_o); //w->distance_star(x, y); //w->distance(x, y);

        if (dist < cl2r) //within the distance range
        {
            {
                bool zone=w->inZone_star(child); //true; //w->inZone(child); //w->inZone_star(child);
                if(zone)
                    child->walls.push_back(w);
            }
        }//end if
    }//end for

    //compute the separation to corners
    for (CIT iterC=corners.begin(); iterC != corners.end(); ++iterC)
    {
        Corner* c = *iterC;
        double dist = c->distance(c_o);
        //OK, close enough
        if (dist < cl2r)
        {
            {
                //check with the Zone of the previous wall
                bool zone=c->inZone_star(child); //true; //c->inZone(child); //c->inZone_star(child);
                if(zone)
                    child->corners.push_back(c);
            }
        }//end if
    }//end for

//        return;

    //
    //check the closest features of the box corner
    //
    //the closest feature of the box corner should be inside the features
    //of this box
    //
    {

        BoxNode mid;
        mid.pos=c_o;
        determine_clearance(mid);
        if(mid.nearest_feature==NULL) return;

        list<Feature*> features;
        features.insert(features.end(),child->corners.begin(), child->corners.end());
        features.insert(features.end(),child->walls.begin(), child->walls.end());
        if( std::find(features.begin(), features.end(), mid.nearest_feature)==features.end() )
        {
            //some closest feature is lost....
            //this means the box is not in the zone of the closest feature....
            child->corners.clear();
            child->walls.clear();
            return;
        }
    }
}


void Box::determine_clearance(BoxNode& node)
{
    //double x=node.x;
    //double y=node.y;
    const Point2d& np=node.pos;

    //compute the closest wall
    Wall* nearestWall=NULL;
    double mindistW=FLT_MAX;
    for (list<Wall*>::iterator iterW = walls.begin(); iterW != walls.end(); ++iterW)
    {
        Wall* w = *iterW;

        double dist = w->distance(np);

        if( fabs(dist-mindistW)<1e-10 )
        {
            if( w->distance_sign(np)==0 && w->isRight(np) ) //in zone
                nearestWall = *iterW;
        }
        else if (dist < mindistW) //shorter distance
        {
            mindistW = dist;
            nearestWall = *iterW;
        }
    }

    //
    // compute a closest corner that is closer than  (mindistW +1) (?? why +1)
    //
    double mindistC = FLT_MAX; //mindistC may not exist, so init to a bigger number
    Corner* nearestCorner = NULL;
    for (list<Corner*>::iterator iterC = corners.begin(); iterC != corners.end(); ++iterC)
    {
        Corner* c = *iterC;
        double dist = c->distance(np);

        if( fabs(dist-mindistC)<1e-5 ) //if(dist == mindistC)
        {
            if( c->inZone_star(np) ) //in the zone
            {
                nearestCorner = *iterC;
            }

        }
        else if (dist < mindistC)  //shorter distance
        {
            mindistC = dist;
            nearestCorner = *iterC;
        }
    }

    //
    // determine the feature and shortest
    //

    bool in_zone_w=false;
    bool in_zone_c=false;

    if(nearestWall!=NULL) in_zone_w=nearestWall->inZone_star(np);
    if(nearestCorner!=NULL) in_zone_c=nearestCorner->inZone_star(np);

    if( fabs(mindistW-mindistC)<1e-5 )
    {
        if(in_zone_w){
            node.clearance=mindistW;
            node.nearest_feature=nearestWall;
        }

        if(in_zone_c){
            node.clearance=mindistC;
            node.nearest_feature=nearestCorner;
        }
    }
    else if (mindistW < mindistC)
    {
        node.clearance=mindistW;
        if(nearestWall!=NULL){
            node.nearest_feature=nearestWall;
        }
    }
    else //mindistW > mindistC
    {
        node.clearance=mindistC;
        if(nearestCorner!=NULL){
            node.nearest_feature=nearestCorner;
        }
    }
    //
    //-----------------------------------
    //
}

/*
//
// find the nearest feature and the clearance
// results are stored in "node"
//
void Box::determine_clearance(BoxNode& node)
{

    double x=node.x;
    double y=node.y;

    //compute the closest wall
    Wall* nearestWall=NULL;
    double mindistW=FLT_MAX;
    for (list<Wall*>::iterator iterW = walls.begin(); iterW != walls.end(); ++iterW)
    {
        Wall* w = *iterW;
        double dist = w->distance_star(x, y); //w->distance_star(x, y); //w->distance(x, y);
        if( fabs(dist-mindistW)<1e-10 )
        {
            if( w->distance_sign(x,y)==0 && w->isRight(x,y) )
                nearestWall = *iterW;
        }
        else if (dist < mindistW) //shorter distance
        {
            mindistW = dist;
            nearestWall = *iterW;
        }
    }

    //
    // compute a closest corner that is closer than  (mindistW +1) (?? why +1)
    //
    double mindistC = FLT_MAX; //mindistC may not exist, so init to a bigger number
    Corner* nearestCorner = NULL;
    for (list<Corner*>::iterator iterC = corners.begin(); iterC != corners.end(); ++iterC)
    {
        Corner* c = *iterC;
        double dist = c->distance(x, y);

        if( fabs(dist-mindistC)<1e-5 ) //if(dist == mindistC)
        {
            if( c->inZone_star(x,y) )
            {
                //in the zone
                nearestCorner = *iterC;
            }

        }
        else if (dist < mindistC)  //shorter distance
        {
            mindistC = dist;
            nearestCorner = *iterC;
        }
    }

    if (mindistW<=mindistC)
    {
        node.clearance=mindistW;
        if(nearestWall!=NULL){
            node.nearest_feature=nearestWall;
        }
    }
    else
    {
        node.clearance=mindistC;
        if(nearestCorner!=NULL){
            node.nearest_feature=nearestCorner;
        }
    }
}
*/


//  split (eps)
//
//  --returns false if we fail to split for some reason
//
bool Box::split(double epsilon, double maxEpsilon)
{
    if (this->height < epsilon || this->width < epsilon)
    {
        return false;
    }


    //if ((!this->isLeaf || this->status != IN)
//
// Chee: "status != IN"
//
//  is equiv. to
//
//  "status == OUT || status == UNKNOWN || status == ON".
//
//  But we weaken the ON part, by adding a maxEpsilon criterion:
//
    if ( (!this->isLeaf || this->status == OUT || this->status == UNKNOWN)
    || (this->status == ON  && this->height < maxEpsilon   && this->width < maxEpsilon) )
    {
        return false;
    }

    //record the time of this split event, will be used to set priority of children
    ++Box::counter;
    Box* children[4];
    double child_w=width/2;
    double child_h=height/2;
    double x_l=o[0]-width/4;
    double x_r=o[0]+width/4;
    double y_l=o[1]-height/4;
    double y_u=o[1]+height/4;

    children[0] = new Box( Point2d(x_l, y_u), child_w, child_h);
    children[1] = new Box( Point2d(x_r, y_u), child_w, child_h);
    children[2] = new Box( Point2d(x_r, y_l), child_w, child_h);
    children[3] = new Box( Point2d(x_l, y_l), child_w, child_h);

    for (int i = 0; i < 4; ++i)
    {
        if(children[i]==NULL)
        {
            cerr<<"! Error: Box::split: Not enough memory"<<endl;
            return false;
        }

        children[i]->depth = this->depth + 1;
        BoxNode node;
        node.pos=children[i]->o;
        determine_clearance(node);
        children[i]->cl_m=node.clearance;

        //distribute the feature
        distribute_features2box(children[i]);
    }

    //
    // for internal nodes, children [i] is the i-th child
    // for leaves, children [i] is the pointer to first node in i-th adj list
    //
    for (int i = 0; i < 4; ++i)
    {
        //find three other directions
        int prev = (i + 3) % 4;
        int next = (i + 1) % 4;
        int cross = (i + 2) % 4;

        //update easy cases
        children[i]->pChildren[i] = pChildren[i];
        children[i]->pChildren[next] = children[next];
        children[i]->pChildren[cross] = children[prev];

        //init box neighbor iterator for direction i
        BoxIter* iter = new BoxIter(this, i);
        Box* neighbor = iter->First();

        if (!neighbor)
        {
            continue;
        }

        // if neighbor are no smaller
        if (neighbor->depth <= this->depth)
        {
            //after split child 'next' should also point to
            //neighbor in direction i
            children[next]->pChildren[i] = neighbor;

            //if neighbor's cross direction point to this, it should
            //instead point to child 'next' after split
            if (neighbor->pChildren[cross] == this)
            {
                neighbor->pChildren[cross] = children[next];
            }
            continue;
        }

        Box* prevNeighbor = neighbor;

        //indicate if we go across the boundary between child 'i'
        //and 'next' the first time
        bool firstTimeCrossBetweenChildren = true;

        //if neighbor smaller
        while(neighbor != iter->End())
        {
            //within the strip of child next, neighbor's cross direction
            //should point to next
            if (!isOverLimit(children[next], neighbor))
            {
                neighbor->pChildren[cross] = children[next];
            }

            //within the strip of child i, neighbor's cross
            //direction should point to i
            else if (!isOverLimit(children[i], neighbor))
            {
                neighbor->pChildren[cross] = children[i];

                //first time cross between child i and next,
                //should update next's i direction pointer
                if (firstTimeCrossBetweenChildren)
                {
                    firstTimeCrossBetweenChildren = false;
                    children[next]->pChildren[i] = prevNeighbor;
                }
            }
            else
            {
                assert(0);
            }
            prevNeighbor = neighbor;
            neighbor = iter->Next();
        }
    }


    //setup the children/parent relation
    for (int i = 0; i < 4; ++i)
    {
        this->pChildren[i] = children[i];
        this->pChildren[i]->pParent = this;
    }

    this->isLeaf = false;

    return true;
}

//
// determine if the corners and walls in this class are separable
// return true if separable
// otherwise return false
//
int Box::separable_features_count(list<Wall*>& walls, list<Corner*>& corners)
{
    list<Feature*> insep_features;
    getInseparableFeatures(walls,corners,insep_features);

    //done
    return insep_features.size();
}

//same as above but using the features in this class
int Box::separable_features_count()
{
    return separable_features_count(walls,corners);
}

//
// given a set of features (both corner and wall)
// determine a set of inseparable features
//
// Note: after this call, pSet of features (Wall or Corner) will point to the root of the set
//
void Box::getInseparableFeatures(list<Wall*>& walls, list<Corner*>& corners, list<Feature*>& insep)
{
     static list<Set*> features;

     //previously used sets, remove and clean them
     if(features.empty()==false)
     {
         for(list<Set*>::iterator i=features.begin();i!=features.end();i++)
         {
             Set * s=*i;
             s->pFeature->pSet=NULL;
             delete s;
         }//end for i
         features.clear();
     }

     //now start building the new sets
     typedef list<Wall*>::iterator WIT;
     typedef list<Corner*>::iterator CIT;

     //create a set for each wall
     for (WIT it = walls.begin(); it != walls.end(); it++){
         Set* set=new Set((Feature*)*it);
         assert(set);
         features.push_back(set);
     }//end for

     //create a set for each corner
     for (CIT it = corners.begin(); it != corners.end(); it++){
         Set* set=new Set((Feature*)*it);
         assert(set);
         features.push_back(set);
     }//end for

     UnionFind UF;

     //start to merge
     for (WIT it = walls.begin(); it != walls.end(); it++){
         Wall* w=*it;
         if(w->dst->pSet!=NULL) UF.Union(w,w->dst);
         if(w->src->pSet!=NULL) UF.Union(w,w->src);
     }//end for

     for (CIT it = corners.begin(); it != corners.end(); it++){
         Corner* c=*it;
         if(c->preWall)  if(c->preWall->pSet!=NULL) UF.Union(c,c->preWall);
         if(c->nextWall) if(c->nextWall->pSet!=NULL) UF.Union(c,c->nextWall);
     }//end for

     //now check the number of Sets
     set<Feature*> featureset;
     for (WIT it = walls.begin(); it != walls.end(); it++){
         featureset.insert(UF.Find(*it));
     }//end for

     for (CIT it = corners.begin(); it != corners.end(); it++){
         featureset.insert(UF.Find(*it));
     }//end for

     //remember the result
     insep.clear();
     insep.insert(insep.end(),featureset.begin(),featureset.end());
}

bool Box::in(const Point2d& p)
{
    double w2=width/2;
    double h2=height/2;

    if( p[0]<o[0]-w2 || p[0]>o[0]+w2 ) return false;
    if( p[1]<o[1]-h2 || p[1]>o[1]+h2) return false;
    return true;
}

Box * Box::find(const Point2d& p)
{
    if(isLeaf) return this;

    for(int i=0;i<4;i++){
        if(pChildren[i]->in(p))
            return pChildren[i]->find(p);
    }

    return NULL;
}

//recursively obtain all leaves of this box
void Box::getLeaves(list<Box*>& leaves)
{
    if(isLeaf){
        leaves.push_back(this);
        return;
    }

    for(int i=0;i<4;i++) pChildren[i]->getLeaves(leaves);
}


int Box::feature_count(Feature* f, list<Feature*>& all_features)
{
    typedef list<Feature*>::iterator IT;
    int count=0;
    for(IT i=all_features.begin();i!=all_features.end();i++){
        if( (*i)==f ) count++;
    }
    return count;
}

VorSegment Box::createVorSegment(const Point2d& n1, const Point2d& n2)
{
    VorSegment seg;
    seg.p=n1;
    seg.q=n2;
    return seg;
}

//see either cutBy_simplified or cutBy_complex
char Box::cutBy(Feature * f, Feature * g, const Point2d& n1, const Point2d& n2)
{
#if CKY_SIMPLIFIED
    return cutBy_simplified(f,g,n1,n2);
#else
    return cutBy_complex(f,g,n1,n2);
#endif
}

//
// check if the edge (n1,n2) is cut by the separator of features f and g
//
// this function is correct unless (1) f is an edge and g is a corner and
// (2) n1 and n2 are closer to f but the separator still cuts through the
// interior of the edge
//
// This case is prevented by predicate C3
//
// return value:
//  '0': the separator does not touch (n1,n2)
//  '1': the separator intersects the interior of (n1,n2) in the regular way
//  'p': the separator go through both n1 and n2
//  's':the separator go through n1 (s)
//  't':the separator go through n2 (t)
//
char Box::cutBy_complex(Feature * f, Feature * g, const Point2d& n1, const Point2d& n2)
{
    f=closestInISF(f,n1,n2);
    g=closestInISF(g,n1,n2);

    double d_n1f=f->distance(n1);
    double d_n1g=g->distance(n1);
    double d_n2f=f->distance(n2);
    double d_n2g=g->distance(n2);

    if(d_n1f<d_n1g && d_n2f<d_n2g) return '0'; //n1 is closer to f && n2 is also closer to f
    if(d_n1g<d_n1f && d_n2g<d_n2f) return '0'; //n1 is closer to g && n2 is also closer to g

    if(d_n1f==d_n1g && d_n2f==d_n2g) return 'p'; //the separator is aligned with n1 and n2...

    if(d_n1f==d_n1g) return 's'; //the separator is go through n1...

    if(d_n1f==d_n1g) return 't'; //the separator is go through n2...

    return '1'; //regular case
}

//
// check if the edge (n1,n2) is cut by the separator of features f and g
//
// this function is correct unless (1) f is an edge and g is a corner and
// (2) n1 and n2 are closer to f but the separator still cuts through the
// interior of the edge
//
// This case is prevented by predicate C3
//
// return value:
//  '0': the separator does not touch (n1,n2)
//  '1': the separator intersects the interior of (n1,n2) in the regular way
//
char Box::cutBy_simplified(Feature * f, Feature * g, const Point2d& n1, const Point2d& n2)
{
    f=closestInISF(f,n1,n2);
    g=closestInISF(g,n1,n2);

    double d_n1f=f->distance(n1);
    double d_n1g=g->distance(n1);
    double d_n2f=f->distance(n2);
    double d_n2g=g->distance(n2);

    if(d_n1f<d_n1g && d_n2f<d_n2g) return '0'; //n1 is closer to f && n2 is also closer to f
    if(d_n1g<d_n1f && d_n2g<d_n2f) return '0'; //n1 is closer to g && n2 is also closer to g

    return '1';
}

void Box::printBuildVorWarning(short typeCount[5])
{
    // see the return code from Box::cutBy
    cerr<<"! WARNING: Case not analyzed: "
        <<" 0:"<<typeCount[0]
        <<" 1:"<<typeCount[1]
        <<" p:"<<typeCount[2]
        <<" s:"<<typeCount[3]
        <<" t:"<<typeCount[4]<<endl;
}

//
// build edge segments of this box sharing with its neighbors in quadtree
//
int Box::buildBdSegs(vector<BdSeg> Bd[4])
{
    Point2d UL, LL, UR, LR; //Box corners, upper left, lower left, upper right, lower right
    getCorners(UL,UR,LR,LL);

    //go through each border border
    BdSeg seg;
    int count=0;
    for(int i=0;i<4;i++)
    {
        Point2d n1, n2, mid;
        if(i==0){
            n1=UL;
            n2=UR;
            mid.set(o[0],UL[1]);
        }
        else if(i==1){
            n1=UR;
            n2=LR;
            mid.set(UR[0],o[1]);
        }
        else if(i==2){
            n1=LR;
            n2=LL;
            mid.set(o[0],LR[1]);
        }
        else {
            n1=LL;
            n2=UL;
            mid.set(LL[0],o[1]);
        }

        if(pChildren[i]==NULL)
        {
            seg.n1=n1;
            seg.n2=n2;
            Bd[i].push_back(seg);
            count++;
        }
        else{
            if(pChildren[i]->depth>depth){
                seg.n1=n1;
                seg.n2=mid;
                Bd[i].push_back(seg);
                seg.n1=mid;
                seg.n2=n2;
                Bd[i].push_back(seg);
                count+=2;
            }
            else{
                seg.n1=n1;
                seg.n2=n2;
                Bd[i].push_back(seg);
                count++;
            }
        }
    }//end for i

    return count;
}

//
// build edge segments of this box sharing with its neighboring boxes with ON status
//
int Box::buildOnBdSegs(vector<BdSeg> Bd[4])
{
    list<Box*> nei_list[4];
    getAllNeighthbors(nei_list); //get a list of neighbors, one list from each side

    //go through each border border
    BdSeg seg;
    int count=0;
    for(int i=0;i<4;i++)
    {
        if(nei_list[i].empty()) //no neighbors
        {
            seg=getBdSeg(i);
            Bd[i].push_back(seg);
            count++;
        }
        else
        {
            int x=(i+2)%4; //opposing edge index

            for(list<Box*>::iterator j=nei_list[i].begin();j!=nei_list[i].end();j++)
            {
                Box * b=*j;
                if(b->status!=ON) continue;
                if(b->depth>depth)
                {
                    seg=b->getBdSeg(x);
                    swap(seg.n1,seg.n2);
                }
                else
                    seg=getBdSeg(i);
                Bd[i].push_back(seg);
                count++;
            }//end for j
        }
    }//end for i

    return count;
}


//
// An edge segments of this box, 0=N, 1=E, 2=S, 3=W
//
BdSeg Box::getBdSeg(short index)
{
    double w2=width/2;
    double h2=height/2;
    BdSeg seg;

    double x_l=o[0]-w2;
    double x_r=o[0]+w2;
    double y_l=o[1]-h2;
    double y_u=o[1]+h2;

    if(index==0){ //north
        seg.n1.set(x_l, y_u);
        seg.n2.set(x_r, y_u);
    }
    else if(index==1){ //east
        seg.n1.set(x_r, y_u);
        seg.n2.set(x_r, y_l);
    }
    else if(index==2){ //south
        seg.n1.set(x_r, y_l);
        seg.n2.set(x_l, y_l);
    }
    else { //west
        seg.n1.set(x_l, y_l);
        seg.n2.set(x_l, y_u);
    }

    return seg;
}

void Box::buildVor(Feature * f, Feature * g)
{
#if CKY_SIMPLIFIED
    buildVor_simplified(f,g);
#else
    buildVor_complex(f,g);
#endif
}

//
//build the Voronoi curve with 2 features
//
void Box::buildVor_simplified(Feature * f, Feature * g)
{
    vector<BdSeg> Bd[4];
    short typeCount[5]={0,0,0,0,0}; //'0', 'r', 'p', '1','2'

    int segSize=buildOnBdSegs(Bd);

    for(short i=0;i<4;i++)
    {
        for(short j=0;j<(short)Bd[i].size();j++)
        {
            BdSeg& seg=Bd[i][j];
            seg.type=cutBy(f,g,seg.n1,seg.n2);
            seg.approxX();
            switch(seg.type)
            {
                case '0': typeCount[0]++; break;
                case '1': typeCount[1]++; break;
                default: assert(0); break; //no such type
            }
        }//end for j
    }//end for i

    //no intersection at all
    if( typeCount[0]==segSize) return; //nothing to do

    // some boundary has no intersection and some edges has special intersection
    // i.e. the separator cuts through the corner of the box without going
    // into the interior of the box
    if(typeCount[0]!=0 && typeCount[3]+typeCount[4]+typeCount[0]==segSize)
    {
        return; //nothing to do
    }

    //the separator cuts through the corners of the box...
    //no regular cuts, 2 type '1' cuts and 2 type '2' cuts
    if(typeCount[1]==2)
    {
        list<Point2d> nodes;
        for(short i=0;i<4;i++)
        {
            for(short j=0;j<(short)Bd[i].size();j++)
            {
                BdSeg& seg=Bd[i][j];
                if(seg.type=='1'){
                    nodes.push_back(seg.x);
                }
            }//end j
        }//end i

        assert(nodes.size()==2);
        vor_segments.push_back(createVorSegment(nodes.front(),nodes.back()));

        return; //done
    }

    //
    printBuildVorWarning(typeCount);

    return; //done
}



//
//build the Voronoi curve with 2 features
//
void Box::buildVor_complex(Feature * f, Feature * g)
{
    vector<BdSeg> Bd[4];
    short typeCount[5]={0,0,0,0,0}; //'0', 'r', 'p', '1','2'

    int segSize=buildBdSegs(Bd);

    for(short i=0;i<4;i++)
    {
        for(short j=0;j<(short)Bd[i].size();j++)
        {
            BdSeg& seg=Bd[i][j];
            seg.type=cutBy(f,g,seg.n1,seg.n2);

            //this determines the voronoi node on the boundary of the box
            if(seg.type!='0')
            {
                //Feature * a=closestInISF(f,seg.n1,seg.n2);
                //Feature * b=closestInISF(g,seg.n1,seg.n2);
                bool r=buildVorNode(seg,f,g);
                if(!r)  seg.type='0';
            }

            //else if(seg.type=='s') seg.x=seg.n1;
            //else if(seg.type=='t') seg.x=seg.n2;

            //seg.approxX();
            switch(seg.type)
            {
                case '0': typeCount[0]++; break;
                case '1': typeCount[1]++; break;
                case 'p': typeCount[2]++; break;
                case 's': typeCount[3]++; break;
                case 't': typeCount[4]++; break;
                default: assert(0); break; //no such type
            }
        }//end for j
    }//end for i

    //no intersection at all
    if( typeCount[0]==segSize) return; //nothing to do

    // some boundary has no intersection and some edges has special intersection
    // i.e. the separator cuts through the corner of the box without going
    // into the interior of the box
    if(typeCount[0]!=0 && typeCount[3]+typeCount[4]+typeCount[0]==segSize)
    {
        return; //nothing to do
    }

    //the separator cuts through the border of the box...
    if(typeCount[2]!=0) //case 'p'
    {
        for(short i=0;i<4;i++)
        {
            for(short j=0;j<(short)Bd[i].size();j++)
            {
                BdSeg& seg=Bd[i][j];
                if(seg.type=='p'){
                    vor_segments.push_back(createVorSegment(seg.n1,seg.n2));
                }
            }//end j
        }//end i

        return; //done
    }


    //now we don't have 'p' cases
    //so the separator cuts through the corners of the box...
    list<Point2d> nodes;
    for(short i=0;i<4;i++)
    {
        for(short j=0;j<(short)Bd[i].size();j++)
        {
            BdSeg& seg=Bd[i][j];
            if(seg.type=='0') continue;
            if(nodes.empty())
                nodes.push_back(seg.x);
            else{
                if( (seg.x-nodes.back()).normsqr()<1e-10 ) continue;
                nodes.push_back(seg.x);
            }
        }//end j
    }//end i

    if(nodes.size()==1) return; //cut through the corner...

    if(nodes.size()>1 && (nodes.front()-nodes.back()).normsqr()<1e-10 ) nodes.pop_back();

    if(nodes.size()!=2)
    {
        cerr<<"! There are more than 2 Voronoi nodes from 2 features:"<<endl;
        cout<<"nodes.size()="<<nodes.size()<<endl;
        for(list<Point2d>::iterator i=nodes.begin();i!=nodes.end();i++)
            cout<<"pos="<<*i<<endl;

        for(short i=0;i<4;i++)
        {
            for(short j=0;j<(short)Bd[i].size();j++)
            {
                BdSeg& seg=Bd[i][j];
                cout<<"! "<<seg.n1<<","<<seg.n2<<" type="<<seg.type<<" x="<<seg.x<<endl;

            }//end for j

        }//end for i
    }

    vor_segments.push_back(createVorSegment(nodes.front(),nodes.back()));


    /*
    //no regular cuts, some 's' cuts and some 't' cuts
    if(typeCount[1]==0 && typeCount[3]>0 && typeCount[4]>0)
    {
        list<Point2d> nodes;
        for(short i=0;i<4;i++)
        {
            for(short j=0;j<(short)Bd[i].size();j++)
            {
                BdSeg& seg=Bd[i][j];
                nodes.push_back(seg.x);
            }//end j
        }//end i

        assert(nodes.size()==2);
        vor_segments.push_back(createVorSegment(nodes.front(),nodes.back()));

        return; //done
    }

    //2 regular cuts and segSize-2 type '0' cuts (ie. no cuts)
    if(typeCount[1]==2 && typeCount[0]==segSize-2)
    {
        list<Point2d> nodes;
        for(short i=0;i<4;i++)
        {
            for(short j=0;j<(short)Bd[i].size();j++)
            {
                BdSeg& seg=Bd[i][j];
                if(seg.type=='1'){
                    nodes.push_back(seg.x);
                }
            }//end j
        }//end i

        assert(nodes.size()==2);
        vor_segments.push_back(createVorSegment(nodes.front(),nodes.back()));

        return; //done
    }

    //
    printBuildVorWarning(typeCount);
*/

    return; //done

    /*
    //collect all intersections
    list<BoxNode> nodes;
    getBoundaryIntersections(f,g,nodes);

    if(nodes.empty()) return;

    //find two far-apart points and connect them...
    double max_d=-FLT_MAX;
    VorSegment seg;
    for(list<BoxNode>::iterator i=nodes.begin();i!=nodes.end();i++)
    {
        list<BoxNode>::iterator j=i;
        j++;
        for(;j!=nodes.end();j++){
            double dx=i->x-j->x;
            double dy=i->y-j->y;
            double d=dx*dx+dy*dy;
            if(d>max_d){
                max_d=d;
                seg.p[0]=i->x;
                seg.p[1]=i->y;
                seg.q[0]=j->x;
                seg.q[1]=j->y;
            }//end if
        }//end for j
    }//end for i

    //record the segment
    vor_segments.push_back(seg);
    //done
     */
}


//build Voronoi vertex from 3 features
Point2d Box::buildVorVertex(Feature * f, Feature * g, Feature * h)
{
    return o;

    //build a segment that is a bisector and then call buildVorNode
    BdSeg bisector;
    Point2d o;
    Vector2d v;
    Feature * f1=NULL;
    Feature * f2=NULL;

    if(isWall(f)==isWall(g)){
        getBisector(f,g,o,v);
        f1=f;
        f2=h;
    }
    else if(isWall(f)==isWall(h))
    {
        getBisector(f,h,o,v);
        f1=f;
        f2=g;
    }
    else //if(isWall(g)==isWall(h))
    {
        getBisector(g,h,o,v);
        f1=g;
        f2=f;
    }

    v=v.normalize();
    bisector.n1=o+v*1000;
    bisector.n2=o-v*1000;
    buildVorNode(bisector,f1,f2);
    return bisector.x;
}

// build a node (intersection between seg and Vor) from features f, g
bool Box::buildVorNode(BdSeg& seg, Feature * f, Feature * g)
{
    Point2d s=seg.n1;
    Point2d e=seg.n2;

    seg.type=cutBy(f,g,s,e); //check if the seg intersects the bisector of f,g

    if(seg.type=='s'){
        seg.x=seg.n1;
    }
    else if(seg.type=='t'){
        seg.x=seg.n2;
    }
    else
        if(seg.type=='1'){//find the intersection....

        bool found_intersection=false;
        Point2d x( (s[0]+e[0])/2, (s[1]+e[1])/2 );

        /*
        if(isWall(f)==isWall(g))
        {
            Point2d z;
            Vector2d bv;
            getBisector(f,g,z,bv);

            bv=bv.normalize();

            double a[2]={z[0]+bv[0]*10000,z[1]+bv[1]*10000}; //TODO: replace 100000 with the root box width/height
            double b[2]={z[0]-bv[0]*10000,z[1]-bv[1]*10000};
            double c[2]={seg.n1[0],seg.n1[1]};
            double d[2]={seg.n2[0],seg.n2[1]};
            double p[2];
            char code=SegSegInt(c,d,a,b,p);
            if(code!='0') //so p is defined
            {
                x=p;
                //seg.x.set(p);
                //found_intersection=true;
            }
            //else return false;
        }
        */


        if(found_intersection==false) //more general case but slower...
        {

            //cout<<"====="<<endl;
            //
            do
            {
                double d_xf=f->distance(x);
                double d_xg=g->distance(x);
                //double d_xh=h->distance(x);

    //            if( min(d_xf,d_xg)>d_xh ) //the closest point is closer to h...
    //                return false;

                if( fabs(d_xf-d_xg)<1e-10 || (s-e).normsqr()<1e-10 )
                {
                    seg.x=x;
                    found_intersection=true;
                    break;
                }

                if( cutBy(f,g,s,x) != '0')
                {
                    e=x;
                }
                else if( cutBy(f,g,x,e) != '0')
                {
                    s=x;
                }
                else{
                    return false;
                    seg.x=x;
                    found_intersection=true;
                    break;
                }
                //else
                //    assert(false);

                x.set( (s[0]+e[0])/2, (s[1]+e[1])/2 );

            }
            while(true);
        }

        assert(found_intersection);

        //if(found_intersection==false) seg.approxX(); //failed to find intersection...
    }

    return true;
}


//
// build a node (intersection between seg and Vor) from features f, g, h
//
bool Box::buildVorNode(BdSeg& seg, Feature * f, Feature * g, Feature * h)
{
    //compute the node
    buildVorNode(seg,f,g);

    if(seg.type=='0') return false;

    //check if the intersection is further from h than f,g
    if(h->distance(seg.x)>=f->distance(seg.x))
    {

#if CKY_SIMPLIFIED
        seg.approxX(); //reset seg.x to approximate value
#endif

        return true;
    }

    return false; //no node
}

// build a Voronoi curve from features f, g, h
void Box::buildVorCurve(Feature * f, Feature * g, Feature * h)
{
    vector<BdSeg> Bd[4];
    buildOnBdSegs(Bd);
    int counter=0;
    for(short i=0;i<4;i++)
    {
        for(short j=0;j<(short)Bd[i].size();j++)
        {
            BdSeg& seg=Bd[i][j];
            if( buildVorNode(seg,f,g,h) ) counter++;
        }//end for j
    }//end for i

    if(counter>=2) buildVor(f,g);
}

//
// build Voronoi curves with 3 features
//
void Box::buildVor(Feature * f, Feature * g, Feature * h)
{
     char vvt_code=VVT(f,g,h);

     //yes, there is a vertex!
     if(vvt_code=='1')
     {
         vector<BdSeg> Bd[4];
         buildBdSegs(Bd);

         list<Point2d> nodes;
         for(short i=0;i<4;i++)
         {
             for(short j=0;j<(short)Bd[i].size();j++)
             {
                 BdSeg& seg=Bd[i][j];
                 if( buildVorNode(seg,f,g,h) )
                     nodes.push_back(seg.x);

                 if( buildVorNode(seg,g,h,f) )
                     nodes.push_back(seg.x);

                 if( buildVorNode(seg,h,f,g) )
                     nodes.push_back(seg.x);

             }//end for j
         }//end for i

         Point2d vv=buildVorVertex(f,g,h);

         //add voronoi curves between the computed nodes and the center of the box
         for(list<Point2d>::iterator j=nodes.begin();j!=nodes.end();j++)
         {
             VorSegment seg;
             seg.p=*j;
             seg.q=vv;
             vor_segments.push_back(seg);
         }//end for j
     }
     else
     {
         //no vertex in the box
         //build the Voronoi curves
         buildVorCurve(f,g,h);
         buildVorCurve(g,h,f);
         buildVorCurve(h,f,g);
     }
}


///
/// build Voronoi curves with more than 3 features
/// @param a list of inseparable features, containing more than 3 elements
///
void Box::buildVorDegenerate(const list<Feature*>& insep)
{
    cerr<<"! Error: buildVorDegenerate not implemented yet"<<endl;
    assert(false);
}

//build vor curves by calling either buildVor_VF or buildVor_CKY
void Box::buildVor()
{
    if(vor_build_option==0)
    {
        buildVor_CKY();
    }
    else{
        buildVor_VF();
    }
}


void Box::buildVor_CKY()
{
    if(status!=Box::ON) return;

    //this build sets of inseparable features
    list<Feature*> insep;
    getInseparableFeatures(walls, corners,insep);

    switch(insep.size())
    {
        case 0: return; //no vor features
        case 1: return; //no vor features
        case 2: buildVor(insep.front(),insep.back()); return;
        case 3: buildVor(insep.front(), *(++insep.begin()), insep.back()); return;
        default: //more than 3...
            buildVorDegenerate(insep);
            return;
    }

}

void Box::buildVor_VF()
{
    if(status!=Box::ON) return;

    UnionFind UF;

    //this build sets of inseparable features
    separable_features_count();

    //find actual features of the box nodes
    BoxNode UL, LL, UR, LR; //upper left, lower left, upper right, lower right
    getCorners(UL,UR,LR,LL);

    determine_clearance(UL);
    determine_clearance(LL);
    determine_clearance(UR);
    determine_clearance(LR);

    Feature * ulf=(UL.nearest_feature==NULL)?NULL:UF.Find(UL.nearest_feature);
    Feature * llf=(LL.nearest_feature==NULL)?NULL:UF.Find(LL.nearest_feature);
    Feature * urf=(UR.nearest_feature==NULL)?NULL:UF.Find(UR.nearest_feature);
    Feature * lrf=(LR.nearest_feature==NULL)?NULL:UF.Find(LR.nearest_feature);

    if(lrf==NULL || llf==NULL || urf==NULL || lrf==NULL)
        return;

    list<BoxNode> mids[4]; //confusing name, this is for storing the intersections
                           //between Vor and each edge of the box (north, east, south, west)

    Feature * mid_features[4]={NULL,NULL,NULL,NULL}; //closest feature of the mid pt of north, east, south, west edge
    list<Feature *> all_features;
    all_features.push_back(ulf);
    all_features.push_back(llf);
    all_features.push_back(urf);
    all_features.push_back(lrf);

    //north edge (top)
    if(ulf!=urf && ulf!=NULL && urf!=NULL)
    {
        BoxNode mid;
        mid.pos.set(o[0],o[1]+height/2);

        if(pChildren[0]==NULL)
            mids[0].push_back(mid);
        else{
            if(pChildren[0]->depth>depth){
                determine_clearance(mid);
                Feature * mf=(mid.nearest_feature==NULL)?NULL:UF.Find(mid.nearest_feature);
                mid_features[0]=mf;
                all_features.push_back(mf);
                if(ulf==mf && mf!=urf){
                    mid.pos[0]=o[0]+width/4;
                    mids[0].push_back(mid);
                }
                else if(ulf!=mf && mf==urf){
                    mid.pos[0]=o[0]-width/4;
                    mids[0].push_back(mid);
                }
                else{
                    //cout<<"More complicated case"<<endl;
                    mid.pos[0]=o[0]-width/4;
                    mids[0].push_back(mid);
                    mid.pos[0]=o[0]+width/4;
                    mids[0].push_back(mid);
                }
            }
            else{
                mids[0].push_back(mid);
            }
        }
    }

    //east edge (right)
    if(urf!=lrf && urf!=NULL && lrf!=NULL)
    {
        BoxNode mid;
        mid.pos.set(o[0]+width/2,o[1]);

        if(pChildren[1]==NULL)
            mids[1].push_back(mid);
        else{
            if(pChildren[1]->depth>depth){
                determine_clearance(mid);
                Feature * mf=(mid.nearest_feature==NULL)?NULL:UF.Find(mid.nearest_feature);
                mid_features[1]=mf;
                all_features.push_back(mf);
                if(urf==mf && mf!=lrf){
                    mid.pos[1]=o[1]-height/4;
                    mids[1].push_back(mid);
                }
                else if(urf!=mf && mf==lrf){
                    mid.pos[1]=o[1]+height/4;
                    mids[1].push_back(mid);
                }
                else{
                    //cout<<"More complicated case"<<endl;
                    mid.pos[1]=o[1]+height/4;
                    mids[1].push_back(mid);
                    mid.pos[1]=o[1]-height/4;
                    mids[1].push_back(mid);
                }
            }
            else{
                mids[1].push_back(mid);
            }
        }
    }

    //south edge (bottom)
    if(lrf!=llf && lrf!=NULL && llf!=NULL)
    {
        BoxNode mid;
        mid.pos[0]=o[0];
        mid.pos[1]=o[1]-height/2;
        if(pChildren[2]==NULL)
            mids[2].push_back(mid);
        else{
            if(pChildren[2]->depth>depth){
                determine_clearance(mid);
                Feature * mf=(mid.nearest_feature==NULL)?NULL:UF.Find(mid.nearest_feature);
                mid_features[2]=mf;
                all_features.push_back(mf);
                if(lrf==mf && mf!=llf){
                    mid.pos[0]=o[0]-width/4;
                    mids[2].push_back(mid);
                }
                else if(lrf!=mf && mf==llf){
                    mid.pos[0]=o[0]+width/4;
                    mids[2].push_back(mid);
                }
                else{
                    mid.pos[0]=o[0]+width/4;
                    mids[2].push_back(mid);
                    mid.pos[0]=o[0]-width/4;
                    mids[2].push_back(mid);
                }
            }
            else{
                mids[2].push_back(mid);
            }
        }
    }

    //west edge (left)
    if(llf!=ulf && llf!=NULL && ulf!=NULL)
    {
        BoxNode mid;
        mid.pos[0]=o[0]-width/2;
        mid.pos[1]=o[1];
        if(pChildren[3]==NULL)
            mids[3].push_back(mid);
        else{
            if(pChildren[3]->depth>depth){
                determine_clearance(mid);
                Feature * mf=(mid.nearest_feature==NULL)?NULL:UF.Find(mid.nearest_feature);
                mid_features[3]=mf;
                all_features.push_back(mf);
                if(llf==mf && mf!=ulf){
                    mid.pos[1]=o[1]+height/4;
                    mids[3].push_back(mid);
                }
                else if(llf!=mf && mf==ulf){
                    mid.pos[1]=o[1]-height/4;
                    mids[3].push_back(mid);
                }
                else{
                    mid.pos[1]=o[1]-height/4;
                    mids[3].push_back(mid);
                    mid.pos[1]=o[1]+height/4;
                    mids[3].push_back(mid);
                }
            }
            else{
                mids[3].push_back(mid);
            }
        }//end if
    }

    //number of nodes
    int degree=mids[0].size()+mids[1].size()+mids[2].size()+mids[3].size();

    if(degree==3)
    {
        for(int i=0;i<4;i++)
        {
            for(list<BoxNode>::iterator j=mids[i].begin();j!=mids[i].end();j++)
            {
                VorSegment seg;
                seg.p[0]=j->pos[0];
                seg.p[1]=j->pos[1];
                seg.q[0]=o[0];
                seg.q[1]=o[1];
                vor_segments.push_back(seg);
            }//end for j
        }//end for i
    }
    else if(degree==2)
    {
        list<BoxNode> nodes;
        for(int i=0;i<4;i++) nodes.insert(nodes.end(),mids[i].begin(),mids[i].end());
        VorSegment seg;
        seg.p=nodes.front().pos;
        seg.q=nodes.back().pos;
        vor_segments.push_back(seg);
    }
    else
    {
        //connect the points
        //check upper-left corner
        int ul_fc=feature_count(ulf,all_features);
        if(mid_features[3]==ulf) ul_fc--;
        if(mid_features[0]==ulf) ul_fc--;

        if( ul_fc==1 ){ //unique
            VorSegment seg;
            BoxNode n3=(mid_features[3]==ulf)?mids[3].front():mids[3].back();
            BoxNode n0=(mid_features[0]==ulf)?mids[0].back():mids[0].front();
            seg.p=n3.pos;
            seg.q=n0.pos;
            vor_segments.push_back(seg);
        }

        //connect the points
        //check upper-right corner
        int ur_fc=feature_count(urf,all_features);
        if(mid_features[0]==urf) ur_fc--;
        if(mid_features[1]==urf) ur_fc--;

        if( ur_fc==1 ){
            VorSegment seg;
            BoxNode n0=(mid_features[0]==urf)?mids[0].front():mids[0].back();
            BoxNode n1=(mid_features[1]==urf)?mids[1].back():mids[1].front();
            seg.p=n0.pos;
            seg.q=n1.pos;
            vor_segments.push_back(seg);
        }

        //connect the points
        //check lower right corner
        int lr_fc=feature_count(lrf,all_features);
        if(mid_features[1]==lrf) lr_fc--;
        if(mid_features[2]==lrf) lr_fc--;

        if( lr_fc==1 ){
            VorSegment seg;
            BoxNode n1=(mid_features[1]==lrf)?mids[1].front():mids[1].back();
            BoxNode n2=(mid_features[2]==lrf)?mids[2].back():mids[2].front();
            seg.p=n1.pos;
            seg.q=n2.pos;
            vor_segments.push_back(seg);
        }

        //connect the points
        //check lower lower-left corner
        int ll_fc=feature_count(llf,all_features);
        if(mid_features[2]==llf) ll_fc--;
        if(mid_features[3]==llf) ll_fc--;

        if( ll_fc==1 ){
            VorSegment seg;
            BoxNode n2=(mid_features[2]==llf)?mids[2].front():mids[2].back();
            BoxNode n3=(mid_features[3]==llf)?mids[3].back():mids[3].front();
            seg.p=n2.pos;
            seg.q=n3.pos;
            vor_segments.push_back(seg);
        }

        //check north edge
        if(ulf==urf){

            if(ul_fc==2)
            {
                VorSegment seg;
                seg.q=mids[3].back().pos;
                seg.p=mids[1].front().pos;
                vor_segments.push_back(seg);
            }
        }

        //check east edge
        if(urf==lrf){
            if(ur_fc==2)
            {
                VorSegment seg;
                seg.p=mids[0].back().pos;
                seg.q=mids[2].front().pos;
                vor_segments.push_back(seg);
            }
        }

        //check south edge
        if(lrf==llf){
            if(lr_fc==2)
            {
                if(mid_features[1]!=NULL || mid_features[3]!=NULL)
                {
                    VorSegment seg;
                    seg.p=mids[1].back().pos;
                    seg.q=mids[3].front().pos;
                    vor_segments.push_back(seg);
                }
            }
        }


        //check west edge
        if(llf==ulf){

            if(ll_fc==2)
            {
                if(mid_features[0]!=NULL || mid_features[2]!=NULL)
                {
                    VorSegment seg;
                    seg.p=mids[2].back().pos;
                    seg.q=mids[0].front().pos;
                    vor_segments.push_back(seg);
                }
            }
        }

    }//end if(sfc!=3)

}

//check if a feature is wall or corner
bool Box::isWall(Feature * f)
{
    return dynamic_cast<Wall*>(f)!=NULL;
}


//
// Voronoi vertex test
//
// return:
//
// '0': failed, no intersection between the box and the bisection of w1/w2
// 'f': failed, all intersections are closer to f/g (the f/g feature in the paper)
// 'h': failed, all intersections are closer to h (the h feature in the paper)
// '1': passed, there is a vertex in the box
//
char Box::VVT(Feature * f1, Feature * f2, Feature * f3)
{
    bool is_f1_wall=isWall(f1);
    bool is_f2_wall=isWall(f2);
    bool is_f3_wall=isWall(f3);

    if(is_f1_wall==is_f2_wall)
    {
        return VVT_ordered(f1,f2,f3);
    }

    if(is_f1_wall==is_f3_wall)
    {
        return VVT_ordered(f1,f3,f2);
    }

    if(is_f2_wall==is_f3_wall)
    {
        return VVT_ordered(f2,f3,f1);
    }

    //this is impossible to happen
    return '0';
}

//
// f&g are the same type (wall or corner) of feature
//
// '0': failed, no intersection between the box and the bisection of w1/w2
// 'f': failed, all intersections are closer to f/g (the f/g feature in the paper)
// 'h': failed, all intersections are closer to h (the h feature in the paper)
// '1': passed, there is a vertex in the box
//
char Box::VVT_ordered(Feature * f, Feature * g, Feature * h)
{
    BoxNode UL, LL, UR, LR; //Box corners, upper left, lower left, upper right, lower right
    getCorners(UL,UR,LR,LL);

    //
    BoxNode Nx,Ex,Sx,Wx; //intersections from the North, East, South, West box edges
    bool bNx=false, bEx=false, bSx=false, bWx=false;

    if(isWall(f))
    {
        Wall * w1=dynamic_cast<Wall*>(f);
        Wall * w2=dynamic_cast<Wall*>(g);
        bNx=intersection(w1,w2,UL,UR,Nx);
        bEx=intersection(w1,w2,UR,LR,Ex);
        bSx=intersection(w1,w2,LR,LL,Sx);
        bWx=intersection(w1,w2,LL,UL,Wx);
    }
    else
    {
        Corner * c1=dynamic_cast<Corner*>(f);
        Corner * c2=dynamic_cast<Corner*>(g);
        bNx=intersection(c1,c2,UL,UR,Nx);
        bEx=intersection(c1,c2,UR,LR,Ex);
        bSx=intersection(c1,c2,LR,LL,Sx);
        bWx=intersection(c1,c2,LL,UL,Wx);
    }

    if(!bNx && !bEx && !bSx && !bWx ){
        return '0'; //no intersection...
    }

    bool all_closer_to_f=true; //are all intersections closer to f1/f2
    bool all_closer_to_h=true; //are all intersections closer to f3

    if(bNx){ //there is an intersection on north edge
        double d2f=f->distance(Nx.pos);
        double d2h=h->distance(Nx.pos);
        if(d2f<=d2h) all_closer_to_h=false;
        if(d2h<=d2f) all_closer_to_f=false;

        //cout<<"N ("<<Nx.x<<","<<Nx.y<<"): d2f="<<d2f<<" d2h="<<d2h<<endl;
    }

    if(bEx){ //there is an intersection on east edge
        double d2f=f->distance(Ex.pos);
        double d2h=h->distance(Ex.pos);
        if(d2f<=d2h) all_closer_to_h=false;
        if(d2h<=d2f) all_closer_to_f=false;

        //cout<<"E ("<<Ex.x<<","<<Ex.y<<"): d2f="<<d2f<<" d2h="<<d2h<<endl;
    }

    if(bSx){ //there is an intersection on south edge
        double d2f=f->distance(Sx.pos);
        double d2h=h->distance(Sx.pos);
        if(d2f<=d2h) all_closer_to_h=false;
        if(d2h<=d2f) all_closer_to_f=false;

        //cout<<"S ("<<Sx.x<<","<<Sx.y<<"): d2f="<<d2f<<" d2h="<<d2h<<endl;
    }

    if(bWx){ //there is an intersection on west edge
        double d2f=f->distance(Wx.pos);
        double d2h=h->distance(Wx.pos);
        if(d2f<=d2h) all_closer_to_h=false;
        if(d2h<=d2f) all_closer_to_f=false;

        //cout<<"W ("<<Wx.x<<","<<Wx.y<<"): d2f="<<d2f<<" d2h="<<d2h<<endl;
    }

    if(all_closer_to_h) return 'h'; //all intersections are closer to f3 then to f1/f2
    if(all_closer_to_f) return 'f'; //all intersections are closer to f1&f2 then to f3

    return '1';
}

//get the bisector of two features f and g
//f & g must have the same type (both Wall or Corner)
void Box::getBisector(Feature * f, Feature * g, Point2d& bo, Vector2d& bv)
{
    assert(isWall(f)==isWall(g));

    if(isWall(f)) getBisector((Wall*)f, (Wall*)g, bo, bv);
    else getBisector((Corner*)f, (Corner*)g, bo, bv);
}

// compute the bisector of w1 and w2
// bo //bisector origin
// bv //bisector vector, not normalized
void Box::getBisector(Wall * w1, Wall * w2, Point2d& bo, Vector2d& bv)
        //double& bo_x, double& bo_y, double& bv_x, double& bv_y)
{
    const Point2d& a=w1->src->pos;
    const Point2d& b=w2->src->pos;
    Vector2d v=w1->dst->pos-a;
    Vector2d u=w2->dst->pos-b;
    double d_v=v.norm();
    double d_u=u.norm();

    if(d_v==0 || d_u==0)
    {
        cerr<<"! Error: getWallBisector: Input contains 0 length walls"<<endl;
        assert(0);
    }

    double v_xu_y=v[0]*u[1];
    double v_yu_x=v[1]*u[0];

    if(v_xu_y==v_yu_x) //parallel
    {
        //project w1->src (a) to w2
        Vector2d w=a-w2->src->pos;
        Point2d a_prime=w2->src->pos+u*((u*w)/d_u);

        bo[0]=(a[0]+a_prime[0])/2;
        bo[1]=(a[1]+a_prime[1])/2;
        bv[0]=u[0];
        bv[1]=u[1];

    }
    else //not parallel
    {
        //find the intersection of lines containing these two walls
        if(u[0]==0){ //u is vertical...
            double t=(b[0]-a[0])/v[0]; //v[0 cannot be 0 otherwise both segments are parallel...
            bo[0]=b[0];
            bo[1]=a[1]+t*v[1];
        }
        else{ //not vertical
            double t=(a[1]*u[0]-a[0]*u[1]+b[0]*u[1]-b[1]*u[0])/(v_xu_y-v_yu_x);
            bo[0]=a[0]+t*v[0];
            bo[1]=a[1]+t*v[1];
        }

        bv[0]=(u[1]/d_u+v[1]/d_v)/2;
        bv[1]=-(u[0]/d_u+v[0]/d_v)/2;
    }
}

// compute the bisector of w1 and w2
// bo //bisector origin
// bv //bisector vector, not normalized
void Box::getBisector(Corner * c1, Corner * c2, Point2d& bo, Vector2d& bv)
{
    bo[0] = (c1->pos[0]+c2->pos[0])/2;
    bo[1] = (c1->pos[1]+c2->pos[1])/2;
    Vector2d tmp=c1->pos-c2->pos;
    bv[0]=tmp[1];
    bv[1]=-tmp[0];
}

//
// compute the intersection between the bisector of (f1,f2) and the line segment connecting n1 and n2
//
// the return value is true if there is an intersection between the bisector of f1/f2 and the
// segment connecting n1 and n2
//
// "cross" contains the intersection if the return is true
//
bool Box::intersection(Feature * f1, Feature * f2, const BoxNode& n1, const BoxNode& n2, BoxNode& cross)
{
    bool is_f1_wall=isWall(f1);
    bool is_f2_wall=isWall(f2);

    if( is_f1_wall && is_f2_wall) //two walls
    {
        Wall * w1=dynamic_cast<Wall*>(f1);
        Wall * w2=dynamic_cast<Wall*>(f2);
        return intersection(w1,w2,n1,n2,cross);
    }
    else if( is_f1_wall==false && is_f2_wall==false) //two corners
    {
        Corner * c1=dynamic_cast<Corner*>(f1);
        Corner * c2=dynamic_cast<Corner*>(f2);
        return intersection(c1,c2,n1,n2,cross);
    }
    else{
        //tricky case
        cross.pos[0]=(n1.pos[0]+n2.pos[1])/2;
        cross.pos[1]=(n1.pos[1]+n2.pos[1])/2;
        return false;
    }
    //done
}



bool Box::intersection(Wall * w1, Wall * w2, const BoxNode& n1, const BoxNode& n2, BoxNode& cross)
{
    Point2d z;   //bisector origin
    Vector2d bv; //bisector vector

    //find bisector
    getBisector(w1,w2,z,bv);

//    if(bv_x!=bv_x){
//        cout<<"FOUND NAN"<<endl;
//        getBisector(w1,w2,z_x,z_y,bv_x,bv_y);
//    }

    //find intersection...
    double a[2]={z[0]+bv[0]*100000,z[1]+bv[1]*100000};
    double b[2]={z[0]-bv[0]*100000,z[1]-bv[1]*100000};
    double c[2]={n1.pos[0],n1.pos[1]};
    double d[2]={n2.pos[0],n2.pos[1]};
    double p[2];


    char code=SegSegInt(c,d,a,b,p);
    //cout<<"segseg code="<<code<<endl;

    if(code!='0'){
        cross.pos.set(p[0],p[1]);
        return true;
    }

    //!!
    cross.pos.set( (n1.pos[0]+n2.pos[0])/2, (n1.pos[1]+n2.pos[1])/2);

    return false;
}

bool Box::intersection(Corner * c1, Corner * c2, const BoxNode& n1, const BoxNode& n2, BoxNode& cross)
{
    //bisector
    BoxNode mid;
    Vector2d bv; //bisector
    getBisector(c1,c2,mid.pos,bv);

    //find intersection...
    double a[2]={mid.pos[0]+bv[0]*100000,mid.pos[1]+bv[1]*100000};
    double b[2]={mid.pos[0]-bv[0]*100000,mid.pos[1]-bv[1]*100000};
    double c[2]={n1.pos[0],n1.pos[1]};
    double d[2]={n2.pos[0],n2.pos[1]};
    double p[2];

    char code=SegSegInt(c,d,a,b,p);
    BoxNode result;

    if(code!='0'){
        cross.pos.set(p[0],p[1]);
        return true;
    }

    //!!
    result.pos.set( (n1.pos[0]+n2.pos[0])/2, (n1.pos[1]+n2.pos[1])/2);

    return false;
}


//collect the intersections between the boundary of this box and the bisector of the features f and g
void Box::getBoundaryIntersections(Feature * f, Feature * g, list<BoxNode>& crossings)
{
    BoxNode UL, LL, UR, LR; //Box corners, upper left, lower left, upper right, lower right
    getCorners(UL,UR,LR,LL);

    bool f_is_wall=isWall(f);
    bool g_is_wall=isWall(g);

    if(f_is_wall!=g_is_wall) //wall/corner or corner/wall...
    {
        //this shouldn't happen due to predicate #3...
        cerr<<"! ERROR: Box:getBoundaryIntersections: no method to handle Wall/Corner case"<<endl;
        //assert(false);
        return;
    }

    BoxNode Nx,Ex,Sx,Wx; //intersections from the North, East, South, West box edges
    bool bNx=false, bEx=false, bSx=false, bWx=false;

    if(isWall(f))
    {
        Wall * w1=dynamic_cast<Wall*>(f);
        Wall * w2=dynamic_cast<Wall*>(g);
        bNx=intersection(w1,w2,UL,UR,Nx);
        bEx=intersection(w1,w2,UR,LR,Ex);
        bSx=intersection(w1,w2,LR,LL,Sx);
        bWx=intersection(w1,w2,LL,UL,Wx);
    }
    else
    {
        Corner * c1=dynamic_cast<Corner*>(f);
        Corner * c2=dynamic_cast<Corner*>(g);
        bNx=intersection(c1,c2,UL,UR,Nx);
        bEx=intersection(c1,c2,UR,LR,Ex);
        bSx=intersection(c1,c2,LR,LL,Sx);
        bWx=intersection(c1,c2,LL,UL,Wx);
    }

    //collect all intersections
    if(bNx) crossings.push_back(Nx);
    if(bEx) crossings.push_back(Ex);
    if(bSx) crossings.push_back(Sx);
    if(bWx) crossings.push_back(Wx);
}


//collect all neighbors of this box
//this box must be a leaf and quadtree must be balanced
void Box::getAllNeighthbors(list<Box*>& nei_list)
{
    assert(isLeaf); //must be a leaf

    for(short i=0;i<4;i++)
    {
        Box * nei=pChildren[i];
        if(nei==NULL) continue;

        nei_list.push_back(nei); //remember this neighbor

        if(nei->depth>depth) //deeper
        {
            //one of the nei's neighbor is also the neighbor of this box...
            //this neighbor must has the same depth as nei->depth
            for(short j=0;j<4;j++){
                Box * nei_nei=nei->pChildren[j];
                if(nei_nei==NULL || nei_nei==this) continue;
                if(nei_nei->depth!=nei->depth) continue;
                if(isNeighbor(nei_nei)) nei_list.push_back(nei_nei); //remember this neighbor
            }
        }//end if

    }//end for i
}


//collect all neighbors of this box
//this box must be a leaf and quadtree must be balanced
void Box::getAllNeighthbors(list<Box*> nei_list[4])
{
    assert(isLeaf); //must be a leaf

    for(short i=0;i<4;i++)
    {
        Box * nei=pChildren[i];
        if(nei==NULL) continue;

        nei_list[i].push_back(nei); //remember this neighbor

        if(nei->depth>depth) //deeper
        {
            //one of the nei's neighbor is also the neighbor of this box...
            //this neighbor must has the same depth as nei->depth
            for(short j=0;j<4;j++){
                Box * nei_nei=nei->pChildren[j];
                if(nei_nei==NULL || nei_nei==this) continue;
                if(nei_nei->depth!=nei->depth) continue;
                if(isNeighbor(nei_nei)) nei_list[i].push_back(nei_nei); //remember this neighbor
            }
        }//end if

    }//end for i
}

//check if the box b and this box are neighbors
//this box and box b must be leaves and quadtree must be balanced
bool Box::isNeighbor(Box * b)
{
    assert(isLeaf); //must be a leaf
    if(b==this) return false;
    for(short i=0;i<4;i++) if(pChildren[i]==b) return true;
    for(short i=0;i<4;i++) if(b->pChildren[i]==this) return true;
    return false;
}


//find the closest feature in the inseparable features (ISF)
//to the end points in seg
Feature * Box::closestInISF(Feature * f, const Point2d& n1, const Point2d& n2)
{
    typedef list<Wall*>::iterator   WIT;
    typedef list<Corner*>::iterator CIT;

    double min_d=f->distance(n1);
    Feature * min_f=f;

    //go through each wall
    for (WIT iterW=walls.begin(); iterW != walls.end(); ++iterW)
    {
        Wall * w=*iterW;
        if(UnionFind().Find(w)==UnionFind().Find(f))
        {
            double d=w->distance(n1);
            if(d<min_d){
                min_d=d;
                min_f=w;
            }

            d=w->distance(n2);
            if(d<min_d){
                min_d=d;
                min_f=w;
            }
        }
    }

    //go through each corner
    for (CIT iterC=corners.begin(); iterC != corners.end(); ++iterC)
    {
        Corner * c=*iterC;
        if(UnionFind().Find(c)==UnionFind().Find(f))
        {
            double d=c->distance(n1);
            if(d<min_d){
                min_d=d;
                min_f=c;
            }

            d=c->distance(n2);
            if(d<min_d){
                min_d=d;
                min_f=c;
            }
        }
    }

    return min_f;
}


