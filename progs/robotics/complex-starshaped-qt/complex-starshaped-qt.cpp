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
#include "MainWindow.h"
static MainWindow *window;

#define mw_out (*window)
#include "complex.h"
#include "complex-starshaped-qt.h"


int main(int argc, char* argv[]) {

    if (argc >1) interactive = atoi(argv[1]);//interactive or no
    if (argc >2) cfgName = argv[2];		//config file
    if (argc >3) fileName = argv[3];		//enviroment file
    if (argc >4) inputDir = argv[4];		//Path for input files
    if (argc >5) robotDir = argv[5];		//Path for robot files
    if (argc >6) robotName = argv[6];	//robot
    if (argc >7) alpha[0] = atof(argv[7]);	// startx
    if (argc >8) alpha[1] = atof(argv[8]);	// starty
    if (argc >9) alpha[2] = atof(argv[9]);	// startTheta
    if (argc >10) beta[0] = atof(argv[10]);	// goalx
    if (argc >11) beta[1] = atof(argv[11]);	// goaly
    if (argc >12) beta[2] = atof(argv[12]);	// goalTheta
    if (argc >13) epsilon = atof(argv[13]);	// resolution parameter
    if (argc >14) R0 	= atof(argv[14]);	// robot radius
    if (argc > 15) boxWidth = atof(argv[15]);		// boxWidth
    if (argc > 16) boxHeight = atof(argv[16]);	// boxHeight
    if (argc > 17) windowPosX = atoi(argv[17]);	// window X pos
    if (argc > 18) windowPosY = atoi(argv[18]);	// window Y pos
    if (argc > 19) QType   = atoi(argv[19]); // PriorityQ Type(random or no)
    if (argc > 20) seed   = atoi(argv[20]);	 // for random number generator
    if (argc > 21) deltaX  = atof(argv[21]); // x-translation of input file
    if (argc > 22) deltaY  = atof(argv[22]); // y-translation of input file
    if (argc > 23) scale  = atof(argv[23]);		// scaling of input file
    if (argc > 24 && argv[24][0]!='@')verboseOption  = atoi(argv[24]);		// verbosity of output


    bool foundFiles = false;
    workingDir = QDir::currentPath().toStdString();

    // Test if the build directory is complex-starshaped-qt. If so,
    // the path to the current working directory will
    // include /complex-starshaped-qt
    unsigned long indexOfDesiredDir = workingDir.rfind("/complex-starshaped-qt/");
    if (indexOfDesiredDir != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 5);

        // Set current working directory to complex-starshaped-qt
        QDir::setCurrent(workingDir.c_str());
        foundFiles = true;
    }

    // Test if program was downloaded from Github, and is the build directory.
    // Downloading it from Github will result in the folder having the name
    // /2-links-master instead of /complex-starshaped-qt
    if (!foundFiles &&
        (indexOfDesiredDir = workingDir.rfind("/complex-starshaped-qt/")) != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 12);

        // Set current working directory to /complex-starshaped-qt-master
        QDir::setCurrent(workingDir.c_str());

        foundFiles = true;
    }

    // Test if a build directory (/build-complex-starshaped-qt-...) was created. This directory
    // will reside in the same directory as /complex-starshaped-qt
    if (!foundFiles &&
        (indexOfDesiredDir = workingDir.rfind("/build-complex-starshaped-qt")) != std::string::npos) {
        QDir dir(workingDir.substr(0, indexOfDesiredDir).c_str());

        if (dir.exists("complex-starshaped-qt/complex-starshaped-qt.pro")) {                  // Test if /complex-starshaped-qt exists
            workingDir = workingDir.substr(0, indexOfDesiredDir) + "/complex-starshaped-qt";

            // Set current working directory to complex-starshaped-qt
            QDir::setCurrent(workingDir.c_str());

            foundFiles = true;
        }
    }

    // /complex-starshaped-qt could not be found
    if (!foundFiles) {
        std::cerr << std::endl << "!! WARNING !!\n"
        << "The program may not work correctly or at all because the folder "
        "containing the program's files cannot be found.\n"
        "Make sure that the program is inside of a folder named \"complex-starshaped-qt\".\n";
    }

    if (interactive > 0) {	// non-interactive
        // do something...
        cout << "Non Interactive Run of Triangle Robot" ;
    }

    QApplication app(argc, argv);
    //fptr = fopen("debug.txt", "w");
    //if(fptr == NULL) return 0;

    parseExampleList();
    window = new MainWindow();

    parseExampleFile();
    srand(seed);
    run();

    window->show();

    if (interactive > 0) {	// non-interactive
        // do something...
        mw_out << "Non Interactive Run of Triangle Robot\n";
        if (noPath) mw_out << "No Path Found!\n";
        else        mw_out << "Path was Found!\n" ;
        return 0;
    }

    return app.exec();
}

