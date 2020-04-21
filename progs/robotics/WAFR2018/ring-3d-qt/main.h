#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

#include "./sss/SoftSubdivisionSearch.h"
#include "./utility/Parser.h"
#include "./utility/RenderUtils.h"
#include "./utility/Timer.h"
#include "CORE/geom3d/point3d.h"
#include "CORE/geom3d/polygon3d.h"
#include "display/Color.h"
#include "display/MainWindow.h"
#include "path/path.h"
#include "sss/Graph.h"
#include "strategy/PriorityQueue.h"

// External Routines ========================================
//
void run();
SoftSubdivisionSearch *sss;

// GLOBAL INPUT Parameters ========================================
//

// debug usage
FILE *g_fptr = NULL;

#define mw_out (*window)
MainWindow *window;

string cfgName("WAFR18_rand100_2.cfg");
char cfgNameList[200][200];
int numCfg = 0;
string workingDir;
string inputDir("inputs/WAFR18");  // Path for input files
string fileName("empty.txt");      // Input file name
string defaultFilePath("inputs/empty.txt");

Point3d start(300, 450, 260);  // start configuration
Point3d goal(175, 50, 360);    // goal configuration
Point3d startRot(0, 1, 0);     // x hat, y hat, z hat
Point3d goalRot(-1, 0, 0);
double epsilon = 4;      // resolution parameter
double boxWidth = 512;   // Initial box width
double boxHeight = 512;  // Initial box height
double R = 36;           // Ring radius
int windowPosX = 260;    // X Position of Window
int windowPosY = 150;    // Y Position of Window
int searchType = 2;   // The Priority Queue can be sequential (1) or random (0)
int interactive = 0;  // Run interactively? Yes (0) or No (1)
int seed =
    999;  // seed for random number generator (Could also be used for BFS, etc)

// map parameter
double scale = 1;   // scaling of input environment
double deltaX = 0;  // x-translation of input environment
double deltaY = 0;  // y-translation of input environment
double deltaZ = 0;  // z-translation of input environment

// camera setting
Point3d eye(512, 512, 512);
Point3d at(0, 0, 0);
Vector up(0, 1, 0);
Vector eyeVector(0, 0, 0);

// render setting
int transparency = 80;
double boxTransparency = 0.2f;
double distanceZ = 900;
unsigned int iPathSeg = 0;
float inSegCount = 1;
float segCount = 32;
bool showPath = true;
bool showTrace = false;
bool generateTrace = false;
bool showBox = false;  // don't draw box
bool showBoxBoundary = false;
bool showAnim = true;
bool pauseAnim = false;
bool replayAnim = false;
int animationSpeed = 99;
// float view_rotate_angles[3] = {0, 0, 0};

vector<ConfBox3d *> path;
vector<vector<float>> pathDisplay;
static int runCounter = 1;
bool noPath = true;  // True means there is "No path".
bool moveAway(true);
int frameRate = 50;
bool verbose(false);

Point3d Bt(0, 0, 0);
Point3d Br(0, 1, 0);
double Btw(8);
double Brw(2);
bool showRing(false);
Triangle3d W;
bool showWall(false);
Segment3d E;
bool showEdge(false);
bool showCorner(false);
int traceDense(16);

// Global Variables ========================================
//
Point3d obj_pos(static_cast<float>(-boxWidth / 2.),
                static_cast<float>(-boxWidth / 2.),
                static_cast<float>(2. * boxWidth));
float view_rotate[16] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                         0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
float view_rotate_x[16] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
float view_rotate_y[16] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
float view_rotate_z[16] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
Point3d topViewPos(static_cast<float>(boxWidth / 2.0),
                   static_cast<float>(boxWidth / 2.0),
                   static_cast<float>(boxWidth * 2.5));
