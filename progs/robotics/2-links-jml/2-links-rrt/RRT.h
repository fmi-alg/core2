/*
 * RRT.h
 *
 *  Created on: Oct 12, 2013
 *      Author: zhonghua
 */

#ifndef RRT_H_
#define RRT_H_

#ifdef _WIN32
#pragma warning( disable : 4018 4267 4244 4305)
#endif

/*

//
// 
// interval tree
//
//
#include "RectangleTree.h"
typedef itree::RectKD<itree::EndPoint,2> Rect2D; //2d rectangle...
typedef itree::MiddleStructure<Rect2D,2> MTree;
typedef itree::RectangleTree<MTree, 2> ITree; //interval tree

*/

///////////////////////////////////////////////////////////////////////////////
// start define graph here...
#ifndef _STL_PORT
#define _STL_PORT
#endif
#include "graph/Graph.h"
#include "graph/GraphAlgo.h"
using namespace graph;

///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include "Vector.h"
#include "polygon.h"
using namespace mathtool;
using namespace std;

#ifndef SQR
#define SQR(x) ((x)*(x))
#endif

///////////////////////////////////////////////////////////////////////////////

#include <Point.h>
using namespace mathtool;

///////////////////////////////////////////////////////////////////////////////

struct CFG
{
    //all these values are between -1 and 1
	double x;
	double y;
	double t1;	//theta1
	double t2;	//theta2

	bool ws; //true if the cfg is in parametric form. false if the cfg is in physical form.

	CFG() { x=y=t1=t2=0; ws=true; }

	CFG(const CFG& c)
	{
		this->init(c.x, c.y, c.t1, c.t2, c.ws);
	}

	void init(double x, double y, double t1, double t2, bool ws=true)
	{
		this->x = x;
		this->y = y;
		this->t1 = t1;
		this->t2 = t2;
		this->ws = ws;
	}

	CFG(double x, double y, double t1, double t2, bool ws=true)
	{
		this->init(x, y, t1, t2, ws);
	}

	static CFG randomCfg()
	{
	    CFG c;
	    c.x=drand48(); if(drand48()<0.5) c.x=-c.x;
	    c.y=drand48(); if(drand48()<0.5) c.y=-c.y;
	    c.t1=drand48(); if(drand48()<0.5) c.t1=-c.t1;
	    c.t2=drand48(); if(drand48()<0.5) c.t2=-c.t2;

	    return c;
	}

	/*
	double dis(const CFG& c) const
	{
		return sqrt(SQR(x-c.x) + SQR(y-c.y) + SQR(t1-c.t1) + SQR(t2-c.t2));
	}
	*/

	CFG to_physical(double dx, double dy, const Point2d& o) const
	{
	    assert(ws);
	    CFG nc;
	    nc.x=x*dx*0.5+o[0];
	    nc.y=y*dy*0.5+o[1];
	    nc.t1=t1*180;
	    nc.t2=t2*180;
	    nc.ws=false;

	    return nc;
	}

    CFG to_paramtric(double dx, double dy, const Point2d& o)  const
    {
        assert(ws==false);

        CFG nc;
        nc.x=(x-o[0])/(dx*0.5);
        nc.y=(y-o[1])/(dy*0.5);
		nc.t1=this->euler_round(t1/180);
        nc.t2=this->euler_round(t2/180);
        nc.ws=true;

        return nc;
    }

	inline double normT() const
	{
	    assert(ws);
		return sqrt(SQR(x) + SQR(y));
	}

	inline double normR() const
	{
	    assert(ws);
	    return sqrt(SQR(this->t1) + SQR(this->t2));
	}

	inline double norm() const
	{
	    assert(ws);
	    return sqrt(normsqr());
	}

    inline double normsqr() const
    {
        assert(ws);
        return SQR(x) + SQR(y) + .01*SQR(t1) + .01*SQR(t2);
    }

	CFG normalize() const
	{
	    assert(ws);
		CFG nor = *this;
		double n = norm();
		if(n != 0) { nor.x/=n; nor.y/=n; nor.t1/=n; nor.t2/=n; }
		return nor;
	}

	CFG operator+(const CFG& rhs) const
	{
	    assert(ws);
		CFG r(this->x + rhs.x, this->y + rhs.y, this->t1 + rhs.t1, this->t2 + rhs.t2);
		r.round();
		return r;
	}

	CFG operator-(const CFG& rhs) const
	{
	    assert(ws);
		CFG r(this->x - rhs.x, this->y - rhs.y, this->t1-rhs.t1, this->t2-rhs.t2);
		r.round();
		return r;
	}

	CFG operator*(const double rhs) const
	{
	    assert(ws);
		CFG r(this->x*rhs, this->y*rhs, this->t1*rhs, this->t2*rhs);
		r.round();
		return r;
	}

	bool inbound() const
	{
	    assert(ws);
	    if(x>1 || x<-1) return false;
	    if(y>1 || y<-1) return false;
	    if(t1>1 || t1<-1) return false;
	    if(t2>1 || t2<-1) return false;
	    return true;
	}

	void round()
	{
	    assert(ws);
	    //x=translation_round(x);
	    //y=translation_round(y);
	    t1=euler_round(t1);
	    t2=euler_round(t2);
	}

	CFG operator/(const double rhs) const
	{
		if(rhs != 0) {
			double s = 1.0 / rhs;
			return this->operator *(s);
		}
		else {
			return CFG();
		}
	}

	//compute e1 - e2
	//e1 and e2 should be between -1 and 1
	double euler_diff(double e1, double e2) const
	{
	    double diff1=(e1-e2);
	    double diff2=(e1<e2)?(e1+2-e2):(e1-e2+2);
	    double d = ( fabs(diff1)<fabs(diff2) )? diff1: diff2;
	    assert(d<=1 && d>=-1); //the result must be between -1 and 1s
	    return d;
	}

    //around e between -1 and 1
    double translation_round(double t) const
    {
        if(t>1) return 1;
        if(t<-1) return -1;
        return t;
    }

	//around e between -1 and 1
    double euler_round(double e) const
    {
        return fmod((e+1),2)-1;
    }

	bool operator==(const CFG& cfg) const { return (cfg.x==x && cfg.y==y && cfg.t1==t1 && cfg.t2==t2); }

	friend ostream& operator<<(ostream& o, const CFG& c) {
		o<<"("<<c.x<<","<<c.y<<","<<c.t1<<","<<c.t2<<")";
		return o;
	}
};

struct RRT_ROBOT {

	int L1;
	int L2;
	CFG cfg;
	Vector2d p[3];

	void init(int l1, int l2, const CFG& c) {
		L1=l1;
		L2=l2;
		this->setCFG(c);
	}

	void setCFG(const CFG& c)
	{
		this->cfg = c;
		this->p[0][0] = c.x;
		this->p[0][1] = c.y;
		float ratio=PI/180;
		this->p[1] = this->p[0] + Vector2d(this->L1*cos(c.t1*ratio), this->L1*sin(c.t1*ratio));
		this->p[2] = this->p[0] + Vector2d(this->L2*cos(c.t2*ratio), this->L2*sin(c.t2*ratio));	
	}
};

struct RRT_NODE {
	CFG cfg;
	RRT_NODE* parent;
	RRT_NODE(const CFG& c, RRT_NODE* parent = NULL) {
		this->cfg = c;
		this->parent = parent;
	}
};

struct RRT_RECT 
{
	double x;
	double y;
	double w;
	double h;
	RRT_RECT() {}
	RRT_RECT(double x, double y, double w, double h) {this->x=x;this->y=y;this->w=w;this->h=h;}

	bool contains(const Vector2d& v) const {
			return (v[0] >= this->x && v[0] <= this->x + this->w && v[1] >= this->y && v[1] <= this->y + this->h);
	}

	bool contains(double x, double y) const {
		return (x >= this->x && x <= this->x + this->w && y >= this->y && y <= this->y + this->h);
	}
};


///////////////////////////////////////////////////////////////////////////////
class Graph_Node 
{
    friend ostream & operator<<( ostream & out, const Graph_Node & node );
    friend istream & operator>>( istream &  in, Graph_Node & node );
public:
    ///////////////////////////////////////////////////////////////////////
    //  Constructor/Destructor
    Graph_Node();
    ~Graph_Node();

    bool operator==( const Graph_Node & other ) const;

    //////////////////////////////////////////////////////////////////////
    //      Access Method
    static Graph_Node & InvalidData(){return m_InvalidValue;}
    void setCFG(CFG & cfg) { m_cfg=cfg; }
    const CFG & getCFG() const { return m_cfg; }
    void setClearance(float clear) { m_Clear=clear; }
    float getClearance() const { return m_Clear; }
    void setID(VID id) { m_ID=id; }
    VID getID() const { return m_ID; }

//////////////////////////////////////////////////////////////////////
//      Private Method & Data
private:
    static Graph_Node m_InvalidValue;
    VID m_ID;
    CFG m_cfg;
    float m_Clear; //clearance
};

///////////////////////////////////////////////////////////////////////////////
class Graph_Edge 
{
public:
    friend ostream & operator<<( ostream & out, const Graph_Edge & node );
    friend istream & operator>>( istream &  in, Graph_Edge & node );
public:
    ///////////////////////////////////////////////////////////////////////
    //  Constructor/Destructor
    Graph_Edge();
    Graph_Edge(float weight);
    ~Graph_Edge();

    bool operator==( const Graph_Edge & other ) const;

    //////////////////////////////////////////////////////////////////////
    //      Access Method
    static Graph_Edge & InvalidWeight(){return m_InvalidValue;}
    float Weight(){ return m_Weight; }
    static float MaxWeight(){ return 1e10; }

//////////////////////////////////////////////////////////////////////
//      Private Method & Data
private:
    static Graph_Edge m_InvalidValue;
    float m_Weight;
};

ostream & operator<<( ostream & out, const Graph_Node & node );
istream & operator>>( istream &  in, Graph_Node & node );
ostream & operator<<( ostream & out, const Graph_Edge & node );
istream & operator>>( istream &  in, Graph_Edge & node );

///////////////////////////////////////////////////////////////////////////////
//weighted directed graph
typedef Graph<UG<Graph_Node,Graph_Edge>,NMG<Graph_Node,Graph_Edge>,WG<Graph_Node,Graph_Edge>,Graph_Node,Graph_Edge> WDG;

///////////////////////////////////////////////////////////////////////////////
typedef vector<RRT_NODE*> RRT_TREE;
typedef vector<c_ply> RRT_OBJS;
typedef vector<CFG> PATH;

typedef vector<RRT_NODE*>::iterator NIT;
typedef vector<CFG>::iterator PIT;
typedef vector<CFG>::const_iterator CPIT;
typedef RRT_TREE::const_iterator CTIT;
typedef RRT_OBJS::const_iterator CRIT;
typedef RRT_OBJS::iterator RIT;



//
//
// A generic motion planner
//
//
class Planner 
{

public:

	//workspace width, heght, translational resolution, rotational resolution...
	Planner(float w, float h, float tr, float rr);

	virtual bool findPath(const CFG& start, const CFG& goal)=0;

	const PATH& getPath() const { return this->m_path; }

	void addObj(const c_ply& ply) { this->m_objs.push_back(ply); }

	RRT_OBJS& getPolygons() { return this->m_objs; }

	const CFG& getStart() const { return this->m_start; }

	const CFG& getGoal() const { return this->m_goal; }

	RRT_ROBOT& getRobot() { return this->m_robot; }

	CFG to_parametric(const CFG& cfg);

	CFG to_physical(const CFG& cfg);

protected:

	bool isValid(const CFG& cfg);

	bool isValid(const CFG& c1, const CFG& c2);

	//build interval tree
	void build_itree();

	/// return true if there is collision
	/// otherwise return false...
	bool query_itree(const CFG& cfg);

	bool m_found;

	RRT_ROBOT m_robot;

	CFG m_start;
	CFG m_goal;

	RRT_OBJS m_objs;

	float env_width;			// ENV WIDTH
	float env_height;			// ENV HEIGHT
	
	float env_TR;				// TRANSLATIONAL RESOLUTION
	float env_RR;				// Rotational RESOLUTION (deg)

	PATH m_path;

	//ITree m_iTree; //an iterval tree

	//segment made of ply_vertex * ptr and ptr->next
	vector<ply_vertex *> m_segments;
};


//
//
// RRT planner
//
//
class RRT : public Planner
{

public:

	RRT(float w, float h, float tr, float rr, unsigned int max_sample, float expand_step, float bias, float close_to_goal);

	virtual ~RRT();

	virtual bool findPath(const CFG& start, const CFG& goal);

	RRT_TREE& getTree() { return m_tree; }

protected:

	bool moveTo(const CFG& nearest_cfg, const CFG& rand_cfg, CFG& out_cfg);

	RRT_NODE* nearest(const CFG& cfg);

	RRT_TREE m_tree;

	unsigned int m_max_sample;

	float m_expand_step; //maximum rrt expand distance per step

	float m_goal_bias;

	float m_close_to_goal;
};


//
//
// PRM planner
//
//
class PRM : public Planner
{

public:

	PRM(float w, float h, float tr, float rr, unsigned int n_sample, unsigned int k_connection);

	virtual ~PRM();

	virtual bool findPath(const CFG& start, const CFG& goal);

	WDG& getGraph() { return m_graph; }

protected:

	virtual void sample();

	virtual void connect();

	void sortedPairs(list< pair<int,int> >& close);

	void connectNodes(list< pair<int,int> >& close);

	int connect2Map(const CFG& cfg);

	bool findPathV1V2(int v1, int v2, PATH& path);

	WDG m_graph;

	unsigned int m_n_sample;
	unsigned int m_k_closest;

	bool m_skip_same_cc;
	bool m_create_good_loops;
};


//
//
// Gaussian PRM planner
//
//
class GaussianPRM : public PRM
{

public:

	GaussianPRM(float w, float h, float tr, float rr, unsigned int n_sample, unsigned int k_connection, float mean, float std);

	virtual ~GaussianPRM();

protected:

	virtual void sample();

	float m_mean_d, m_std;
};


#endif /* RRT_H_ */
