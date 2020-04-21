/*
 * main.h
 *
 *  Created on: Nov 27, 2012
 *      Author: zhonghua
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <sstream>
using namespace std;
#include "RRT.h"

#include "draw.h"

string filename;
int samples = 20;
int mc = 0;

Planner * planner=NULL;


#endif /* MAIN_H_ */
