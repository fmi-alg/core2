/* **************************************
 File: 2-links.cpp

 Description:
 This is the entry point for the running the SSS algorithm
 for a 2-link robot amidst a collection of polygonal obstacles.

 To run, call with these positional arguments
 (some sample values are given):

 > ./2-link [interactive = 0] \
		[start-x = 10] [start-y = 360] [start-theta1 = 0] [start-theta2= 90]\
		[goal-x = 500] [goal-y = 20][goal-theta1 = 180]  [goal-theta2= 270]\
		[epsilon = 5] \
		[len1 = 30] [len2 = 50]\
		[fileName = input2.txt] \
		[boxWidth = 512] [boxHeight = 512] \
		[windoxPosX = 400] [windowPosY = 200] \
		[Qtype = 3] [seed = 111] [inputDir = inputs] \
		[offsetX = 0] [offsetY = 0] [scale = 1] \
		[verbose = 0] [title = "Eg 4: Bug trap example" ] \
		[smarterStrategy = 1-or-0] \
		[threshold-for-smarterStrategy = 1-to-8] \
		&

 where:
 interactive 	 		is nature of run
 (0 is interactive, >0 is non-interactive)
 start (x,y,theta1,theta2)	is initial configuration
 goal (x,y,theta1,theta2)	is final configuration
 epsilon			is resolution parameter
 (1 or greater)
 len1, len2			are lengths of the 2 links
 fileName			is input (text) file describing the environment
 box Width/Height		is initial box dimensions
 windowPos(x,y)		is initial position of window
 Qtype			is type of the priority queue
 (0=random, 1=BFS, 2=Greedy, 3=Dist+Size, 4=Vor)
 seed				is seed for random number generator
 inputDir			is directory for input files
 offset(X,Y), scale		is the offset and scaling for input environment
 smarterStrategy		chooses either original splitting or smarter strategy
 threshold			is the parameter used by smarter strategy
 (say, a small integer between 0 and 10).

 See examples of running this program in the Makefile.

 Format of input environment: see README FILE

 HISTORY: May-Oct, 2013: 2-links version by Zhongdi Luo and Chee Yap
 (started out by adapting the triangle code of Cong Wang)

 Since Core Library  Version 2.1
 $Id: 2-links.cpp,v 1.3 2012/10/26 04:26:52 cheeyap Exp cheeyap $
 ************************************** */

#ifdef __CYGWIN32__
#include "GL/glui.h"
#endif
#ifdef __linux__
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glui.h>
//#include "GL/glui.h"
#endif
#ifdef _WIN32
#include <gl/glew.h>
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUI/glui.h>
#endif

#include "QuadTree.h"
#include "PriorityQueue.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.h"
#include "Timer.h"
#include "stdlib.h"
#include "Polygon.h"

#include <set>
//#include "CoreIo.h"
//#include <pthread.h>

using namespace std;

// SHAPE OF TRIANGULAR ROBOT:
// 	It is a triangle inscribed in a disc centered at the origin.
// 	The radius of the disc is R0, which the user can specify below.
//
// 	The triangle is determined by two angles 
//
//		0 < theta_1 < theta_2 < 2
//
//	There is an implicit third angle, which is theta_0 = 0.
//
//      The angles values are in multiples of Pi radians
//	(so theta_1 = 1.0 corresponds to Pi radians or 180 degrees)
//
//     	E.g., an equilateral robot would be 
//		theta_1 = 0.6667, theta_2 = 1.3333.
//
// CHOOSE ONE OF THESE:
//
// (a) Acute Triangle Robot:  THIS IS THE DEFAULT -- most examples
// 	in the Makefile are designed to give interesting results with this robot.
//
//double triRobo[2] = { 0.833333333, 1.0 };
//
// (b) Equilateral Triangle Robot:
// double triRobo[2] = {0.666666667, 1.333333333};
//
// (c) Stick Robot (very thin)
//  double triRobo[2] = {0.95, 1.05};
//
// (d) Right-Angle Isosceles Robot 
// double triRobo[2] = {0.5, 1.0};
//
// (e) Off-Center Robot 
// double triRobo[2] = {0.3, 0.6};

// GLOBAL INPUT Parameters ========================================
//////////////////////////////////////////////////////////////////////////////////
double alpha[4] = { 300, 120, 80, 30 };		// start configuration
double beta[4] = { 30, 30, 100, 50 };		// goal configuration
double epsilon = 8;			// resolution parameter
Box* boxA;				// start box (containing alpha)
Box* boxB;				// goal box (containing beta)
double boxWidth = 512;			// Initial box width
double boxHeight = 512;			// Initial box height

// Added by Zhongdi 05/08/2013 begin
// length of 2 links
double L1 = 35;
double L2 = 30;

// thickness of robot
double thickness = 10;

double R0 = 0;				// will be set to max(L1,L2)
// Added by Zhongdi 05/08/2013 end

int windowPosX = 320;			// X Position of Window
int windowPosY = 20;			// Y Position of Window
string fileName("bugtrap.txt"); 		// Input file name
//string fileName("input150.txt"); 		// Input file name
string inputDir("inputs"); 		// Path for input files
int QType = 2;				// The Priority Queue can be
//    sequential (0) or random (1)
int interactive = 0;			// Run interactively?
//    Yes (0) or No (1)
int seed = 111;				// seed for random number generator
// (Could also be used for BFS, etc)
double deltaX = 0;			// x-translation of input environment
double deltaY = 0;			// y-translation of input environment
double scale = 1;				// scaling of input environment
bool noPath = true;			// True means there is "No path.

bool hideBoxBoundary = false;  		// don't draw box boundary
bool shadeOption = false;        // don't shade the obstacles
bool verboseOption = false;		// don't print various statistics

int drawPathOption = 0;
int twoStrategyOption = 0; //  Two-Strategy Option    0: original 1: smarter
string title("2-links Control Panel");	// title for control panel
int sizeOfPhiB = 0;

vector<Box*> PATH;

extern vector<Polygon> polygons;
extern vector<int> srcInPolygons;

//dijkstraQueue<Cmp> dijQ;

int timePerFrame = 10;

// GLOBAL VARIABLES ========================================
//////////////////////////////////////////////////////////////////////////////////
int freeCount = 0;
int stuckCount = 0;
int mixCount = 0;
int mixSmallCount = 0;

//controls triangle drawing along path
const int TRIS_TO_SKIP = 20;
const double DIST_TO_SKIP = 2;

int renderCount = 0;
//int countAAA = 0;
//int countBBB = 0;
//int countCCC = 0;

stringstream ssout;
stringstream ssoutLastTime;
stringstream ssTemp;
stringstream ssInfo;

//volatile bool renderLock = false;
//volatile bool timerLock = false;
//bool blinkFlag = false;

vector<Box*> boxClicked;

//volatile bool renderReady = false;

char** argvSave;
int argcSave;

bool leafBoxesDrawed = false;

int runCount = 0;

int firstPolygonClockwise = 0;

// GLUI controls ========================================
//////////////////////////////////////////////////////////////////////////////////
GLUI_RadioGroup* radioStepsPerFrame;
GLUI_EditText* textCustomSpeed;
GLUI_EditText* textCurrentStep;
GLUI_RadioGroup* radioDrawPathOption;
GLUI_RadioGroup* radioQType;
GLUI_RadioGroup* radioDrawOption;
GLUI_RadioGroup* radioShadeOption;
GLUI_RadioGroup* radio2StrategyOption;
GLUI_EditText* textPhiB;
GLUI_RadioGroup* radioVerboseOption;
GLUI_EditText* editInput;
GLUI_EditText* editDir;
//GLUI_EditText* editRadius;
GLUI_EditText* editL1;
GLUI_EditText* editL2;
GLUI_EditText* editThickness;
GLUI_EditText* editEpsilon;
GLUI_EditText* editAlphaX;
GLUI_EditText* editAlphaY;
GLUI_EditText* editAlphaTheta1;
GLUI_EditText* editAlphaTheta2;
GLUI_EditText* editBetaX;
GLUI_EditText* editBetaY;
GLUI_EditText* editBetaTheta1;
GLUI_EditText* editBetaTheta2;
GLUI_EditText* editSeed;

GLUI_TextBox* textBox;
GLUI_TextBox* boxSelectedInfo;
//GLUI_RadioGroup* radioDrawPathOption;

GLuint fbo;
GLuint depthBuffer;			// Our handle to the depth render buffer
GLuint img;					// Our handle to a texture
// External Routines ========================================
//////////////////////////////////////////////////////////////////////////////////
void renderScene(void);
void parseConfigFile(Box*);
//void idle();
void replaySplitting();
void runAnimation();
void previousStep(int);
void nextStep(int);
void run();
void genEmptyTree();
void drawPath(vector<Box*>&);
extern int fileProcessor(string inputfile);
void drawCircle(float Radius, int numPoints, double x, double y, double r,
		double g, double b);
void filledCircle(double radius, double x, double y, double r, double g,
		double b);
void drawLine();
//void drawTri(Box*);
//void drawTri(Box*, double, double);
void drawLinks(Box*);
//void drawLinks(Box*, double, double);
void resetAndRun();
void reset();
void initFbo();
void redrawFBO();
int checkClockwise(Polygon p);

//void *thread_render(void* arg);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<Box*> allLeaf;
vector<Set*> allSet;
QuadTree* QT;

int totalSteps = 0;
int currentStep = 0;
int stepIncrease = 0;
int currentPathStep = 0;

// 0: no replay 1: replay splitting 2: stop splitting 3: animation 4: stop animation
int animationOption = 0;

GLUI_Button* buttonRun;
GLUI_Button* previousStepReplay;
GLUI_Button* buttonReplay;
GLUI_Button* nextStepReplay;
GLUI_Button* previousStepAnimation;
GLUI_Button* buttonAnimation;
GLUI_Button* nextStepAnimation;
GLUI_Button* buttonReset;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//find path using simple heuristic:
//use distance to beta as key in PQ, see dijkstraQueue
template<typename Cmp>
bool findPath(Box* a, Box* b, QuadTree* QT, int& ct) {
	bool isPath = false;
	vector<Box*> toReset;
	a->dist2Source = 0;
//	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!237 a  " << a->x << " " << a->y
//			<< " " << a->xi[0] << " " << a->xi[1] << " " << a->xi[2] << " "
//			<< a->xi[3] << " " << a->status << endl;
//	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!238 a->dist2Source  "
//			<< a->dist2Source << endl;
	dijkstraQueue<Cmp> dijQ;
	dijQ.push(a);
	toReset.push_back(a);
	while (!dijQ.empty()) {

		Box* current = dijQ.extract();
		current->visited = true;

//		int aaa = dijQ.size();
//		cout<<current->x<< " "<<current->y<<" "<<current->width<<" "<<current->status<<" "<<endl;

		// if current is MIXED, try expand it and push the children that is
		// ACTUALLY neighbors of the source set (set containing alpha) into the dijQ again
		if (current->status == Box::MIXED) {
			vector<Box*> cldrn;
			if (QT->expand(current, cldrn)) {
				++ct;
				for (int i = 0; i < (int) cldrn.size(); ++i) {

					// go through neighbors of each child to see if it's in source set
					// if yes, this child go into the dijQ					
					bool isNeighborOfSourceSet = false;
					for (int j = 0; j < 4 && !isNeighborOfSourceSet; ++j) {
						for (vector<Box*>::iterator iter =
								cldrn[i]->Nhbrs[j].begin();
								iter < cldrn[i]->Nhbrs[j].end(); ++iter) {
							Box* n = *iter;
//							cout
//									<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!264 dist2Source != 0  "
//									<< n->x << " " << n->y << " " << n->xi[0]
//									<< " " << n->xi[1] << " " << n->xi[2] << " "
//									<< n->xi[3] << " " << endl;
//							cout
//									<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!267 a->dist2Source  "
//									<< a->dist2Source << endl;
//							cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!268 a  "
//									<< a->x << " " << a->y << " " << a->xi[0]
//									<< " " << a->xi[1] << " " << a->xi[2] << " "
//									<< a->xi[3] << " " << endl;
							if (n->dist2Source == 0) {
//								cout
//										<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!265 dist2Source = 0  "
//										<< n->x << " " << n->y << endl;
								isNeighborOfSourceSet = true;
								break;
							}
						}
					}

					if (isNeighborOfSourceSet) {
						switch (cldrn[i]->getStatus()) {
						//if it's FREE, also insert to source set
						case Box::FREE:
							cldrn[i]->dist2Source = 0;
//							cout
//									<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!277 dist2Source = 0  "
//									<< cldrn[i]->x << " " << cldrn[i]->y
//									<< endl;
							dijQ.push(cldrn[i]);
							toReset.push_back(cldrn[i]);
							break;
						case Box::MIXED:
							dijQ.push(cldrn[i]);
							toReset.push_back(cldrn[i]);
							break;
						case Box::STUCK:
							//cerr << "inside FindPath: STUCK case not treated" << endl;
							break;
						case Box::UNKNOWN:
							//cerr << "inside FindPath: UNKNOWN case not treated" << endl;
							break;
						}
					}
				}
			}
			if (current->shouldSplit2D && current->height / 2 >= epsilon
					&& current->width / 2 >= epsilon) {
				dijQ.push(current);
				toReset.push_back(current);
//				cout<<"push"<<endl;
			}

			continue;
		}

//		cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!302 a->dist2Source  "
//				<< a->dist2Source << endl;
		//found path!
		if (current->status == Box::FREE
				&& current->contains(beta[0], beta[1], beta[2], beta[3])) {
			isPath = true;
			break;
		}

		if (current->status == Box::FREE) {
			// if current is not MIXED, then must be FREE
			// go through it's neighbors and add FREE and MIXED ones to dijQ
			// also add FREE ones to source set
			for (int i = 0; i < 4; ++i) {
				for (vector<Box*>::iterator iter = current->Nhbrs[i].begin();
						iter < current->Nhbrs[i].end(); ++iter) {
					Box* neighbor = *iter;
					if (!neighbor->visited && neighbor->dist2Source == -1
							&& (neighbor->status == Box::FREE
									|| neighbor->status == Box::MIXED)) {
						//					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!317 "
						//							<< neighbor->x << " " << neighbor->y << endl;
						if (neighbor->status == Box::FREE) {
							neighbor->dist2Source = 0;
							//						cout
							//								<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!316 dist2Source = 0  "
							//								<< neighbor->x << " " << neighbor->y << endl;
						}
						dijQ.push(neighbor);
						toReset.push_back(neighbor);
					}
				}
			}
		}

	}

//	cout<<"before reset"<<endl;

	//these two fields are also used in dijkstraShortestPath
	// need to reset
	for (int i = 0; i < (int) toReset.size(); ++i) {
		toReset[i]->visited = false;
		toReset[i]->dist2Source = -1;
	}

	return isPath;
}

void TimerFunction(int p) {
	if (animationOption == 1) {
		currentStep += stepIncrease;
	}

	if (!noPath && animationOption == 3) {
		currentPathStep++;
	}

//	ssInfo.str("");
////	ssout<<ssoutLastTime;
//	ssInfo << ssout.str();
//
//	textBox->set_text(ssInfo.str().c_str());
//	glutPostRedisplay();
	glutTimerFunc(timePerFrame, TimerFunction, 0);
//	glFlush();

}

bool sortByXi0(const Box* b1, const Box* b2) {
	return b1->xi[0] < b2->xi[0];
}
bool sortByXi2(const Box* b1, const Box* b2) {
	return b1->xi[2] < b2->xi[2];
}

void processMouse(int button, int state, int x, int y) {

//	cout<<button<<" "<<state<<endl;
	int reverseY = 512 - y;
	if (state == GLUT_DOWN) {

		int minWidth = 999;
		boxClicked.clear();
//		cout << "426" << endl;
		if (!allLeaf.empty() && allLeaf.size() != 0) {
//			Box* lastTempBox = NULL;
			for (vector<Box*>::iterator it = allLeaf.begin();
					it != allLeaf.end(); ++it) {
				Box* tempBox = *it;

				int tempWidth = tempBox->width;
//				cout << "434" << endl;
				if (tempWidth <= minWidth && x > tempBox->x - tempWidth / 2
						&& x <= tempBox->x + tempWidth / 2
						&& reverseY > tempBox->y - tempWidth / 2
						&& reverseY <= tempBox->y + tempWidth / 2) {
					if (tempWidth < minWidth) {
						boxClicked.clear();
						minWidth = tempWidth;
					}
					if (tempWidth == minWidth) {
						boxClicked.push_back(tempBox);
					}

//					lastTempBox = tempBox;
				}

			}
//			cout << "446" << endl;
//			if(boxClicked.empty()){
//			boxClicked.push_back(lastTempBox);
//			cout << lastTempBox->width << endl;
//			cout << "449" << endl;
//			}
//			cout << x << " " << reverseY << " " << minWidth << " "
//					<< boxClicked.size() << endl;
//			cout << "453" << endl;
			stringstream tempStream;
			tempStream << "Selected Box Info:" << endl;
			tempStream << "Status: ";
			switch (boxClicked.front()->status) {
			case FREE:
				tempStream << "Totally Free";
				break;
			case STUCK:
				tempStream << "Stuck";
				break;
			case MIXED:
				if (boxClicked.back()->status == Box::FREE) {
					tempStream << "Partially Free";
				} else {
					tempStream << "Mixed";
				}
				break;
			case UNKNOWN:
				tempStream << "Unknown";
				break;
			}
			tempStream << endl;
			tempStream << "x: " << boxClicked.front()->x << " y: "
					<< boxClicked.front()->y << " width: "
					<< boxClicked.front()->width << endl;

			std::sort(boxClicked.begin(), boxClicked.end(), sortByXi0);
			double lastXi = 361.0;
			for (vector<Box*>::iterator it = boxClicked.begin();
					it != boxClicked.end(); ++it) {
				Box* tempBox = *it;
				if (tempBox->status == Box::FREE) {
					if (tempBox->xi[0] != lastXi) {
						tempStream << "L1 range: [" << tempBox->xi[0] << ", "
								<< tempBox->xi[1] << "] " << endl;
						lastXi = tempBox->xi[0];
					}

				}
			}

			std::sort(boxClicked.begin(), boxClicked.end(), sortByXi2);
			lastXi = 361.0;
			for (vector<Box*>::iterator it = boxClicked.begin();
					it != boxClicked.end(); ++it) {
				Box* tempBox = *it;
				if (tempBox->status == Box::FREE) {
					if (tempBox->xi[2] != lastXi) {
						tempStream << "L2 range: [" << tempBox->xi[2] << ", "
								<< tempBox->xi[3] << "] " << endl;
						lastXi = tempBox->xi[2];
					}
				}
			}

			boxSelectedInfo->set_text(tempStream.str().c_str());
		}
	}
	redrawFBO();
}

int main(int argc, char* argv[]) {

	argvSave = argv;
	argcSave = argc;

	reset();

// Added by Zhongdi 05/08/2013 begin
// calculate the R of the robot
	R0 = max(L1, L2);
// Added by Zhongdi 05/08/2013 end

	cout << "Before interactive, Qtype= " << QType << "\n";

	if (interactive > 0) {	// non-interactive
		// do something...
		cout << "Non Interactive Run of 2-links Robot" << endl;
		//if (noPath)
		//	cout << "No Path Found!" << endl;
		//else
		//	cout << "Path was Found!" << endl;
		//return 0;
	}

//	alpha[2] /= 180.0;		// start theta, convert from degree to radian
//	beta[2] /= 180.0;		// goal theta, convert from degree to radian
//	alpha[3] /= 180.0;		// start theta, convert from degree to radian
//	beta[3] /= 180.0;		// goal theta, convert from degree to radian

// Else, set up for GLUT/GLUI interactive display:

	if (interactive == 0) {
//cout<<"before glutInit\n";
		glutInit(&argc, argv);
		glutInitWindowPosition(windowPosX, windowPosY);
		glutInitWindowSize(boxWidth, boxWidth);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		int windowID = glutCreateWindow(title.c_str());

		initFbo();

//		GLUI_Master.set_glutIdleFunc(NULL);

		glutDisplayFunc(renderScene);

		glutTimerFunc(timePerFrame, TimerFunction, 0);
		glutMouseFunc(processMouse);

//		cout << "mainloop!!!!!!!!!!!" << endl;

		//Chee: trying to get demos to take a "title string"
		std::stringstream sss;
		sss << "2-Links Control: " << title;	// create full file name
		std::string title_string = sss.str();
//		const char * test("2-Links Demo");

		GLUI *glui = GLUI_Master.create_glui(title_string.c_str(), 0,
				windowPosX + boxWidth + 20, windowPosY);

		// SETTING UP THE CONTROL PANEL:

		editInput = glui->add_edittext("Input file:", GLUI_EDITTEXT_TEXT);
		editInput->set_text((char*) fileName.c_str());
		editDir = glui->add_edittext("Input Directory:", GLUI_EDITTEXT_TEXT);
		editDir->set_text((char*) inputDir.c_str());
		// Chee added this "grouping" for Robot Specifications:
		GLUI_Panel * robot_box_panel = glui->add_panel("Robot Specs");
//		editRadius = glui->add_edittext_to_panel(robot_box_panel, "Radius:",
//				GLUI_EDITTEXT_FLOAT);
//		editRadius->set_float_val(R0);
		editL1 = glui->add_edittext_to_panel(robot_box_panel, "L1:",
				GLUI_EDITTEXT_FLOAT);
		editL1->set_float_val(L1);
		editL2 = glui->add_edittext_to_panel(robot_box_panel, "L2:",
				GLUI_EDITTEXT_FLOAT);
		editL2->set_float_val(L2);
		editThickness = glui->add_edittext_to_panel(robot_box_panel,
				"Thickness:", GLUI_EDITTEXT_FLOAT);
		editThickness->set_float_val(thickness);
		editEpsilon = glui->add_edittext_to_panel(robot_box_panel, "Epsilon:",
				GLUI_EDITTEXT_FLOAT);
		editEpsilon->set_float_val(epsilon);
		//editRadius = glui->add_edittext( "Radius:", GLUI_EDITTEXT_FLOAT );
		//editRadius->set_float_val(R0);
		//editEpsilon = glui->add_edittext( "Epsilon:", GLUI_EDITTEXT_FLOAT );
		//editEpsilon->set_float_val(epsilon);

		// Chee added this "grouping" for Alpha
		GLUI_Panel * alpha_box_panel = glui->add_panel(
				"Alpha (start configuration)");
		editAlphaX = glui->add_edittext_to_panel(alpha_box_panel, "x:",
				GLUI_EDITTEXT_FLOAT);
		editAlphaX->set_float_val(alpha[0]);
		editAlphaY = glui->add_edittext_to_panel(alpha_box_panel, "y:",
				GLUI_EDITTEXT_FLOAT);
		editAlphaY->set_float_val(alpha[1]);
		editAlphaTheta1 = glui->add_edittext_to_panel(alpha_box_panel, "theta1",
				GLUI_EDITTEXT_FLOAT);
		editAlphaTheta1->set_float_val(alpha[2]);
		editAlphaTheta2 = glui->add_edittext_to_panel(alpha_box_panel, "theta2",
				GLUI_EDITTEXT_FLOAT);
		editAlphaTheta2->set_float_val(alpha[3]);
		// ORIGINAL:
		//editAlphaX = glui->add_edittext( "alpha.x:", GLUI_EDITTEXT_FLOAT );
		//editAlphaX->set_float_val(alpha[0]);
		//editAlphaY = glui->add_edittext( "alpha.y:", GLUI_EDITTEXT_FLOAT );
		//editAlphaY->set_float_val(alpha[1]);
		//editAlphaTheta = glui->add_edittext("alpha.theta:",GLUI_EDITTEXT_FLOAT);
		//editAlphaTheta->set_float_val(alpha[2]);

		// Chee added this "grouping" for Beta
		GLUI_Panel * beta_box_panel = glui->add_panel(
				"Beta (goal configuration)");
		editBetaX = glui->add_edittext_to_panel(beta_box_panel, "x:",
				GLUI_EDITTEXT_FLOAT);
		editBetaX->set_float_val(beta[0]);
		editBetaY = glui->add_edittext_to_panel(beta_box_panel, "y:",
				GLUI_EDITTEXT_FLOAT);
		editBetaY->set_float_val(beta[1]);
		editBetaTheta1 = glui->add_edittext_to_panel(beta_box_panel, "theta1",
				GLUI_EDITTEXT_FLOAT);
		editBetaTheta1->set_float_val(beta[2]);
		editBetaTheta2 = glui->add_edittext_to_panel(beta_box_panel, "theta2",
				GLUI_EDITTEXT_FLOAT);
		editBetaTheta2->set_float_val(beta[3]);
		// ORIGINAL:
		//editBetaX = glui->add_edittext( "beta.x:", GLUI_EDITTEXT_FLOAT );
		//editBetaX->set_float_val(beta[0]);
		//editBetaY = glui->add_edittext( "beta.y:", GLUI_EDITTEXT_FLOAT );
		//editBetaY->set_float_val(beta[1]);
		//editBetaTheta =glui->add_edittext("beta.theta:",GLUI_EDITTEXT_FLOAT );
		//editBetaTheta->set_float_val(beta[2]);

		editSeed = glui->add_edittext("seed:", GLUI_EDITTEXT_INT);
		editSeed->set_int_val(seed);

		GLUI_Panel * runAndReset_panel = glui->add_panel("", GLUI_PANEL_NONE);
		buttonRun = glui->add_button_to_panel(runAndReset_panel, "Run", -1,
				(GLUI_Update_CB) run);
//		buttonRun->set_name("Run"); // Hack, to avoid "unused warning" (Chee)
		glui->add_column_to_panel(runAndReset_panel);
		buttonReset = glui->add_button_to_panel(runAndReset_panel, "Reset", 1,
				(GLUI_Update_CB) resetAndRun);

		radioDrawPathOption = glui->add_radiogroup(0, -1,
				(GLUI_Update_CB) redrawFBO);
		glui->add_radiobutton_to_group(radioDrawPathOption, "Animation");
		glui->add_radiobutton_to_group(radioDrawPathOption,
				"Initial Configuration");
		glui->add_radiobutton_to_group(radioDrawPathOption,
				"Static Subdivision");
		glui->add_radiobutton_to_group(radioDrawPathOption, "Static Path");

		glui->add_separator();

		GLUI_Panel * replay_panel = glui->add_panel("Animation",
				GLUI_PANEL_EMBOSSED);
		GLUI_Panel * replaySplitting_panel = glui->add_panel_to_panel(
				replay_panel, "", GLUI_PANEL_NONE);
		previousStepReplay = glui->add_button_to_panel(replaySplitting_panel,
				"<-", 0, (GLUI_Update_CB) previousStep);
		previousStepReplay->set_w(1);
		glui->add_column_to_panel(replaySplitting_panel);
		buttonReplay = glui->add_button_to_panel(replaySplitting_panel,
				"Replay spliting", -1, (GLUI_Update_CB) replaySplitting);
		buttonReplay->set_name("Replay Spliting");
		glui->add_column_to_panel(replaySplitting_panel);
		nextStepReplay = glui->add_button_to_panel(replaySplitting_panel, "->",
				0, (GLUI_Update_CB) nextStep);
		nextStepReplay->set_w(1);

//		glui->add_separator();
		radioStepsPerFrame = glui->add_radiogroup_to_panel(replay_panel);
		glui->add_radiobutton_to_group(radioStepsPerFrame,
				"slow (1 frame/step)");
		glui->add_radiobutton_to_group(radioStepsPerFrame,
				"normal (10 frames/step)");
		glui->add_radiobutton_to_group(radioStepsPerFrame,
				"fast (100 frames/step)");
//		glui->add_radiobutton_to_group(radioStepsPerFrame,
//						"   ");
//		textCustomSpeed= glui->add_edittext_to_panel(replay_panel,
//				"Current Step", GLUI_EDITTEXT_INT);
		radioStepsPerFrame->set_int_val(2);

		textCurrentStep = glui->add_edittext_to_panel(replay_panel,
				"Current Step", GLUI_EDITTEXT_INT);
		textCurrentStep->set_int_val(currentStep);
//		glui->add_separator();

		GLUI_Panel * animation_panel = glui->add_panel_to_panel(replay_panel,
				"", GLUI_PANEL_NONE);
		previousStepAnimation = glui->add_button_to_panel(animation_panel, "<-",
				1, (GLUI_Update_CB) previousStep);
		previousStepAnimation->set_w(1);
		glui->add_column_to_panel(animation_panel);
		buttonAnimation = glui->add_button_to_panel(animation_panel,
				"Path Animation", -1, (GLUI_Update_CB) runAnimation);
		buttonAnimation->set_name("Path Animation");
		glui->add_column_to_panel(animation_panel);
		nextStepAnimation = glui->add_button_to_panel(animation_panel, "->", 1,
				(GLUI_Update_CB) nextStep);
		nextStepAnimation->set_w(1);

		// New column:
		glui->add_column(true);

		glui->add_separator();
		radioQType = glui->add_radiogroup();

		glui->add_radiobutton_to_group(radioQType, "(0) Random Heuristic");
		glui->add_radiobutton_to_group(radioQType, "(1) BFS");
		glui->add_radiobutton_to_group(radioQType, "(2) Greedy");
		glui->add_radiobutton_to_group(radioQType, "(3) Dist+Size");
		glui->add_radiobutton_to_group(radioQType,
				"(4) Voronoi Heuristic (deprecated)");
		radioQType->set_int_val(QType);

		glui->add_separator();
		radioDrawOption = glui->add_radiogroup(0, -1,
				(GLUI_Update_CB) redrawFBO);
		glui->add_radiobutton_to_group(radioDrawOption, "Show Box Boundary");
		glui->add_radiobutton_to_group(radioDrawOption, "Hide Box Boundary");

		glui->add_separator();
		radioShadeOption = glui->add_radiogroup(0, -1,
				(GLUI_Update_CB) redrawFBO);
		glui->add_radiobutton_to_group(radioShadeOption, "Clear Shading");
		glui->add_radiobutton_to_group(radioShadeOption,
				"Shade Obstacles (doesn't support concave ones)");

		glui->add_separator();
		radioVerboseOption = glui->add_radiogroup();

		glui->add_radiobutton_to_group(radioVerboseOption, "Non-Verbose");
		glui->add_radiobutton_to_group(radioVerboseOption,
				"Verbose (print statistics)");

		radioVerboseOption->set_int_val(verboseOption);

		glui->add_separator();
		radio2StrategyOption = glui->add_radiogroup();

		glui->add_radiobutton_to_group(radio2StrategyOption,
				"Split Until Epsilon or Phi(B) empty");
		glui->add_radiobutton_to_group(radio2StrategyOption,
				"Smarter Strategy");
		textPhiB = glui->add_edittext("Phi(B) = ", GLUI_EDITTEXT_INT);
		textPhiB->set_int_val(sizeOfPhiB);
//		glui->add_radiobutton_to_group(radio2StrategyOption,
//				"Smarter Strategy With Voronoi Approach");

		radio2StrategyOption->set_int_val(twoStrategyOption);
		glui->add_separator();

		// box selected info
		boxSelectedInfo = new GLUI_TextBox(glui, true);
//		boxSelectedInfo->set_name("box selected");
		boxSelectedInfo->set_h(120);
		boxSelectedInfo->set_w(310);

		textBox = new GLUI_TextBox(glui, true);
		textBox->set_h(250);
		textBox->set_w(310);
//		textBox->disable();

		// Quit button
		glui->add_button("Quit", 0, (GLUI_Update_CB) exit);

		glui->set_main_gfx_window(windowID);
	}
//	run();

//	renderScene();
//	glutPostRedisplay();

//cout<<"before run\n";
// PERFORM THE INITIAL RUN OF THE ALGORITHM
//==========================================
	run(); 	// make it do something interesting from the start!!!
//	glFlush();

// SHOULD WE STOP or GO INTERACTIVE?
//==========================================
//cout<<"after run\n";
	if (interactive > 0) {	// non-interactive
		// do something...
		cout << "Non Interactive Run of Disc Robot" << endl;
		if (noPath)
			cout << "No Path Found!" << endl;
		else
			cout << "Path was Found!" << endl;
		return 0;
	} else {
		glutMainLoop();
	}

	return 0;
}

void genEmptyTree() {
	Box* root = new Box(boxWidth / 2, boxHeight / 2, boxWidth, boxHeight);

	Box::r0 = R0;
	Box::l1 = L1;
	Box::l2 = L2;
	Box::thickness = thickness;

//todo
//	Box::THETA_MIN = min(min(triRobo[0], triRobo[1] - triRobo[0]),
//			2 - triRobo[1]);
// todo added by Zhongdi 05/08/2013
//	Box::THETA_MIN = 0.1;

	Box::pAllLeaf = &allLeaf;

	if (!allLeaf.empty() && allLeaf.size() != 0) {
		for (vector<Box*>::iterator it = allLeaf.begin(); it != allLeaf.end();
				++it) {
			delete *it;
		}
	}

	if (!allSet.empty() && allSet.size() != 0) {
		for (vector<Set*>::iterator it = allSet.begin(); it != allSet.end();
				++it) {
			delete *it;
		}
	}

	allLeaf.clear();
	allLeaf.push_back(root);
	allSet.clear();

	parseConfigFile(root);
	root->updateStatus();

	if (QT) {
		delete (QT);
	}
	QT = new QuadTree(root, epsilon, QType, seed++); // Note that seed keeps changing!

	if (verboseOption)
		cout << "done genEmptyTree \n";
}
//void idle() {
//	cout << "idle() \n" << endl;
//	glutPostRedisplay();
//	textBox->set_text(ssTemp.str().c_str());
//}

void runAnimation() {
//	textCurrentStep->set_int_val(currentStep);
	radioDrawPathOption->set_int_val(0);

	if (animationOption != 3) {
		animationOption = 3;
		if (radioStepsPerFrame->get_int_val() == 0) {
			timePerFrame = 200;
		} else if (radioStepsPerFrame->get_int_val() == 1) {
			timePerFrame = 50;
		} else {
			timePerFrame = 10;
		}

		buttonAnimation->set_name("Stop Animation");
		if ((unsigned) currentPathStep >= PATH.size()) {
			currentPathStep = 0;
		}
	} else {
		animationOption = 4;
		stepIncrease = 0;
		buttonAnimation->set_name("Path Animation");
	}

	glutPostRedisplay();
}

void replaySplitting() {
	radioDrawPathOption->set_int_val(0);
	textCurrentStep->set_int_val(currentStep);
	if (animationOption != 1) {
		animationOption = 1;
		if (radioStepsPerFrame->get_int_val() == 0) {
			stepIncrease = 1;
		} else if (radioStepsPerFrame->get_int_val() == 1) {
			stepIncrease = 10;
		} else {
			stepIncrease = 100;
		}

		if ((unsigned) currentStep >= allLeaf.size()) {
			currentStep = 1;
		}
		buttonReplay->set_name("Stop Spliting");
	} else {
		animationOption = 2;
		stepIncrease = 0;
		buttonReplay->set_name("Replay Spliting");
	}

//	if (interactive == 0 && animationOption == 1) {
//		renderReady = true;
//	} else {
//		renderReady = false;
//	}
	glutPostRedisplay();
}
void previousStep(int id) {
	radioDrawPathOption->set_int_val(0);
	int stepGap = 0;
	if (radioStepsPerFrame->get_int_val() == 0) {
		stepGap = 1;
	} else if (radioStepsPerFrame->get_int_val() == 1) {
		stepGap = 10;
	} else {
		stepGap = 100;
	}

	switch (id) {
	case 0:
		textCurrentStep->set_int_val(currentStep);
		animationOption = 2;
		stepIncrease = 0;
		buttonReplay->set_name("Replay Spliting");
		currentStep -= stepGap;
		if (currentStep <= 1) {
			currentStep = allLeaf.size() - 1;
		}
		break;
	case 1:
		animationOption = 4;
		stepIncrease = 0;
		buttonAnimation->set_name("Path Animation");
		currentPathStep -= stepGap;
		if (currentPathStep <= 0) {
			currentPathStep = PATH.size() - 1;
		}
		break;
	}
	glutPostRedisplay();
}

void nextStep(int id) {
	radioDrawPathOption->set_int_val(0);
	int stepGap = 0;
	if (radioStepsPerFrame->get_int_val() == 0) {
		stepGap = 1;
	} else if (radioStepsPerFrame->get_int_val() == 1) {
		stepGap = 10;
	} else {
		stepGap = 100;
	}
	switch (id) {
	case 0:
		textCurrentStep->set_int_val(currentStep);
		animationOption = 2;
		stepIncrease = 0;
		buttonReplay->set_name("Replay Spliting");

		currentStep += stepGap;
		if ((unsigned) currentStep >= allLeaf.size()) {
			currentStep = 1;
		}
		break;
	case 1:
		animationOption = 4;
		stepIncrease = 0;
		buttonAnimation->set_name("Path Animation");

		currentPathStep += stepGap;

		if ((unsigned) currentPathStep >= PATH.size()) {
			currentPathStep = 0;
		}
		break;
	}
	glutPostRedisplay();
}
void run() {
//	cout << "1111111111111111111111111111111111111111111111111" << endl;
	runCount++;

	ssoutLastTime.str("");
	ssoutLastTime << ssout.str();
	ssout.str("");
	ssout << "Run No. " << runCount << ":" << endl;

	animationOption = 0;
	currentStep = 0;
	currentPathStep = 0;
	leafBoxesDrawed = false;

	buttonReplay->set_name("Replay Spliting");
	buttonAnimation->set_name("Path Animation");
	twoStrategyOption = radio2StrategyOption->get_int_val();
	sizeOfPhiB = textPhiB->get_int_val();

	if (interactive == 0) {
		//update from glui live variables
		fileName = editInput->get_text();
		inputDir = editDir->get_text();
//		R0 = editRadius->get_float_val();
		L1 = editL1->get_float_val();
		L2 = editL2->get_float_val();
		if (L1 > L2) {
			R0 = L1;
		} else {
			R0 = L2;
		}
		thickness = editThickness->get_float_val();
		epsilon = editEpsilon->get_float_val();
		alpha[0] = editAlphaX->get_float_val();
		alpha[1] = editAlphaY->get_float_val();
		alpha[2] = editAlphaTheta1->get_float_val();
		while (alpha[2] >= 360) {
			alpha[2] -= 360;
		}
		while (alpha[2] < 0) {
			alpha[2] += 360;
		}
		editAlphaTheta1->set_float_val(alpha[2]);
		alpha[3] = editAlphaTheta2->get_float_val();
		while (alpha[3] >= 360) {
			alpha[3] -= 360;
		}
		while (alpha[3] < 0) {
			alpha[3] += 360;
		}
		editAlphaTheta2->set_float_val(alpha[3]);
		beta[0] = editBetaX->get_float_val();
		beta[1] = editBetaY->get_float_val();
		beta[2] = editBetaTheta1->get_float_val();
		while (beta[2] >= 360) {
			beta[2] -= 360;
		}
		while (beta[2] < 0) {
			beta[2] += 360;
		}
		editBetaTheta1->set_float_val(beta[2]);
		beta[3] = editBetaTheta2->get_float_val();
		while (beta[3] >= 360) {
			beta[3] -= 360;
		}
		while (beta[3] < 0) {
			beta[3] += 360;
		}
		editBetaTheta2->set_float_val(beta[3]);

		QType = radioQType->get_int_val();
	}

	if (verboseOption) {
		cout << "   radius = " << R0 << ", eps = " << epsilon << endl;
		cout << "   alpha = (" << alpha[0] << ", " << alpha[1] << ", "
				<< alpha[2] << ", " << alpha[3] << ")" << endl;
		cout << "   beta = (" << beta[0] << ", " << beta[1] << ", " << beta[2]
				<< ", " << beta[3] << ")" << endl;
	}

	genEmptyTree();

	if (interactive == 0) {
//		cout << "interactive : " << interactive << endl;
		glutPostRedisplay();
	}
	Timer t;

	t.start();

	noPath = false;	// initially, pretend we have found path
	int ct = 0;	// number of times a node is expanded

	if (QType == 0 || QType == 1) {
		boxA = QT->getBox(alpha[0], alpha[1], alpha[2], alpha[3], ct);
		if (!boxA) {
			noPath = true;
			cout << "Start Configuration is not free\n";
			ssout << "Start Configuration is not free\n";
		}

		boxB = QT->getBox(beta[0], beta[1], beta[2], beta[3], ct);
		if (!boxB) {
			noPath = true;
			cout << "Goal Configuration is not free\n";
			ssout << "Goal Configuration is not free\n";
		}

		// In the following loop, "noPath" should really mean "hasPath"
		//	Otherwise, we should pre-initialize "noPath" to true
		//	before entering loop...
		while (!noPath && !QT->isConnected(boxA, boxB)) {
			if (interactive == 0) {
//				glutPostRedisplay();
			}
			if (!QT->expand()) {
				noPath = true;
			}
			++ct;
		}
	} else if (QType == 2 || QType == 3 || QType == 4) {
		boxA = QT->getBox(alpha[0], alpha[1], alpha[2], alpha[3], ct);
		if (!boxA) {
			noPath = true;
			cout << "Start Configuration is not free\n";
			ssout << "Start Configuration is not free\n";
		}

		boxB = QT->getBox(beta[0], beta[1], beta[2], beta[3], ct);
		if (!boxB) {
			noPath = true;
			cout << "Goal Configuration is not free\n";
			ssout << "Goal Configuration is not free\n";
		}
		if (!noPath) {
			if (QType == 2) {
				noPath = !findPath<DistCmp>(boxA, boxB, QT, ct);
			} else if (QType == 3) {
				noPath = !findPath<DistPlusSizeCmp>(boxA, boxB, QT, ct);
			} else if (QType == 4) {
				noPath = !findPath<VorCmp>(boxA, boxB, QT, ct);
			}
		}
	}

	t.stop();
	if (!noPath) {
		Graph graph;
		PATH.clear();

//		cout<<"before dijkstraShortestPath"<<endl;
		PATH = graph.dijkstraShortestPath(boxA, boxB);
//		cout<<"after dijkstraShortestPath"<<endl;
	}
	if (verboseOption)
		cout << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
	cout << ">>\n";
	if (!noPath)
		cout << ">>      ----->>  Path Found !" << endl;
	else
		cout << ">>      ----->>  No Path !" << endl;
	cout << ">>\n";
	cout << ">>      ----->>  Time used: " << t.getElapsedTimeInMilliSec()
			<< " ms" << endl;
	cout << ">>\n";
// cout << ">>      ----->>  Qtype: " << QType << "\n";
	cout << ">>      ----->>  Qtype: ";
	switch (QType) {
	case 0:
		cout << "Random Strategy\n";
		break;
	case 1:
		cout << "BFS Strategy\n";
		break;
	case 2:
		cout << "Greedy Strategy\n";
		break;
	case 3:
		cout << "Dist+Size Strategy\n";
		break;
	case 4:
		cout << "Voronoi Strategy\n";
		break;
	}
	cout << ">>\n";
	if (verboseOption)
		cout << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
	if (verboseOption) {
		cout << "    Expanded " << ct << " times" << endl;
		cout << "    total Free boxes: " << freeCount << endl;
		cout << "    total Stuck boxes: " << stuckCount << endl;
		cout << "    total Mixed boxes smaller than epsilon: " << mixSmallCount
				<< endl;
		cout << "    total Mixed boxes bigger than epsilon: "
				<< mixCount - mixSmallCount << endl;
	}

	totalSteps = allLeaf.size();

//	stringstream ssout;

	if (!noPath)
		ssout << "    ---->>   PATH FOUND !" << endl;
	else
		ssout << "    ---->>  NO PATH !" << endl;
	ssout << "    ---->>   TIME USED: " << t.getElapsedTimeInMilliSec() << " ms"
			<< endl;
	ssout << "    ---->>   TOTAL STEPS: " << totalSteps << endl;
	ssout << "    ---->>   Strategy No (" << QType << ")" << endl;
	if (verboseOption) {
		ssout << "    Expanded " << ct << " times" << endl;
		ssout << "    total Free boxes: " << freeCount << endl;
		ssout << "    total Stuck boxes: " << stuckCount << endl;
		ssout << "    total Mixed boxes smaller than epsilon: " << mixSmallCount
				<< endl;
		ssout << "    total Mixed boxes bigger than epsilon: "
				<< mixCount - mixSmallCount << endl;
	}
//	if(animationOption == 3 || animationOption == 4){
//		ssout << ssTemp.str().c_str();
//	}
	ssout << endl;
	ssout << ssoutLastTime.str();
	textBox->set_text(ssout.str().c_str());
//	GLUI_Master.sync_live_all();
	freeCount = stuckCount = mixCount = mixSmallCount = 0;
	cout << "####################### END of RUN ######################\n";

//	renderReady = false;
//	renderReady = true;
	// run the moving animation after calculation
	runAnimation();

}		//run

//void drawTri(Box* b) {
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glColor3f(1, 0, 0);
//	glLineWidth(2);
//	glBegin(GL_TRIANGLES);
//
//	glVertex2f(R0 * cos((b->xi[0]) * PI) + b->x,
//			R0 * sin((b->xi[0]) * PI) + b->y);
//	glVertex2f(R0 * cos((triRobo[0] + b->xi[0]) * PI) + b->x,
//			R0 * sin((triRobo[0] + b->xi[0]) * PI) + b->y);
//	glVertex2f(R0 * cos((triRobo[1] + b->xi[0]) * PI) + b->x,
//			R0 * sin((triRobo[1] + b->xi[0]) * PI) + b->y);
//
//	glEnd();
//	glLineWidth(1.0);
//}
//
//void drawTri(Box* b, double x, double y) {
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glColor3f(1, 0, 0);
//	glLineWidth(2);
//	glBegin(GL_TRIANGLES);
//
//	glVertex2f(R0 * cos((b->xi[0]) * PI) + x, R0 * sin((b->xi[0]) * PI) + y);
//	glVertex2f(R0 * cos((triRobo[0] + b->xi[0]) * PI) + x,
//			R0 * sin((triRobo[0] + b->xi[0]) * PI) + y);
//	glVertex2f(R0 * cos((triRobo[1] + b->xi[0]) * PI) + x,
//			R0 * sin((triRobo[1] + b->xi[0]) * PI) + y);
//
//	glEnd();
//	glLineWidth(1.0);
//}

void drawLinks(Box* b) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0x00 / 255.0, 0x00 / 255.0, 0x33 / 255.0);
	glLineWidth(thickness);
	if (thickness < 3) {
		glLineWidth(3);
	}
// draw link1
	glBegin(GL_LINES);
	glVertex2f(b->x, b->y);
	glVertex2f(L1 * cos((b->xi[0] / 180) * PI) + b->x,
			L1 * sin((b->xi[0] / 180) * PI) + b->y);
	glEnd();
	glLineWidth(0.1);
	filledCircle(thickness / 2 - 0.1, b->x, b->y, 0x00 / 255.0, 0x00 / 255.0,
			0x33 / 255.0);
	filledCircle(thickness / 2 - 0.1, L1 * cos((b->xi[0] / 180) * PI) + b->x,
				L1 * sin((b->xi[0] / 180) * PI) + b->y, 0x00 / 255.0, 0x00 / 255.0,
				0x33 / 255.0);
//// draw the arrows
//	glLineWidth(2);
//	glBegin(GL_LINES);
//	glVertex2f((L1 - 5) * cos(((b->xi[0] - 5) / 180) * PI) + b->x,
//			(L1 - 5) * sin(((b->xi[0] - 5) / 180) * PI) + b->y);
//	glVertex2f(L1 * cos((b->xi[0] / 180) * PI) + b->x,
//			L1 * sin((b->xi[0] / 180) * PI) + b->y);
//	glEnd();
//	glBegin(GL_LINES);
//	glVertex2f((double) ((L1 - 5) * cos(((b->xi[0] + 5) / 180) * PI) + b->x),
//			(double) ((L1 - 5) * sin(((b->xi[0] + 5) / 180) * PI) + b->y));
//	glVertex2f(L1 * cos((b->xi[0] / 180) * PI) + b->x,
//			L1 * sin((b->xi[0] / 180) * PI) + b->y);
//	glEnd();

// draw link2
	glLineWidth(thickness);
	if (thickness < 3) {
		glLineWidth(3);
	}
	glColor3f(0xFF / 255.0, 0x00 / 255.0, 0x33 / 255.0);
	glBegin(GL_LINES);
	glVertex2f(b->x, b->y);
	glVertex2f(L2 * cos((b->xi[2] / 180) * PI) + b->x,
			L2 * sin((b->xi[2] / 180) * PI) + b->y);
	glEnd();
	glLineWidth(0.1);
	filledCircle(thickness / 2 - 0.1, b->x, b->y, 0xFF / 255.0, 0x00 / 255.0,
			0x33 / 255.0);
	filledCircle(thickness / 2 - 0.1, L2 * cos((b->xi[2] / 180) * PI) + b->x,
				L2 * sin((b->xi[2] / 180) * PI) + b->y, 0xFF / 255.0, 0x00 / 255.0,
				0x33 / 255.0);
//// draw the arrows
//	glLineWidth(2);
//	glBegin(GL_LINES);
//	glVertex2f((L2 - 5) * cos(((b->xi[2] - 5) / 180) * PI) + b->x,
//			(L2 - 5) * sin(((b->xi[2] - 5) / 180) * PI) + b->y);
//	glVertex2f(L2 * cos((b->xi[2] / 180) * PI) + b->x,
//			L2 * sin((b->xi[2] / 180) * PI) + b->y);
//	glEnd();
//	glBegin(GL_LINES);
//	glVertex2f((L2 - 5) * cos(((b->xi[2] + 5) / 180) * PI) + b->x,
//			(L2 - 5) * sin(((b->xi[2] + 5) / 180) * PI) + b->y);
//	glVertex2f(L2 * cos((b->xi[2] / 180) * PI) + b->x,
//			L2 * sin((b->xi[2] / 180) * PI) + b->y);
//	glEnd();
//	std::cout << "hahahahhhhhhhhhhhhhhhhhh  box x=" << b->x << " y=" << b->y
//			<< endl;
//	std::cout << "hahahahhhhhhhhhhhhhhhhhh  box xi[0]=" << b->xi[0] << " xi[1]="
//			<< b->xi[1] << endl;
	glLineWidth(1.0);
}

void drawLinksSrcDst(double* configuration) {
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0x00 / 255.0, 0x00 / 255.0, 0x33 / 255.0);
	glLineWidth(thickness);
	if (thickness < 3) {
		glLineWidth(3);
	}
	glBegin(GL_LINES);
	glVertex2f(configuration[0], configuration[1]);
	glVertex2f(L1 * cos((configuration[2] / 180) * PI) + configuration[0],
			L1 * sin((configuration[2] / 180) * PI) + configuration[1]);
	glEnd();
	glLineWidth(0.1);
	filledCircle(thickness / 2 - 0.1,
			configuration[0], configuration[1],
			0x00 / 255.0, 0x00 / 255.0, 0x33 / 255.0);
	filledCircle(thickness / 2 - 0.1,
				L1 * cos((configuration[2] / 180) * PI) + configuration[0],
				L1 * sin((configuration[2] / 180) * PI) + configuration[1],
				0x00 / 255.0, 0x00 / 255.0, 0x33 / 255.0);

	glLineWidth(thickness);
	if (thickness < 3) {
		glLineWidth(3);
	}
	glColor3f(0xFF / 255.0, 0x00 / 255.0, 0x33 / 255.0);
	glBegin(GL_LINES);
	glVertex2f(configuration[0], configuration[1]);
	glVertex2f(L2 * cos((configuration[3] / 180) * PI) + configuration[0],
			L2 * sin(configuration[3] / 180 * PI) + configuration[1]);
	glEnd();

	glLineWidth(0.1);
	filledCircle(thickness / 2 - 0.1,
			configuration[0], configuration[1],
			0xFF / 255.0, 0x00 / 255.0, 0x33 / 255.0);
	filledCircle(thickness / 2 - 0.1,
				L2 * cos((configuration[3] / 180) * PI) + configuration[0],
				L2 * sin(configuration[3] / 180 * PI) + configuration[1],
				0xFF / 255.0, 0x00 / 255.0, 0x33 / 255.0);

	glLineWidth(1.0);
}

//void drawLinks(Box* b, double x, double y) {
////	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glColor3f(0, 0, 0);
//	glLineWidth(4);
//	glBegin(GL_LINES);
//	glVertex2f(x, y);
//	glVertex2f(L1 * cos((b->xi[0] / 180) * PI) + x,
//			L1 * sin((b->xi[0] / 180) * PI) + y);
//	glEnd();
//	glColor3f(1, 1, 1);
//	glBegin(GL_LINES);
//	glVertex2f(x, y);
//	glVertex2f(L2 * cos((b->xi[2] / 180) * PI) + x,
//			L2 * sin(b->xi[2] / 180 * PI) + y);
//	glEnd();
//	glLineWidth(1.0);
//}

void drawPath(vector<Box*>& path) {
	glColor3f(0x99 / 255.0, 0xCC / 255.0, 0xFF / 255.0);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(beta[0], beta[1]);
	for (int i = 0; i < (int) path.size(); ++i) {
		glVertex2f(path[i]->x, path[i]->y);
	}
	glVertex2f(alpha[0], alpha[1]);
	glEnd();
	glLineWidth(1.0);

	int skipped = 0;
	double distSkipped = 0;

	if (path.size() != 0) {
//		cout<<"drawPath path.size() = "<<path.size() << endl;
		for (int i = path.size() - 1; i >= 0; i--) {
			if ((animationOption == 3 || animationOption == 4)
					&& (unsigned) i != path.size() - 1 - currentPathStep) {

				continue;
			}

//			cout<<"i = "<<i << endl;
			if (i > 0) {
				double dist = sqrt(
						(path[i]->x - path[i - 1]->x)
								* (path[i]->x - path[i - 1]->x)
								+ (path[i]->y - path[i - 1]->y)
										* (path[i]->y - path[i - 1]->y));
				distSkipped += dist;
				++skipped;
				//control triangles drawing:
				//enable (&& dist>= 1e-9) to hide same spot rotation
//				cout<<"i = "<<i <<"skipped ="<<skipped<< " distSkipped = "<<distSkipped<<endl;
				if (((animationOption != 3 && animationOption != 4
						&& skipped > TRIS_TO_SKIP)
						|| ((animationOption == 3 || animationOption == 4)
								&& (skipped > TRIS_TO_SKIP
										|| distSkipped >= DIST_TO_SKIP))
//						|| distSkipped >= DIST_TO_SKIP
				))// && dist>= 1e-9 )
				{
					drawLinks(path[i]);
//					cout<<"drawLinks"<<endl;
//					drawCircle(R0, 100, path[i]->x, path[i]->y, 0, 0, 1);
					skipped = 0;
					distSkipped = 0;
				}
			} else {
				drawLinks(path[i]);
//				drawCircle(R0, 100, path[i]->x, path[i]->y, 0, 0, 1);
			}
			if ((animationOption == 3 || animationOption == 4)
					&& (unsigned) i == path.size() - 1 - currentPathStep) {

				ssTemp.str("");
//				ssTemp << ssout.str();
				ssTemp << endl;
				ssTemp << "Current Box Info:" << endl;
				ssTemp << "x = " << path[i]->x << ", y = " << path[i]->y;
				ssTemp << ", width = " << path[i]->width << endl << endl;
				ssTemp << "safe range of L1 is " << endl << "from "
						<< path[i]->xi[0] << " degree to " << path[i]->xi[1]
						<< " degree" << endl;
				ssTemp << "safe range of L2 is " << endl << "from "
						<< path[i]->xi[2] << " degree to " << path[i]->xi[3]
						<< " degree" << endl << endl;
				ssTemp << "Free Neighbours Info:" << endl;
				for (int m = 0; m < 4; ++m) {
					for (vector<Box*>::iterator it = path[i]->Nhbrs[m].begin();
							it != path[i]->Nhbrs[m].end(); ++it) {
						Box* neighbor = *it;
						if (!neighbor->isFree()) {
							continue;
						}
						switch (m) {
						case 0:
							ssTemp << "right: ";
							break;
						case 1:
							ssTemp << "above: ";
							break;
						case 2:
							ssTemp << "left: ";
							break;
						case 3:
							ssTemp << "under: ";
							break;
						}
						ssTemp << "x = " << neighbor->x << ", y = "
								<< neighbor->y;
						ssTemp << ", width = " << neighbor->width << endl;
						ssTemp << "L1 Range: " << "from " << neighbor->xi[0]
								<< " degree to " << neighbor->xi[1] << " degree"
								<< endl;
						ssTemp << "L2 Range: " << "from " << neighbor->xi[2]
								<< " degree to " << neighbor->xi[3] << " degree"
								<< endl;
					}
				}

//				textBox->set_text(ssTemp.str().c_str());
//				GLUI_Master.sync_live_all();
//				glutSwapBuffers();
//				glutPostRedisplay();
				break;
			}
		}
	}

//	if (animationOption == 3) {
//		currentPathStep++;
//	}

}

void drawQuad(Box* b) {

//	cout<<"888888888888888888888888888888888888"<<endl;

	if (b == NULL || b == 0 || !b->isLeaf || b->status == Box::UNKNOWN) {
		return;
	}

//if (b->status != Box::FREE)
//{
//	return;
//}
//	cout<<"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"<<endl;
	switch (b->status) {
	case Box::FREE:
		if (b->xi[0] != 0 || b->xi[1] != 360 || b->xi[2] != 0
				|| b->xi[3] != 360) {
			glColor3f(0x66 / 255.0, 0xCC / 255.0, 0x99 / 255.0);
		} else {
			glColor3f(0x33 / 255.0, 0x99 / 255.0, 0x33 / 255.0);
		}
		break;
	case Box::STUCK:
		glColor3f(0xCC / 255.0, 0x33 / 255.0, 0x33 / 255.0);
		break;
	case Box::MIXED:
		glColor3f(0xFF / 255.0, 0xFF / 255.0, 0x66 / 255.0);
		if (b->height < 2 * epsilon || b->width < 2 * epsilon) {
			glColor3f(0x99 / 255.0, 0x99 / 255.0, 0x99 / 255.0);
		}
		break;
	case Box::UNKNOWN:
//todo
//std::cout << "UNKNOWN in drawQuad" << std::endl;
		break;
	}

	if (!boxClicked.empty()) {
		if (b->x == boxClicked.back()->x && b->y == boxClicked.back()->y
				&& b->width == boxClicked.back()->width) {
//			if(blinkFlag){
			glColor3f(0x33 / 255.0, 0x99 / 255.0, 0xCC / 255.0);
//				blinkFlag = false;
//			}else{
//				glColor3f(0xFF/255.0, 0xFF/255.0, 0xCC/255.0);
//				blinkFlag = true;
//			}

//			cout<<b->x<<boxClicked.front()->x <<endl;
//					cout<<b->y<<boxClicked.front()->y <<endl;
//					cout<<b->width<<boxClicked.front()->width <<endl;
		}

	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex2f(b->x - b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y + b->height / 2);
	glVertex2f(b->x - b->width / 2, b->y + b->height / 2);
	glEnd();

	if (!hideBoxBoundary) {
		glColor3f(0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glVertex2f(b->x - b->width / 2, b->y - b->height / 2);
		glVertex2f(b->x + b->width / 2, b->y - b->height / 2);
		glVertex2f(b->x + b->width / 2, b->y + b->height / 2);
		glVertex2f(b->x - b->width / 2, b->y + b->height / 2);
		glEnd();
	}
}

void drawWalls(Box* b) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0, 0.0, 0.8, 0.9);
	glLineWidth(2.0);
	for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end();
			++iter) {
		Wall* w = *iter;
		glBegin(GL_LINES);
		glVertex2f(w->src->x, w->src->y);
		glVertex2f(w->dst->x, w->dst->y);
		glEnd();
	}

	if (shadeOption == 1) {
		glColor4f(0.8, 0.8, 0.8, 0.7);
		for (int i = 0; (unsigned) i < polygons.size(); i++) {
			if (i == 0 && firstPolygonClockwise == 1) {
				continue;
			}
			glBegin(GL_POLYGON);
			for (int j = 0; (unsigned) j < polygons[i].corners.size(); j++) {
				glVertex2f(polygons[i].corners[j]->x,
						polygons[i].corners[j]->y);
			}
			glEnd();
		}
	}
	glLineWidth(1.0);
	glDisable(GL_BLEND);
}

void drawCircle(float Radius, int numPoints, double x, double y, double r,
		double g, double b) {
	glColor3f(r, g, b);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= numPoints; ++i) {
		float Angle = i * (2.0 * 3.1415926 / numPoints);
		float X = cos(Angle) * Radius;
		float Y = sin(Angle) * Radius;
		glVertex2f(X + x, Y + y);
	}
	glEnd();
	glLineWidth(1.0);
}

void filledCircle(double radius, double x, double y, double r, double g,
		double b) {
	int numPoints = 100;
	glColor3f(r, g, b);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= numPoints; ++i) {
		float Angle = i * (2.0 * 3.1415926 / numPoints);
		float X = cos(Angle) * radius;
		float Y = sin(Angle) * radius;
		glVertex2f(X + x, Y + y);
	}
	glEnd();
}

void drawLine() {
//	if (noPath) {
//		glColor3f(0, 0, 0);
//	} else {
	glColor3f(0xFF / 255.0, 0x99 / 255.0, 0x66 / 255.0);
//	}
	glLineWidth(3.0);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x0F0F);
	glBegin(GL_LINES);
	glVertex2f(alpha[0], alpha[1]);
	glVertex2f(beta[0], beta[1]);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glLineWidth(1.0);
}

void renderScene(void) {

//	cout << "renderScene!!!!!!!!!!!!!!!!!" << endl;

//	cout << "renderCount :" << renderCount << endl;
	renderCount++;

	hideBoxBoundary = radioDrawOption->get_int_val();
	shadeOption = radioShadeOption->get_int_val();
	verboseOption = radioVerboseOption->get_int_val();
	drawPathOption = radioDrawPathOption->get_int_val();

	if (animationOption == 1 || animationOption == 2) {
		leafBoxesDrawed = false;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	//draw leaf boxes to fbo
	if (!leafBoxesDrawed) {
		if (!(animationOption == 1 || animationOption == 2)) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		glScalef(2.0 / boxWidth, 2.0 / boxHeight, 0);
		glTranslatef(-boxWidth / 2, -boxHeight / 2, 0);

//		glDisable(GL_BLEND);

		// TODO fill the initial board yellow
		glColor3f(0xFF / 255.0, 0xFF / 255.0, 0x66 / 255.0);
		Box* b = allLeaf[0];

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);
		glVertex2f(b->x - b->width / 2, b->y - b->height / 2);
		glVertex2f(b->x + b->width / 2, b->y - b->height / 2);
		glVertex2f(b->x + b->width / 2, b->y + b->height / 2);
		glVertex2f(b->x - b->width / 2, b->y + b->height / 2);
		glEnd();

		if (!allLeaf.empty() && allLeaf.size() != 0) {
			int i = 0;
			for (vector<Box*>::iterator it = allLeaf.begin();
					it != allLeaf.end(); ++it) {
				i++;
				if (animationOption == 0 || animationOption == 3
						|| animationOption == 4) {
					drawQuad(*it);
				}

				if (animationOption == 1 || animationOption == 2) {
					if (currentStep >= i) {
						drawQuad(*it);
//					cout << "currentStep:  " << currentStep << endl;
					}

					if (currentStep == i) {
						break;
					}
				}
			}
		}
		if (!(animationOption == 1 || animationOption == 2)) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			leafBoxesDrawed = true;
		}
	}

//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	if (!(animationOption == 1 || animationOption == 2)) {
		// clear default frame buffer for the whole window
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glScalef(2.0 / boxWidth, 2.0 / boxHeight, 0);
		glTranslatef(-boxWidth / 2, -boxHeight / 2, 0);

		//draw fbo to screen by render GL_QUADS usig texture mapping
		glBindTexture(GL_TEXTURE_2D, img);
		glEnable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		Box* b = allLeaf[0];
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(b->x - b->width / 2, b->y - b->height / 2);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(b->x + b->width / 2, b->y - b->height / 2);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(b->x + b->width / 2, b->y + b->height / 2);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(b->x - b->width / 2, b->y + b->height / 2);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	if (drawPathOption != 0) {
		animationOption = 0;
		stepIncrease = 0;
		buttonAnimation->set_name("Path Animation");
		buttonReplay->set_name("Replay Spliting");
	}

	if (drawPathOption == 3) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if (!noPath && drawPathOption != 2) {
		drawPath(PATH);
	}

	if (drawPathOption == 1) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glPolygonMode(GL_FRONT, GL_LINE);

	drawCircle(R0, 100, alpha[0], alpha[1], 0, 1, 1);	// start
	drawCircle(R0 + 10, 100, alpha[0], alpha[1], 0, 1, 1);	// start
	drawCircle(R0, 100, beta[0], beta[1], 1, 0, 1);	// goal

	double r0 = 5;
	if (r0 > R0)
		r0 = R0;
	filledCircle(r0, alpha[0], alpha[1], 0.2, 0.2, 1.0);	//blue start center
	filledCircle(r0, beta[0], beta[1], 0.8, 0.8, 0.2);	//yellow goal center

//	cout
//			<< "7777777777777777777777777777777777777777777777777777777777777777777777"
//			<< endl;
	drawWalls(QT->pRoot);

	drawLine();

//	if (boxA) {
//		drawLinks(boxA, alpha[0], alpha[1]);
//	}
//	if (boxB) {
//		drawLinks(boxB, beta[0], beta[1]);
//	}

	drawLinksSrcDst(alpha);
	drawLinksSrcDst(beta);

//	glFlush();
	glutSwapBuffers();
//	if (animationOption == 1 || animationOption == 3) {
////		sleep(1);
//		glutPostRedisplay();
//	}
	glutPostRedisplay();
//	renderLock = false;
}

//void *thread_render(void* arg) {
//
//	cout << "laksjdlfka;klsdjf" << endl;
//
//	run();
//
//}

/* ********************************************************************** */
// skip blanks, tabs, line breaks and comment lines,
// 	leaving us at the beginning of a token (or EOF)
// 	(This code is taken from core2/src/CoreIo.cpp)
int skip_comment_line(std::ifstream & in) {
	int c;

	do {
		c = in.get();
		while (c == '#') {
			do {	// ignore the rest of this line
				c = in.get();
			} while (c != '\n');
			c = in.get(); // now, reach the beginning of the next line
		}
	} while (c == ' ' || c == '\t' || c == '\n'); //ignore white spaces and newlines

	if (c == EOF)
		std::cout << "unexpected end of file." << std::endl;

	in.putback(c); // this is non-white and non-comment char!
	return c;
}  //skip_comment_line

// skips '\' followed by '\n'
// 	NOTE: this assumes a very special file format (e.g., our BigInt File format)
// 	in which the only legitimate appearance of '\' is when it is followed
// 	by '\n' immediately!  
int skip_backslash_new_line(std::istream & in) {
	int c = in.get();

	while (c == '\\') {
		c = in.get();

		if (c == '\n')
			c = in.get();
		else
// assuming the very special file format noted above!
			cout
					<< "continuation line \\ must be immediately followed by new line.\n";
	} //while
	return c;
} //skip_backslash_new_line

/* ********************************************************************** */

void parseConfigFile(Box* b) {
	polygons.clear();
	srcInPolygons.clear();

	std::stringstream ss;
	ss << inputDir << "/" << fileName;	// create full file name
	std::string s = ss.str();

	fileProcessor(s);	// this will clean the input and put in

	ifstream ifs("output-tmp.txt");
	if (!ifs) {
		cerr << "cannot open input file" << endl;
		exit(1);
	}

	// First, get to the beginning of the first token:
	skip_comment_line(ifs);

	int nPt, nPolygons;	// previously, nPolygons was misnamed as nFeatures
	ifs >> nPt;

	//Read input points:
	vector<double> pts(nPt * 2);
	for (int i = 0; i < nPt; ++i) {
		ifs >> pts[i * 2] >> pts[i * 2 + 1];
	}

	//Read input polygons:
	ifs >> nPolygons;
	string temp;
	std::getline(ifs, temp);
	for (int i = 0; i < nPolygons; ++i) {
		Polygon tempPolygon;
		string s;
		std::getline(ifs, s);
		stringstream ss(s);
		vector<Corner*> ptVec;
		set<int> ptSet;
		while (ss) {
			int pt;
			/// TODO:
			ss >> pt;
			pt -= 1;	//1 based array
			if (ptSet.find(pt) == ptSet.end()) {
				ptVec.push_back(
						new Corner(pts[pt * 2] * scale + deltaX,
								pts[pt * 2 + 1] * scale + deltaY));

				b->addCorner(ptVec.back());
				b->vorCorners.push_back(ptVec.back());
				ptSet.insert(pt);
				if (ptVec.size() > 1) {
					Wall* w = new Wall(ptVec[ptVec.size() - 2],
							ptVec[ptVec.size() - 1]);
					b->addWall(w);
					b->vorWalls.push_back(w);
				}
			}	//if
			//new pt already appeared, a loop is formed.
			//should only happen on first and last pt
			else {
				if (ptVec.size() > 1) {
					Wall* w = new Wall(ptVec[ptVec.size() - 1], ptVec[0]);
					b->addWall(w);
					b->vorWalls.push_back(w);
					break;
				}
			}
		}		// while(ss)
		tempPolygon.corners = ptVec;
		tempPolygon.corners.push_back(ptVec[0]);
		polygons.push_back(tempPolygon);
		if (pointInPolygon(alpha[0], alpha[1], tempPolygon)) {
			srcInPolygons.push_back(1);
		} else {
			srcInPolygons.push_back(0);
		}
		if (i == 0) {		// if first polygon is clockwise, set globalMark
			firstPolygonClockwise = checkClockwise(tempPolygon);
		}
	}		//for i=0 to nPolygons
	ifs.close();
	if (true) {
		cout << "input file name = " << s << endl;
		cout << "nPt=" << nPt << endl;
		cout << "nPolygons=" << nPolygons << endl;
	}
}		//parseConfigFile

void resetAndRun() {
	reset();

	editAlphaX->set_float_val(alpha[0]);
	editAlphaY->set_float_val(alpha[1]);
	editAlphaTheta1->set_float_val(alpha[2]);
	editAlphaTheta2->set_float_val(alpha[3]);
	editBetaX->set_float_val(beta[0]);
	editBetaY->set_float_val(beta[1]);
	editBetaTheta1->set_float_val(beta[2]);
	editBetaTheta2->set_float_val(beta[3]);
	editEpsilon->set_float_val(epsilon);
	editL1->set_float_val(L1);
	editL2->set_float_val(L2);
	editThickness->set_float_val(thickness);
	radioQType->set_int_val(QType);
	editSeed->set_int_val(seed);
	radioVerboseOption->set_int_val(verboseOption);
	radio2StrategyOption->set_int_val(0);
	textPhiB->set_int_val(sizeOfPhiB);
	radioDrawOption->set_int_val(0);
	radioShadeOption->set_int_val(0);
	radioDrawPathOption->set_int_val(0);

	run();

}

void reset() {
	if (argcSave > 1)
		interactive = atoi(argvSave[1]);	// Interactive (0) or no (>0)
	if (argcSave > 2)
		alpha[0] = atof(argvSave[2]);		// start x
	if (argcSave > 3)
		alpha[1] = atof(argvSave[3]);		// start y
	if (argcSave > 4)
		alpha[2] = atof(argvSave[4]);// start theta1, convert from degree to radian
	if (argcSave > 5)
		alpha[3] = atof(argvSave[5]);// start theta2, convert from degree to radian
	if (argcSave > 6)
		beta[0] = atof(argvSave[6]);		// goal x
	if (argcSave > 7)
		beta[1] = atof(argvSave[7]);		// goal y
	if (argcSave > 8)
		beta[2] = atof(argvSave[8]);// goal theta1, convert from degree to radian
	if (argcSave > 9)
		beta[3] = atof(argvSave[9]);// goal theta2, convert from degree to radian
	if (argcSave > 10)
		epsilon = atof(argvSave[10]);		// epsilon (resolution)
	if (argcSave > 11)
		L1 = atof(argvSave[11]);		// robot length1
	if (argcSave > 12)
		L2 = atof(argvSave[12]);		// robot length2
	if (argcSave > 13)
		thickness = atof(argvSave[13]);		// robot thickness
	if (argcSave > 14)
		fileName = argvSave[14]; 		// Input file name
	if (argcSave > 15)
		boxWidth = atof(argvSave[15]);		// boxWidth
	if (argcSave > 16)
		boxHeight = atof(argvSave[16]);	// boxHeight
	if (argcSave > 17)
		windowPosX = atoi(argvSave[17]);	// window X pos
	if (argcSave > 18)
		windowPosY = atoi(argvSave[18]);	// window Y pos
	if (argcSave > 19)
		QType = atoi(argvSave[19]);	// PriorityQ Type (random or no)
	if (argcSave > 20)
		seed = atoi(argvSave[20]);		// for random number generator
	if (argcSave > 21)
		inputDir = argvSave[21];		// path for input files
	if (argcSave > 22)
		deltaX = atof(argvSave[22]);	// x-translation of input file
	if (argcSave > 23)
		deltaY = atof(argvSave[23]);	// y-translation of input file
	if (argcSave > 24)
		scale = atof(argvSave[24]);		// scaling of input file
	if (argcSave > 25)
		verboseOption = atoi(argvSave[25]);	// verboseOption
	if (argcSave > 26)
		title = argvSave[26];		// title
	if (argcSave > 27)
		twoStrategyOption = atoi(argvSave[27]);	// two Strategy Option (0: original 1: smarter)
	if (argcSave > 28)
		sizeOfPhiB = atoi(argvSave[28]);// threshold for splitting angles  |Phi(B)|
}

//init FBO
void initFbo() {
	//Initialize GLEW
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		exit(1);
	}

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.2f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glViewport(0, 0, boxWidth, boxHeight);

	// Setup our FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Create the render buffer for depth
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, boxWidth,
			boxHeight);

	// Now setup a texture to render to
	glGenTextures(1, &img);
	glBindTexture(GL_TEXTURE_2D, img);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, boxWidth, boxHeight, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// And attach it to the FBO so we can render to it
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			img, 0);

	// Attach the depth render buffer to the FBO as it's depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, depthBuffer);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		exit(1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// Unbind the FBO for now
}

void redrawFBO() {
	leafBoxesDrawed = false;
	glutPostRedisplay();
}

// check whether a polygon is clockwise or counter-clockwise.
// return value
// 0: other issues
// 1: clockwise
// 2: counter-clockwise

int checkClockwise(Polygon p) {
	if (p.corners.size() == 0) {
		return 0;
	}

	double maxX = -100000;
	int maxI = -1;
	int prevI = -1;
	int nextI = -1;
//	cout<< p.corners.size()<<endl;
	for (int i = 0; (unsigned) i < p.corners.size(); i++) {
		if (p.corners[i]->x > maxX) {
			maxX = p.corners[i]->x;
			maxI = i;
			if (maxI == 0) {
				prevI = p.corners.size() - 2;
			} else {
				prevI = maxI - 1;
			}

			if ((unsigned) maxI == p.corners.size() - 2) {
				nextI = 0;
			} else {
				nextI = maxI + 1;
			}
		}
	}
	if ((p.corners[nextI]->y - p.corners[maxI]->y)
			/ (p.corners[nextI]->x - p.corners[maxI]->x == 0 ?
					-0.01 : (p.corners[nextI]->x - p.corners[maxI]->x))
			- (p.corners[prevI]->y - p.corners[maxI]->y)
					/ (p.corners[prevI]->x - p.corners[maxI]->x == 0 ?
							-0.01 : (p.corners[prevI]->x - p.corners[maxI]->x))
			> 0) {
//		cout<<(p.corners[nextI]->y - p.corners[maxI]->y)
//					/ (p.corners[nextI]->x - p.corners[maxI]->x == 0? -0.01 : (p.corners[nextI]->x - p.corners[maxI]->x))
//					<< " "<< (p.corners[prevI]->y - p.corners[maxI]->y)
//							/ (p.corners[prevI]->x - p.corners[maxI]->x == 0? -0.01 : (p.corners[prevI]->x - p.corners[maxI]->x))<<endl;
//
//		cout<< p.corners[prevI]->y<<endl;
//		cout<< p.corners[maxI]->y<<endl;

		return 1;
	} else {
		return 2;
	}

}
//void drawPathOption{
//
//}

