#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>

#include "./sss/SoftSubdivisionSearch.h"
#include "./sss/Graph.h"
#include "./strategy/PriorityQueue.h"
#include "./utility/Timer.h"
#include "./utility/Parser.h"
#include "./utility/RenderUtils.h"
#include "MainWindow.h"


// External Routines ========================================
//
void run();
SoftSubdivisionSearch* sss;
/////SoftSubdivisionSearch* genEmptyTree();

// GLOBAL INPUT Parameters ========================================
//
char running_dir[300];
double alpha[3] = {375, 420, 50};   // start configuration
double beta[3] = {175, 150, 450};   // goal configuration
double epsilon = 10;                // resolution parameter
double boxWidth = 512;              // Initial box width
double R = 10;                      // Ball radius
double L = 0;                       // Rod length
int windowPosX = 250;               // X Position of Window
int windowPosY = 150;               // Y Position of Window
int QType = 1;                      // The Priority Queue can be sequential (1) or random (0)
int interactive = 0;                // Run interactively? Yes (0) or No (1)
int seed = 999;                     // seed for random number generator (Could also be used for BFS, etc)
double eye[3] = {0, 256, 1280};
double at[3] = {0, 256, 0};
double up[3] = {0, 1, 0};
double eyeVector[3];
int transparency = 80;
double distanceZ = 900;
float view_rotate_angles[3] = {0, 0, 0};

// Global Variables ========================================
//

#define mw_out (*window)
static MainWindow *window;
vector<ConfBox3d*> path;
static int runCounter = 1;

float initialRot[3] = {0, 1, 0};
float finalRot[3] = {0, 0, 1};
float obj_pos[3] = { static_cast<float>(-boxWidth/2.), static_cast<float>(-boxWidth/2.), static_cast<float>(2.*boxWidth) };
float view_rotate[16] = { 1.0, 0.0, 0.0, 0.0,
                          0.0, 1.0, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0 };
float view_rotate_x[16] = { 1.0, 0.0, 0.0, 0.0,
                          0.0, 1.0, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0 };
float view_rotate_y[16] = { 1.0, 0.0, 0.0, 0.0,
                          0.0, 1.0, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0 };
float view_rotate_z[16] = { 1.0, 0.0, 0.0, 0.0,
                          0.0, 1.0, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0 };
float topViewPos[3] = { static_cast<float>(boxWidth / 2.0), static_cast<float>(boxWidth / 2.0), static_cast<float>(boxWidth * 2.5) };

unsigned int iPathSeg = 0;
unsigned int inSegCount = 0;

bool noPath = true;         // True means there is "No path".
bool showBox = false;  		// don't draw box boundary
bool showAnim = false;
bool finishedAnim = false;

int frameRate = 50;
float segCount = 0;
