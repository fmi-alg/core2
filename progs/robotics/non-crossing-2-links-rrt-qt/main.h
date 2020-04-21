/*
 *  main.h
 *	
 *  Author: zhonghua, Ching-Hsiang (Tom) Hsu
 *	Email: chhsu@nyu.edu
 *  Created on: Nov 27, 2012
 *  Modified: Oct. 31, 2016 (Tom)
 */

#ifndef MAIN_H_
#define MAIN_H_


#include <QDir>
#include <QApplication>
#include <QSurfaceFormat>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#include "RRT.h"
#include "utils/Timer.h"
#include "MainWindow.h"

int samples = 20;
int mc = 0;

Planner * planner;


#endif /* MAIN_H_ */
