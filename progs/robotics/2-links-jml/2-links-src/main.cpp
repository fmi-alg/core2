/*
 * main.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: zhonghua
 */

#include <iostream>
#include <fstream>
#include <string>

#include "main.h"

int seed;
RRT_CFG start(250,360,0,150);
RRT_CFG goal(260,80, 70, 320);
int l1,l2;
float scale=1.0;
float deltaX=0.0, deltaY=0.0;

void readInput(const string& filename) {
	ifstream fin;
	fin.open(filename.c_str());
	char buf[1024];
	int size = -1;
	int left = -1;
	int num_of_polygons = -1;
	vector<float> pts;
	while(!fin.eof())
	{
		fin.getline(buf, 1024);
		string line(buf);
		if(buf[0] == '#' || line.size() == 0) continue;
		cout<<line<<endl;
		stringstream ss(line);
		if(size == -1) {
			ss >> size;		//read point size
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
			while(ss>>num_of_polygons) {}
			cout<<"number of polygons = "<<num_of_polygons<<endl;
		}
		// polygon
		else {
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
			cout<<"polygon size = "<<ply.getSize()<<" area = "<<ply.getArea()<<" type = "<<ply.getType()<<endl;

			rrt->addObj(ply);
		}
		//
		//sstream
	}
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

	if(argc >= 16) {
		seed = atoi(argv[15]);
	}

	return 1;
}

int main(int argc, char** argv)
{
	if(!parseArg(argc, argv))
	{
		cout<<"Usage: \t"<<argv[0]<<" input_file [x1 y1 t11 t12 x2 y2 t21 t22 l1 l2 scale dx dy seed]"<<endl;
		return 0;
	}
	else
	{
		rrt = new RRT();

		readInput(filename);	// arg1

		//return 0;

		srand(seed);

		// init robot
		rrt->getRobot().init(l1, l2, start);	// len1, len2, startConf, (?goal)

		// find the path
		if(rrt->findPath(start, goal))
		{
			rrt->getRobot().setCFG(start);

			vector<RRT_CFG> path = rrt->getPath();

			cout<<"Path : "<<endl;

			for(PIT it=path.begin(); it!=path.end(); ++it) {
				cout<<*it<<endl;
			}

			cout<<"length = "<<path.size()<<endl;
		}
		else {
			cout<<"path not found!"<<endl;
		}


		Draw3D(argc,argv);
	}
}
