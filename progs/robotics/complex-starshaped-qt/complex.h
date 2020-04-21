#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <set>
#include "QuadTree.h"
#include "PriorityQueue.h"
#include "Graph.h"
#include "Timer.h"
#include "stdlib.h"
#include "Polygon.h"
#include "Vertex.h"
#include "triangulate.h"
#include "Triangle.h"


static int runCount=1;

using namespace std;

// GLOBAL INPUT Parameters ========================================
//////////////////////////////////////////////////////////////////////////////////
string cfgName("L_rand100.cfg");    // Input example name
string fileName("map_parking.txt"); 		// Input file name
string inputDir("inputs");              // Path for input files
string workingDir;

vector<Triangle> compRoboTriangles;
string robotDir("robots");
string robotName("L.rob");

double alpha[3] = { 400, 360, 0 };		// start configuration
double beta[3] = { 100, 110, 0 };         // goal configuration
double epsilon = 2;			// resolution parameter

double R0 = 90;
double boxWidth = 512;			// Initial box width
double boxHeight = 512;			// Initial box height
int windowPosX = 320;			// X Position of Window
int windowPosY = 20;			// Y Position of Window
int QType = 2;                  // Search Strategy
int interactive = 0;			// Run interactively?
int seed = 11;                  // seed for random number generator

double deltaX = 0;			// x-translation of input environment
double deltaY = 0;			// y-translation of input environment
double scale = 1;			// scaling of input environment
bool noPath = true;			// True means there is "No path.

bool hideBoxBoundary = true;  		// don't draw box boundary
bool hideBox = false;  		// don't draw box
bool verboseOption = true;         // don't print various statistics
int drawPathOption = 0;
int twoStrategyOption = 0;          //  Two-Strategy Option    0: original 1: smarter
int sizeOfPhiB = 0;

char cfgNameList[200][200];
int numEg = 0;
Box* boxA;				// start box (containing alpha)
Box* boxB;				// goal box (containing beta)
vector<Box*> PATH;
double mouseX, mouseY;

int timePerFrame = 10;

// GLOBAL VARIABLES ========================================
//////////////////////////////////////////////////////////////////////////////////
int freeCount = 0;
int stuckCount = 0;
int mixCount = 0;
int mixSmallCount = 0;

int renderCount = 0;

stringstream ssout;
stringstream ssoutLastTime;
stringstream ssTemp;
stringstream ssInfo;
vector<Box*> boxClicked;

extern std::vector<int> expansions;
extern vector<Polygon> polygons;
extern vector<int> srcInPolygons;

bool leafBoxesDrawed(false);
int firstPolygonClockwise = 0;

vector<Box*> allLeaf;
vector<Set*> allSet;
QuadTree* QT;

int totalSteps = 0;
int currentStep = 0;
int stepIncrease = 0;
int currentPathStep = 0;

bool step(false);

bool showAnim(true);
bool pauseAnim(false);
bool replayAnim(false);
bool showFilledObstacles(false);
int animationSpeed(99);
int animationSpeedScale(5000);
int animationSpeedScaleBox(100);


// color coding variable ========================================
Color clr_totalFREE(0, 1, 0, 0.5);     // green
Color clr_partialFREE(0.25, 1, 0.25, 0.5); // dark green
Color clr_MIXED(1, 1, 0, 0.5); // yellow
Color clr_STUCK(1, 0, 0, 0.5); // red
Color clr_EPS(0.5, 0.5, 0.5, 1); // grey
Color clr_UNKNOWN(1, 1, 0, 0.5); // white

Color clr_start(1,0,1,0.5);    // purple
Color clr_goal(0.25,0,0.25,0.5); // dark purple
Color clr_robot(0.5,0,0.5,0.5); // purple

Color clr_path(0.5,0,0,1); // dark red
Color clr_obstacle(0,0,1,1); // blue
Color clr_boundary(1,1,1,1); // black

//FILE *fptr;

// External Routines ========================================
//////////////////////////////////////////////////////////////////////////////////
void renderScene(void);
void parseExampleList();
void parseExampleFile();
void parseConfigFile(Box*);
void replaySplitting();

void previousStep(int);
void nextStep(int);
void run();
void genEmptyTree();
void drawPath(vector<Box*>&);
extern int fileProcessor(string inputfile);
void drawCircle(float Radius, int numPoints, double x, double y, double r, double g, double b);
void drawLine();
void drawTri(Box*);
void drawTri(Box*, double, double);

void initFbo();
void redrawFBO();
int checkClockwise(Polygon p);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//find path using simple heuristic:
//use distance to beta as key in PQ, see dijkstraQueue
template<typename Cmp>
bool findPath(Box* a, Box* b, QuadTree* QT, int& ct) {

    bool isPath = false;
    vector<Box*> toReset;
    a->dist2Source = 0;
    dijkstraQueue<Cmp> dijQ;
    dijQ.push(a);
    toReset.push_back(a);
    while(!dijQ.empty())
    {
        Box* current = dijQ.extract();
        current->visited = true;

        // if current is MIXED, try expand it and push the children that is
        // ACTUALLY neighbors of the source set (set containing alpha) into the dijQ again
        if (current->status == Box::MIXED)
        {
            vector<Box*> cldrn;
            if (QT->expand(current, cldrn))
            {
                ++ct;
                for (int i = 0; i < (int)cldrn.size(); ++i)
                {
                    // go through neighbors of each child to see if it's in source set
                    // if yes, this child go into the dijQ
                    bool isNeighborOfSourceSet = false;
                    for (int j = 0; j < 6 && !isNeighborOfSourceSet; ++j)
                    {
                        for (vector<Box*>::iterator iter = cldrn[i]->Nhbrs[j].begin(); iter < cldrn[i]->Nhbrs[j].end(); ++iter)
                        {
                            Box* n = *iter;
                            if (n->dist2Source == 0)
                            {
                                isNeighborOfSourceSet = true;
                                break;
                            }
                        }

                    }

                    if (isNeighborOfSourceSet)
                    {
                        switch (cldrn[i]->getStatus())
                        {
                            //if it's FREE, also insert to source set
                            case Box::FREE:
                                cldrn[i]->dist2Source = 0;
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
            continue;
        }

        //found path!
        if (current == b)
        {
            isPath = true;
            break;
        }

        // if current is not MIXED, then must be FREE
        // go through it's neighbors and add FREE and MIXED ones to dijQ
        // also add FREE ones to source set
        if (current->status == Box::FREE){ // bug fix by Zhongdi (Aug 19, 2013)
                         // -- added if(status==FREE) test
          for (int i = 0; i < 6; ++i)
          {
              for (vector<Box*>::iterator iter = current->Nhbrs[i].begin(); iter < current->Nhbrs[i].end(); ++iter)
              {
                  Box* neighbor = *iter;
                  if (!neighbor->visited && neighbor->dist2Source == -1 && (neighbor->status == Box::FREE || neighbor->status == Box::MIXED))
                  {
                      if (neighbor->status == Box::FREE)
                      {
                          neighbor->dist2Source = 0;
                      }
                      dijQ.push(neighbor);
                      toReset.push_back(neighbor);
                  }
              }
          }//for
        }//if (bug fix)
    }

    //these two fields are also used in dijkstraShortestPath
    // need to reset
    for (int i = 0; i < (int)toReset.size(); ++i)
    {
        toReset[i]->visited = false;
        toReset[i]->dist2Source = -1;
    }

    return isPath;
}


bool loadComplexRobot(string& robot_file, vector<Triangle>& compRoboTri) {

  fileProcessor(robot_file);	// this will clean the input and put in

  ifstream infile("output-tmp.txt");
  if (!infile) {
      cerr << "cannot open input file" << endl;
      exit(1);
  }

  int nv, nt;
  vector< Vector2d > verts;


  infile >> nv;


  double x,y;
  double maxdist = 0;
  for (int i = 0; i < nv; ++i) {
    infile >> x >> y;
    verts.push_back(Vector2d(x,y));
    maxdist = max(maxdist, x*x + y*y);
  }
  maxdist = sqrt(maxdist);

  compRoboTri.clear();
  if (infile >> nt) { // if manually input triangles
    for (int i = 0; i < nt; ++i)  {
      int a,b,c;
      infile >> a >> b >> c;
      compRoboTri.emplace_back(Pose(verts[a].GetX()/maxdist,verts[a].GetY()/maxdist),
                               Pose(verts[b].GetX()/maxdist,verts[b].GetY()/maxdist),
                               Pose(verts[c].GetX()/maxdist,verts[c].GetY()/maxdist));
    }
  }
  else {  // otherwise, construct the triangles by neighbouring verts and the origin
    for (auto it = verts.begin(); it !=verts.end(); ++it) {
      if (it->GetX() == 0 && it->GetY() == 0) it = verts.erase(it);
    }
    Vector2d origin(0,0);
    for(unsigned int k = 0; k < verts.size(); ++k) {
      unsigned int j = (k + 1) % verts.size();
      compRoboTri.emplace_back(Pose(origin.GetX(),origin.GetY()),
                               Pose(verts[k].GetX()/maxdist,verts[k].GetY()/maxdist),
                               Pose(verts[j].GetX()/maxdist,verts[j].GetY()/maxdist));
      }
  }


  if (verboseOption) {
      mw_out << "Robot has "<< nv << " verts, " << nt << " triangles.\n";
  }
  return true;
}


void genEmptyTree() {

    Box* root = new Box(boxWidth / 2, boxHeight / 2, boxWidth, boxHeight);

    Box::r0 = R0;
    
    Box::pAllLeaf = &allLeaf;

    if (!allLeaf.empty() && allLeaf.size() != 0) {
        for (vector<Box*>::iterator it = allLeaf.begin(); it != allLeaf.end(); ++it) {
            delete *it;
        }
    }

    if (!allSet.empty() && allSet.size() != 0) {
        for (vector<Set*>::iterator it = allSet.begin(); it != allSet.end(); ++it) {
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
    QT = new QuadTree(root, epsilon, QType);

    //if (verboseOption)  mw_out << "done genEmptyTree \n";
}

void run() {
    mw_out << "Run No. " << runCount++ << "\n";

    currentStep = 0;
    currentPathStep = 0;
    leafBoxesDrawed = false;

    // bo debug
    string robot_file = robotDir + "/" + robotName;
    mw_out << "robot file name = " << robot_file << "\n";
    if (interactive == 0) robot_file = workingDir + "/" + robot_file;
    if (!loadComplexRobot(robot_file, compRoboTriangles)){
        return;
    }


    //if (interactive == 0) {
    // inputs are always in degrees
    // human only under stand degrees
    while (alpha[2] >= 360) {
        alpha[2] -= 360;
    }
    while (alpha[2] < 0) {
        alpha[2] += 360;
    }
    alpha[2] = alpha[2]*deg2rad;

    while (beta[2] >= 360) {
        beta[2] -= 360;
    }
    while (beta[2] < 0) {
        beta[2] += 360;
    }
    beta[2] = beta[2]*deg2rad;
    //}

    if (verboseOption) {
        mw_out << "   radius = " << R0 << ", eps = " << epsilon<<"\n" ;
        mw_out << "   alpha = (" << alpha[0] << ", " << alpha[1] << ", " << alpha[2] << ")\n" ;
        mw_out << "    beta = (" <<  beta[0] << ", " <<  beta[1] << ", " <<  beta[2] << ")\n" ;
    }

    genEmptyTree();

    Timer t;
    t.start();

    noPath = false;	// initially, pretend we have found path
    int ct = 0;	// number of times a node is expanded



    if (QType == 0 || QType == 1) {
        boxA = QT->getBox(alpha[0], alpha[1], alpha[2], ct);
        if (!boxA) {
            noPath = true;
            mw_out << "Start Configuration is not free\n";
        }

        boxB = QT->getBox(beta[0], beta[1], beta[2], ct);
        if (!boxB) {
            noPath = true;
            mw_out << "Goal Configuration is not free\n";
        }

        // In the following loop, "noPath" should really mean "hasPath"
        // Otherwise, we should pre-initialize "noPath" to true
        // before entering loop...
        while (!noPath && !QT->isConnected(boxA, boxB)) {
            if (!QT->expand()) {
                noPath = true;
            }
            ++ct;
        }
    }
    else if (QType == 2 || QType == 3 || QType == 4) {
        boxA = QT->getBox(alpha[0], alpha[1], alpha[2], ct);
        if (!boxA) {
            noPath = true;
            mw_out << "Start Configuration is not free\n";
        }

        boxB = QT->getBox(beta[0], beta[1], beta[2], ct);
        if (!boxB) {
            noPath = true;
            mw_out << "Goal Configuration is not free\n";
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
        PATH = graph.dijkstraShortestPath(boxA, boxB);
    }

    if (verboseOption)
        mw_out << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
    mw_out << ">>\n";

    if (!noPath)
        mw_out << ">>      ----->>  Path Found !\n" ;
    else
        mw_out << ">>      ----->>  No Path !\n";

    mw_out << ">>\n";
    mw_out << ">>      ----->>  Time used: " << t.getElapsedTimeInMilliSec()
            << " ms\n" ;
    //mw_out << ">>      ----->>  Expansion steps: ";
    //mw_out << (int) expansions.size()-1;
    //mw_out << "\n";
    mw_out << ">>\n";
    mw_out << ">>      ----->>  Search Strategy: ";
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
        mw_out << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
    if (verboseOption) {
        mw_out << "    Expanded " << ct << " times\n" ;
        mw_out << "    total Free boxes: " << freeCount<<"\n" ;
        mw_out << "    total Stuck boxes: " << stuckCount <<"\n";
        mw_out << "    total Mixed boxes smaller than epsilon: " << mixSmallCount<<"\n";

        mw_out << "    total Mixed boxes bigger than epsilon: "
                << mixCount - mixSmallCount <<"\n";
    }

    totalSteps = allLeaf.size();

    if (!noPath)
        ssout << "    ---->>   PATH FOUND !";
    else
        ssout << "    ---->>  NO PATH !" ;
    //ssout << "    ---->>   TIME USED: " << t.getElapsedTimeInMilliSec() << " ms";
    //ssout << "    ---->>   TOTAL STEPS: " << totalSteps ;
    //ssout << "    ---->>   Strategy No (" << QType << ")" ;
    if (verboseOption) {
        ssout << "    Expanded " << ct << " times" ;
        ssout << "    total Free boxes: " << freeCount ;
        ssout << "    total Stuck boxes: " << stuckCount ;
        ssout << "    total Mixed boxes smaller than epsilon: " << mixSmallCount;
        ssout << "    total Mixed boxes bigger than epsilon: "
                << mixCount - mixSmallCount ;
    }
    ssout << ssoutLastTime.str();
    freeCount = stuckCount = mixCount = mixSmallCount = 0;
    mw_out << "############## END of RUN #########\n";
} //run


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


char cfgPath[200], tmp[200], sptr[200];

void parseExampleFile() {

    sprintf(cfgPath, "%s/%s", inputDir.c_str(), cfgName.c_str());
    FILE *fptr = fopen(cfgPath, "r");
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
        if (strcmp(sptr, "startTheta") == 0) {
            sptr = strtok(NULL, "=: ");
            alpha[2] = atof(sptr);
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
        if (strcmp(sptr, "goalTheta") == 0) {
            sptr = strtok(NULL, "=: \t");
            beta[2] = atof(sptr);
        }

        if (strcmp(sptr, "epsilon") == 0) {
            sptr = strtok(NULL, "=: \t");
            epsilon = atof(sptr);
        }

        if (strcmp(sptr, "R") == 0) {
            sptr = strtok(NULL, "=: \t");
            R0 = atof(sptr);
        }

        if (strcmp(sptr, "inputDir") == 0) {
            sptr = strtok(NULL, "=: #\t");
            inputDir = sptr;
        }
        if (strcmp(sptr, "fileName") == 0) {
            sptr = strtok(NULL, "=: #\t");
            fileName = sptr;
        }

        if (strcmp(sptr, "robotName") == 0) {
            sptr = strtok(NULL, "=: #\t");
            robotName = sptr;
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

        if (strcmp(sptr, "verbose") == 0) {
            sptr = strtok(NULL, "=: \t");
            verboseOption = atoi(sptr);
        }

//        if (strcmp(sptr, "clr_totalFREE") == 0) {
//            for(int i=0;i<3;++i){
//                sptr = strtok(NULL, "=: \t");
//                clr_totalFREE[i] = atof(sptr);
//            }
//        }

//        if (strcmp(sptr, "clr_partialFREE") == 0) {
//            for(int i=0;i<3;++i){
//                sptr = strtok(NULL, "=: \t");
//                clr_partialFREE[i] = atof(sptr);
//            }
//        }

//        if (strcmp(sptr, "clr_MIXED") == 0) {
//            for(int i=0;i<3;++i){
//                sptr = strtok(NULL, "=: \t");
//                clr_MIXED[i] = atof(sptr);
//            }
//        }

//        if (strcmp(sptr, "clr_STUCK") == 0) {
//            for(int i=0;i<3;++i){
//                sptr = strtok(NULL, "=: \t");
//                clr_STUCK[i] = atof(sptr);
//            }
//        }

//        if (strcmp(sptr, "clr_EPS") == 0) {
//            for(int i=0;i<3;++i){
//                sptr = strtok(NULL, "=: \t");
//                clr_EPS[i] = atof(sptr);
//            }
//        }

//        if (strcmp(sptr, "clr_UNKNOWN") == 0) {
//            for(int i=0;i<3;++i){
//                sptr = strtok(NULL, "=: \t");
//                clr_UNKNOWN[i] = atof(sptr);
//            }
//        }

//        if (strcmp(sptr, "clr_start") == 0) {
//            for(int i=0;i<3;++i){
//                sptr = strtok(NULL, "=: \t");
//                clr_start[i] = atof(sptr);
//            }
//        }

//        if (strcmp(sptr, "clr_goal") == 0) {
//            for(int i=0;i<3;++i){
//                sptr = strtok(NULL, "=: \t");
//                clr_goal[i] = atof(sptr);
//            }
//        }

//        if (strcmp(sptr, "clr_path") == 0) {
//            for(int i=0;i<3;++i){
//                sptr = strtok(NULL, "=: \t");
//                clr_path[i] = atof(sptr);
//            }
//        }

//        if (strcmp(sptr, "clr_robot") == 0) {
//            for(int i=0;i<3;++i){
//                sptr = strtok(NULL, "=: \t");
//                clr_robot[i] = atof(sptr);
//            }
//        }

//        if (strcmp(sptr, "clr_obstacle") == 0) {
//            for(int i=0;i<3;++i){
//                sptr = strtok(NULL, "=: \t");
//                clr_obstacle[i] = atof(sptr);
//            }
//        }

//        if (strcmp(sptr, "clr_boundary") == 0) {
//            for(int i=0;i<3;++i){
//                sptr = strtok(NULL, "=: \t");
//                clr_boundary[i] = atof(sptr);
//            }
//        }
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
    if ((p.corners[nextI]->y - p.corners[maxI]->y) / (p.corners[nextI]->x - p.corners[maxI]->x == 0 ?
        -0.01 :
        (p.corners[nextI]->x - p.corners[maxI]->x)) - (p.corners[prevI]->y - p.corners[maxI]->y) / (p.corners[prevI]->x - p.corners[maxI]->x == 0 ?
        -0.01 :
        (p.corners[prevI]->x - p.corners[maxI]->x)) > 0) {

        return 1;
    } else {
        return 2;
    }
}

