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

////////////////////////////////////////////////////
// Implementations
////////////////////////////////////////////////////

// updateStatus()
	// determine the status of the box: UNKNOWN, IN, OUT, ON
	//
	//Chee: void updateStatus(double maxEps)
void Box::updateStatus()
	{
		if (status != UNKNOWN)
		{
			return;
		}

	/*Chee: this is not the right place to use maxEps...
		if(this->height>maxEps)
		{
            status = IN; //need more split
            return;
		}
	*/

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
		                    bool r1=w1->isRight(w2->src->x,w2->src->y);
		                    bool r2=w1->isRight(w2->dst->x,w2->dst->y);
		                    bool r3=w2->isRight(w1->src->x,w1->src->y);
		                    bool r4=w2->isRight(w1->dst->x,w1->dst->y);

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
        UL.x=x-width/2; UL.y=y+height/2;
        LL.x=x-width/2; LL.y=y-height/2;
        UR.x=x+width/2; UR.y=y+height/2;
        LR.x=x+width/2; LR.y=y-height/2;

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
	}//updateStatus()


	// distribute_features2box( child )

void Box::distribute_features2box(Box * child)
	{
	    typedef list<Wall*>::iterator   WIT;
	    typedef list<Corner*>::iterator CIT;

	    //center x,y
	    double x=child->x;
	    double y=child->y;

	    //clearance+2*radius of the box
	    double cl2r=child->rB*2+child->cl_m; //clearance + 2*rB

	    //
	    //compute the separation to walls
          for (WIT iterW=walls.begin(); iterW != walls.end(); ++iterW)
          {
            Wall* w = *iterW;
            double dist = w->distance(x, y); //w->distance_star(x, y); //w->distance(x, y);

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
            double dist = c->distance(x, y);
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
            mid.x=x;
            mid.y=y;
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
	}// distribute_features2box

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

            double dist = w->distance(x, y);

            if( fabs(dist-mindistW)<1e-10 )
            {
                if( w->distance_sign(x,y)==0 && w->isRight(x,y) ) //in zone
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
                if( c->inZone_star(x,y) ) //in the zone
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

        if(nearestWall!=NULL) in_zone_w=nearestWall->inZone_star(x,y);
        if(nearestCorner!=NULL) in_zone_c=nearestCorner->inZone_star(x,y);

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
	}//determine_clearance(BoxNode&)

//  split(eps, maxEpsilon)
	//
	// 	--returns false if we fail to split for some reason
	//
bool Box::split (double epsilon, double maxEpsilon)
	{
		if (this->height < epsilon || this->width < epsilon)
		{
			return false;
		}


        //if ((!this->isLeaf || this->status != IN)
	//	
	// Chee: "status != IN"
	//
	// 	is equiv. to
	//
	// 	"status == OUT || status == UNKNOWN || status == ON".
	//
	// 	But we weaken the ON part, by adding a maxEpsilon criterion:
	//
        if ( (!this->isLeaf	|| this->status == OUT || this->status == UNKNOWN)
	  	|| (this->status == ON	&& 
		    this->height < maxEpsilon	&& this->width < maxEpsilon
		   )
	   )
        {
            return false;
        }

		//record the time of this split event, will be used to set priority of children
		++Box::counter;

		Box* children[4];
		children[0] = new Box(x - width / 4, y + height / 4, width / 2, height / 2);
		children[1] = new Box(x + width / 4, y + height / 4, width / 2, height / 2);
		children[2] = new Box(x + width / 4, y - height / 4, width / 2, height / 2);
		children[3] = new Box(x - width / 4, y - height / 4, width / 2, height / 2);

		for (int i = 0; i < 4; ++i)
		{
		    if(children[i]==NULL)
		    {
		        cerr<<"! Error: Box::split: Not enough memory"<<endl;
		        return false;
		    }

			children[i]->depth = this->depth + 1;
			BoxNode node;
			node.x=children[i]->x;
			node.y=children[i]->y;
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
	}//split (eps,Eps)

//file: Box.cpp
