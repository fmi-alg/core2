/*
 * RRT.cpp
 *
 *  Created on: Oct 12, 2013
 *      Author: zhonghua
 */

#include "RRT.h"

const int 	RRT_MAX_SAMPLES = 65535;	// MAXMIUM SAMPLES
const float RRT_ENV_WIDTH = 512;			// ENV WIDTH
const float RRT_ENV_HEIGHT = 512;			// ENV HEIGHT

const float RRT_TR = 2;						// TRANSLATIONAL RESOLUTION
const float RRT_RR = 5;						// Rotational RESOLUTION (deg)
const int RRT_MAX_STEP = 20;				// MAXMIUM movement steps
const float RRT_BIAS = 0.1;					// bias towards to the goal
const float RRT_CLOSE_TO_GOAL = 5;


RRT::RRT() {
	c_ply bb(c_ply::PIN);
	bb.beginPoly();
	bb.addVertex(-10, -10);
	bb.addVertex(-10, RRT_ENV_HEIGHT + 10);
	bb.addVertex(RRT_ENV_WIDTH + 10, RRT_ENV_HEIGHT + 10);
	bb.addVertex(RRT_ENV_WIDTH + 10, -10);
	bb.endPoly(false);

	this->addObj(bb);
}

RRT::~RRT() {
	// clear memory
	for(NIT it=this->m_tree.begin(); it!=this->m_tree.end(); ++it) {
		delete *it;
	}
}



bool RRT::findPath(const RRT_CFG& start, const RRT_CFG& goal) {
	this->m_start = start;
	this->m_goal = goal;

	RRT_NODE* start_node = new RRT_NODE(start);
	this->m_tree.push_back(start_node);

	double min_dist = start.dis(goal);

	this->m_found = false;

	for(int i=0;i<RRT_MAX_SAMPLES && !m_found;i++) {
		RRT_CFG rand_cfg = this->generateRandomCFG();
		//if(!isValid(rand_cfg)) continue;

		RRT_NODE* nearest_node = this->nearest(rand_cfg);

		//if(nearest_node->cfg.dis(rand_cfg) < RRT_ST) continue;

		RRT_CFG out_cfg;

		if(this->moveTo(nearest_node->cfg, rand_cfg, out_cfg)) {
			RRT_NODE* new_node = new RRT_NODE(out_cfg, nearest_node);
			this->m_tree.push_back(new_node);
			//cout<<"expend to cfg = "<<out_cfg<<endl;

			double dist = out_cfg.dis(goal);
			if(dist < min_dist) {
				min_dist = dist;
				cout<<"["<< m_tree.size() << "/"<<i<<"] min_dist = "<<min_dist<<endl;
			}

			if(dist <= RRT_CLOSE_TO_GOAL) {
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

// ========================================
// TODO: check whether a cfg is valid or not
bool RRT::isValid(const RRT_CFG& cfg){

	this->m_robot.setCFG(cfg);
	for(RIT it=this->m_objs.begin(); it!=this->m_objs.end(); ++it) {

		if(it->getType() == c_ply::POUT) {
			if(it->enclosed(this->m_robot.p[0])) return false;
			if(it->enclosed(this->m_robot.p[1])) return false;
			if(it->enclosed(this->m_robot.p[2])) return false;

			for(int i=0;i<2*m_robot.SAMPLES;i++)
			{
				if(it->enclosed(this->m_robot.ps[i])) return false;
			}
		}else {
			if(!it->enclosed(this->m_robot.p[0])) return false;
			if(!it->enclosed(this->m_robot.p[1])) return false;
			if(!it->enclosed(this->m_robot.p[2])) return false;

			for(int i=0;i<2*m_robot.SAMPLES;i++)
			{
				if(!it->enclosed(this->m_robot.ps[i])) return false;
			}
		}
	}

	return true;
}

bool RRT::moveTo(const RRT_CFG& nearest_cfg, const RRT_CFG& rand_cfg, RRT_CFG& out_cfg)
{
	RRT_CFG dir = (rand_cfg - nearest_cfg).normalize();
	RRT_CFG dir_goal = (m_goal - nearest_cfg).normalize();

	RRT_CFG step = (dir * (1 - RRT_BIAS) + dir_goal * RRT_BIAS) * RRT_TR;
	step.x *= RRT_TR;
	step.y *= RRT_TR;
	step.t1 *= RRT_RR;
	step.t2 *= RRT_RR;

	double dist = (rand_cfg - nearest_cfg).normal();

	int steps = min((int)(dist/RRT_TR), RRT_MAX_STEP);

	for(int i=1;i<=steps;i++){
		RRT_CFG now_cfg = nearest_cfg + step*i;
		if(!this->isValid(now_cfg)) {
			if(i>1) { out_cfg = nearest_cfg + step*(i-1); return true; }
			return false;
		}
		if(i==steps || now_cfg.dis(m_goal) < 5*RRT_TR) {
			out_cfg = now_cfg;
			return true;
		}
	}

	return false;
}

RRT_NODE* RRT::nearest(const RRT_CFG& cfg){
	RRT_NODE* node = NULL;
	double dist = 1e99;
	for(NIT it = this->m_tree.begin(); it!=this->m_tree.end(); ++it){
		double d = (*it)->cfg.dis(cfg);
		if(d<dist) { dist = d; node = (*it); }
	}
	return node;
}

//TODO: generate a random cfg according to the environment
RRT_CFG RRT::generateRandomCFG(){
	RRT_CFG cfg;
	cfg.x = (double)rand()*RRT_ENV_WIDTH/RAND_MAX;
	cfg.y = (double)rand()*RRT_ENV_WIDTH/RAND_MAX;
//	double r1 = (double)rand()/RAND_MAX;
//	if(r1<0.5)
//	{
//		if(r1<0.25)
//		{
//			cfg.t2 = cfg.t1 = (double)rand()*360/RAND_MAX;
//		}
//		else{
//			cfg.t1 = (double)rand()*360/RAND_MAX;
//			cfg.t2 = 180 + cfg.t1;
//			if(cfg.t2 > 360) cfg.t2 -= 360;
//		}
//	}else{
		cfg.t1 = (double)rand()*360/RAND_MAX;
		cfg.t2 = (double)rand()*360/RAND_MAX;
	//}
	return cfg;
}
