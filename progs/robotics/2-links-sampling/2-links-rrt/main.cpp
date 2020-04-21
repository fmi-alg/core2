/*
 * main.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: zhonghua, George Mason University
 *  
 *  Modified: Jyh-Ming Lien, Nov. 23, 2013
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include "main.h"

int seed;

//environment related parameters
CFG start(250,360,0,150,false);
CFG goal(260,80, 70, 320,false);
int l1,l2;
float scale=1.0;
float deltaX=0.0, deltaY=0.0;

float env_width = 512;			// ENV WIDTH
float env_height = 512;			// ENV HEIGHT
float env_TR = 0.02;				// TRANSLATIONAL RESOLUTION
float env_RR = 0.02;				// Rotational RESOLUTION (deg)

//common samping-based motion planner parameters
string method="planner"; //planner, prm, ...
unsigned int max_sample_size=10000;

//planner parameters
float rrt_step_size=0.01;
float rrt_bias = 0.1;					// bias towards to the goal
float rrt_close_to_goal = env_TR;

//prm parameters
unsigned int prm_closest_k=15;

//gaussian prm paramters
float gauss_mean_d=0.1;
float gauss_std=0.1;

/* ********************************************************************** */
// skip blanks, tabs, line breaks and comment lines,
//  leaving us at the beginning of a token (or EOF)
//  (This code is taken from CORE lib)
inline int skip_comment_line (std::ifstream & in)
{
      int c;

      do {
        c = in.get();
        while ( c == '#' ) {
          do {// ignore the rest of this line
            c = in.get();
          } while ( c != '\n' );
          c = in.get(); // now, reach the beginning of the next line
        }
      } while (c == ' ' || c == '\t' || c == '\n' || c == '\r'); //ignore white spaces and newlines

      in.putback(c);  // this is non-white and non-comment char!
      return c;

}//skip_comment_line


void readInput(const string& filename) 
{
	ifstream fin;
	fin.open(filename.c_str());
	char buf[1024];
	int size = -1;
	int left = -1;
	int num_of_polygons = -1;
	vector<float> pts;
	while(skip_comment_line(fin)!=EOF)
	{
		fin.getline(buf, 1024);
		string line(buf);
		if(buf[0] == '#' || line.size() == 0) continue;

		//remove trailing white space chars and checking "\\" 
		do{
			while( isspace(line.back()) ) line.pop_back();
			if(line.back()=='\\'){ //there is "\\" at the end... read more
				line.pop_back();
				fin.getline(buf, 1024);
				string append(buf);
				line.append(append.begin(),append.end());
			}
			else break;
		}while(true);

		//cout<<"line=\""<<line<<"\""<<endl;

		stringstream ss(line);
		if(size == -1) {
			ss >> size;		//read point size

			//cout<<"pt size="<<size<<endl;

			left = size*2;
		}
		else if(left != 0)
		{
			float p;
			while(ss>>p) {
				pts.push_back(p);
				left--;
			}
		}
		else if(num_of_polygons == -1)
		{
			ss>>num_of_polygons; //) { cout<<"num_of_polygons="<<num_of_polygons<<endl; }
			//cout<<"number of polygons = "<<num_of_polygons<<endl;
		}
		// polygon
		else if(num_of_polygons>0)
		{
			//c_polygon polygon;
			c_ply ply(c_ply::POUT);
			ply.beginPoly();
			vector<int> indics;
			int index;
			//ss>>index;
			while(ss>>index) {
				index--;
				ply.addVertex(pts[index*2]* scale + deltaX, pts[index*2+1]* scale + deltaY, false);
			}
			ply.endPoly(false);
			//cout<<"polygon size = "<<ply.getSize()<<" area = "<<ply.getArea()<<" type = "<<ply.getType()<<endl;

			planner->addObj(ply);
			num_of_polygons--;
		}
		//
		//sstream
	}

    //assert(planner->getPolygons().size()==num_of_polygons+1); //+1 for the bounding box
}

int parseArg(int argc, char** argv)
{
	if(argc == 1) return 0;

	filename = "inputs/basic3.txt";
	seed = time(NULL);

	if(argc > 1)
	{
		filename = string(argv[1]);
	}

	if(argc >= 10) {

		start.x = atof(argv[2]);
		start.y = atof(argv[3]);
		start.t1 = atof(argv[4]);
		start.t2 = atof(argv[5]);

		goal.x = atof(argv[6]);
		goal.y = atof(argv[7]);
		goal.t1 = atof(argv[8]);
		goal.t2 = atof(argv[9]);

		l1 = 50;
		l2 = 80;
	}

	if(argc >= 12) {
		l1 = atoi(argv[10]);
		l2 = atoi(argv[11]);
	}

	if(argc >= 13)
	{
		scale = atof(argv[12]);
	}

	if(argc >= 15){
		deltaX = atof(argv[13]);
		deltaY = atof(argv[14]);
	}

	if(argc >=16){
		method = argv[15];
	}

	if(argc >= 17) {
		seed = atoi(argv[16]);
	}

	if(argc >=18){
		max_sample_size=atoi(argv[17]);
	}

	if(method=="rrt" || method=="RRT" || method=="Rrt")
	{
		if(argc >=19){
			rrt_step_size=atof(argv[18]);
		}

		if(argc >=20){
			rrt_bias=atof(argv[19]);
		}

		if(argc >=21){
			rrt_close_to_goal=atof(argv[20]);
		}
	}
	else if(method=="prm" || method=="PRM" || method=="Prm")
	{
		if(argc >=19){
			prm_closest_k=atoi(argv[18]);
		}
	}
	else if(method=="gauss" || method=="GAUSS" || method=="Gauss")
	{
		if(argc >=19){
			prm_closest_k=atoi(argv[18]);
		}

		if(argc >=20){
			gauss_mean_d=atof(argv[19]);
		}

		if(argc >=21){
			gauss_std=atof(argv[20]);
		}
	}
	else{
		cerr<<"! Error: Unknown method="<<method<<endl;
		return false;
	}

	cout<<"- method="<<method<<endl;
	cout<<"- seed="<<seed<<endl;
	cout<<"- start="<<start<<endl;
	cout<<"- goal="<<goal<<endl;
	cout<<"- planner max sample size="<<max_sample_size<<endl;

	if(method=="rrt" || method=="RRT" || method=="Rrt")
	{
		cout<<"- rrt step size="<<rrt_step_size<<endl;
		cout<<"- rrt goal bias="<<rrt_bias<<endl;
		cout<<"- rrt close to goal="<<rrt_close_to_goal<<endl;
	}
	else if(method=="prm" || method=="PRM" || method=="Prm")
	{
		cout<<"- prm connection k="<<prm_closest_k<<endl;
	}
	else if(method=="gauss" || method=="GAUSS" || method=="Gauss")
	{
		cout<<"- gaussian mean d="<<gauss_mean_d<<endl;
		cout<<"- gaussian std="<<gauss_std<<endl;
	}
	cout<<"- l1="<<l1<<endl;
	cout<<"- l2="<<l2<<endl;
	cout<<"- scale="<<scale<<endl;
	cout<<"- delta X,Y="<<deltaX<<", "<<deltaY<<endl;

	return true;
}

int main(int argc, char** argv)
{
	if(!parseArg(argc, argv))
	{
		cout<<"Usage: \t"<<argv[0]<<" input_file [x1 y1 t11 t12 x2 y2 t21 t22 l1 l2 scale dx dy mehtod seed #samples stepsize]"<<endl;
		return 0;
	}
	else
	{
		planner=NULL;

		if(method=="rrt" || method=="RRT" || method=="Rrt")
			//float w, float h, float tr, float rr, unsigned int max_sample, float expand_step, float bias, float close_to_goal
			planner = new RRT(env_width, env_height, env_TR, env_RR, max_sample_size, rrt_step_size,rrt_bias,rrt_close_to_goal);
		else if(method=="prm" || method=="PRM" || method=="Prm")
			planner = new PRM(env_width, env_height, env_TR, env_RR, max_sample_size,prm_closest_k);
		else if(method=="gauss" || method=="GAUSS" || method=="Gauss")
			planner = new GaussianPRM(env_width, env_height, env_TR, env_RR, max_sample_size,prm_closest_k,gauss_mean_d, gauss_std);
				
		if(planner==NULL)
			return 1;

		readInput(filename);

		srand(seed);

		// init robot
		planner->getRobot().init(l1, l2, start);

		// find the path
		start = planner->to_parametric(start); 
		goal = planner->to_parametric(goal);

		if(planner->findPath(start, goal))
		{
			planner->getRobot().setCFG(start);

			vector<CFG> path = planner->getPath();

			cout<<"- Path found (";

			/*for(PIT it=path.begin(); it!=path.end(); ++it) {
				cout<<*it<<endl;
			}*/

			cout<<"length = "<<path.size()<<")"<<endl;
		}
		else {
			cout<<"! Path not found"<<endl;
		}


		Draw3D(argc,argv);
	}

	return 0;
}
