
// Custom
#include "CORE/geom2d/point2d.h"
#include "CORE/geom2d/polygon2d.h"
#include "sss/Graph.h"
#include "strategy/PriorityQueue.h"
#include "subdivision/QuadTree.h"
#include "path/path.h"
#include "display/MainWindow.h"
#include "display/Color.h"
#include "utility/Timer.h"

// QT
#include <QApplication>
#include <QSurfaceFormat>
#include <QDir>

// Standard Library
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>

#define cw_out (*mainWin)


// GLOBAL VARIABLES ========================================
//
string cfgName("disc_vor2.cfg");
string fileName("oneblock.txt"); 	// Input file name
string inputDir("inputs"); 		// Path for input files
string workingDir;

QuadTree* QT = nullptr;
vector<Box*> allLeaf;
vector<Box*> PATH;

Point2d alpha(60, 320);	// start configuration
Point2d beta(460, 160);	// goal configuration
double epsilon = 6;             // resolution parameter
Box* boxA = nullptr;            // start box (containing alpha)
Box* boxB = nullptr;            // goal box (containing beta)
double boxWidth = 512;			// Initial box width
double boxHeight = 512;			// Initial box height
double R0 = 30;                 // Robot radius
int windowPosX = 400;			// X Position of Window
int windowPosY = 200;			// Y Position of Window
char cfgNameList[200][200];
int numEg = 0;
int SearchType = 3;
int RouteType = 2;
bool interactive = true;        // Run interactively?
int seed = 111;                 // seed for random number generator
                                    // Could also be used for BFS, etc
double deltaX = 0;              // x-translation of input environment
double deltaY = 0;              // y-translation of input environment
double scale = 1;               // scaling of input environment

bool noPath = true;             // True means there is "No path"
bool moveAway(true);

bool hideBoxBoundary = true;  	// don't draw box boundary
bool hideBox = true;  		// don't draw box

int freeCount = 0;              // Green boxes
int stuckCount = 0;             // Red boxes
int mixCount = 0;               // Yellow boxes
int mixSmallCount = 0;          // Grey boxes

double d_mouseX, d_mouseY;  // clicked mouse position
static MainWindow* mainWin; // for debug

vector<Polygon2d> polygons;

bool showAnim(true);
bool pauseAnim(false);
bool replayAnim(false);
bool showTrace(false);
bool showFilledObstacles(false);
int animationSpeed(99);
int animationSpeedScale(5000);
int animationSpeedScaleBox(100);

// color coding variable ========================================
Color clr_totalFREE(0, 1, 0, 0.8);         // green
Color clr_partialFREE(0.25, 1, 0.25, 0.8); // dark green
Color clr_MIXED(1, 1, 0, 0.8); // yellow
Color clr_STUCK(1, 0, 0, 0.8); // red
Color clr_EPS(0.5, 0.5, 0.5, 1); // grey
Color clr_UNKNOWN(1, 1, 0, 0.5); // white

Color clr_start(1,0,1,0.8);      // purple
Color clr_goal(0.25,0,0.25,0.8); // dark purple
Color clr_robot(0.5,0,0.5,0.8);  // purple

Color clr_path(0.5,0,0,1); // dark red
Color clr_obstacle(0,0,1,1); // blue
Color clr_boundary(1,1,1,1); // black

// External Variables & Routines =================================
//
extern std::vector<int> expansions;
extern int animationSpeed;
extern int animationSpeedScale;
extern int animationSpeedScaleBox;
extern int fileProcessor(string inputfile);

// Forward Declarations ==========================================
//

void run();
void safe_exit(int, const char*);
void parseExampleList();
void parseExampleFile();
void parseConfigFile(Box*);


//find path using simple heuristic:
//use distance to beta as key in PQ, see dijkstraQueue
template<typename Cmp>
bool findPath(Box* a, Box* b, QuadTree* QT, int& ct){

  bool isPath = false;

  a->dist2Source = 0;
  dijkstraQueue<Cmp> dijQ;
  dijQ.push(a);

  dijkstraQueue<Cmp> dijQ2;
  if(SearchType == PriorityQ::BIVORONOI){
    b->dist2Source = 0;
    dijQ2.push(b);
  }

  bool alter = true;

  while(!QT->isConnect(a, b) && !dijQ.empty()){
    Box* current;
    if(SearchType == PriorityQ::BIVORONOI){
      if(dijQ.empty() || dijQ2.empty()){
        break;
      }
      if(alter) current = dijQ.extract();
      else      current = dijQ2.extract();
    }
    else{
      current = dijQ.extract();
    }

    // if current is MIXED, try expand it and push the children that is
    // ACTUALLY neighbors of the source set (set containing alpha) into the dijQ again
    if(current->status == MIXED){
      if(QT->expand(current)){
        ++ct;
        for(int i=0; i<4; ++i){
          // go through neighbors of each child to see if it's in source set
          // if yes, this child go into the dijQ
          bool isNeighborOfSourceSet = false;
          for(int j=0; j<4 && !isNeighborOfSourceSet; ++j){
            BoxIter* iter = new BoxIter(current->pChildren[i], j);
            Box* n = iter->First();
            while(n && n != iter->End()){
              if(n->dist2Source == 0){
                isNeighborOfSourceSet = true;
                break;
              }
              n = iter->Next();
            }
          }
          if(isNeighborOfSourceSet){
            switch(current->pChildren[i]->getBoxStatus()){
              //if it's FREE, also insert to source set
              case FREE:
              case MIXED:
                current->pChildren[i]->inQueue = true;
                current->pChildren[i]->bidirection = current->bidirection;
                if(alter) dijQ.push(current->pChildren[i]);
                else      dijQ2.push(current->pChildren[i]);
              break;
              case STUCK:
                //cw_out << "inside FindPath: STUCK case not treated\n";
              break;
              case UNKNOWN:
                //cw_out << "inside FindPath: UNKNOWN case not treated\n";
              break;
            }
          }
        }
      }
      continue;
    }

    //found path!
    if(current == b && SearchType != PriorityQ::BIVORONOI){
      isPath = true;
      break;
    }

    // if current is not MIXED, then must be FREE
    // go through it's neighbors and add FREE and MIXED ones to dijQ
    // also add FREE ones to source set
    if(current->status == FREE){
      current->dist2Source = 0;
      for(int i=0; i<4; ++i){
        BoxIter* iter = new BoxIter(current, i);
        Box* neighbor = iter->First();
        while(neighbor && neighbor != iter->End()){
          if(!neighbor->inQueue && neighbor->dist2Source == -1 &&
             (neighbor->status == FREE || neighbor->status == MIXED)){
            neighbor->inQueue = true;
            neighbor->bidirection = current->bidirection;
            if(alter) dijQ.push(neighbor);
            else      dijQ2.push(neighbor);
          }
          neighbor = iter->Next();
        }
      }
    }

    if(SearchType == PriorityQ::BIVORONOI) alter = !alter;
  } // end while

  if(SearchType != PriorityQ::BIVORONOI)
    return !dijQ.empty();
  else
    return !dijQ.empty() && !dijQ2.empty();
}

//find path using simple heuristic:
//use distance to beta as key in PQ, see dijkstraQueue
template<typename Cmp>
Box* findVoronoiBox(Box* freeBox, QuadTree* QT){
  freeBox->dist2Source = 0;
  dijkstraQueue<Cmp> dijQ;
  dijQ.push(freeBox);
  while(!dijQ.empty()){
    Box* current = dijQ.extract();
    // if current is MIXED, try expand it and push the children that is
    // ACTUALLY neighbors of the source set (set containing alpha) into the dijQ again
    if(current->status == MIXED){
      if(QT->expand(current)){
        for(int i=0; i<4; ++i){
          // go through neighbors of each child to see if it's in source set
          // if yes, this child go into the dijQ
          bool isNeighborOfSourceSet = false;
          for(int j=0; j<4 && !isNeighborOfSourceSet; ++j){
            BoxIter* iter = new BoxIter(current->pChildren[i], j);
            Box* n = iter->First();
            while(n && n != iter->End()){
              if(n->dist2Source == 0){
                isNeighborOfSourceSet = true;
                break;
              }
              n = iter->Next();
            }
          }
          if(isNeighborOfSourceSet){
            switch(current->pChildren[i]->getBoxStatus()){
              //if it's FREE, also insert to source set
              case FREE:
              case MIXED:
                current->pChildren[i]->inQueue = true;
                dijQ.push(current->pChildren[i]);
              break;
              case STUCK:
                //cw_out << "inside FindPath: STUCK case not treated\n";
              break;
              case UNKNOWN:
                //cw_out << "inside FindPath: UNKNOWN case not treated\n";
              break;
            }
          }
        }
      }
      continue;
    }

    //found path!
    if(current->corners_v.size()+current->walls_v.size() > 1){
      return current;
    }

    // if current is not MIXED, then must be FREE
    // go through it's neighbors and add FREE and MIXED ones to dijQ
    // also add FREE ones to source set
    if(current->status == FREE){
      current->dist2Source = 0;
      for(int i=0; i<4; ++i){
        BoxIter* iter = new BoxIter(current, i);
        Box* neighbor = iter->First();
        while(neighbor && neighbor != iter->End()){
          if(!neighbor->inQueue && neighbor->dist2Source == -1 &&
             (neighbor->status == FREE || neighbor->status == MIXED)){
            neighbor->inQueue = true;
            dijQ.push(neighbor);
          }
          neighbor = iter->Next();
        }
      }
    }
  } // end while
  return freeBox;
}


// MAIN PROGRAM: ========================================
int main(int argc, char* argv[]){
  // We want the current working directory to be .../disc, (.../disc-master)
  // so that it can access necessary files
  // The current working directory will be correctly set as long as the name of the
  // folder is /disc or /disc-master
  workingDir = QDir::currentPath().toStdString();
  bool foundFiles = false;

  // Test if the build directory is disc. If so,
  // the path to the current working directory will
  // include /disc
  unsigned long indexOfDesiredDir = workingDir.rfind("/disc-qt/");
  if(indexOfDesiredDir != std::string::npos) {
    workingDir = workingDir.substr(0, indexOfDesiredDir + 5);

    // Set current working directory to disc
    QDir::setCurrent(workingDir.c_str());

    foundFiles = true;
  }

  // Test if program was downloaded from Github, and is the build directory.
  // Downloading it from Github will result in the folder having the name
  // /disc-master instead of /disc
  if(!foundFiles &&
    (indexOfDesiredDir = workingDir.rfind("/disc-qt/")) != std::string::npos) {
    workingDir = workingDir.substr(0, indexOfDesiredDir + 12);

    // Set current working directory to /disc-qt
    QDir::setCurrent(workingDir.c_str());

    foundFiles = true;
  }

  // Test if a build directory (/build-disc-...) was created. This directory
  // will reside in the same directory as /disc or /disc-master
  if(!foundFiles &&
    (indexOfDesiredDir = workingDir.rfind("/build-disc-qt-")) != std::string::npos) {
    QDir dir(workingDir.substr(0, indexOfDesiredDir).c_str());

    if(dir.exists("disc-qt/disc-qt.pro")) {                  // Test if /disc exists
      workingDir = workingDir.substr(0, indexOfDesiredDir) + "/disc-qt";

      // Set current working directory to disc
      QDir::setCurrent(workingDir.c_str());

      foundFiles = true;
    }
    else if(dir.exists("disc-qt/disc-qt.pro")) {  // Test if /disc-master exists
      workingDir = workingDir.substr(0, indexOfDesiredDir) + "/disc-qt";

      // Set current working directory to disc
      QDir::setCurrent(workingDir.c_str());

      foundFiles = true;
    }
  }

  // Neither /disc nor /disc-master could be found
  if (!foundFiles) {
    std::cerr << std::endl << "!! WARNING !!\n"
    << "The program may not work correctly or at all because the folder "
    "containing the program's files cannot be found.\n"
    "Make sure that the program is inside of a folder named \"disc\".\n"
    "If it is not, rename the folder to \"disc\" before running again.\n";
  }


  if (argc > 1) interactive = !atoi(argv[1]);	// nature of run
  // (origionally set 0 = true, >0 = false)
  if (argc > 2) alpha.setX(atof(argv[2]));		// start x
  if (argc > 3) alpha.setY(atof(argv[3]));		// start y
  if (argc > 4) beta.setX(atof(argv[4]));		// goal x
  if (argc > 5) beta.setY(atof(argv[5]));		// goal y
  if (argc > 6) epsilon  = atof(argv[6]);		// epsilon (resolution)
  if (argc > 7) R0       = atof(argv[7]);		// robot radius
  if (argc > 8) fileName = argv[8];           // Input file name
  if (argc > 9) boxWidth = atof(argv[9]);		// boxWidth
  if (argc > 10) boxHeight = atof(argv[10]);	// boxHeight
  if (argc > 11) windowPosX = atoi(argv[11]);	// window X pos
  if (argc > 12) windowPosY = atoi(argv[12]);	// window Y pos
  if (argc > 13) SearchType   = atoi(argv[13]);	// PriorityQ Type (random or no)
  if (argc > 14) seed    = atoi(argv[14]);	// for random number generator
  if (argc > 15) inputDir  = argv[15];		// path for input files
  if (argc > 16) deltaX  = atof(argv[16]);	// x-translation of input file
  if (argc > 17) deltaY  = atof(argv[17]);	// y-translation of input file
  if (argc > 18) scale   = atof(argv[18]);		// scaling of input file

  // cout << "before interactive, Qtype= " << SearchType << "\n";

//  if (!interactive) {
//    cout << "Non Interactive Run of Disc Robot\n";
//    run();

//    if (noPath)
//      cout << "No Path Found!\n";
//    else
//      cout << "Path was Found!\n";
//    return 0;
//  }


  // Otherwise, set up Qt and display
  //============================================================//
  //============      Set Up Qt Control Panel       ============//
  //============================================================//

  // Allow creation of Qt GUI
  QApplication app(argc, argv);

  // Create gui
  parseExampleList();
  parseExampleFile();
  mainWin = new MainWindow();

  //============================================================//
  //==============      Proform Initial Run       ==============//
  //============================================================//
  run();
  cw_out << "[ Ran to Completion ]\n";

  // Control Window can't be shown until
  //   run is called. Otherwise, necessary
  //   values will be null
  mainWin->show();

  // Leave execution of up to user and
  //   returns when window is closed
  return app.exec();
}

void genEmptyTree(){
  Box* root = new Box(Point2d(boxWidth/2, boxHeight/2), boxWidth, boxHeight);
  Box::r0 = R0;

  // in testing phase, need to comment out these allLeaf and expension
  Box::pAllLeaf = &allLeaf;
  if(!allLeaf.empty() && allLeaf.size() != 0){
    for(vector<Box*>::iterator it = allLeaf.begin(); it != allLeaf.end(); ++it){
      delete *it;
    }
  }
  allLeaf.clear();
  allLeaf.push_back(root);

  parseConfigFile(root);

  root->checkVoronoiFeatureSet();
  root->findCleanrance3(root->mB);

  root->checkFeatureSet();

  if(QT) delete(QT);
  QT = new QuadTree(root, epsilon, SearchType);
}

void run(){
  Timer t;
  // start timer
  t.start();

  genEmptyTree();

  noPath = false;	// Confusing use of "noPath"
  int ct = 0;

  if (SearchType == PriorityQ::RANDOM || SearchType == PriorityQ::BFS){
    // Expand box containing alpha until it is free.
    // If alpha is not, no path = true
    boxA = QT->getBox(alpha);
    while (boxA && !boxA->isFree()){
      if (!QT->expand(boxA)){
        noPath = true; // Confusing use of "noPath"
        break;
      }
      ct++;
      boxA = QT->getBox(boxA, alpha);
    }

    // Expand box containing beta until it is free.
    // If beta is not, no path = true
    boxB = QT->getBox(beta);
    while (!noPath && boxB && !boxB->isFree()){
      if (!QT->expand(boxB)){
        noPath = true;
        break;
      }
      ct++;
      boxB = QT->getBox(boxB, beta);
    }

    while(!noPath && !QT->isConnect(boxA, boxB)){
      if(!QT->expand()){ // should ct be passed to expand?
        noPath = true;
      }
      ct++;
    }
  }
  else if(SearchType == PriorityQ::GREEDY){
    boxA = QT->getBox(alpha);
    while (boxA && !boxA->isFree()){
      if(!QT->expand(boxA)){
        noPath = true;
        break;
      }
      boxA = QT->getBox(boxA, alpha);
    }

    boxB = QT->getBox(beta);
    while (!noPath && boxB && !boxB->isFree()){
      if(!QT->expand(boxB)){
        noPath = true;
        break;
      }
      boxB = QT->getBox(boxB, beta);
    }

    if(!noPath)
      noPath = !findPath<DistCmp>(boxA, boxB, QT, ct);
  }
  else if(SearchType == PriorityQ::VORONOI){
    moveAway = true;
    boxA = QT->getBox(alpha);
    boxA->bidirection = 1;
    while(boxA && !boxA->isFree()){
      if(!QT->expand(boxA)){
        noPath = true;
        break;
      }
      boxA = QT->getBox(boxA, alpha);
    }
    if(!noPath && boxA->corners_v.size()+boxA->walls_v.size() < 2)
      boxA = findVoronoiBox<DistCmpSpecial>(boxA, QT);

    boxB = QT->getBox(beta);
    boxB->bidirection = -1;
    while(!noPath && boxB && !boxB->isFree()){
      if(!QT->expand(boxB)){
        noPath = true;
        break;
      }
      boxB = QT->getBox(boxB, beta);
    }
    if(!noPath && boxB->corners_v.size()+boxB->walls_v.size() < 2)
      boxB = findVoronoiBox<DistCmpSpecial>(boxB, QT);

    moveAway = false;
    if(!noPath)
      noPath = !findPath<VorCmp>(boxA, boxB, QT, ct);
  }
  else if(SearchType == PriorityQ::BIVORONOI){
    moveAway = true;
    boxA = QT->getBox(alpha);
    boxA->bidirection = 1;
    while(boxA && !boxA->isFree()){
      if(!QT->expand(boxA)){
        noPath = true;
        break;
      }
      boxA = QT->getBox(boxA, alpha);
    }
    boxA->bidirection = 1;
    if(!noPath && boxA->corners_v.size()+boxA->walls_v.size() < 2)
      boxA = findVoronoiBox<DistCmpSpecial>(boxA, QT);
    boxA->bidirection = 1;

    boxB = QT->getBox(beta);
    boxB->bidirection = -1;
    while(!noPath && boxB && !boxB->isFree()){
      if(!QT->expand(boxB)){
        noPath = true;
        break;
      }
      boxB = QT->getBox(boxB, beta);
    }
    boxB->bidirection = -1;
    if(!noPath && boxB->corners_v.size()+boxB->walls_v.size() < 2)
      boxB = findVoronoiBox<DistCmpSpecial>(boxB, QT);
    boxB->bidirection = -1;

    moveAway = false;
    if(!noPath)
      noPath = !findPath<BiVorCmp>(boxA, boxB, QT, ct);
  }

  // stop timer
  t.stop();
  if(!noPath){
    if(RouteType == Route::R_BFS)
      PATH = Path::bfsShortestPath(boxA, boxB);
    else if(RouteType == Route::R_SAFE)
      PATH = Path::safeShortestPath(boxA, boxB);
    else if(RouteType == Route::R_ASTAR)
      PATH = Path::dijkstraShortestPath(boxA, boxB);
    else{
      RouteType = Route::R_ASTAR;
      PATH = Path::dijkstraShortestPath(boxA, boxB);
    }
  }

  // print the elapsed time in millisec
  cw_out << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
  cw_out << ">>\n";
  if (!noPath)
    cw_out << ">>     Path found !\n";
  else
    cw_out << ">>     No Path !\n";
  cw_out << ">>\n";
  cw_out << ">>     Time used: " << t.getElapsedTimeInMilliSec() << " ms\n";
  cw_out << ">>     Expansion steps: " << ct << "\n";
  cw_out << ">>\n";
  cw_out << ">>     Search Strategy: ";

  if(SearchType == PriorityQ::RANDOM)
    cw_out << "Random Strategy\n";
  if(SearchType == PriorityQ::BFS)
    cw_out << "BFS Strategy\n";
  if(SearchType == PriorityQ::GREEDY)
    cw_out << "Greedy Strategy\n";
  if(SearchType == PriorityQ::VORONOI)
    cw_out << "Voronoi Strategy\n";
  if(SearchType == PriorityQ::BIVORONOI)
    cw_out << "Bidirectional Voronoi Strategy\n";

  cw_out << ">>\n";
  cw_out << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
  //cw_out << "Expanded " << ct << " times\n";
  cw_out << "total Free boxes: " << freeCount << "\n";
  cw_out << "total Stuck boxes: " << stuckCount << "\n";
  cw_out << "total Mixed boxes smaller than epsilon: " << mixSmallCount << "\n";
  cw_out << "total Mixed boxes bigger than epsilon: " << mixCount - ct - mixSmallCount << "\n";
  freeCount = stuckCount = mixCount = mixSmallCount = 0;
}

/* ********************************************************************** */
// skip blanks, tabs, line breaks and comment lines,
// 	leaving us at the beginning of a token (or EOF)
// 	(This code is taken from core2/src/CoreIo.cpp)
int skip_comment_line (std::ifstream & in) {
  int c;

  do {
    c = in.get();
    while ( c == '#' ) {
      do {// ignore the rest of this line
        c = in.get();
      } while ( c != '\n' );
      c = in.get(); // now, reach the beginning of the next line
    }
  } while (c == ' ' || c == '\t' || c == '\n');	//ignore white spaces and newlines

  if (c == EOF)
  std::cerr << "unexpected end of file.\n";

  in.putback(c);  // this is non-white and non-comment char!
  return c;
} // skip_comment_line

// skips '\' followed by '\n'
// 	NOTE: this assumes a very special file format (e.g., our BigInt File format)
// 	in which the only legitimate appearance of '\' is when it is followed
// 	by '\n' immediately!  
int skip_backslash_new_line (std::istream & in) {
  int c = in.get();

  while (c == '\\') {
    c = in.get();

    if (c == '\n')
      c = in.get();
    else // assuming the very special file format noted above!
      std::cerr << "continuation line \\ must be immediately followed by new line.\n";
  } // while
  return c;
} // skip_backslash_new_line

/* ********************************************************************** */

char cfgPath[200], sptr[200], tmp[200];
void parseExampleList(){

  //    8/24/2016: Tom
  //               new way to parse files in a folder
  string cfgDir = workingDir + "/" + inputDir;
  QDir tmpDir(cfgDir.c_str());
  QStringList fileList = tmpDir.entryList();
  while(!fileList.empty()){
    QString fileName_l = fileList.front();
    strcpy(sptr, fileName_l.toStdString().c_str());
    int len = strlen(sptr);
    if(len > 4 && sptr[len-1] == 'g' && sptr[len-2] == 'f' && sptr[len-3] == 'c' && sptr[len-4] == '.'){
      strcpy(cfgNameList[numEg], sptr);
      ++numEg;
    }
    fileList.pop_front();
  }
}

void parseExampleFile() {
  sprintf(cfgPath, "%s/%s", inputDir.c_str(), cfgName.c_str());
  FILE *fptr = fopen(cfgPath, "r");
  if (fptr == NULL) return ;

  while(fgets(tmp, 200, fptr) != NULL){
    char *sptr = strtok(tmp, "=: \t");

    // comments
    if(strcmp(sptr, "#") == 0){
      continue;
    }

    if(strcmp(sptr, "interactive") == 0){
      sptr = strtok(NULL, "=: \t");
      interactive = atoi(sptr);
    }

    // start configuration
    if(strcmp(sptr, "startX") == 0){
      sptr = strtok(NULL, "=: \t");
      alpha.setX(atof(sptr));
    }
    if(strcmp(sptr, "startY") == 0){
      sptr = strtok(NULL, "=: \t");
      alpha.setY(atof(sptr));
    }

    // goal configuration
    if(strcmp(sptr, "goalX") == 0){
      sptr = strtok(NULL, "=: \t");
      beta.setX(atof(sptr));
    }
    if(strcmp(sptr, "goalY") == 0){
      sptr = strtok(NULL, "=: \t");
      beta.setY(atof(sptr));
    }

    if(strcmp(sptr, "epsilon") == 0){
      sptr = strtok(NULL, "=: \t");
      epsilon = atof(sptr);
    }

    if(strcmp(sptr, "R") == 0){
      sptr = strtok(NULL, "=: \t");
      R0 = atof(sptr);
    }

    if(strcmp(sptr, "inputDir") == 0){
      sptr = strtok(NULL, "=: #\t");
      inputDir = sptr;
    }
    if(strcmp(sptr, "fileName") == 0){
      sptr = strtok(NULL, "=: #\t");
      fileName = sptr;
    }

    // box width and height
    if(strcmp(sptr, "boxWidth") == 0){
      sptr = strtok(NULL, "=: \t");
      boxWidth = atof(sptr);
    }
    if (strcmp(sptr, "boxHeight") == 0){
      sptr = strtok(NULL, "=: \t");
      boxHeight = atof(sptr);
    }

    // windows position
    if(strcmp(sptr, "windowPosX") == 0){
      sptr = strtok(NULL, "=: \t");
      windowPosX = atoi(sptr);
    }
    if(strcmp(sptr, "windowPosY") == 0){
      sptr = strtok(NULL, "=: \t");
      windowPosY = atoi(sptr);
    }

    if(strcmp(sptr, "Qtype") == 0){
      sptr = strtok(NULL, "=: \t");
      SearchType = atoi(sptr);
    }

    if(strcmp(sptr, "seed") == 0){
      sptr = strtok(NULL, "=: \t");
      seed = atoi(sptr);
    }

    if(strcmp(sptr, "xtrans") == 0){
      sptr = strtok(NULL, "=: \t");
      deltaX = atof(sptr);
    }

    if (strcmp(sptr, "ytrans") == 0){
      sptr = strtok(NULL, "=: \t");
      deltaY = atof(sptr);
    }

    if(strcmp(sptr, "scale") == 0){
      sptr = strtok(NULL, "=: \t");
      scale = atof(sptr);
    }

    if(strcmp(sptr, "animationSpeed") == 0){
      sptr = strtok(NULL, "=: \t");
      animationSpeed = atoi(sptr);
    }
    if(strcmp(sptr, "animationSpeedScale") == 0){
      sptr = strtok(NULL, "=: \t");
      animationSpeedScale = atoi(sptr);
    }
    if(strcmp(sptr, "animationSpeedScaleBox") == 0){
      sptr = strtok(NULL, "=: \t");
      animationSpeedScaleBox = atoi(sptr);
    }
  }
}

void parseConfigFile(Box* b){
  polygons.clear();

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
  cw_out<< "nPt = " << nPt << "\n";

  //skip_comment_line ( ifs );	// again, clear white space
  vector<double> pts(nPt*2);
  for (int i = 0; i < nPt; ++i){
    ifs >> pts[i*2] >> pts[i*2+1];
  }

  //skip_comment_line ( ifs );	// again, clear white space
  ifs >> nPolygons;
  //skip_comment_line ( ifs );	// again, clear white space
  cw_out << "nPolygons = " << nPolygons << "\n";
  string temp;
  getline(ifs, temp);

  for(int i=0; i<nPolygons; ++i){
    string s;
    getline(ifs, s);
    stringstream ss(s);
    vector<Corner*> ptVec;
    set<int> ptSet;
    while(ss){
      int pt;
      /// TODO:
      ss >> pt;
      pt -= 1; //1 based array
      if(ptSet.find(pt) == ptSet.end()){
        ptVec.push_back(new Corner(Point2d(pts[pt*2]*scale+deltaX, pts[pt*2+1]*scale+deltaY)));

        b->addCorner(ptVec.back());
        ptSet.insert(pt);
        if(ptVec.size() > 1){
          Wall* w = new Wall(ptVec[ptVec.size()-2], ptVec[ptVec.size()-1]);
          b->addWall(w);
        }
      }
      //new pt already appeared, a loop is formed. should only happen on first and last pt
      else{
        if(ptVec.size() > 1){
          Wall* w = new Wall(ptVec[ptVec.size()-1], ptVec[0]);
          b->addWall(w);
          break;
        }
      }
    }

    ptVec.push_back(ptVec[0]);
    std::vector<Point2d> tmpPoints;
    for(unsigned i=0;i<ptVec.size();++i)
      tmpPoints.push_back(ptVec[i]->p);
    Polygon2d tmpPolygon;
    tmpPolygon = Polygon2d(tmpPoints);
    polygons.push_back(tmpPolygon);
  }
  ifs.close();
  std::remove("output-tmp.txt");
}
