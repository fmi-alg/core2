#include "./sss/SoftSubdivisionSearch.h"
#include "./strategy/PriorityQueue.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "./sss/Graph.h"
#include "./utility/Timer.h"
#include <set>
#include "./utility/Parser.h"
#include "./utility/RenderUtils.h"

// External Routines ========================================
//
void renderCustomView(void);
void keyOperations(void);
void reshapeCustomView(int width, int height);
void idle(int v);
void keyPressed (unsigned char key, int x, int y);
void keyUp (unsigned char key, int x, int y);
void run();
SoftSubdivisionSearch* genEmptyTree();

SoftSubdivisionSearch* sss;

// GLOBAL INPUT Parameters ========================================
//
double alpha[3] = {75, 360, 100};  // start configuration
double beta[3] = {450, 50, 400};  // goal configuration
double epsilon = 5;      // resolution parameter
double boxWidth = 512;      // Initial box width
double R0 = 30;        // Robot radius
int windowPosX = 250;      // X Position of Window
int windowPosY = 150;      // Y Position of Window
int QType = 1;        // The Priority Queue can be sequential (1) or random (0)
int interactive = 0;      // Run interactively? Yes (0) or No (1)
unsigned int seed = 111;        // seed for random number generator (Could also be used for BFS, etc)
double eye[3] = {0, 256, 1280};
double at[3] = {0, 256, 0};
double up[3] = {0, 1, 0};
double eyeVector[3];
int transparency = 80;
float view_rotate_angles[3] = {0, 0, 0};
float initialRot[3] = {0, 1, 0};
float finalRot[3] = {0, 0, 1};

vector<ConfBox3d*> path;
bool noPath = true;      // True means there is "No path".

// GLUI controls ========================================
//
GLUI_RadioGroup* radioQType;
GLUI_RadioGroup* radioDrawOption;
GLUI_EditText* editInput;
GLUI_EditText* editDir;
GLUI_EditText* editRadius;
GLUI_EditText* editEpsilon;
GLUI_EditText* editAlphaX;
GLUI_EditText* editAlphaY;
GLUI_EditText* editAlphaZ;
GLUI_EditText* editBetaX;
GLUI_EditText* editBetaY;
GLUI_EditText* editBetaZ;
GLUI_EditText* editEyeX;
GLUI_EditText* editEyeY;
GLUI_EditText* editEyeZ;
GLUI_EditText* editLookAtX;
GLUI_EditText* editLookAtY;
GLUI_EditText* editLookAtZ;
GLUI_EditText* editSeed;
GLUI_Rotation *viewRot;
GLUI_TextBox * output;
stringstream outputStream;

// Global Variables ========================================
//
int runCounter = 1;
int customViewWindowID;
int topViewWindowID;
float topViewXYAspect;
float customViewXYAspect;
float obj_pos[] = { static_cast<float>(-boxWidth/2.), static_cast<float>(-boxWidth/2.), static_cast<float>(2.*boxWidth) };
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

bool showAnim = true;
unsigned int iPathSeg = 0;
unsigned int inSegCount = 0;
bool finishedAnim = false;
int frameRate = 65;
float segCount = 0;

bool useUserDefinedViewpoint = false;
bool useCustomViewAngles = false;
