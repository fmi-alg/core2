/* **************************************
   File: disc.cpp

   Description: 
	This is the entry point for the running the SSS algorithm
	for a disc robot amidst a collection of polygonal obstacles.

	To run, call with these positional arguments:

	> ./disc 	[interactive = 1] \
			[alpha-x = 10] [alpha-y = 360] \
			[beta-x = 500] [beta-y = 20] \
			[epsilon = 1] \
			[R0 = 30] \
			[fileName = input2.txt] \
			[boxWidth = 512] [boxHeight = 512] \
			[windoxPosX = 400] [windowPosY = 200] \
			[Qtype = 0] [seed = 111] [inputDir = inputs] \
			[deltaX = 0] [deltaY = 0] [scale = 1] \

	where 
		interactive 	 	is nature of run
            (originally set as 0 = interactive, >0 = non-interactive)
		alpha			is start configuration
		beta			is goal configuration
		epsilon			is resolution parameter
		R0			    is robot radius
		fileName		is input file describing the environment
		box Width/Height	is initial box dimensions
		windowPos		is position of window
		Qtype			is type of the priority queue
						(sequential=0, random=1)
		seed			is seed for random number generator
		inputDir		is directory for input files
		deltaX, deltaY, scale	is translation and scaling of input environment

	NOTE: see several examples of running this program in the Makefile.
	
	Format of input environment: see README FILE

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang
 
   June-July, 2015:
        Bryant Curto (CS Department Summer Research Fellowship)
          > Ported disc program to Qt (to avoid Apple's depreciation
            of GLUT/GLUI and OpenGL)
          > Implemented functions exclusively from OpenGL Core Contex
 
        Issues:
          > Memory leaks in several classes including this file,
            excluding:
                -ControlWindow.{h, cpp}
                -Display.{h, cpp}
                -shader.{h, cpp}
                -ShaderException.{h, cpp}
 

   Since Core Library  Version 2.1
   $Id: disc.cpp,v 1.1 2012/10/26 03:58:52 cheeyap Exp cheeyap $
 ************************************** */
#define cw_out (*controlWin)

// Custom
#include "QuadTree.h"
#include "PriorityQueue.h"
#include "Graph.h"
#include "Timer.h"
#include "ControlWindow.h"

// QT
#include <QApplication>
#include <QSurfaceFormat>

// Standard Library
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>


// GLOBAL VARIABLES ========================================
//
    int debug_counter = 0;

    QuadTree* QT = nullptr;

    double alpha[2] = {10, 360};	// start configuration
    double beta[2] = {500, 20};		// goal configuration
    double epsilon = 5;             // resolution parameter
    Box* boxA = nullptr;            // start box (containing alpha)
    Box* boxB = nullptr;            // goal box (containing beta)
    double boxWidth = 512;			// Initial box width
    double boxHeight = 512;			// Initial box height
    double R0 = 30;                 // Robot radius
    int windowPosX = 400;			// X Position of Window
    int windowPosY = 200;			// Y Position of Window
    string egName("SoCG_disc_eg1");
    string fileName("input2.txt"); 	// Input file name
    string inputDir("inputs"); 		// Path for input files
    int QType = 1;                  // The Priority Queue can be
                                        // sequential (0) or random (1)
    bool interactive = true;        // Run interactively?
    int seed = 111;                 // seed for random number generator
                                        // Could also be used for BFS, etc
    double deltaX = 0;              // x-translation of input environment
    double deltaY = 0;              // y-translation of input environment
    double scale = 1;               // scaling of input environment

    bool noPath = true;             // True means there is "No path"

    bool hideBoxBoundary = false;   // Black outline around boxes

    int freeCount = 0;              // Green boxes
    int stuckCount = 0;             // Red boxes
    int mixCount = 0;               // Yellow boxes
    int mixSmallCount = 0;          // Grey boxes




// Qt/OpenGL Variables ===========================================
//
    ControlWindow* controlWin; //this used to be static, John removed for debugging

// External Variables & Routines =================================
//
    extern int fileProcessor(string inputfile);

// Forward Declarations ==========================================
//

void run();
void safe_exit(int, const char*);
void parseExampleFile();
void parseConfigFile(Box*);




void showAgain(std::string str);
void showAgain(std::string str){
    if(str.size()!=0){
        fileName=str;
        controlWin->updateDisplay();
    }
    controlWin->show();

}



//find path using simple heuristic:
//use distance to beta as key in PQ, see dijkstraQueue
bool findPath(Box* a, Box* b, QuadTree* QT, int& ct)
{
	bool isPath = false;
	vector<Box*> toReset;
	a->dist2Source = 0;
	dijkstraQueue dijQ;
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
			if (QT->expand(current))
			{
				++ct;
				for (int i = 0; i < 4; ++i)
				{
					// go through neighbors of each child to see if it's in source set
					// if yes, this child go into the dijQ
					bool isNeighborOfSourceSet = false;
					for (int j = 0; j < 4 && !isNeighborOfSourceSet; ++j)
					{
                        BoxIter* iter = new BoxIter(current->pChildren[i], j);
						Box* n = iter->First();
						while (n && n != iter->End())
						{
							if (n->dist2Source == 0)
							{
								isNeighborOfSourceSet = true;
								break;
							}
							n = iter->Next();
						}							
					}					
					if (isNeighborOfSourceSet)
					{

						switch (current->pChildren[i]->getStatus())
						{
							//if it's FREE, also insert to source set
							case Box::FREE:
								current->pChildren[i]->dist2Source = 0;
								dijQ.push(current->pChildren[i]);
								toReset.push_back(current->pChildren[i]);
								break;
							case Box::MIXED:
								dijQ.push(current->pChildren[i]);
								toReset.push_back(current->pChildren[i]);
								break;
							case Box::STUCK:
								cw_out << "inside FindPath: STUCK case not treated\n";
								break;
							case Box::UNKNOWN:
                                cw_out << "inside FindPath: UNKNOWN case not treated\n";
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
		for (int i = 0; i < 4; ++i)
		{
			BoxIter* iter = new BoxIter(current, i);
			Box* neighbor = iter->First();
			while (neighbor && neighbor != iter->End())
			{
				if (!neighbor->visited && neighbor->dist2Source == -1 && (neighbor->status == Box::FREE || neighbor->status == Box::MIXED))
				{					
					if (neighbor->status == Box::FREE)
					{
						neighbor->dist2Source = 0;
					}						
					dijQ.push(neighbor);	
					toReset.push_back(neighbor);
				}
				neighbor = iter->Next();
			}
		}
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


// MAIN PROGRAM: ========================================
int main(int argc, char* argv[])
{
    // We want the current working directory to be .../disc, (.../disc-master)
    // so that it can access necessary files
    // The current working directory will be correctly set as long as the name of the
    // folder is /disc or /disc-master
    std::string workingDir = QDir::currentPath().toStdString();
    bool foundFiles = false;

    // Test if the build directory is disc. If so,
    // the path to the current working directory will
    // include /disc
    int indexOfDesiredDir = workingDir.rfind("/disc/");
    if (indexOfDesiredDir != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 5);
        
        // Set current working directory to disc
        QDir::setCurrent(workingDir.c_str());
        
        foundFiles = true;
    }
    
    // Test if program was downloaded from Github, and is the build directory.
    // Downloading it from Github will result in the folder having the name
    // /disc-master instead of /disc
    if (!foundFiles &&
        (indexOfDesiredDir = workingDir.rfind("/disc-master/")) != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 12);
        
        // Set current working directory to /disc-master
        QDir::setCurrent(workingDir.c_str());
        
        foundFiles = true;
    }
    
    // Test if a build directory (/build-disc-...) was created. This directory
    // will reside in the same directory as /disc or /disc-master
    if (!foundFiles &&
        (indexOfDesiredDir = workingDir.rfind("/build-disc-")) != std::string::npos) {
        QDir dir(workingDir.substr(0, indexOfDesiredDir).c_str());
        
        if (dir.exists("disc/disc.pro")) {                  // Test if /disc exists
            workingDir = workingDir.substr(0, indexOfDesiredDir) + "/disc";
            
            // Set current working directory to disc
            QDir::setCurrent(workingDir.c_str());
            
            foundFiles = true;
        } else if (dir.exists("disc-master/disc.pro")) {  // Test if /disc-master exists
            workingDir = workingDir.substr(0, indexOfDesiredDir) + "/disc-master";
            
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
    if (argc > 2) alpha[0] = atof(argv[2]);		// start x
	if (argc > 3) alpha[1] = atof(argv[3]);		// start y
	if (argc > 4) beta[0]  = atof(argv[4]);		// goal x
	if (argc > 5) beta[1]  = atof(argv[5]);		// goal y
	if (argc > 6) epsilon  = atof(argv[6]);		// epsilon (resolution)
	if (argc > 7) R0       = atof(argv[7]);		// robot radius
	if (argc > 8) fileName = argv[8];           // Input file name
	if (argc > 9) boxWidth = atof(argv[9]);		// boxWidth
	if (argc > 10) boxHeight = atof(argv[10]);	// boxHeight
	if (argc > 11) windowPosX = atoi(argv[11]);	// window X pos
	if (argc > 12) windowPosY = atoi(argv[12]);	// window Y pos
	if (argc > 13) QType   = atoi(argv[13]);	// PriorityQ Type (random or no)
	if (argc > 14) seed    = atoi(argv[14]);	// for random number generator
	if (argc > 15) inputDir  = argv[15];		// path for input files
	if (argc > 16) deltaX  = atof(argv[16]);	// x-translation of input file
	if (argc > 17) deltaY  = atof(argv[17]);	// y-translation of input file
	if (argc > 18) scale   = atof(argv[18]);		// scaling of input file

    // cout << "before interactive, Qtype= " << QType << "\n";
    
	if (!interactive) {
        cout << "Non Interactive Run of Disc Robot\n";
        run();
        
	    if (noPath)
	    	cout << "No Path Found!\n";
	    else
	    	cout << "Path was Found!\n";
	    return 0;
    }
    
     
	// Otherwise, set up Qt and display
    //============================================================//
    //============      Set Up Qt Control Panel       ============//
    //============================================================//
    
    // Set up OpenGL version and profile
    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    
    // Allow creation of Qt GUI
    QApplication app(argc, argv);
    
    // Create gui
    controlWin = new ControlWindow();
    
    // Set functions to be called when
    //   run and exit are pressed
    controlWin->setRunCallback(run);
    controlWin->setExitCallback(safe_exit);
    
    //============================================================//
    //==============      Proform Initial Run       ==============//
    //============================================================//
    parseExampleFile();
	run();
    cw_out << "[ Ran to Completion ]\n";
    
    // Control Window can't be shown until
    //   run is called. Otherwise, necessary
    //   values will be null
    controlWin->show();
        
    // Leave execution of up to user and
    //   returns when window is closed
    return app.exec();
}

void genEmptyTree()
{
	Box* root = new Box(boxWidth/2, boxHeight/2,
                        boxWidth, boxHeight);
	Box::r0 = R0;

	parseConfigFile(root);
	root->updateStatus();

	if (QT) {
		delete(QT);
	}
    QT = new QuadTree(root, epsilon, QType, seed);

    cw_out << "inside genEmpty:  Qtype= " << QType << "\n";
}

void run()
{
    cw_out << "inside run:  Qtype= " << QType << "\n";

	Timer t;
	// start timer
	t.start();

	genEmptyTree();

	noPath = false;	// Confusing use of "noPath"
    int ct = 0;

	if (QType == 0 || QType == 1)
	{
        // Expand box containing alpha until it is free.
        // If alpha is not, no path = true
		boxA = QT->getBox(alpha[0], alpha[1]);
		while (boxA && !boxA->isFree())
		{
			
			if (!QT->expand(boxA))
			{
				noPath = true; // Confusing use of "noPath"
				break;
			}
			ct++;
			boxA = QT->getBox(boxA, alpha[0], alpha[1]);
		}

        // Expand box containing beta until it is free.
        // If beta is not, no path = true
        boxB = QT->getBox(beta[0], beta[1]);
		while (!noPath && boxB && !boxB->isFree())
		{
			
			if (!QT->expand(boxB))
			{
				noPath = true;
				break;
			}
			ct++;
			boxB = QT->getBox(boxB, beta[0], beta[1]);
		}
		
		while(!noPath && !QT->isConnect(boxA, boxB))
		{
			
			if (!QT->expand()) // should ct be passed to expand?
			{
				noPath = true;
			}
			ct++;
		}
	} 
	else if(QType == 2)
	{
		boxA = QT->getBox(alpha[0], alpha[1]);
		while (boxA && !boxA->isFree())
		{
			if (!QT->expand(boxA))
			{
				noPath = true;
				break;
			}
			boxA = QT->getBox(boxA, alpha[0], alpha[1]);
		}

		boxB = QT->getBox(beta[0], beta[1]);
		while (!noPath && boxB && !boxB->isFree())
		{
			if (!QT->expand(boxB))
			{
				noPath = true;
				break;
			}
			boxB = QT->getBox(boxB, beta[0], beta[1]);
		}

		noPath = !findPath(boxA, boxB, QT, ct);
	}	

	// stop timer
	t.stop();
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

    if (QType == 0)
        cw_out << "Random Strategy\n";
    if (QType == 1)
        cw_out << "BFS Strategy\n";
    if (QType == 2)
        cw_out << "Greedy Strategy\n";

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
}//skip_comment_line

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
	  }//while
	  return c;
}//skip_backslash_new_line

/* ********************************************************************** */

char egPath[200], tmp[200];
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

        // goal configuration
        if (strcmp(sptr, "goalX") == 0) {
            sptr = strtok(NULL, "=: \t");
            beta[0] = atof(sptr);
        }
        if (strcmp(sptr, "goalY") == 0) {
            sptr = strtok(NULL, "=: \t");
            beta[1] = atof(sptr);
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
    }
}

void parseConfigFile(Box* b)
{
    using namespace std;
    
    stringstream ss;
    ss << inputDir << "/" << fileName;	// create full file name
    string s = ss.str();
    cw_out << "input file name = " << s << "\n";
    
    if(fileProcessor(s)==2){// this will clean the input and put in
                                // output-tmp.txt
        
        cw_out<<"!!!!!!!!!!!!!!\nERROR!!\nUnable to open that file. Please check the file name\n";
        cw_out<<"Using input2.txt\n!!!!!!!!!!!!\n\n";
        s=inputDir+"/input2.txt";
        fileName="input2.txt";
        controlWin->defaultFile();

    }

    
    ifstream ifs("output-tmp.txt");
    if (!ifs)
    {
        cerr << "cannot open input file\n";
        exit(1);
    }
    
    // First, get to the beginning of the first token:
    skip_comment_line ( ifs );
    
    int nPt, nPolygons;	// previously, nPolygons was misnamed as nFeatures
    ifs >> nPt;
    cw_out<< "nPt = " << nPt << "\n";
    
    //skip_comment_line ( ifs );	// again, clear white space
    vector<double> pts(nPt*2);
    for (int i = 0; i < nPt; ++i)
    {
        ifs >> pts[i*2] >> pts[i*2+1];
    }
    
    //skip_comment_line ( ifs );	// again, clear white space
    ifs >> nPolygons;
    //skip_comment_line ( ifs );	// again, clear white space
    cw_out << "nPolygons = " << nPolygons << "\n";
    string temp;
    getline(ifs, temp);

    for (int i = 0; i < nPolygons; ++i)
    {
        string s;
        getline(ifs, s);
        stringstream ss(s);
        vector<Corner*> ptVec;
        set<int> ptSet;
        while (ss)
        {
            int pt;
            /// TODO:
            ss >> pt;
            pt -= 1; //1 based array
            if (ptSet.find(pt) == ptSet.end())
            {
                ptVec.push_back(new Corner(pts[pt*2]*scale+deltaX,
                                           pts[pt*2+1]*scale+deltaY));
                
                b->addCorner(ptVec.back());
                ptSet.insert(pt);
                if (ptVec.size() > 1)
                {
                    Wall* w = new Wall(ptVec[ptVec.size()-2], ptVec[ptVec.size()-1]);
                    b->addWall(w);
                }
            }
            //new pt already appeared, a loop is formed. should only happen on first and last pt
            else
            {
                if (ptVec.size() > 1)
                {
                    Wall* w = new Wall(ptVec[ptVec.size()-1], ptVec[0]);
                    b->addWall(w);
                    break;
                }	
            }
        }
    }
    ifs.close();
}

/*
 * Safely exit Control Window
 * Print error code and discription if one occurs.
 * close and delete 'controlWin'.
 */
void safe_exit(int error, const char* description) {
    if (error) {
        cerr << "\n\n\n"
                "**************************************************\n"
                "WARNING!!\n"
                "**************************************************\n\n"

                "Exit status code: " << error << "\n"
                "Error: " << description << "\n";
    }

    controlWin->close();
    delete controlWin;
}




