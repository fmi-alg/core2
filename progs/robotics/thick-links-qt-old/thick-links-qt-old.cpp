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

#include <QDir>
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

#include "MainWindow.h"
#include "QApplication"
#include <QSurfaceFormat>
#include <set>



#define mw_out (*window)

static MainWindow *window;

static int runCount=1;

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
string egName("non-crossing-2links_Troom.eg");
char egNameList[200][200];
int numEg = 0;

FILE *fp;

double alpha[4] = { 125, 115, 355, 5 };		// start configuration
double beta[4] = { 230, 470, 275, 265 };		// goal configuration
double epsilon = 4;			// resolution parameter
Box* boxA;				// start box (containing alpha)
Box* boxB;				// goal box (containing beta)
double boxWidth = 512;			// Initial box width
double boxHeight = 512;			// Initial box height

// Added by Zhongdi 05/08/2013 begin
// length of 2 links
double L1 = 140;
double L2 = 139;

// thickness of robot
double thickness = 10;

double R0 = 0;				// will be set to max(L1,L2)
// Added by Zhongdi 05/08/2013 end

int windowPosX = 320;			// X Position of Window
int windowPosY = 20;			// Y Position of Window
string fileName("T-room.txt"); 		// Input file name
//string fileName("input150.txt"); 		// Input file name
string inputDir("inputs"); 		// Path for input files
int QType = 2;				// The Priority Queue can be
//    sequential (0) or random (1)
int interactive = 0;			// Run interactively?
//    Yes (0) or No (1)
int seed = 11;				// seed for random number generator
// (Could also be used for BFS, etc)
double deltaX = 0;			// x-translation of input environment
double deltaY = -95;			// y-translation of input environment
double scale = 0.08;				// scaling of input environment
bool noPath = true;			// True means there is "No path.

bool hideBox = true;
bool hideBoxBoundary = true;  		// don't draw box boundary

bool verboseOption = false;		// don't print various statistics

int drawPathOption = 0;
int twoStrategyOption = 0; //  Two-Strategy Option    0: original 1: smarter
int sizeOfPhiB = 0;
//////////////////////
int crossingOption = 0; //  Crossing Option    0: original  1: non-crossing
double bandwidth = 0;

vector<Box*> PATH;



extern std::vector<int> expansions;


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

int renderCount = 0;
//int countAAA = 0;
//int countBBB = 0;
//int countCCC = 0;

stringstream ssout;
stringstream ssoutLastTime;
stringstream ssTemp;
stringstream ssInfo;

vector<Box*> boxClicked;

//volatile bool renderReady = false;


bool leafBoxesDrawed = false;

int firstPolygonClockwise = 0;

// External Routines ========================================
//////////////////////////////////////////////////////////////////////////////////
void renderScene(void);
void parseExampleList();
void parseExampleFile();
void parseConfigFile(Box*);
//void idle();
void replaySplitting();

void previousStep(int);
void nextStep(int);
void run();
void genEmptyTree();
void drawPath(vector<Box*>&);
extern int fileProcessor(string inputfile);
void drawCircle(float Radius, int numPoints, double x, double y, double r,
        double g, double b);
void drawLine();
//void drawTri(Box*);
//void drawTri(Box*, double, double);
void drawLinks(Box*);
//void drawLinks(Box*, double, double);

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



extern int renderSteps;
extern bool step;



bool runAnim(true);
bool pauseAnim(false);
bool replayAnim(false);
extern int animationSpeed;
extern int animationSpeedScale;
extern int animationSpeedScaleBox;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//find path using simple heuristic:
//use distance to beta as key in PQ, see dijkstraQueue
template<typename Cmp>
bool findPath(Box* a, QuadTree* QT, int& ct) {
    bool isPath = false;
    vector<Box*> toReset;
    a->dist2Source = 0;
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
                        for (vector<Box*>::iterator iter = cldrn[i]->Nhbrs[j].begin(); iter < cldrn[i]->Nhbrs[j].end(); ++iter) {
                            Box* n = *iter;

                            fprintf(fp, "ch %d %lf neighbor (%d) %lf %lf width %lf angle %lf %lf %lf %lf\n", j, n->dist2Source, n->status, n->x, n->y, n->width
                                    , n->xi[0], n->xi[1], n->xi[2], n->xi[3]);

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


int main(int argc, char* argv[]) {


    bool foundFiles = false;
    std::string workingDir = QDir::currentPath().toStdString();

    // Test if the build directory is thick-links. If so,
    // the path to the current working directory will
    // include /thick-links
    int indexOfDesiredDir = workingDir.rfind("/thick-links-qt-old/");
    if (indexOfDesiredDir != (int)std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 5);

        // Set current working directory to thick-links
        QDir::setCurrent(workingDir.c_str());

        foundFiles = true;
    }

    // Test if program was downloaded from Github, and is the build directory.
    // Downloading it from Github will result in the folder having the name
    // /thick-links-master instead of /thick-links
    if (!foundFiles &&
        (indexOfDesiredDir = workingDir.rfind("/thick-links-qt-old/")) != (int)std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 12);

        // Set current working directory to /thick-links-master
        QDir::setCurrent(workingDir.c_str());

        foundFiles = true;
    }

    // Test if a build directory (/build-thick-links-...) was created. This directory
    // will reside in the same directory as /thick-links or /thick-links-qt-old
    if (!foundFiles &&
        (indexOfDesiredDir = workingDir.rfind("/build-thick-links-qt-old")) != (int)std::string::npos) {
        QDir dir(workingDir.substr(0, indexOfDesiredDir).c_str());

        if (dir.exists("thick-links-qt-old/thick-links-qt-old.pro")) {                  // Test if /v2-links exists
            workingDir = workingDir.substr(0, indexOfDesiredDir) + "/thick-links-qt-old";

            // Set current working directory to thick-links
            QDir::setCurrent(workingDir.c_str());

            foundFiles = true;
        } else if (dir.exists("thick-links-qt-old/thick-links-qt-old.pro")) {  // Test if /thick-links-qt-old exists
            workingDir = workingDir.substr(0, indexOfDesiredDir) + "/thick-links-qt-old";

            // Set current working directory to thick-links
            QDir::setCurrent(workingDir.c_str());

            foundFiles = true;
        }
    }

    // Neither /thick-links nor /thick-links-qt-old could be found
    if (!foundFiles) {
        std::cerr << std::endl << "!! WARNING !!\n"
        << "The program may not work correctly or at all because the folder "
        "containing the program's files cannot be found.\n"
        "Make sure that the program is inside of a folder named \"thick-links-qt-old\".\n"
        "If it is not, rename the folder to \"thick-links-qt-old\" before running again.\n";
    }


// Added by Zhongdi 05/08/2013 begin
// calculate the R of the robot
    R0 = max(L1, L2);
// Added by Zhongdi 05/08/2013 end

    if (interactive > 0) {	// non-interactive
    }

    if (interactive == 0) {
    }

    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    // Allow creation of Qt GUI
    QApplication app(argc, argv);
    fp = fopen("debug.txt", "w");
    if(fp == NULL) return 0;







    //testing
    parseExampleList();
    parseExampleFile();
    window = new MainWindow();
    run();
    window->show();




    if (interactive > 0) {	// non-interactive
        // do something...
        mw_out << "Non Interactive Run of Disc Robot\n";
        if (noPath)
            mw_out << "No Path Found!\n";
        else
            mw_out << "Path was Found!\n" ;
        return 0;
    } else {


    }

    return app.exec();
}

void genEmptyTree() {

    Box* root = new Box(boxWidth / 2, boxHeight / 2, boxWidth, boxHeight);

    Box::r0 = R0;
    Box::l1 = L1;
    Box::l2 = L2;
    Box::thickness = thickness;

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
    QT = new QuadTree(root, epsilon, QType); // Note that seed keeps changing!

    if (verboseOption)
        mw_out << "done genEmptyTree \n";
}


void run() {
    runCount++;

    ssoutLastTime.str("");
    ssoutLastTime << ssout.str();
    ssout.str("");
    ssout << "Run No. " << runCount << ":" << endl;

    currentStep = 0;
    currentPathStep = 0;
    leafBoxesDrawed = false;

    if (interactive == 0) {
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
            mw_out << "Start Configuration is not free\n";
        }

        boxB = QT->getBox(beta[0], beta[1], beta[2], beta[3], ct);
        if (!boxB) {
            noPath = true;
            mw_out << "Goal Configuration is not free\n";
        }
        if (!noPath) {
            if (QType == 2) {
                noPath = !findPath<DistCmp>(boxA, QT, ct);
            } else if (QType == 3) {
                noPath = !findPath<DistPlusSizeCmp>(boxA, QT, ct);
            } else if (QType == 4) {
                noPath = !findPath<VorCmp>(boxA, QT, ct);
            }
        }
    }

    t.stop();
    if (!noPath) {
        Graph graph;
        PATH.clear();
        PATH = graph.dijkstraShortestPath(boxA);
    }
    if (verboseOption)
        cout << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
    if (!noPath)
        mw_out << ">>      ----->>  Path Found !\n";
    else
        mw_out << ">>      ----->>  No Path !\n";
    mw_out << ">>\n";
    mw_out << ">>      ----->>  Time used: " << t.getElapsedTimeInMilliSec() << " ms\n";
    mw_out << ">>\n";
    mw_out << ">>      ----->>  Qtype: ";
    switch (QType) {
    case 0:
        mw_out << "Random Strategy\n";
        break;
    case 1:
        mw_out << "BFS Strategy\n";
        break;
    case 2:
        mw_out << "Greedy Strategy\n";
        break;
    case 3:
        mw_out << "Dist+Size Strategy\n";
        break;
    case 4:
        mw_out << "Voronoi Strategy\n";
        break;
    }
    mw_out << ">>\n";
    if (verboseOption)
        cout << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
    if (verboseOption) {
        cout << "    Expanded " << ct << " times\n" << endl;
        cout << "    total Free boxes: " << freeCount << "\n";
        cout << "    total Stuck boxes: " << stuckCount << "\n";
        cout << "    total Mixed boxes smaller than epsilon: " << mixSmallCount << endl;
        cout << "    total Mixed boxes bigger than epsilon: " << mixCount - mixSmallCount << endl;
    }

    totalSteps = allLeaf.size();
    freeCount = stuckCount = mixCount = mixSmallCount = 0;
    mw_out << "################## END of RUN #################\n";
}// run


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
        mw_out << "unexpected end of file.\n" ;

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
            mw_out
                    << "continuation line \\ must be immediately followed by new line.\n";
    } //while
    return c;
} //skip_backslash_new_line


char egPath[200], tmp[200];
void parseExampleList(){
    sprintf(egPath, "ls -R > tmpList");
    system(egPath);

    sprintf(egPath, "tmpList");
    FILE *fptr = fopen(egPath, "r");
    if(fptr == NULL) return ;
    while(fgets(tmp, 200, fptr) != NULL){
        char *sptr = strtok(tmp, " \n");
        while(sptr != NULL){
            int len = strlen(sptr);
            if(len > 3 && sptr[len-1] == 'g' && sptr[len-2] == 'e' && sptr[len-3] == '.'){
                strcpy(egNameList[numEg], sptr);
                ++numEg;
            }
            sptr = strtok(NULL, " \n");
        }
    }

    sprintf(egPath, "rm -rf tmpList");
    system(egPath);
}

void parseExampleFile() {

    sprintf(egPath, "%s/%s", inputDir.c_str(), egName.c_str());
    FILE *fptr = fopen(egPath, "r");
    if (fptr == NULL) return ;

    while (fgets(tmp, 200, fptr) != NULL){
        char *sptr = strtok(tmp, "=: \t");

        // comments
        if (strcmp(sptr, "#") == 0) {
            continue;
        }

        if (strcmp(sptr, "interactive") == 0) {
            sptr = strtok(NULL, "=: \t");
            interactive = atoi(sptr);
        }

        // start configuration
        if (strcmp(sptr, "startX") == 0) {
            sptr = strtok(NULL, "=: \t");
            alpha[0] = atof(sptr);
        }
        if (strcmp(sptr, "startY") == 0) {
            sptr = strtok(NULL, "=: \t");
            alpha[1] = atof(sptr);
        }
        if (strcmp(sptr, "startTheta1") == 0) {
            sptr = strtok(NULL, "=: ");
            alpha[2] = atof(sptr);
        }
        if (strcmp(sptr, "startTheta2") == 0) {
            sptr = strtok(NULL, "=: \t");
            alpha[3] = atof(sptr);
        }

        // goal configuration
        if (strcmp(sptr, "goalX") == 0) {
            sptr = strtok(NULL, "=: \t");
            beta[0] = atof(sptr);
        }
        if (strcmp(sptr, "goalY") == 0) {
            sptr = strtok(NULL, "=: \t");
            beta[1] = atof(sptr);
        }
        if (strcmp(sptr, "goalTheta1") == 0) {
            sptr = strtok(NULL, "=: \t");
            beta[2] = atof(sptr);
        }
        if (strcmp(sptr, "goalTheta2") == 0) {
            sptr = strtok(NULL, "=: \t");
            beta[3] = atof(sptr);
        }

        if (strcmp(sptr, "epsilon") == 0) {
            sptr = strtok(NULL, "=: \t");
            epsilon = atof(sptr);
        }

        if (strcmp(sptr, "len1") == 0) {
            sptr = strtok(NULL, "=: \t");
            L1 = atof(sptr);
        }
        if (strcmp(sptr, "len2") == 0) {
            sptr = strtok(NULL, "=: \t");
            L2 = atof(sptr);
        }
        if (strcmp(sptr, "thickness") == 0) {
            sptr = strtok(NULL, "=: \t");
            thickness = atof(sptr);
        }

        if (strcmp(sptr, "inputDir") == 0) {
            sptr = strtok(NULL, "=: #\t");
            inputDir = sptr;
        }
        if (strcmp(sptr, "fileName") == 0) {
            sptr = strtok(NULL, "=: #\t");
            fileName = sptr;
        }

        // box width and height
        if (strcmp(sptr, "boxWidth") == 0) {
            sptr = strtok(NULL, "=: \t");
            boxWidth = atof(sptr);
        }
        if (strcmp(sptr, "boxHeight") == 0) {
            sptr = strtok(NULL, "=: \t");
            boxHeight = atof(sptr);
        }

        // windows position
        if (strcmp(sptr, "windowPosX") == 0) {
            sptr = strtok(NULL, "=: \t");
            windowPosX = atoi(sptr);
        }
        if (strcmp(sptr, "windowPosY") == 0) {
            sptr = strtok(NULL, "=: \t");
            windowPosY = atoi(sptr);
        }

        if (strcmp(sptr, "Qtype") == 0) {
            sptr = strtok(NULL, "=: \t");
            QType = atoi(sptr);
        }

        if (strcmp(sptr, "seed") == 0) {
            sptr = strtok(NULL, "=: \t");
            seed = atoi(sptr);
        }

        if (strcmp(sptr, "step") == 0) {
            sptr = strtok(NULL, "=: \t");
            step = atoi(sptr);
        }

        if (strcmp(sptr, "xtrans") == 0) {
            sptr = strtok(NULL, "=: \t");
            deltaX = atof(sptr);
        }

        if (strcmp(sptr, "ytrans") == 0) {
            sptr = strtok(NULL, "=: \t");
            deltaY = atof(sptr);
        }

        if (strcmp(sptr, "scale") == 0) {
            sptr = strtok(NULL, "=: \t");
            scale = atof(sptr);
        }

        if (strcmp(sptr, "verbose") == 0) {
            sptr = strtok(NULL, "=: \t");
            verboseOption = atoi(sptr);
        }


        if (strcmp(sptr, "animationSpeed") == 0) {
            sptr = strtok(NULL, "=: \t");
            animationSpeed = atoi(sptr);
        }
        if (strcmp(sptr, "animationSpeedScale") == 0) {
            sptr = strtok(NULL, "=: \t");
            animationSpeedScale = atoi(sptr);
        }
        if (strcmp(sptr, "animationSpeedScaleBox") == 0) {
            sptr = strtok(NULL, "=: \t");
            animationSpeedScaleBox = atoi(sptr);
        }
    }
}

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
        stringstream sss(s);
        vector<Corner*> ptVec;
        set<int> ptSet;
        while (sss) {
            int pt;
            /// TODO:
            sss >> pt;
            pt -= 1;	//1 based array
            if (ptSet.find(pt) == ptSet.end()) {
                ptVec.push_back(
                        new Corner(pts[pt * 2] * scale + deltaX,
                                pts[pt * 2 + 1] * scale + deltaY));

                b->addCorner(ptVec.back());
                b->vorCorners.push_back(ptVec.back());
                ptSet.insert(pt);
                if (ptVec.size() > 1) {
                    Wall* w = new Wall(ptVec[ptVec.size() - 2], ptVec[ptVec.size() - 1]);
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
        }
        else {
            srcInPolygons.push_back(0);
        }
        if (i == 0) {		// if first polygon is clockwise, set globalMark
               firstPolygonClockwise = checkClockwise(tempPolygon);
        }
    }		//for i=0 to nPolygons
    ifs.close();
    if (true) {
        mw_out << "input file name = " << s <<"\n";
        mw_out << "nPt=" << nPt<<"\n";
        mw_out << "nPolygons=" << nPolygons <<"\n";
    }
}		//parseConfigFile


int checkClockwise(Polygon p) {
    if (p.corners.size() == 0) {
        return 0;
    }

    double maxX = -100000;
    int maxI = -1;
    int prevI = -1;
    int nextI = -1;
//	mw_out<< p.corners.size()<<endl;
    for (int i = 0; (unsigned)i < p.corners.size(); i++) {
        if (p.corners[i]->x > maxX) {
            maxX = p.corners[i]->x;
            maxI = i;
            if (maxI == 0) {
                prevI = p.corners.size() - 2;
            } else {
                prevI = maxI - 1;
            }

            if ((unsigned)maxI == p.corners.size() - 2) {
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
//		mw_out<<(p.corners[nextI]->y - p.corners[maxI]->y)
//					/ (p.corners[nextI]->x - p.corners[maxI]->x == 0? -0.01 : (p.corners[nextI]->x - p.corners[maxI]->x))
//					<< " "<< (p.corners[prevI]->y - p.corners[maxI]->y)
//							/ (p.corners[prevI]->x - p.corners[maxI]->x == 0? -0.01 : (p.corners[prevI]->x - p.corners[maxI]->x))<<endl;
//
//		mw_out<< p.corners[prevI]->y<<endl;
//		mw_out<< p.corners[maxI]->y<<endl;

        return 1;
    } else {
        return 2;
    }

}

