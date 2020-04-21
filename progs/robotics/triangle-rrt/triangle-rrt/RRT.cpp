/*
 * RRT.cpp
 *
 *  Created on: Oct 12, 2013
 *      Author: zhonghua
 */

#include "RRT.h"
#include <cfloat>
#include "intersection.h"
#include "Gauss.h"

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

//
//
// A generic motion planner
//
//

Planner::Planner(float w, float h, float tr, float rr)
{ 

	m_found = false;
	env_width=w; env_height=h; env_TR=tr; env_RR=rr; 

	//add a bounding box
	c_ply bb(c_ply::PIN);
	bb.beginPoly();
	bb.addVertex(-10, -10);
	bb.addVertex(-10, env_height + 10);
	bb.addVertex(env_width + 10, env_height + 10);
	bb.addVertex(env_width + 10, -10);
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
			for(int i=0;i<3;i++)
				if(it->enclosed(this->m_robot.p[i])) return false;
		}
		else 
		{	
			for(int i=0;i<3;i++)
				if(!it->enclosed(this->m_robot.p[i])) return false;
		}

		//check edges...
		for( int l=0;l<3;l++)
		{
			//robot link
			const Point2d& a=this->m_robot.p[l];
			const Point2d& b=this->m_robot.p[(l+1)%3];

			ply_vertex * ptr=it->getHead();
			
			do{
				ply_vertex * next=ptr->getNext();
			
				//create p
				const Point2d& c=ptr->getPos();
				const Point2d& d=next->getPos();

				//
				bool r=SegSegInt(a.get(),b.get(),c.get(),d.get());
				if(r) return false; //collision found

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

    //dir.theta /= 10;				// avoid spin

	double dist = dir.norm();     // distance form nearest to rand
	double exp_dist=min(dist,(double)m_expand_step); // move in dir for dist

	dir=dir.normalize()*exp_dist;

	int steps = (int) ceil(max((dir.normT()/env_TR), (dir.normR()/env_RR)));

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
        if(isValid(cfg)==false)
		{
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

    //for each node find k cloest 
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
	
    for( int i=0;i<nSize;i++ )
	{
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
// PRM planner
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

