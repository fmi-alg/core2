#define mw_out std::cout<<'\t'
#include "complex.h"

int main(int argc, char* argv[]) {

    if (argc >2) {
      cfgName = argv[2];		//config file
      parseExampleFile();
    }
    if (argc >1) interactive = atoi(argv[1]);//interactive or no

    if (argc > 3  && argv[3][0]!='@')fileName = argv[3];	//enviroment file
    if (argc > 4  && argv[4][0]!='@')inputDir = argv[4];	//Path for input files
    if (argc > 5  && argv[5][0]!='@')robotDir = argv[5];	//Path for robot files
    if (argc > 6  && argv[6][0]!='@')robotName = argv[6];	//robot 
    if (argc > 7  && argv[7][0]!='@')alpha[0] = atof(argv[7]);	// startx	
    if (argc > 8  && argv[8][0]!='@')alpha[1] = atof(argv[8]);	// starty
    if (argc > 9  && argv[9][0]!='@')alpha[2] = atof(argv[9]);	// startTheta
    if (argc > 10 && argv[10][0]!='@')beta[0] = atof(argv[10]);	// goalx
    if (argc > 11 && argv[11][0]!='@')beta[1] = atof(argv[11]);	// goaly
    if (argc > 12 && argv[12][0]!='@')beta[2] = atof(argv[12]);	// goalTheta
    if (argc > 13 && argv[13][0]!='@')epsilon = atof(argv[13]);	// resolution parameter
    if (argc > 14 && argv[14][0]!='@')R0 	= atof(argv[14]);	// robot radius
    if (argc > 15 && argv[15][0]!='@')boxWidth = atof(argv[15]);		// boxWidth
    if (argc > 16 && argv[16][0]!='@')boxHeight = atof(argv[16]);	// boxHeight
    if (argc > 17 && argv[17][0]!='@')windowPosX = atoi(argv[17]);	// window X pos
    if (argc > 18 && argv[18][0]!='@')windowPosY = atoi(argv[18]);	// window Y pos
    if (argc > 19 && argv[19][0]!='@')QType   = atoi(argv[19]); // PriorityQ Type(random or no)
    if (argc > 20 && argv[20][0]!='@')seed   = atoi(argv[20]);	 // for random number generator
    if (argc > 21 && argv[21][0]!='@')deltaX  = atof(argv[21]); // x-translation of input file
    if (argc > 22 && argv[22][0]!='@')deltaY  = atof(argv[22]); // y-translation of input file
    if (argc > 23 && argv[23][0]!='@')scale  = atof(argv[23]);		// scaling of input file
    if (argc > 24 && argv[24][0]!='@')verboseOption  = atoi(argv[24]);		// verbosity of output


    if (interactive > 0) {	// non-interactive
        // do something...
        mw_out << "Non Interactive Run of Complex Robot" ;
    }

    srand(seed);
    run();

    if (interactive > 0) {	// non-interactive
        if (noPath) mw_out << "No Path Found!\n";
        else        mw_out << "Path was Found!\n" ;
        return 0;
    }

    return 0;
}
