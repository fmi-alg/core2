/* **************************************
   File: main.cpp

   Description:
   This is the entry point for the running the SSS algorithm
   for a rod robot amidst a collection of polyhedrons (obstacles).
   The framework will ultimately support a variety of robots.

   NOTES:
   0. This file originated in the (3d) ball program done by
   	Surin Ahn (June 2013) for the
    2013 Siemens Science & Math Competition under the
	supervision of Professor Yap.  Surin was a
    Regional Finalist (one out of 100 nationwide) from New York.
    In 2014, she went to Princeton in Computer Science.
   1. See examples of running this program in the Makefile.
   2. Format of input environment: see README FILE
   3. Use WASD keys to rotate the scene
   4. In Fall of 2014, we started to reorgnized all the programs
   	to be consistent with our general SSS framework.

Author:
    Ching-Hsiang Hsu, Ziyao Wei and Chee Yap (Fall 2015)


   ************************************** */

#include "./main.h"

#include <algorithm>
#include <string>
#include <vector>

#include <QApplication>

using namespace std;

SoftSubdivisionSearch* initializeSSS(double alpha[3], double beta[3], char* dir) {
    ConfBox3d* root = new ConfBox3d(boxWidth/2, boxWidth/2, boxWidth/2, boxWidth);
    ConfBox3d::r0 = R;

    parseConfigFile(root, dir);

    cout<<"inside : initializeSSS Qtype= " << QType << "\n";
    return new SoftSubdivisionSearch(root, epsilon, QType, seed++, alpha, beta);  // Note that seed keeps changing!
}

void run() {
    mw_out << "Run " << runCounter++ << ":\n";


    initialRot[0] = 0;
    initialRot[1] = 1;
    initialRot[2] = 0;
    finalRot[0] = -1;
    finalRot[1] = 0;
    finalRot[2] = 0;


    Timer t;
    // start timer
    t.start();

    if (sss) {
        delete(sss);
    }

    sss = initializeSSS(alpha, beta, running_dir);
    path = sss->softSubdivisionSearch();
    int ct = sss->getNumSubdivisions();
    noPath = path.empty();

    // stop timer
    t.stop();
    // print the elapsed time in millisec
    //cout << ">>>>>>>>>>>>>>>>>>>>>>>>\n";
    //mw_out << ">>\n";
    mw_out << ">>\tTime used: " << t.getElapsedTimeInMilliSec() << " ms\n";
    //mw_out << ">>\n";

    if (!noPath) {
        mw_out << ">>\tPath found !\n";
    } else {
        mw_out << ">>\tNo Path !\n";
    }
    cout << ">>\n";
    cout << ">>>>>>>>>>>>>>>>>>>>>>>>\n";
    mw_out << ">>\tExpanded " << ct << " times" << "\n";
    mw_out << ">>\ttotal Free boxes: " << sss->freeCount << "\n";
    mw_out << ">>\ttotal Stuck boxes: " << sss->stuckCount << "\n";
    cout << "total Mixed boxes < epsilon: " << sss->mixSmallCount << endl;
    cout << "total Mixed boxes > epsilon: " << sss->mixCount - ct - sss->mixSmallCount << endl;
    cout << endl;
}

void logNonInteractiveRun(bool noPath) {
    // do something...
    cout << "Non Interactive Run of Disc Robot" << endl;
    if (noPath) {
        cout << "No Path Found!" << endl;
    } else {
        cout << "Path was Found!" << endl;
    }
}

// MAIN PROGRAM: ========================================
int main(int argc, char* argv[]) {
    // (Tom)
    char tmp[300];
    strcpy(tmp, argv[0]);
    strcpy(running_dir, "");
    char *ptr;
    ptr = strtok(tmp, "/");
    while(ptr != NULL) {
        if(strcmp(ptr, "build-ball-3d-Desktop_Qt_5_5_0_clang_64bit-Debug") == 0) {
            break;
        }
        strcat(running_dir, "/");
        strcat(running_dir, ptr);
        ptr = strtok(NULL, "/");
    }
    strcat(running_dir, "/ball-3d-qt/");


    if (argc > 1) interactive = atoi(argv[1]);  // Interactive (0) or no (>0)
    if (argc > 2) alpha[0] = atof(argv[2]);    // start x
    if (argc > 3) alpha[1] = atof(argv[3]);    // start y
    if (argc > 4) alpha[2] = atof(argv[4]);    // start z
    if (argc > 5) beta[0] = atof(argv[5]);    // goal x
    if (argc > 6) beta[1] = atof(argv[6]);    // goal y
    if (argc > 7) beta[2] = atof(argv[7]);    // goal z
    if (argc > 8) epsilon = atof(argv[8]);    // epsilon (resolution)
    if (argc > 9) R       = atof(argv[9]);    // ball radius
    if (argc > 10) fileName = argv[10];     // Input file name
    if (argc > 11) boxWidth = atof(argv[11]);  // boxWidth
    if (argc > 12) windowPosX = atoi(argv[12]);  // window X pos
    if (argc > 13) windowPosY = atoi(argv[13]);  // window Y pos
    if (argc > 14) QType   = atoi(argv[14]);  // PriorityQ Type (random or no)
    if (argc > 15) seed   = static_cast<unsigned int>(atoi(argv[15]));    // for random number generator
    if (argc > 16) {
        inputDir  = argv[16];    // path for input files
        std::stringstream ss;
        ss << inputDir << "/" << fileName;  // create full file name
        defaultFilePath = ss.str();
    }
    if (argc > 17) deltaX  = atof(argv[17]);  // x-translation of input file
    if (argc > 18) deltaY  = atof(argv[18]);  // y-translation of input file
    if (argc > 19) deltaZ = atof(argv[19]);    // z-translation of input file
    if (argc > 20) scale  = atof(argv[20]);    // scaling of input file
    if (argc > 21) eye[0] = atof(argv[21]);
    if (argc > 22) eye[1] = atof(argv[22]);
    if (argc > 23) eye[2] = atof(argv[23]);
    if (argc > 24) at[0] = atof(argv[24]);
    if (argc > 25) at[1] = atof(argv[25]);
    if (argc > 26) at[2] = atof(argv[26]);
    if (argc > 27) up[0] = atof(argv[27]);
    if (argc > 28) up[1] = atof(argv[28]);
    if (argc > 29) up[2] = atof(argv[29]);
    if (argc > 30) transparency = atoi(argv[30]);
    if (argc > 31) frameRate = atoi(argv[31]);
    if (argc > 34) {
        view_rotate_angles[0] = atof(argv[32]);
        view_rotate_angles[1] = atof(argv[33]);
        view_rotate_angles[2] = atof(argv[34]);
    }

    cout << "before interactive, Qtype= " << QType << endl;

    if (interactive > 0) {  // non-interactive
        logNonInteractiveRun(noPath);
        return 0;
    }

    // Allow creation of Qt GUI
    QApplication app(argc, argv);
    window = new MainWindow();    
    run();
    window->show();
    showAnim = false;

    return app.exec();
}
