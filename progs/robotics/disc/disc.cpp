/* **************************************
   File: disc.cpp

   Description: 
	This is the entry point for the running the SSS algorithm
	for a disc robot amidst a collection of polygonal obstacles.

	To run, call with these positional arguments:

	> ./disc 	[interactive = 0] \
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
	       					(0=interactive, >0 is non-interactive)
		alpha			is start configuration
		beta			is goal configuration
		epsilon			is resolution parameter
		R0			is robot radius
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

   Since Core Library  Version 2.1
   $Id: disc.cpp,v 1.1 2012/10/26 03:58:52 cheeyap Exp cheeyap $
 ************************************** */

#include "QuadTree.h"
#include "PriorityQueue.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.h"
#include "Timer.h"

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include <GLUI/glui.h>
#endif

#include <set>
//#include "CoreIo.h"

using namespace std;

QuadTree* QT;

// GLOBAL INPUT Parameters ========================================
//
	double alpha[2] = {10, 360};		// start configuration
	double beta[2] = {500, 20};		// goal configuration
	double epsilon = 1;			// resolution parameter
	Box* boxA;				// start box (containing alpha)
	Box* boxB;				// goal box (containing beta)
	double boxWidth = 512;			// Initial box width
	double boxHeight = 512;			// Initial box height
	double R0 = 30;				// Robot radius 
	int windowPosX = 400;			// X Position of Window
	int windowPosY = 200;			// Y Position of Window
	string fileName("input2.txt"); 		// Input file name
	string inputDir("inputs"); 		// Path for input files 
	int QType = 0;				// The Priority Queue can be
						//    sequential (0) or random (1)
	int interactive = 0;			// Run interactively?
						//    Yes (0) or No (1)
	int seed = 111;				// seed for random number generator
						// (Could also be used for BFS, etc)
	double deltaX=0;			// x-translation of input environment
	double deltaY=0;			// y-translation of input environment
	double scale=1;				// scaling of input environment

	bool noPath = true;			// True means there is "No path.
 
	bool hideBoxBoundary = false;  //don't draw box boundary

	int freeCount = 0;
	int stuckCount = 0;
	int mixCount = 0;
	int mixSmallCount = 0;

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
	GLUI_EditText* editBetaX;
	GLUI_EditText* editBetaY;
	GLUI_EditText* editSeed;

// External Routines ========================================
//
void renderScene(void);
void parseConfigFile(Box*);
void run();
void genEmptyTree();
void drawPath(vector<Box*>&);
extern int fileProcessor(string inputfile);

// THIS IS JUST FOR THE GreedyBestFirst Heuristic!
//    -- should be completely general!
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
								cerr << "inside FindPath: STUCK case not treated" << endl;
								break;
							case Box::UNKNOWN:
								cerr << "inside FindPath: UNKNOWN case not treated" << endl;
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
	if (argc > 1) interactive = atoi(argv[1]);	// Interactive (0) or no (>0)
	if (argc > 2) alpha[0] = atof(argv[2]);		// start x
	if (argc > 3) alpha[1] = atof(argv[3]);		// start y
	if (argc > 4) beta[0] = atof(argv[4]);		// goal x
	if (argc > 5) beta[1] = atof(argv[5]);		// goal y
	if (argc > 6) epsilon = atof(argv[6]);		// epsilon (resolution)
	if (argc > 7) R0      = atof(argv[7]);		// robot radius
	if (argc > 8) fileName = argv[8]; 		// Input file name
	if (argc > 9) boxWidth = atof(argv[9]);		// boxWidth
	if (argc > 10) boxHeight = atof(argv[10]);	// boxHeight
	if (argc > 11) windowPosX = atoi(argv[11]);	// window X pos
	if (argc > 12) windowPosY = atoi(argv[12]);	// window Y pos
	if (argc > 13) QType   = atoi(argv[13]);	// PriorityQ Type (random or no)
	if (argc > 14) seed   = atoi(argv[14]);		// for random number generator
	if (argc > 15) inputDir  = argv[15];		// path for input files
	if (argc > 16) deltaX  = atof(argv[16]);	// x-translation of input file
	if (argc > 17) deltaY  = atof(argv[17]);	// y-translation of input file
	if (argc > 18) scale  = atof(argv[18]);		// scaling of input file

cout<<"before interactive, Qtype= " << QType << "\n";

	if (interactive > 0) {	// non-interactive
	    // do something...
	    cout << "Non Interactive Run of Disc Robot" << endl;
	    if (noPath)
	    	cout << "No Path Found!" << endl;
	    else
	    	cout << "Path was Found!" << endl;
	    return 0;
	}

	// Else, set up for GLUT/GLUI interactive display:
	
//cout<<"before glutInit\n";
	glutInit(&argc, argv);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutInitWindowSize(boxWidth, boxWidth);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	int windowID = glutCreateWindow("Motion Planning");
	glutDisplayFunc(renderScene);
	GLUI_Master.set_glutIdleFunc( NULL );
	GLUI *glui = GLUI_Master.create_glui( "control", 0, windowPosX + boxWidth + 20, windowPosY );
	
	// SETTING UP THE CONTROL PANEL:
	editInput = glui->add_edittext( "Input file:", GLUI_EDITTEXT_TEXT );
	editInput->set_text((char*)fileName.c_str());
	editDir = glui->add_edittext( "Input Directory:", GLUI_EDITTEXT_TEXT );
	editDir->set_text((char*)inputDir.c_str());
	editRadius = glui->add_edittext( "Radius:", GLUI_EDITTEXT_FLOAT );
	editRadius->set_float_val(R0);
	editEpsilon = glui->add_edittext( "Epsilon:", GLUI_EDITTEXT_FLOAT );
	editEpsilon->set_float_val(epsilon);

	editAlphaX = glui->add_edittext( "alpha.x:", GLUI_EDITTEXT_FLOAT );
	editAlphaX->set_float_val(alpha[0]);
	editAlphaY = glui->add_edittext( "alpha.y:", GLUI_EDITTEXT_FLOAT );
	editAlphaY->set_float_val(alpha[1]);
	editBetaX = glui->add_edittext( "beta.x:", GLUI_EDITTEXT_FLOAT );
	editBetaX->set_float_val(beta[0]);
	editBetaY = glui->add_edittext( "beta.y:", GLUI_EDITTEXT_FLOAT );
	editBetaY->set_float_val(beta[1]);
	editSeed = glui->add_edittext( "seed:", GLUI_EDITTEXT_INT );
	editSeed->set_int_val(seed);

	GLUI_Button* buttonRun = glui->add_button( "Run", -1, (GLUI_Update_CB)run);
	buttonRun->set_name("Run me"); // Hack, but to avoid "unused warning" (Chee)


	// New column:
	glui->add_column(true);

	glui->add_separator();
	radioQType = glui->add_radiogroup();
	glui->add_radiobutton_to_group(radioQType, "Random");
	glui->add_radiobutton_to_group(radioQType, "BFS");
	glui->add_radiobutton_to_group(radioQType, "A-star");
	glui->add_separator();

	radioDrawOption = glui->add_radiogroup(0, -1, (GLUI_Update_CB)renderScene);
	glui->add_radiobutton_to_group(radioDrawOption, "Show Box Boundary");
	glui->add_radiobutton_to_group(radioDrawOption, "Hide Box Boundary");
	glui->add_separator();

	// Quit button
	glui->add_button( "Quit", 0, (GLUI_Update_CB)exit );

	glui->set_main_gfx_window( windowID );

//cout<<"before run\n";
	// PERFORM THE INITIAL RUN OF THE ALGORITHM
	//==========================================
	run(); 	// make it do something interesting from the start!!!

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
	}
	else
		glutMainLoop();

	return 0;
}

void genEmptyTree()
{
	Box* root = new Box(boxWidth/2, boxHeight/2, boxWidth, boxHeight);
	Box::r0 = R0;

	parseConfigFile(root);
	root->updateStatus();

	if (QT)
	{
		delete(QT);
	}
	QT = new QuadTree(root, epsilon, QType, seed++);  // Note that seed keeps changing!

cout<<"inside genEmpty:  Qtype= " << QType << "\n";
}

void run()
{
	//update from glui live variables
	fileName = editInput->get_text();
	inputDir = editDir->get_text();
	R0 = editRadius->get_float_val();
	epsilon = editEpsilon->get_float_val();
	alpha[0] = editAlphaX->get_float_val();
	alpha[1] = editAlphaY->get_float_val();
	beta[0] = editBetaX->get_float_val();
	beta[1] = editBetaY->get_float_val();
    QType = radioQType->get_int_val();	

cout<<"inside run:  Qtype= " << QType << "\n";

	Timer t;
	// start timer
	t.start();

	genEmptyTree();

	noPath = false;	// Confusing use of "noPath"
	int ct = 0;

	if (QType == 0 || QType == 1) // 0=random, 1=BFS
	{
		boxA = QT->getBox(alpha[0], alpha[1]);
		while (boxA && !boxA->isFree())
		{
			if (!QT->expand(boxA))
			{
				noPath = true; // Confusing use of "noPath"
				break;
			}
			++ct;
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
			++ct;
			boxB = QT->getBox(boxB, beta[0], beta[1]);
		}
		// similar to findPath (for QType 2) -- so it is a duplicated logic
		while(!noPath && !QT->isConnect(boxA, boxB)) {
			if (!QT->expand()) // should ct be passed to expand?
			{
				noPath = true;
			}
			++ct;
		}
	} 
	else if(QType == 2)
	{
		boxA = QT->getBox(alpha[0], alpha[1]);
		// split until the box containing A is free (or, NOPATH)
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
		// split until the box containing B is free (or, NOPATH)
		while (!noPath && boxB && !boxB->isFree())
		{
			if (!QT->expand(boxB))
			{
				noPath = true;
				break;
			}
			boxB = QT->getBox(boxB, beta[0], beta[1]);
		}

		// findPath will split until exists path from boxA to boxB (or, NOPATH)
		noPath = !findPath(boxA, boxB, QT, ct);
	}	

	// stop timer
	t.stop();
	// print the elapsed time in millisec
	cout << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
	cout << ">>\n";
	cout << ">>     Time used: " << t.getElapsedTimeInMilliSec() << " ms\n";
	cout << ">>\n";

	glutPostRedisplay();

	if (!noPath) cout << ">>     Path found !" << endl;
	else  cout << ">>     No Path !" << endl;
	cout << ">>\n";
	cout << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
	cout << "Expanded " << ct << " times" << endl;
	cout << "total Free boxes: " << freeCount << endl;
	cout << "total Stuck boxes: " << stuckCount << endl;
	cout << "total Mixed boxes smaller than epsilon: " << mixSmallCount << endl;
	cout << "total Mixed boxes bigger than epsilon: " << mixCount - ct - mixSmallCount << endl;
	freeCount = stuckCount = mixCount = mixSmallCount = 0;
}

void drawPath(vector<Box*>& path)
{
	glColor3f(0.5, 0, 0.25);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);	
	glVertex2f(beta[0], beta[1]);
	for (int i = 0; i < (int)path.size(); ++i)
	{
		glVertex2f(path[i]->x, path[i]->y);	
	}
	glVertex2f(alpha[0], alpha[1]);
	glEnd();
	glLineWidth(1.0);
}

void drawQuad(Box* b)
{
	switch(b->status)
	{
	case Box::FREE:
		glColor3f(0.25, 1, 0.25);
		break;
	case Box::STUCK:
		glColor3f(1, 0.25, 0.25);
		break;
	case Box::MIXED:
		glColor3f(1, 1, 0.25);
		if (b->height < epsilon || b->width < epsilon)
		{
			glColor3f(0.5, 0.5, 0.5);
		}
		break;
	case Box::UNKNOWN:
		std::cout << "UNKNOWN in drawQuad" << std::endl;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex2f(b->x - b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y + b->height / 2);
	glVertex2f(b->x - b->width / 2, b->y + b->height / 2);
	glEnd();

	if (!hideBoxBoundary)
	{
		glColor3f(0, 0 , 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glVertex2f(b->x - b->width / 2, b->y - b->height / 2);
		glVertex2f(b->x + b->width / 2, b->y - b->height / 2);
		glVertex2f(b->x + b->width / 2, b->y + b->height / 2);
		glVertex2f(b->x - b->width / 2, b->y + b->height / 2);
		glEnd();
	}	
}

void drawWalls(Box* b)
{
	glColor3f(1, 1, 1);
	glLineWidth(2.0);
	for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end(); ++iter)
	{
		Wall* w = *iter;
		glBegin(GL_LINES);
		glVertex2f(w->src->x, w->src->y);
		glVertex2f(w->dst->x, w->dst->y);
		glEnd();
	}
	glLineWidth(1.0);
}

void treeTraverse(Box* b)
{	
	if (!b)
	{
		return;
	}
	if (b->isLeaf)
	{
		drawQuad(b);
		return;
	}
	for (int i = 0; i < 4; ++i)
	{
		treeTraverse(b->pChildren[i]);
	}
}

void drawCircle( float Radius, int numPoints, double x, double y, double r, double g, double b)
{	
	glColor3d(r,g,b);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
	glBegin(GL_POLYGON);
	for( int i = 0; i <= numPoints; ++i )
	{
		float Angle = i * (2.0* 3.1415926 / numPoints);  
		float X = cos( Angle )*Radius;  
		float Y = sin( Angle )*Radius;
		glVertex2f( X + x, Y + y);
	}
	glEnd();
}

void filledCircle( double radius, double x, double y, double r, double g, double b) 
{
	int numPoints = 100;
	glColor3d(r,g,b);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	for( int i = 0; i <= numPoints; ++i )
	{
		float Angle = i * (2.0* 3.1415926 / numPoints);  
		float X = cos( Angle )*radius;  
		float Y = sin( Angle )*radius;
		glVertex2f( X + x, Y + y);
	}
	glEnd();
}

void drawLine()
{
	if (noPath)
	{
		glColor3f(0, 0, 0);
	} 
	else
	{
		glColor3f(1, 0, 0);
	}
	glLineWidth(3.0);
	glBegin(GL_LINES);
	glVertex2f(alpha[0], alpha[1]);
	glVertex2f(beta[0], beta[1]);
	glEnd();
	glLineWidth(1.0);
}


void renderScene(void) 
{
	hideBoxBoundary = radioDrawOption->get_int_val();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glScalef(2.0/boxWidth, 2.0/boxHeight, 0);
	glTranslatef(-boxWidth/2, -boxHeight/2, 0);

	treeTraverse(QT->pRoot);

	glPolygonMode(GL_FRONT, GL_LINE);

	drawCircle(R0, 100, alpha[0], alpha[1], 0, 0, 1);	// start
	drawCircle(R0, 100, beta[0], beta[1], 0, 0, 1);		// goal

	double r0 = 5;
	if (r0>R0) r0=R0;
	filledCircle(r0, alpha[0], alpha[1], 0.2, 0.2, 1.0);	//blue start center
	filledCircle(r0, beta[0], beta[1], 0.4, 0.4, 0.1);	//yellow goal center

	drawWalls(QT->pRoot);

	drawLine();

	if (!noPath)
	{
		vector<Box*> path = Graph::dijketraShortestPath(boxA, boxB);
		drawPath(path);
	}

	glutSwapBuffers();
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
	    std::cout << "unexpected end of file." << std::endl;
	
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
	      cout<< "continuation line \\ must be immediately followed by new line.\n";
	  }//while
	  return c;
}//skip_backslash_new_line

/* ********************************************************************** */

void parseConfigFile(Box* b)
{	
	std::stringstream ss;
	ss << inputDir << "/" << fileName;	// create full file name 
	std::string s = ss.str();
cout << "input file name = " << s << endl;	

	fileProcessor(s);	// this will clean the input and put in
				// output-tmp.txt
	
	ifstream ifs( "output-tmp.txt" );
	if (!ifs)
	{
		cerr<< "cannot open input file" << endl;
		exit(1);
	}

	// First, get to the beginning of the first token:
	skip_comment_line ( ifs );

	int nPt, nPolygons;	// previously, nPolygons was misnamed as nFeatures
	ifs >> nPt;
cout<< "nPt=" << nPt << endl;

	//skip_comment_line ( ifs );	// again, clear white space
	vector<double> pts(nPt*2);
	for (int i = 0; i < nPt; ++i)
	{
		ifs >> pts[i*2] >> pts[i*2+1];
	}

	//skip_comment_line ( ifs );	// again, clear white space
	ifs >> nPolygons;
	//skip_comment_line ( ifs );	// again, clear white space
cout<< "nPolygons=" << nPolygons << endl;
	string temp;
	std::getline(ifs, temp);
	for (int i = 0; i < nPolygons; ++i)
	{
		string s;
		std::getline(ifs, s);
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
