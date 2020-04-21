/*
 * RRT.h
 *
 *  Created on: Oct 12, 2013
 *      Author: zhonghua
 */

#ifndef RRT_H_
#define RRT_H_

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




struct RRT_CFG {
	double x;
	double y;
	double t1;	//theta1
	double t2;	//theta2
	RRT_CFG() {}
	RRT_CFG(const RRT_CFG& c) {
		this->init(c.x, c.y, c.t1, c.t2);
	}
	void init(double x, double y, double t1, double t2) {
		this->x = x;
		this->y = y;
		this->t1 = t1;
		this->t2 = t2;
	}
	RRT_CFG(double x, double y, double t1, double t2) {
		this->init(x, y, t1, t2);
	}

	double dis(const RRT_CFG& c) const {
		return sqrt(SQR(x-c.x) + SQR(y-c.y)) + 0.02*sqrt(SQR(t1-c.t1) + SQR(t2-c.t2));
	}

	inline double normal() const {
		return sqrt(SQR(x) + SQR(y));
	}

	RRT_CFG normalize() const {
		RRT_CFG nor = *this;
		double normal_trans = sqrt(SQR(this->x) + SQR(this->y));
		if(normal_trans != 0) { nor.x/=normal_trans; nor.y/=normal_trans; }
		double normal_rot = sqrt(SQR(this->t1) + SQR(this->t2));
		if(normal_rot != 0) { nor.t1/=normal_rot; nor.t2/=normal_rot; }
		return nor;
	}

	RRT_CFG operator+(const RRT_CFG& rhs) const{
			RRT_CFG r(this->x + rhs.x, this->y + rhs.y, this->t1 + rhs.t1, this->t2 + rhs.t2);
			return r;
	}

	RRT_CFG operator-(const RRT_CFG& rhs) const{
		RRT_CFG r(this->x - rhs.x, this->y - rhs.y, this->t1-rhs.t1, this->t2-rhs.t2);
		return r;
	}

	RRT_CFG operator*(const double rhs) const{
			RRT_CFG r(this->x*rhs, this->y*rhs, this->t1*rhs, this->t2*rhs);
			return r;
	}

	RRT_CFG operator/(const double rhs) const{
		if(rhs != 0) {
			double s = 1.0 / rhs;
			return this->operator *(s);
		}
		else {
			return RRT_CFG();
		}
	}


	friend ostream& operator<<(ostream& o, const RRT_CFG& c) {
		o<<"("<<c.x<<","<<c.y<<","<<c.t1<<","<<c.t2<<")";
		return o;
	}
};

struct RRT_ROBOT {
	static const int SAMPLES = 9;
	int L1;
	int L2;
	RRT_CFG cfg;
	Vector2d p[3];
	Vector2d ps[2*SAMPLES+1];
	void init(int l1, int l2, const RRT_CFG& c) {
		L1=l1;
		L2=l2;
		this->setCFG(c);
	}
	void setCFG(const RRT_CFG& c) {
		this->cfg = c;
		this->p[0][0] = c.x;
		this->p[0][1] = c.y;
		this->p[1] = this->p[0] + Vector2d(this->L1*cos(c.t1/180*PI), this->L1*sin(c.t1/180*PI));
		this->p[2] = this->p[0] + Vector2d(this->L2*cos(c.t2/180*PI), this->L2*sin(c.t2/180*PI));

		for(int i=1;i<=SAMPLES;i++)
		{
			this->ps[i] = p[0] + (p[1] - p[0]) * i / (SAMPLES + 1);
			this->ps[i+SAMPLES] = p[0] + (p[2] - p[0]) * i / (SAMPLES + 1);
		}
	}
};

struct RRT_NODE {
	RRT_CFG cfg;
	RRT_NODE* parent;
	RRT_NODE(const RRT_CFG& c, RRT_NODE* parent = NULL) {
		this->cfg = c;
		this->parent = parent;
	}
};

struct RRT_RECT {
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


typedef vector<RRT_NODE*> RRT_TREE;
typedef vector<c_ply> RRT_OBJS;
typedef vector<RRT_CFG> RRT_PATH;

typedef vector<RRT_NODE*>::iterator NIT;
typedef vector<RRT_CFG>::iterator PIT;
typedef vector<RRT_CFG>::const_iterator CPIT;
typedef RRT_TREE::const_iterator CTIT;
typedef RRT_OBJS::const_iterator CRIT;
typedef RRT_OBJS::iterator RIT;

class RRT {
public:
	RRT();
	virtual ~RRT();
	bool findPath(const RRT_CFG& start, const RRT_CFG& goal);
	const RRT_PATH getPath() const { return this->m_path; }

	const RRT_TREE& getTree() const { return this->m_tree; }

	void addObj(const c_ply& ply) { this->m_objs.push_back(ply); }
	RRT_OBJS& getPolygons() { return this->m_objs; }

	const RRT_CFG& getStart() const { return this->m_start; }
	const RRT_CFG& getGoal() const { return this->m_goal; }

	RRT_ROBOT& getRobot() { return this->m_robot; }
private:
	bool isValid(const RRT_CFG& cfg);
	bool moveTo(const RRT_CFG& nearest_cfg, const RRT_CFG& rand_cfg, RRT_CFG& out_cfg);

	bool m_found;
	RRT_ROBOT m_robot;

	RRT_CFG generateRandomCFG();
	RRT_NODE* nearest(const RRT_CFG& cfg);

	RRT_CFG m_start;
	RRT_CFG m_goal;

	RRT_TREE m_tree;

	RRT_OBJS m_objs;

	RRT_PATH m_path;
};

#endif /* RRT_H_ */
