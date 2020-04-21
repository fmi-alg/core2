/*
 *  RRT.cpp
 *
 *  Author: zhonghua, Ching-Hsiang (Tom) Hsu
 *  Email: chhsu@nyu.edu
 *  Created on: Oct 12, 2013
 *  Modified: Oct. 31, 2016
 */

#include "RRT.h"
#include <cfloat>
#include "intersection.h"
#include "mathtool/Gauss.h"

extern FILE* fp;
extern double timeout;
extern bool non_crossing;
extern MainWindow *window;

/////////////////////////////////////////////////////////////////////
//
// Graph_Node
//
/////////////////////////////////////////////////////////////////////


Graph_Node Graph_Node::m_InvalidValue;

Graph_Node::Graph_Node()
{
    m_ID=-1;
    m_Clear=0;
}

Graph_Node::~Graph_Node()
{

}

bool Graph_Node::operator==( const Graph_Node & other ) const
{
    return (m_cfg==other.m_cfg&&m_ID==other.m_ID);
}

ostream & operator<<( ostream & out, const Graph_Node & node )
{
    return out;
}

istream & operator>>( istream &  in, Graph_Node & node )
{
    return in;
}

/////////////////////////////////////////////////////////////////////
//
// Graph_Edge
//
/////////////////////////////////////////////////////////////////////

Graph_Edge Graph_Edge::m_InvalidValue;

Graph_Edge::Graph_Edge()
{
    m_Weight=1;
}

Graph_Edge::Graph_Edge(float weight)
{
    m_Weight=(float)weight;
}

Graph_Edge::~Graph_Edge()
{

}

bool Graph_Edge::operator==( const Graph_Edge & other ) const
{
    return other.m_Weight==m_Weight;
}

ostream & operator<<( ostream & out, const Graph_Edge & node )
{
    return out;
}

istream & operator>>( istream &  in, Graph_Edge & node )
{
    return in;
}

int Find(vector<struct subset> &subsets, int i)
{
    // find root and make root as parent of i (path compression)
    if (subsets[i].parent != i)
        subsets[i].parent = Find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

// A function that does union of two sets of x and y
// (uses union by rank)
void Union(vector<struct subset> &subsets, int x, int y)
{
    int xroot = Find(subsets, x);
    int yroot = Find(subsets, y);

    // Attach smaller rank tree under root of high rank tree
    // (Union by Rank)
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    // If ranks are same, then make one as root and increment
    // its rank by one
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

bool IsSameCCSet(vector<struct subset> &subsets, int x, int y){
    int xx = Find(subsets, x);
    int yy = Find(subsets, y);
    return (xx == yy);
}

//
//
// A generic motion planner
//
//

Planner::Planner(float w, float h, float tr, float rr)
{ 

    env_width=w; env_height=h; env_TR=tr; env_RR=rr;

    //add a bounding box
    c_ply bb(c_ply::PIN);
    bb.beginPoly();
//    bb.addVertex(-10, -10);
//    bb.addVertex(-10, env_height + 10);
//    bb.addVertex(env_width + 10, env_height + 10);
//    bb.addVertex(env_width + 10, -10);
    bb.addVertex(0, 0);
    bb.addVertex(0, env_height + 0);
    bb.addVertex(env_width + 0, env_height + 0);
    bb.addVertex(env_width + 0, 0);
    bb.endPoly(false);

    this->addObj(bb);
}

CFG Planner::to_physical(const CFG& cfg)
{
    Point2d O(env_width/2,env_height/2);
    return cfg.to_physical(env_width,env_height,O);
}

CFG Planner::to_parametric(const CFG& cfg)
{
    Point2d O(env_width/2,env_height/2);
	return cfg.to_paramtric(env_width,env_height,O);
}

double dotProduct(const Point2d& p1, const Point2d& p2)
{
    return p1[0] * p2[0] + p1[1] * p2[1];
}

double crossProduct(const Point2d& p1, const Point2d& p2)
{
    return p1[0] * p2[1] - p1[1] * p2[0];
}

double distanceV(const Point2d& v)
{
    return sqrt( v[0]*v[0]+v[1]*v[1] );
}

double distanceP_P(const Point2d& p1, const Point2d& p2)
{
    return sqrt( (p2[0] - p1[0]) * (p2[0] - p1[0])
               + (p2[1] - p1[1]) * (p2[1] - p1[1]) );
}

double distanceP_Line(const Point2d p, const Point2d p1, const Point2d p2){
    Point2d v1 = p -p1;
    Point2d v2 = p2-p1;
    return fabs(crossProduct(v1,v2))/distanceV(v2);
}

double distanceP_Seg(const Point2d p, const Point2d p1, const Point2d p2){
    Point2d p1p = p-p1, p1p2 = p2-p1;
    Point2d p2p = p-p2, p2p1 = p1-p2;
    if(dotProduct(p1p,p1p2) > 0 && dotProduct(p2p,p2p1) > 0){
        return distanceP_Line(p, p1, p2);
    }
    else{
        return Planner::minVal(distanceP_P(p, p1), distanceP_P(p, p2));
    }
}

// ========================================
// TODO: check whether a cfg is valid or not
bool Planner::isValid(const CFG& cfg)
{

    CFG phy_cfg=to_physical(cfg);
    this->m_robot.setCFG(phy_cfg);

    for(RIT it=this->m_objs.begin(); it!=this->m_objs.end(); ++it)
    {
        //check points
        if(it->getType() == c_ply::POUT)
        {
            if(it->enclosed(this->m_robot.p[0])){
                //fprintf(stderr, "point in\n");
                return false;
            }
        }
        else
        {
            if(!it->enclosed(this->m_robot.p[0])){
                //fprintf(stderr, "point out\n");
                return false;
            }
        }

	//check edges...
	for( int l=1;l<3;l++)
	{
	      //robot link
	      const Point2d& a=this->m_robot.p[0];
	      const Point2d& b=this->m_robot.p[l];

	      if(a.almost_equ(b)) continue;

	      ply_vertex * ptr=it->getHead();

	      do{
		    ply_vertex * next=ptr->getNext();

		    //create p
		    const Point2d& c=ptr->getPos();
		    const Point2d& d=next->getPos();

		    //
		    bool r=SegSegInt(a.get(),b.get(),c.get(),d.get());
		    if(r){
			//fprintf(stderr, "(%lf %lf, %lf %lf) -> (%lf %lf, %lf %lf)\n", a[0], a[1], b[0], b[1], c[0], c[1], d[0], d[1]);
			return false; //collision found
		    }
		    else{
			// thickness is not zero
			double dist = minVal( minVal(distanceP_Seg(a.get(),c.get(),d.get()), distanceP_Seg(b.get(),c.get(),d.get())),
					      minVal(distanceP_Seg(c.get(),a.get(),b.get()), distanceP_Seg(d.get(),a.get(),b.get())) );
			//fprintf(stderr, "dist %lf m_robot.Thickness %lf\n", dist, m_robot.Thickness);
			if(dist < m_robot.Thickness*0.5f){
			    //fprintf(stderr, "Thickness intersects\n");
			    return false;
			}
		    }

		    //next
		    ptr=next;
	      }
	      while(ptr!=it->getHead());

        }//end for
    }

    return true;
}

//connect from c1 to c2
bool Planner::isValid(const CFG& c1, const CFG& c2)
{
    CFG dir = (c2 - c1); //general moving direction

    int steps = (int) ceil( max((dir.normT()/env_TR), (dir.normR()/env_RR)) );

    CFG step = dir/steps;

    for(int i=0;i<=steps;i++)
    {
        CFG now_cfg = c1 + step*i;

	if(!this->isValid(now_cfg))
	{
	    return false;
	}

	if(non_crossing && i){
	    CFG pre_cfg = c1 + step*(i-1);
	    double pre_t1 = pre_cfg.t1, pre_t2 = pre_cfg.t2;
	    double now_t1 = now_cfg.t1, now_t2 = now_cfg.t2;
	    if(now_t1 > pre_t1){
		if(now_t2 > pre_t2){
		    if((pre_t1 < pre_t2 && pre_t2 < now_t1 && pre_t1 < now_t2 && now_t2 < now_t1) ||
		       (pre_t2 < pre_t1 && pre_t1 < now_t2 && pre_t2 < now_t1 && now_t1 < now_t2)){
			return false;
		    }
		}
		else{
		    if(pre_t1 < pre_t2 && pre_t2 < now_t1 || now_t2 < pre_t1 && pre_t1 < pre_t2){
			return false;
		    }
		}
	    }
	    else{
		if(now_t2 > pre_t2){
		    if(now_t1 < pre_t2 && pre_t2 < pre_t1 || pre_t2 < pre_t1 && pre_t1 < now_t2){
			return false;
		    }
		}
		else{
		    if((now_t1 < pre_t2 && pre_t2 < pre_t1 && now_t1 < now_t2 && now_t2 < pre_t1) ||
		       (now_t2 < pre_t1 && pre_t1 < pre_t2 && now_t2 < now_t1 && now_t1 < pre_t2)){
			return false;
		    }
		}
	    }
	}
    }

    return true;
}
//connect from c1 to c2 and find the collision c3
bool Planner::isValid(const CFG& c1, const CFG& c2, CFG& c3, bool toggle)
{
    //fprintf(stderr, "c1 %lf %lf  c2 %lf %lf\n", c1.t1, c1.t2, c2.t1, c2.t2);
    CFG dir = (c2 - c1); //general moving direction

    int steps = (int) ceil( max((dir.normT()/env_TR), (dir.normR()/env_RR)) );

    CFG step = dir/steps;

    for(int i=0;i<=steps;i++)
    {
        CFG now_cfg = c1 + step*i;

	if(toggle){
	    if(this->isValid(now_cfg))
	    {
		c3 = now_cfg;
		return false;
	    }
	}
	else{
	    if(!this->isValid(now_cfg))
	    {
		c3 = now_cfg;
		return false;
	    }
	}

	if(non_crossing && i){
	    CFG pre_cfg = c1 + step*(i-1);
	    double pre_t1 = pre_cfg.t1, pre_t2 = pre_cfg.t2;
	    double now_t1 = now_cfg.t1, now_t2 = now_cfg.t2;
	    if(now_t1 > pre_t1){
		if(now_t2 > pre_t2){
		    if((pre_t1 < pre_t2 && pre_t2 < now_t1 && pre_t1 < now_t2 && now_t2 < now_t1) ||
		       (pre_t2 < pre_t1 && pre_t1 < now_t2 && pre_t2 < now_t1 && now_t1 < now_t2)){
			c3 = now_cfg;
			return false;
		    }
		}
		else{
		    if(pre_t1 < pre_t2 && pre_t2 < now_t1 || now_t2 < pre_t1 && pre_t1 < pre_t2){
			c3 = now_cfg;
			return false;
		    }
		}
	    }
	    else{
		if(now_t2 > pre_t2){
		    if(now_t1 < pre_t2 && pre_t2 < pre_t1 || pre_t2 < pre_t1 && pre_t1 < now_t2){
			c3 = now_cfg;
			return false;
		    }
		}
		else{
		    if((now_t1 < pre_t2 && pre_t2 < pre_t1 && now_t1 < now_t2 && now_t2 < pre_t1) ||
		       (now_t2 < pre_t1 && pre_t1 < pre_t2 && now_t2 < now_t1 && now_t1 < pre_t2)){
			c3 = now_cfg;
			return false;
		    }
		}
	    }
	}
    }

    return true;
}


 //
//compute arrangement vertices (nodes) using interval tree
//
void Planner::build_itree()
{   
	
	/*
    typedef vector<Rect2D*>::iterator IT2;

    vector< Rect2D* > rect;

	for(RIT it=this->m_objs.begin(); it!=this->m_objs.end(); ++it) 
	{
		ply_vertex * ptr=it->getHead();
		do{
			Rect2D * p = new Rect2D();
			assert(p);

			ply_vertex * next=ptr->getNext();
			
			//create p
			const Point2d& p1=ptr->getPos();
			const Point2d& p2=next->getPos();

			buildRect2D(p, p1, p2);

			//store
			rect.push_back(p);
			m_segments.push_back(ptr);

			//next
			ptr=next;

		}while(ptr!=it->getHead());
	}


    if( m_iTree.Build(rect)==false ){
        //failed, use brute force
        cerr<<"! WARNING: Build interval tree failed."<<endl;
    }
	*/
}

//
//void buildRect2D(Rect2D * p, const Point2d& p1, const Point2d& p2)
//{
//
//	float max_x=(p1[0]>p2[0])?p1[0]:p2[0];
//	float min_x=(p1[0]<p2[0])?p1[0]:p2[0];
//	float max_y=(p1[1]>p2[1])?p1[1]:p2[1];
//	float min_y=(p1[1]<p2[1])?p1[1]:p2[1];
//
//	//
//	typedef itree::Interval<itree::EndPoint> Int;
//	p->setInterval( Int(p,itree::EndPoint(min_x), itree::EndPoint(max_x)), 0);
//	p->setInterval( Int(p,itree::EndPoint(min_y), itree::EndPoint(max_y)), 1);
//}

///
/// return true if there is collision
/// otherwise return false...
///
bool Planner::query_itree(const CFG& cfg)
{
	/*
	CFG phy_cfg=to_physical(cfg);
	this->m_robot.setCFG(phy_cfg);

	//build two rects
	for( int l=0;l<2;l++)
	{
		//robot link
		const Point2d& a=this->m_robot.p[l];
		const Point2d& b=this->m_robot.p[l+1];

		Rect2D rect;
		buildRect2D(&rect,a,b);

		m_iTree.query(&rect);

		//rect->endQuery(); //for RectKD_ez...
		Rect2D::Intersect& intersections=rect.getIntersections();

		//add end points of seg as nodes
		for (Rect2D::Intersect::iterator j = intersections.begin(); j != intersections.end(); ++j)
		{
			ply_vertex * ptr=m_segments[*j];
			ply_vertex * next=ptr->getNext();
			const Point2d& c=ptr->getPos();
			const Point2d& d=next->getPos();
			bool r=SegSegInt(a.get(),b.get(),c.get(),d.get());
			if(r) return; //collision found
		}
	}
	*/

	//return false...
	return false;
}

//
//
// RRT planner
//
//
RRT::RRT(float w, float h, float tr, float rr, unsigned int max_sample, float expand_step, float bias, float close_to_goal) 
: Planner(w,h,tr,rr)
{
	//
	m_max_sample=max_sample;
	m_expand_step=expand_step;
	m_goal_bias=bias;
	m_close_to_goal=close_to_goal;
}

RRT::~RRT()
{
	// clear memory
	for(NIT it=this->m_tree.begin(); it!=this->m_tree.end(); ++it)
	{
		delete *it;
	}
}

bool RRT::findPath(const CFG& start, const CFG& goal)
{
	this->m_start = (start); 
	this->m_goal = (goal);

	RRT_NODE* start_node = new RRT_NODE(m_start);
	this->m_tree.push_back(start_node);

	double min_dist = (m_start-m_goal).norm();

	this->m_found = false;

	for(int i=0;i<m_max_sample && !m_found;i++)
	{
	    //cout<<"i="<<i<<endl;

		CFG rand_cfg;

        //bias to the goal...
        if(drand48()<this->m_goal_bias)
            rand_cfg=m_goal;
        else
            rand_cfg=CFG::randomCfg();

            RRT_NODE* nearest_node = this->nearest(rand_cfg);

            //if(nearest_node->cfg.dis(rand_cfg) < RRT_ST) continue;

            CFG out_cfg;

            if(this->moveTo(nearest_node->cfg, rand_cfg, out_cfg))
            {
                    RRT_NODE* new_node = new RRT_NODE(out_cfg, nearest_node);
                    this->m_tree.push_back(new_node);
                    //cout<<"expend to cfg = "<<out_cfg<<endl;

                    double dist = (out_cfg-m_goal).norm();

                    if(dist < min_dist)
                    {
                            min_dist = dist;
                            cout<<"["<< m_tree.size() << "/"<<i<<"] min_dist = "<<min_dist<<endl;
                    }

                    if(dist <= m_close_to_goal)
                    {
                            RRT_NODE* goal_node = new RRT_NODE(m_goal, new_node);
                            this->m_tree.push_back(goal_node);
                            cout<<"goal reached!"<<endl;
                            m_found = true;
                    }
            }

	    if((i + 1) % 10000 == 0) cout<<"["<< m_tree.size() << "/"<<i<<"] min_dist = "<<min_dist<<endl;
	}

	if(m_found) {
	    RRT_NODE* node = this->m_tree.back();
	    while(node) {
		    m_path.push_back(node->cfg);
		    node = node->parent;
	    }

	    std::reverse(m_path.begin(), m_path.end());
	}

	return m_found;
}


bool RRT::moveTo(const CFG& nearest_cfg, const CFG& rand_cfg, CFG& out_cfg)
{
    CFG dir = (rand_cfg - nearest_cfg); //general moving direction
    double dist = dir.norm();     // distance form nearest to rand
    double exp_dist=min(dist,(double)m_expand_step); // move in dir for dist

    dir=dir.normalize()*exp_dist;

    int steps = (int) ceil( max((dir.normT()/env_TR), (dir.normR()/env_RR)) );

    CFG step = dir/steps;

    for(int i=1;i<=steps;i++)
    {
            CFG now_cfg = nearest_cfg + step*i;

	    if(!this->isValid(now_cfg))
	    {
		    //if(i>1) { out_cfg = nearest_cfg + step*(i-1); return true; }
		    return false;
	    }

	    if( i==steps || ( (now_cfg-m_goal).normT() < env_TR && (now_cfg-m_goal).normR() < env_RR) )
	    {
		    out_cfg = now_cfg;
		    return true;
	    }
    }

    return false;
}

RRT_NODE* RRT::nearest(const CFG& cfg)
{
	RRT_NODE* nearest_node = NULL;
	double dist = FLT_MAX;
	for(NIT it = this->m_tree.begin(); it!=this->m_tree.end(); ++it){
	    RRT_NODE * node=*it;
		double d = (node->cfg-cfg).normT();
		if(d<dist) { dist = d; nearest_node = node; }
	}
	return nearest_node;
}


//
//
// PRM planner
//
//

PRM::PRM(float w, float h, float tr, float rr, unsigned int n_sample, unsigned int k_connection)
: Planner(w,h,tr,rr)
{
    m_n_sample=n_sample;
    m_k_closest=k_connection;
    m_skip_same_cc=true;
    m_create_good_loops=false;
}

PRM::~PRM()
{

}

bool PRM::findPath(const CFG& start, const CFG& goal)
{
    this->m_start = (start);
    this->m_goal = (goal);

    if(m_graph.GetVertexCount()==0) sample();
    if(m_graph.GetEdgeCount()==0) connect();

    //find path
    //connect to roadmap...
    vector< pair<int,VID> > ccstats;
    int ccsize=GetCCStats(m_graph,ccstats);

    for(int i=0;i<ccsize;i++)
    {
          cout<<"- Attempt CC["<<i<<"] size="<<ccstats[i].first<<endl;

	  int CCvid=ccstats[i].second;
	  int n1=connect2Map(start,CCvid);  //start connect to rmap
	  if( n1<0 ){
	      cerr<<"! Warning : Start cannot connect to the CC"<<endl;
	      continue;
	  }

	  int n2=connect2Map(goal,CCvid);  //goal connect to rmap
	  if( n2<0 ){
	      cerr<<"! Warning : Goal cannot connect to the CC"<<endl;
	      continue;
	  }

	  //build path from graph
	  m_path.push_back(start);

	  PATH mypath;
	  if( !findPathV1V2(n1,n2,mypath) )
	  {
	      cerr<<"! Warning : Start can not connect to Goal"<<endl;
	      continue;
	  }

	  m_path.insert(m_path.end(),mypath.begin(),mypath.end());
	  m_path.push_back(goal);

          return true;
     }

     cerr<<"! Warning : tried all CCs. Failed to fina a path"<<endl;
}

void PRM::sample()
{
    cout<<"- PRM smaple ("<<m_n_sample<<" nodes) "<<endl;

    //double st=getTime();
    for( int iN=0;iN<m_n_sample;iN++ ){

        CFG cfg=CFG::randomCfg();

        //not vaid...
        if(isValid(cfg)==false){
            continue;
        }

        //put into roadmap
        Graph_Node node; 
        node.setCFG(cfg);
        node.setID(m_graph.GetNextVID());
        int id=m_graph.AddVertex(node);
    }
    
    //double et=getTime();

    cout<<"- PRM sampling done ("<<m_graph.GetVertexCount()<<" valid configurations found)"<<endl; // takes "<<et-st<<" msec"<<endl;
}

void PRM::connect()
{
    cout<<"- PRM connect (k="<<this->m_k_closest<<") "<<endl;

    //double st=getTime();

    list< pair<int,int> > closest;

    //for each node find k closest
    sortedPairs(closest);
    connectNodes(closest);

    //done
    //double et=getTime();
    cout<<"\n- Connect done ("<<m_graph.GetEdgeCount()<<" connections found)"<<endl; // takes "<<et-st<<" msec"<<endl;
}


void PRM::sortedPairs(list< pair<int,int> >& close)
{
    typedef pair<int,int> VPAIR;
    list< pair<float,VPAIR> > sorted;
    typedef list< pair<float,VPAIR> >::iterator SIT;

    vector<VID> vids;
    int nSize=m_graph.GetVerticesVID(vids);
	
    for( int i=0;i<nSize;i++ ){
        CFG cfg_i=m_graph.GetData(vids[i]).getCFG();
        list< pair<float,VPAIR> > sorted_i;
        for( int j=0;j<nSize;j++ ){

            if(i==j) continue;
            CFG cfg_j=m_graph.GetData(vids[j]).getCFG();

            VPAIR v(vids[i],vids[j]);

            float dist=(cfg_j-cfg_i).norm();

            sorted_i.push_back(pair<float,VPAIR>(dist,v));
        }
        sorted_i.sort();
        SIT s=sorted_i.begin();

        for(int ik=0;ik<m_k_closest&&s!=sorted_i.end();ik++,s++){ sorted.push_back(*s); }
    }
    
    sorted.sort();

    {
        for(SIT i=sorted.begin();i!=sorted.end();i++){
            //cout<<"d="<<i->first<<"("<<i->second.first<<","<<i->second.second<<")"<<endl;
            close.push_back(i->second);
        }
    }//end for
}

//connect pairs of nodes in closest  
void PRM::connectNodes(list< pair<int,int> >& closest)
{
    list< pair<int,int> > loop; //store all found loops
    
    cout<<"- Connect ("<<closest.size()<<" pairs)"<<flush;
    int checked_n=0;
    int size=closest.size();
    typedef list< pair<int,int> >::iterator PIT;
    for( PIT i=closest.begin();i!=closest.end();i++, checked_n++)
    {
        //report progress
        int percent1=(checked_n*100.0/size);
        int percent2=(checked_n-1)*100.0/size;
        if( percent1%10==0 && percent1!=percent2 && checked_n!=0) cout<<"=>"<<(percent1)<<"%"<<flush;    
    
        //check the pair
        int vid1=i->first;
        int vid2=i->second;
        
        if( m_graph.IsEdge(vid1,vid2) ) continue;

        if(m_skip_same_cc||m_create_good_loops)
        {
            if(IsSameCC(m_graph,vid1,vid2)){
                //loop.push_back(*i);
                if(m_skip_same_cc){ continue; }
            }
        }//end if

        CFG cfg1=m_graph.GetData(vid1).getCFG();
        CFG cfg2=m_graph.GetData(vid2).getCFG();
		
        if( isValid(cfg1,cfg2) || isValid(cfg2,cfg1) )
        {
            m_graph.AddEdge(vid1, vid2, Graph_Edge( (cfg1-cfg2).norm() ) );
    	}
    }
	
    //find good loops! (this is very slow...)
    //if(m_create_good_loops && m_skip_same_cc)
    //	findGoodLoops(rmap,nodes,loop);
    vector< pair<int,VID> > ccstats;
    int ccsize=GetCCStats(m_graph,ccstats);
    cout<<"\n- CC#="<<ccsize<<"[";
    for(int i=0;i<ccsize;i++){
    	cout<<ccstats[i].first;
    	if(i!=ccsize-1) cout<<", ";
    }
    cout<<"]"<<endl;
}


//connect cfg to rmap
int PRM::connect2Map(const CFG& cfg, int CCvid)
{
    //CFlock * m_F=rmap.getFlock();
    /////////////////////////////////////////////////////////////////
    //CRobot2D& geo=m_F->getGeometry();  //for collision detection
    //if( isCollision(*m_pEnv,geo,pos) ){
    //    return NULL;
    //}

    /////////////////////////////////////////////////////////////////
    vector<VID> nodes;
//    m_graph.GetVerticesData(nodes);
	GetCC(m_graph,CCvid,nodes);

    list< pair<float,int> > sorted;
    typedef vector<VID>::iterator NIT;
    for( NIT i=nodes.begin();i!=nodes.end();i++ )
    {
    	Graph_Node node=m_graph.GetData(*i);
		float dist = (node.getCFG()-cfg).normsqr();
        if( dist<1e-10 ) 
            return *i;
        sorted.push_back( pair<float,int>(dist,*i) );
    }//end for i

    sorted.sort();
    typedef list< pair<float,int> >::iterator LIT;
    for( LIT i=sorted.begin();i!=sorted.end();i++ ){
        Graph_Node node=m_graph.GetData(i->second);
        if( isValid(cfg,node.getCFG()) || isValid(node.getCFG(), cfg) )
            return i->second;
    }//end for i

    return -1;
}

bool PRM::findPathV1V2(int v1, int v2, PATH& path)
{
    if( v1==v2 ) return true;
    vector< pair<Graph_Node,Graph_Edge> > nodes;
    typedef vector< pair<Graph_Node,Graph_Edge> >::iterator NIT;
    int size=FindPathDijkstra<WDG>(m_graph,v1,v2,nodes);
    if( size<=0 ) return false;

    for( NIT i=nodes.begin();i!=nodes.end();i++ ){
        path.push_back(i->first.getCFG());
    }//end for

    return true;
}

//
//
// Toggle PRM planner
//
//
TOGGLEPRM::TOGGLEPRM(float w, float h, float tr, float rr, unsigned int n_sample, unsigned int k_connection_free, unsigned int k_connection_obst) : Planner(w,h,tr,rr)
{
    m_n_sample=n_sample;
    m_k_closest_free=k_connection_free;
    m_k_closest_obst=k_connection_obst;
    m_skip_same_cc=true;
    m_create_good_loops=false;
}

TOGGLEPRM::~TOGGLEPRM()
{

}

bool TOGGLEPRM::findPath(const CFG& start, const CFG& goal)
{
    struct subset set_init;
    Graph_Node node_start, node_goal, node;
    VID vid_n, vid_start, vid_goal;

    this->m_start = (start);
    this->m_goal = (goal);

    if(this->isValid(this->m_start)){
        node_start.setCFG(this->m_start);
        vid_start = m_graph_free.GetNextVID();
        node_start.setID(vid_start);
        m_graph_free.AddVertex(node_start);

        set_init.parent = vid_start;
        set_init.rank = 0;
        m_set_free.push_back(set_init);
    }
    else{
        //fprintf(stderr, "start is not free\n");
        return false;
    }
    if(this->isValid(this->m_goal)){
        node_goal.setCFG(this->m_goal);
        vid_goal = m_graph_free.GetNextVID();
        node_goal.setID(vid_goal);
        m_graph_free.AddVertex(node_goal);

        set_init.parent = vid_goal;
        set_init.rank = 0;
        m_set_free.push_back(set_init);
    }
    else{
        //fprintf(stderr, "goal is not free\n");
        return false;
    }

    //Point2d O(env_width/2,env_height/2);

    bool done = false;
    Timer t; t.start();
    double elapsedTime = t.getElapsedCPUMilliseconds()/1000.0f;

    while(!done && elapsedTime < timeout){

        queue<CFG> q;
        vector<CFG> cfg = sample();
        for(unsigned i=0;i<cfg.size();++i){
            q.push(cfg[i]);
        }

        while(!done && !q.empty() && elapsedTime < timeout){

            CFG n = q.front();
            q.pop();
            if(!this->isValid(n)) // obstacle node
            {
                toggle = true;

                node.setCFG(n);
                vid_n = m_graph_obst.GetNextVID();
                node.setID(vid_n);
                m_graph_obst.AddVertex(node);

                set_init.parent = vid_n;
                set_init.rank = 0;
                m_set_obst.push_back(set_init);

                validNodes.clear();
                connect(vid_n);
                if(validNodes.size() > 0){
                    for(unsigned i=0;i<validNodes.size();++i){
                        q.push(validNodes[i]);
                    }
                }
            }
            else // free node
            {
                toggle = false;

                node.setCFG(n);
                vid_n = m_graph_free.GetNextVID();
                node.setID(vid_n);
                m_graph_free.AddVertex(node);

                set_init.parent = vid_n;
                set_init.rank = 0;
                m_set_free.push_back(set_init);

                collisionNodes.clear();
                connect(vid_n);
                if(collisionNodes.size() > 0){
                    for(unsigned i=0;i<collisionNodes.size();++i){
                        q.push(collisionNodes[i]);
                    }
                }
            }

            if(IsSameCCSet(m_set_free, vid_start, vid_goal)/*IsSameCC(m_graph_free, vid_start, vid_goal)*/){
                done = true;
            }

            elapsedTime = t.getElapsedCPUMilliseconds()/1000.0f;
        }

        if(IsSameCCSet(m_set_free, vid_start, vid_goal)/*IsSameCC(m_graph_free, vid_start, vid_goal)*/){
            done = true;
        }

        elapsedTime = t.getElapsedCPUMilliseconds()/1000.0f;
    }

    PATH mypath;
    done = findPathV1V2(vid_start,vid_goal,mypath);
    if(done){
        m_path.insert(m_path.end(),mypath.begin(),mypath.end());
        m_path.push_back(goal);
    }

    return done;
}

vector<CFG> TOGGLEPRM::sample()
{
  vector<CFG> nodes;
  for( int iN=0;iN<m_n_sample;iN++ ){
      CFG cfg=CFG::randomCfg();
      nodes.push_back(cfg);
  }
  return nodes;
}

void TOGGLEPRM::connect(VID vid_n)
{
    //double st=getTime();

    list< pair<int,int> > closest;

    //for node n find k closest
    sortedPairs(closest, vid_n);
    connectNodes(closest);
}


void TOGGLEPRM::sortedPairs(list< pair<int,int> >& close, VID vid_n)
{
    typedef pair<int,int> VPAIR;
    list< pair<float,VPAIR> > sorted;
    typedef list< pair<float,VPAIR> >::iterator SIT;

    vector<VID> vids;
    int nSize;
    if(toggle) nSize=m_graph_obst.GetVerticesVID(vids);
    else       nSize=m_graph_free.GetVerticesVID(vids);

    list< pair<float,VPAIR> > sorted_i;
    for( int i=0;i<nSize;i++ ){
        if(i==vid_n) continue;

        CFG cfg_i;
        if(toggle) cfg_i=m_graph_obst.GetData(vids[i]).getCFG();
        else       cfg_i=m_graph_free.GetData(vids[i]).getCFG();
        CFG cfg_n;
        if(toggle) cfg_n=m_graph_obst.GetData(vid_n).getCFG();
        else       cfg_n=m_graph_free.GetData(vid_n).getCFG();

        {
            VPAIR v(vids[i],vid_n);
            float dist=(cfg_n-cfg_i).norm();
            sorted_i.push_back(pair<float,VPAIR>(dist,v));
        }
        {
            VPAIR v(vid_n,vids[i]);
            float dist=(cfg_i-cfg_n).norm();
            sorted_i.push_back(pair<float,VPAIR>(dist,v));
        }
    }
    sorted_i.sort();
    SIT s;
    s=sorted_i.begin();
    if(toggle){
        int ik=0;
        for(;ik<1&&s!=sorted_i.end();ik++,++s){
            sorted.push_back(*s);
        }
        for(;ik<m_k_closest_obst&&s!=sorted_i.end();++s){
            VPAIR v = s->second;
            VID v1 = v.first;
            VID v2 = v.second;
            if(!IsSameCCSet(m_set_obst, v1, v2)){
                sorted.push_back(*s);
                ++ik;
            }
        }
        s=sorted_i.begin(); ++s;
        for(;ik<m_k_closest_obst&&s!=sorted_i.end();++s){
            VPAIR v = s->second;
            VID v1 = v.first;
            VID v2 = v.second;
            if(IsSameCCSet(m_set_obst, v1, v2)){
                sorted.push_back(*s);
                ++ik;
            }
        }
    }
    else{
        int ik=0;
        for(;ik<1&&s!=sorted_i.end();ik++,++s){
            sorted.push_back(*s);
        }
        for(;ik<m_k_closest_free&&s!=sorted_i.end();++s){
            VPAIR v = s->second;
            VID v1 = v.first;
            VID v2 = v.second;
            if(!IsSameCCSet(m_set_free, v1, v2)){
                sorted.push_back(*s);
                ++ik;
            }
        }
        s=sorted_i.begin(); ++s;
        for(;ik<m_k_closest_free&&s!=sorted_i.end();++s){
            VPAIR v = s->second;
            VID v1 = v.first;
            VID v2 = v.second;
            if(IsSameCCSet(m_set_free, v1, v2)){
                sorted.push_back(*s);
                ++ik;
            }
        }
    }

    sorted.sort();

    for(SIT i=sorted.begin();i!=sorted.end();i++){
        close.push_back(i->second);
    }
}

//connect pairs of nodes in closest
void TOGGLEPRM::connectNodes(list< pair<int,int> >& closest)
{
    typedef list< pair<int,int> >::iterator PIT;
    if(toggle){
        for( PIT i=closest.begin();i!=closest.end();i++)
        {
            //check the pair
            int vid1=i->first;
            int vid2=i->second;
            CFG cfg1=m_graph_obst.GetData(vid1).getCFG();
            CFG cfg2=m_graph_obst.GetData(vid2).getCFG();
            CFG cfg3;

            if(!m_graph_obst.IsEdge(vid1,vid2)){
                bool valid1 = isValid(cfg1,cfg2,cfg3,toggle);
                bool valid2 = isValid(cfg2,cfg1,cfg3,toggle);
                if(valid1 || valid2){
                    Union(m_set_obst, vid1, vid2);
                    m_graph_obst.AddEdge(vid1, vid2, Graph_Edge( (cfg1-cfg2).norm() ) );
                }
                else validNodes.push_back(cfg3);
            }
        }
    }
    else{
        for(PIT i=closest.begin();i!=closest.end();i++)
        {
            //check the pair
            int vid1=i->first;
            int vid2=i->second;
            CFG cfg1=m_graph_free.GetData(vid1).getCFG();
            CFG cfg2=m_graph_free.GetData(vid2).getCFG();
            CFG cfg3;

            if(!m_graph_free.IsEdge(vid1,vid2)){
                bool collision1 = isValid(cfg1,cfg2,cfg3,toggle);
                bool collision2 = isValid(cfg2,cfg1,cfg3,toggle);
                if(collision1 || collision2){
                    Union(m_set_free, vid1, vid2);
                    m_graph_free.AddEdge(vid1, vid2, Graph_Edge( (cfg1-cfg2).norm() ) );
                }
                else collisionNodes.push_back(cfg3);
            }
        }
    }
}

bool TOGGLEPRM::findPathV1V2(int v1, int v2, PATH& path)
{
    if( v1==v2 ) return true;
    vector< pair<Graph_Node,Graph_Edge> > nodes;
    typedef vector< pair<Graph_Node,Graph_Edge> >::iterator NIT;
    int size=FindPathDijkstra<WDG>(m_graph_free,v1,v2,nodes);
    if( size<=0 ) return false;

    for( NIT i=nodes.begin();i!=nodes.end();i++ ){
        path.push_back(i->first.getCFG());
    }//end for

    return true;
}

//connect cfg to rmap
int TOGGLEPRM::connect2Map(const CFG& cfg, int CCvid)
{
    //CFlock * m_F=rmap.getFlock();
    /////////////////////////////////////////////////////////////////
    //CRobot2D& geo=m_F->getGeometry();  //for collision detection
    //if( isCollision(*m_pEnv,geo,pos) ){
    //    return NULL;
    //}

    /////////////////////////////////////////////////////////////////
    vector<VID> nodes;
//    m_graph.GetVerticesData(nodes);
    GetCC(m_graph_free,CCvid,nodes);

    list< pair<float,int> > sorted;
    typedef vector<VID>::iterator NIT;
    for( NIT i=nodes.begin();i!=nodes.end();i++ )
    {
        Graph_Node node=m_graph_free.GetData(*i);
        float dist = (node.getCFG()-cfg).normsqr();
        if( dist<1e-10 )
            return *i;
        sorted.push_back( pair<float,int>(dist,*i) );
    }//end for i

    sorted.sort();
    typedef list< pair<float,int> >::iterator LIT;
    for( LIT i=sorted.begin();i!=sorted.end();i++ ){
        Graph_Node node=m_graph_free.GetData(i->second);
        if( isValid(cfg,node.getCFG()) || isValid(node.getCFG(), cfg) )
            return i->second;
    }//end for i

    return -1;
}

//
//
// Lazy Toggle PRM planner
//
//
LAZYTOGGLEPRM::LAZYTOGGLEPRM(float w, float h, float tr, float rr, unsigned int n_sample, unsigned int k_connection_free, unsigned int k_connection_obst) : Planner(w,h,tr,rr)
{
    m_n_sample=n_sample;
    m_k_closest_free=k_connection_free;
    m_k_closest_obst=k_connection_obst;
    m_skip_same_cc=true;
    m_create_good_loops=false;
}

LAZYTOGGLEPRM::~LAZYTOGGLEPRM()
{
}

bool LAZYTOGGLEPRM::findPath(const CFG& start, const CFG& goal)
{
    struct subset set_init;
    m_graph_free.EraseGraph();
    m_graph_mixed.EraseGraph();
    m_graph_obst.EraseGraph();

    VID vid_n, vid_start, vid_goal;

    this->m_start = (start);
    this->m_goal = (goal);

    if(this->isValid(this->m_start)){
        Graph_Node node;
        node.setCFG(m_start);
        vid_start = m_graph_mixed.GetNextVID();
        node.setID(vid_start);
        m_graph_mixed.AddVertex(node);

        set_init.parent = vid_start;
        set_init.rank = 0;
        m_set_mixed.push_back(set_init);
    }
    else return false;
    if(this->isValid(this->m_goal)){
        Graph_Node node;
        node.setCFG(this->m_goal);
        vid_goal = m_graph_mixed.GetNextVID();
        node.setID(vid_goal);
        m_graph_mixed.AddVertex(node);

        set_init.parent = vid_goal;
        set_init.rank = 0;
        m_set_mixed.push_back(set_init);
    }
    else return false;

    int runcc = 0;

    queue<CFG> q;
    Timer t; t.start();
    double elapsedTime = t.getElapsedCPUMilliseconds()/1000.0f;
    while(elapsedTime < timeout){

      //fprintf(fp, "%d Lazy Roadmap Construction\n", runcc);

      // Lazy Roadmap Construction
      while(!IsSameCCSet(m_set_mixed, vid_start, vid_goal) && elapsedTime < timeout){
          vector<CFG> cfg = lazySample();
          for(unsigned i=0;i<cfg.size();++i){
              Graph_Node node;
              node.setCFG(cfg[i]);
              vid_n = m_graph_mixed.GetNextVID();
              node.setID(vid_n);
              m_graph_mixed.AddVertex(node);

              set_init.parent = vid_n;
              set_init.rank = 0;
              m_set_mixed.push_back(set_init);

              lazyConnect(vid_n);
          }
          elapsedTime = t.getElapsedCPUMilliseconds()/1000.0f;
      }

      //fprintf(fp, "%d Path Validation\n", runcc);

      // Path Validation
      while(IsSameCCSet(m_set_mixed, vid_start, vid_goal) && elapsedTime < timeout){

          vector< pair<Graph_Node,Graph_Edge> > nodes;
          typedef vector< pair<Graph_Node,Graph_Edge> >::iterator NIT;
          int size=FindPathDijkstra<WDG>(m_graph_mixed,vid_start,vid_goal,nodes);
          if( size<=0 ) break;

          vector<VID> id_mypath;
          PATH mypath;
          //fprintf(stderr, "start\n");
          for( NIT i=nodes.begin();i!=nodes.end();i++ ){
              id_mypath.push_back(i->first.getID());
              mypath.push_back(i->first.getCFG());
              //int f = mypath.size()-1;
              //CFG mypath_phy = to_physical(mypath[f]);
              //fprintf(stderr, "dij path %d %f %f\n", id_mypath[f], mypath_phy.x, mypath_phy.y);
          }//end for
          //fprintf(stderr, "end\n\n");

          // check vertices of the path
          bool verticesAllValid = true;
          for(unsigned i=0;i<mypath.size();++i){
              if(!this->isValid(mypath[i])){
                  verticesAllValid = false;
                  q.push(mypath[i]);

                  vector<VID> nodes;
                  GetCC(m_graph_mixed,id_mypath[i],nodes);
                  m_set_mixed[id_mypath[i]].parent = -1;
                  m_set_mixed[id_mypath[i]].rank = 0;
                  for(int j=0;j<nodes.size();++j){
                      if(nodes[j] != id_mypath[i]){
                          m_set_mixed[nodes[j]].parent = nodes[j];
                          m_set_mixed[nodes[j]].rank = 0;
                      }
                  }

                  m_graph_mixed.DeleteVertex(id_mypath[i]);
                  set<int> sss;
                  sss.insert(id_mypath[i]);

                  for(int j=0;j<nodes.size();++j){
                      if(sss.find(nodes[j]) == sss.end()){
                          sss.insert(nodes[j]);
                          updateSet(m_set_mixed, m_graph_mixed, nodes[j], sss);
                      }
                  }
                  //break;
              }
              else{
                  Graph_Node node;
                  node.setCFG(mypath[i]);
                  node.setID(id_mypath[i]);
                  if(!m_graph_free.IsVertex(id_mypath[i])){
                      m_graph_free.AddVertex(node, id_mypath[i]);
                  }
              }
          }

          if(verticesAllValid){
              // check edges on the path
              bool edgesAllValid = true;
              typedef pair<int,int> VPAIR;
              for(unsigned i=0;i<mypath.size()-1;++i){
                  CFG witness;
                  if(!this->isValid(mypath[i], mypath[i+1], witness, false)){ // All nodes on the path are free!
                      edgesAllValid = false;
                      q.push(witness);
                      m_graph_mixed.DeleteEdge(id_mypath[i], id_mypath[i+1]);
                      if(!IsSameCC(m_graph_mixed, id_mypath[i], id_mypath[i+1])){

//                          if(m_set_mixed[id_mypath[i]].parent == id_mypath[i+1]){
//                              m_set_mixed[id_mypath[i]].parent = id_mypath[i];
//                              m_set_mixed[id_mypath[i]].rank = 0;
//                              //m_set_mixed[id_mypath[i]].rank = m_set_mixed[id_mypath[i]].rank - m_set_mixed[id_mypath[i+1]].rank;
//                          }

//                          if(m_set_mixed[id_mypath[i+1]].parent == id_mypath[i]){
//                              m_set_mixed[id_mypath[i+1]].parent = id_mypath[i+1];
//                              m_set_mixed[id_mypath[i+1]].rank = 0;
//                              //m_set_mixed[id_mypath[i+1]].rank = m_set_mixed[id_mypath[i+1]].rank - m_set_mixed[id_mypath[i]].rank;
//                          }

                          //fprintf(fp, "edge not connect %d %d\n", id_mypath[i], id_mypath[i+1]);
                          vector<VID> nodes;
                          GetCC(m_graph_mixed,id_mypath[i],nodes);
                          for(int j=0;j<nodes.size();++j){
                              //fprintf(fp, "CC1 %d\n", nodes[j]);
                              m_set_mixed[nodes[j]].parent = nodes[j];
                              m_set_mixed[nodes[j]].rank = 0;
                          }
                          updateSet(m_set_mixed, m_graph_mixed, id_mypath[i]);

                          nodes.clear();
                          GetCC(m_graph_mixed,id_mypath[i+1],nodes);
                          for(int j=0;j<nodes.size();++j){
                              //fprintf(fp, "CC2 %d\n", nodes[j]);
                              m_set_mixed[nodes[j]].parent = nodes[j];
                              m_set_mixed[nodes[j]].rank = 0;
                          }
                          updateSet(m_set_mixed, m_graph_mixed, id_mypath[i+1]);
                      }
                  }
                  else{
                      if(!m_graph_free.IsEdge(id_mypath[i], id_mypath[i+1])){
                          m_graph_free.AddEdge(id_mypath[i], id_mypath[i+1]);
                      }
                  }
              }
              if(edgesAllValid){
                  m_path.insert(m_path.end(),mypath.begin(),mypath.end());
                  m_path.push_back(goal);
                  return true;
              }
          }
          elapsedTime = t.getElapsedCPUMilliseconds()/1000.0f;
      }

      //fprintf(fp, "%d witness processing\n", runcc);

      // Witness Processing
      while(!q.empty() && !IsSameCCSet(m_set_mixed, vid_start, vid_goal) && elapsedTime < timeout){
          CFG n = q.front();
          q.pop();
          if(this->isValid(n)) // free node
          {
              Graph_Node node;
              node.setCFG(n);
              vid_n = m_graph_mixed.GetNextVID();
              node.setID(vid_n);
              m_graph_mixed.AddVertex(node);

              set_init.parent = vid_n;
              set_init.rank = 0;
              m_set_mixed.push_back(set_init);

              lazyConnect(vid_n);
          }
          else{               // obstacle node
              Graph_Node node;
              node.setCFG(n);
              vid_n = m_graph_obst.GetNextVID();
              node.setID(vid_n);
              m_graph_obst.AddVertex(node);

              set_init.parent = vid_n;
              set_init.rank = 0;
              m_set_obst.push_back(set_init);

              witness.clear();
              toggleConnect(vid_n);
              if(witness.size() > 0){
                  for(unsigned i=0;i<witness.size();++i){
                      q.push(witness[i]);
                  }
              }
          }
          elapsedTime = t.getElapsedCPUMilliseconds()/1000.0f;
      }
      elapsedTime = t.getElapsedCPUMilliseconds()/1000.0f;
      //fprintf(fp, "%d witness processing finish %lf\n", runcc++, elapsedTime);
    }

    fprintf(fp, "cc mixed graph\n");

    vector< pair<int,VID> > ccstats;
    int ccsize=GetCCStats(m_graph_mixed,ccstats);
    for(int i=0;i<ccsize;i++)
    {
        int CCvid=ccstats[i].second;
        vector<VID> nodes;
        GetCC(m_graph_mixed,CCvid,nodes);
        fprintf(fp, "graph i %d  ccvid %d    set %d\n", i, CCvid, Find(m_set_mixed, CCvid));

        typedef vector<VID>::iterator NIT;
        for( NIT j=nodes.begin();j!=nodes.end();++j){
            fprintf(fp, "graph i %d nodes %d    set %d\n", i, *j, Find(m_set_mixed, *j));
        }
        fprintf(fp,"\n");
    }


    fprintf(fp, "\n\ncc free graph %d %d\n", vid_start, vid_goal);

    ccstats.clear();
    ccsize=GetCCStats(m_graph_free,ccstats);
    for(int i=0;i<ccsize;i++)
    {
        int CCvid=ccstats[i].second;
        vector<VID> nodes;
        GetCC(m_graph_free,CCvid,nodes);
        fprintf(fp, "graph i %d  ccvid %d\n", i, CCvid);

        typedef vector<VID>::iterator NIT;
        for( NIT j=nodes.begin();j!=nodes.end();++j){
            fprintf(fp, "graph i %d nodes %d\n", i, *j);
        }
        fprintf(fp,"\n");
    }



    return false;
}

bool LAZYTOGGLEPRM::findPathV1V2(int v1, int v2, PATH& path)
{
    if( v1==v2 ) return true;
    vector< pair<Graph_Node,Graph_Edge> > nodes;
    typedef vector< pair<Graph_Node,Graph_Edge> >::iterator NIT;
    int size=FindPathDijkstra<WDG>(m_graph_mixed,v1,v2,nodes);
    if( size<=0 ) return false;

    for( NIT i=nodes.begin();i!=nodes.end();i++ ){
        path.push_back(i->first.getCFG());
    }//end for

    return true;
}

void LAZYTOGGLEPRM::updateSet(vector<struct subset> &m_set, WDG m_graph, VID vid_n, set<int> &sss){
    vector<VID> nodes;
    GetCC(m_graph,vid_n,nodes);
    for(int i=0;i<nodes.size();++i){
        if(sss.find(nodes[i]) == sss.end()){
            sss.insert(nodes[i]);
            Union(m_set, vid_n, nodes[i]);
        }
    }
}

void LAZYTOGGLEPRM::updateSet(vector<struct subset> &m_set, WDG m_graph, VID vid_n){
    vector<VID> nodes;
    GetCC(m_graph,vid_n,nodes);
    for(int i=0;i<nodes.size();++i){
        if(nodes[i] != vid_n){
            Union(m_set, vid_n, nodes[i]);
        }
    }
}


vector<CFG> LAZYTOGGLEPRM::lazySample()
{
  vector<CFG> nodes;
  for( int iN=0;iN<m_n_sample;iN++ ){
      CFG cfg=CFG::randomCfg();
      nodes.push_back(cfg);
  }
  return nodes;
}

void LAZYTOGGLEPRM::lazyConnect(VID vid_n)
{
    list< pair<int,int> > closest;

    //for each node find k cloest
    lazySortedPairs(closest, vid_n);
    lazyConnectNodes(closest);
}


void LAZYTOGGLEPRM::lazySortedPairs(list< pair<int,int> >& close, VID vid_n)
{
    typedef pair<int,int> VPAIR;
    list< pair<float,VPAIR> > sorted;
    typedef list< pair<float,VPAIR> >::iterator SIT;

    vector<VID> vids;
    int nSize=m_graph_mixed.GetVerticesVID(vids);

    list< pair<float,VPAIR> > sorted_i;
    for( int i=0;i<nSize;i++ ){
        if(i==vid_n) continue;

        CFG cfg_i=m_graph_mixed.GetData(vids[i]).getCFG();
        CFG cfg_n=m_graph_mixed.GetData(vid_n).getCFG();

        {
            VPAIR v(vids[i],vid_n);
            float dist=(cfg_n-cfg_i).norm();
            sorted_i.push_back(pair<float,VPAIR>(dist,v));
        }
        {
            VPAIR v(vid_n,vids[i]);
            float dist=(cfg_i-cfg_n).norm();
            sorted_i.push_back(pair<float,VPAIR>(dist,v));
        }
    }
    sorted_i.sort();
    SIT s;
    s=sorted_i.begin();
    int ik=0;
    for(;ik<1&&s!=sorted_i.end();ik++,++s){
        sorted.push_back(*s);
    }
    for(;ik<m_k_closest_free&&s!=sorted_i.end();++s){
        VPAIR v = s->second;
        VID v1 = v.first;
        VID v2 = v.second;
        if(!IsSameCCSet(m_set_mixed,v1,v2)){
            sorted.push_back(*s);
            ++ik;
        }
    }
    s=sorted_i.begin(); ++s;
    for(;ik<m_k_closest_free&&s!=sorted_i.end();++s){
        VPAIR v = s->second;
        VID v1 = v.first;
        VID v2 = v.second;
        if(IsSameCCSet(m_set_mixed,v1,v2)){
            sorted.push_back(*s);
            ++ik;
        }
    }

    sorted.sort();

    for(SIT i=sorted.begin();i!=sorted.end();i++){
        close.push_back(i->second);
    }
}

//connect pairs of nodes in closest
void LAZYTOGGLEPRM::lazyConnectNodes(list< pair<int,int> >& closest)
{
    typedef list< pair<int,int> >::iterator PIT;
    for( PIT i=closest.begin();i!=closest.end();i++)
    {
        //check the pair
        int vid1=i->first;
        int vid2=i->second;
        if(!m_graph_mixed.IsEdge(vid1,vid2)){
            CFG cfg1=m_graph_mixed.GetData(vid1).getCFG();
            CFG cfg2=m_graph_mixed.GetData(vid2).getCFG();
            Union(m_set_mixed, vid1, vid2);
            m_graph_mixed.AddEdge(vid1, vid2, Graph_Edge( (cfg1-cfg2).norm() ) );
        }
    }
}

void LAZYTOGGLEPRM::toggleConnect(VID vid_n)
{
    list< pair<int,int> > closest;

    //for each node find k cloest
    toggleSortedPairs(closest, vid_n);
    toggleConnectNodes(closest);
}


void LAZYTOGGLEPRM::toggleSortedPairs(list< pair<int,int> >& close, VID vid_n)
{
    typedef pair<int,int> VPAIR;
    list< pair<float,VPAIR> > sorted;
    typedef list< pair<float,VPAIR> >::iterator SIT;

    vector<VID> vids;
    int nSize=m_graph_obst.GetVerticesVID(vids);

    list< pair<float,VPAIR> > sorted_i;
    for( int i=0;i<nSize;i++ ){
        if(i==vid_n) continue;

        CFG cfg_i=m_graph_obst.GetData(vids[i]).getCFG();
        CFG cfg_n=m_graph_obst.GetData(vid_n).getCFG();

        {
            VPAIR v(vids[i],vid_n);
            float dist=(cfg_n-cfg_i).norm();
            sorted_i.push_back(pair<float,VPAIR>(dist,v));
        }
        {
            VPAIR v(vid_n,vids[i]);
            float dist=(cfg_i-cfg_n).norm();
            sorted_i.push_back(pair<float,VPAIR>(dist,v));
        }
    }
    sorted_i.sort();
    SIT s;
    s=sorted_i.begin();
    int ik=0;
    for(;ik<1&&s!=sorted_i.end();ik++,++s){
        sorted.push_back(*s);
    }
    for(;ik<m_k_closest_obst&&s!=sorted_i.end();++s){
        VPAIR v = s->second;
        VID v1 = v.first;
        VID v2 = v.second;
        if(!IsSameCCSet(m_set_obst,v1,v2)){
            sorted.push_back(*s);
            ++ik;
        }
    }
    s=sorted_i.begin(); ++s;
    for(;ik<m_k_closest_obst&&s!=sorted_i.end();++s){
        VPAIR v = s->second;
        VID v1 = v.first;
        VID v2 = v.second;
        if(IsSameCCSet(m_set_obst,v1,v2)){
            sorted.push_back(*s);
            ++ik;
        }
    }

    sorted.sort();

    for(SIT i=sorted.begin();i!=sorted.end();i++){
        close.push_back(i->second);
    }
}

//connect pairs of nodes in closest
void LAZYTOGGLEPRM::toggleConnectNodes(list< pair<int,int> >& closest)
{
    typedef list< pair<int,int> >::iterator PIT;
    for( PIT i=closest.begin();i!=closest.end();i++)
    {
        //check the pair
        int vid1=i->first;
        int vid2=i->second;
        CFG cfg1=m_graph_obst.GetData(vid1).getCFG();
        CFG cfg2=m_graph_obst.GetData(vid2).getCFG();
        CFG cfg3;

        if(!m_graph_obst.IsEdge(vid1,vid2)){
            bool valid1 = isValid(cfg1,cfg2,cfg3,true);
            bool valid2 = isValid(cfg2,cfg1,cfg3,true);

            if(valid1 || valid2){
                Union(m_set_obst, vid1, vid2);
                m_graph_obst.AddEdge(vid1, vid2, Graph_Edge( (cfg1-cfg2).norm() ) );
            }
            else witness.push_back(cfg3);
        }
    }
}

//
//
// Guassian PRM planner
//
//
GaussianPRM::GaussianPRM(float w, float h, float tr, float rr, unsigned int n_sample, unsigned int k_connection,  float mean, float std)
:PRM(w,h,tr,rr,n_sample,k_connection)
{
    m_std=std;
    m_mean_d=mean;
}

GaussianPRM::~GaussianPRM()
{
}

void GaussianPRM::sample()
{
    cout<<"- Gaussian PRM smaple ("<<m_n_sample<<" nodes) "<<endl;

    //double st=getTime();
    for( int iN=0;iN<m_n_sample;iN++ ){
        CFG cfg=CFG::randomCfg();

        //not vaid...
        if(isValid(cfg))
        {
            continue;
        }

        //create another cfg around this cfg
        CFG cfg2=CFG::randomCfg();
        float d=gauss(m_mean_d,m_std);
        //cout<<"d="<<d<<" m_mean_d="<<m_mean_d<<" m_std="<<m_std<<endl;
        cfg2=cfg+(cfg2-cfg).normalize()*d;
        if(isValid(cfg2)==false)
        {
            continue;
        }

        //put into roadmap
        Graph_Node node;
        node.setCFG(cfg2);
        node.setID(m_graph.GetNextVID());
        int id=m_graph.AddVertex(node);
    }

    //double et=getTime();

    cout<<"- Gaussian PRM sampling done ("<<m_graph.GetVertexCount()<<" valid configurations found)"<<endl; // takes "<<et-st<<" msec"<<endl;
}

