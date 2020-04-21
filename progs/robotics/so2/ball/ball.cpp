/* **************************************
   File: ball.cpp

   Description: 
	This is the entry point for the running the SSS algorithm
	for a ball robot amidst a collection of polyhedrons (obstacles).


	NOTE: see several examples of running this program in the Makefile.
	
	Format of input environment: see README FILE

	**Use WASD keys to rotate the scene**

    	Author: Surin Ahn (June 2013)
 	-- supervised by Professor Yap

 ************************************** */

#include "Octree.h"
#include "PriorityQueue.h"
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
#include "glui.h"
#endif
#ifdef __linux__ 
#include <GL/glui.h>
#endif

#include <set>
//#include "CoreIo.h"

using namespace std;

Octree* OT;

// GLOBAL INPUT Parameters ========================================
//
	double alpha[3] = {75, 360, 100};		// start configuration
	double beta[3] = {450, 50, 400};	// goal configuration
	double epsilon = 5;			// resolution parameter
	Box* boxA;				// start box (containing alpha)
	Box* boxB;				// goal box (containing beta)
	double boxWidth = 512;			// Initial box width
	double R0 = 30;				// Robot radius 
	int windowPosX = 250;			// X Position of Window
	int windowPosY = 150;			// Y Position of Window
	string fileName("input1.txt"); 		// Input file name
	string inputDir("inputs"); 		// Path for input files 
	int QType = 1;				// The Priority Queue can be
						//    sequential (1) or random (0)
	int interactive = 0;			// Run interactively?
						//    Yes (0) or No (1)
	int seed = 111;				// seed for random number generator
						// (Could also be used for BFS, etc)
	double deltaX=0;			// x-translation of input environment
	double deltaY=0;			// y-translation of input environment
	double deltaZ=0;			// z-translation of input environment
	double scale=1;				// scaling of input environment

	bool noPath = true;			// True means there is "No path.
 
	bool hideBoxBoundary = false;  //don't draw box boundary


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
	GLUI_EditText* editSeed;

// External Routines ========================================
//
void renderScene(void);
void keyOperations(void);
void reshape(int width, int height);
void idle(int v);
void keyPressed (unsigned char key, int x, int y);
void keyUp (unsigned char key, int x, int y);
void parseConfigFile(Box*);
void run();
void genEmptyTree();
void drawPath(vector<Box*>&);
extern int fileProcessor(string inputfile);

// Global Variables ========================================
//
int windowID;
float xy_aspect;
float obj_pos[] = { -boxWidth/2., -boxWidth/2., 2.*boxWidth };
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

GLfloat light0_ambient[] =  {0.0, 0.0, 0.0};
GLfloat light0_diffuse[] =  {1.0, 1.0, 1.0};
GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};
GLfloat light0_specular[] = {1.0, 1.0, 1.0};
//GLfloat light1_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
//GLfloat light1_diffuse[] =  {.9f, .6f, 0.0f, 1.0f};
//GLfloat light1_position[] = {-1.0f, -1.0f, 1.0f, 0.0f};

bool showAnim = true;
unsigned int iPathSeg = 0;
bool finishedAnim = false;
int idleTime = 50;

int freeCount = 0;
int stuckCount = 0;
int mixCount = 0;
int mixSmallCount = 0;

//find path using simple heuristic:
//use distance to beta as key in PQ, see dijkstraQueue
bool findPath(Box* a, Box* b, Octree* OT, int& ct)
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
			if (OT->expand(current))
			{
				++ct;
				for (int i = 0; i < 8; ++i)
				{
					// go through neighbors of each child to see if it's in source set
					// if yes, this child go into the dijQ
					bool isNeighborOfSourceSet = false;
					for (int j = 0; j < 6 && !isNeighborOfSourceSet; ++j)
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
								//cerr << "inside FindPath: STUCK case not treated" << endl;
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
		// go through its neighbors and add FREE and MIXED ones to dijQ
		// also add FREE ones to source set 
		for (int i = 0; i < 6; ++i)
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

void animReplay()
{
	iPathSeg = 0;
	finishedAnim = 0;
}

void animSpeedDown()
{
	idleTime *= 2;
	if (idleTime > 1000)
		idleTime = 1000;
}

void animSpeedUp()
{
	idleTime /= 2;
	if (idleTime < 10)
		idleTime = 10;
}


// MAIN PROGRAM: ========================================
int main(int argc, char* argv[])
{
	if (argc > 1) interactive = atoi(argv[1]);	// Interactive (0) or no (>0)
	if (argc > 2) alpha[0] = atof(argv[2]);		// start x
	if (argc > 3) alpha[1] = atof(argv[3]);		// start y
	if (argc > 4) alpha[2] = atof(argv[4]);		// start z
	if (argc > 5) beta[0] = atof(argv[5]);		// goal x
	if (argc > 6) beta[1] = atof(argv[6]);		// goal y
	if (argc > 7) beta[2] = atof(argv[7]);		// goal z
	if (argc > 8) epsilon = atof(argv[8]);		// epsilon (resolution)
	if (argc > 9) R0      = atof(argv[9]);		// robot radius
	if (argc > 10) fileName = argv[10]; 		// Input file name
	if (argc > 11) boxWidth = atof(argv[11]);	// boxWidth
	if (argc > 12) windowPosX = atoi(argv[12]);	// window X pos
	if (argc > 13) windowPosY = atoi(argv[13]);	// window Y pos
	if (argc > 14) QType   = atoi(argv[14]);	// PriorityQ Type (random or no)
	if (argc > 15) seed   = atoi(argv[15]);		// for random number generator
	if (argc > 16) inputDir  = argv[16];		// path for input files
	if (argc > 17) deltaX  = atof(argv[17]);	// x-translation of input file
	if (argc > 18) deltaY  = atof(argv[18]);	// y-translation of input file
	if (argc > 19) deltaZ = atof(argv[19]);		// z-translation of input file
	if (argc > 20) scale  = atof(argv[20]);		// scaling of input file

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
	windowID = glutCreateWindow("Motion Planning");
	
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape); // Tell GLUT to use the method "reshape" for reshaping   
	
	glutTimerFunc(50, idle, 0);
	//GLUI_Master.set_glutIdleFunc( idle );
	GLUI *glui = GLUI_Master.create_glui( "control", 0, windowPosX + boxWidth + 20, windowPosY );


	  /****************************************/
	  /*       Set up OpenGL lights           */
	  /****************************************/
  	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
  	glEnable( GL_NORMALIZE );

	  glEnable(GL_LIGHT0);
	  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	  /*glEnable(GL_LIGHT1);
	  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	  glLightfv(GL_LIGHT1, GL_POSITION, light1_position); */


	  /****************************************/
	  /*          Initial Viewing Position     */
	  /****************************************/
	  // Chee:
	  // The vector view_rotate in GLUI is a 4x4 rotation matrix
	  // We want to rotate the view by "yAng" about the vertical (y) axis:
	  //
	  // First we define yAng:
	  float yAng = 3.14/4;		// 45degrees
	  //
	  // row 1:
	  view_rotate[0] = cos(yAng); view_rotate[1] = .0;
	  		view_rotate[2] = sin(yAng); view_rotate[3] = 0.;
	  // row 2:
	  view_rotate[4] = 0.; view_rotate[5] = 1.;
	  		view_rotate[6] = 0.; view_rotate[7] = 0.;
	  // row 3:
	  view_rotate[8] = -sin(yAng); view_rotate[9] = 0.;
	  		view_rotate[10] = cos(yAng); view_rotate[11] = 0.;
	  // row 4:
	  view_rotate[12] = 0.; view_rotate[13] = 0.;
	  		view_rotate[14] = 0.; view_rotate[15] = 1.;

	  // Unfortunately, we also need to translate along the x-axis to get
	  // 	the model back to the center of the viewport:
	  // The vector obj_pos represents the translational position:
	  // 	it is initially set to:
	  //	float obj_pos[] = { -boxWidth/2, -boxWidth/2, 2*boxWidth };
	  // Here is the x-translation:
	  obj_pos[0] = obj_pos[0] + boxWidth/2;	// shift the model to the right
	  // Here is the z-translation:
	  obj_pos[2] = obj_pos[2] + boxWidth/2;	// shift the model to away from the eye

	  /****************************************/
	  /*          Enable z-buferring          */
	  /****************************************/

	  glEnable(GL_DEPTH_TEST);

	
	// SETTING UP THE CONTROL PANEL:
	editInput = glui->add_edittext( "Input file:", GLUI_EDITTEXT_TEXT );
	editInput->set_text((char*)fileName.c_str());
	editDir = glui->add_edittext( "Input Directory:", GLUI_EDITTEXT_TEXT );
	editDir->set_text((char*)inputDir.c_str());
	
	GLUI_Panel * robot_box_panel = glui->add_panel("Robot Specs");
	   editRadius = glui->add_edittext_to_panel(robot_box_panel,
		   "Radius:", GLUI_EDITTEXT_FLOAT );
	   editRadius->set_float_val(R0);
	   editEpsilon = glui->add_edittext_to_panel(robot_box_panel,
		   "Epsilon:", GLUI_EDITTEXT_FLOAT );
	   editEpsilon->set_float_val(epsilon);

	GLUI_Panel * alpha_box_panel = glui->add_panel("Alpha (start configuration)");
	editAlphaX = glui->add_edittext_to_panel(alpha_box_panel,
		   	"x:", GLUI_EDITTEXT_FLOAT );
	editAlphaX->set_float_val(alpha[0]);
	editAlphaY = glui->add_edittext_to_panel(alpha_box_panel,
		   	"y:", GLUI_EDITTEXT_FLOAT );
	editAlphaY->set_float_val(alpha[1]);
	editAlphaZ = glui->add_edittext_to_panel(alpha_box_panel, "z:", GLUI_EDITTEXT_FLOAT);
	editAlphaZ->set_float_val(alpha[2]);

	GLUI_Panel * beta_box_panel = glui->add_panel("Beta (goal configuration)");
	editBetaX = glui->add_edittext_to_panel(beta_box_panel,
		   	"x:", GLUI_EDITTEXT_FLOAT );
	editBetaX->set_float_val(beta[0]);
	editBetaY = glui->add_edittext_to_panel(beta_box_panel,
		   	"y:", GLUI_EDITTEXT_FLOAT );
	editBetaY->set_float_val(beta[1]);
	editBetaZ = glui->add_edittext_to_panel(beta_box_panel,
		   	"z:", GLUI_EDITTEXT_FLOAT );
	editBetaZ->set_float_val(beta[2]);
	
	editSeed = glui->add_edittext( "seed:", GLUI_EDITTEXT_INT );
	editSeed->set_int_val(seed);

	GLUI_Panel* speedPanel = glui->add_panel("Animation Control");

	GLUI_Button* buttonSpeedDown = glui->add_button_to_panel( speedPanel, "-", -1, (GLUI_Update_CB)animSpeedDown);
	buttonSpeedDown->set_name("-"); 
	buttonSpeedDown->set_w(1);
	glui->add_column_to_panel(speedPanel);
	GLUI_Button* buttonSpeedUp = glui->add_button_to_panel( speedPanel, "+", -1, (GLUI_Update_CB)animSpeedUp);
	buttonSpeedUp->set_name("+"); 
	buttonSpeedUp->set_w(1);
	glui->add_column_to_panel(speedPanel);
	GLUI_Button* buttonReplay = glui->add_button_to_panel( speedPanel, "Replay Animation", -1, (GLUI_Update_CB)animReplay);
	buttonReplay->set_name("replay"); 
	buttonReplay->set_w(1);

	glui->add_separator();
	GLUI_Button* buttonRun = glui->add_button( "Run", -1, (GLUI_Update_CB)run);
	buttonRun->set_name("Run me"); // Hack, but to avoid "unused warning" (Chee)


	// New column:
	glui->add_column(true);

	glui->add_separator();
	radioQType = glui->add_radiogroup();
	glui->add_radiobutton_to_group(radioQType, "Random");
	glui->add_radiobutton_to_group(radioQType, "BFS");
	glui->add_radiobutton_to_group(radioQType, "A-star");
	radioQType->set_int_val(QType);
	
	glui->add_separator();
	radioDrawOption = glui->add_radiogroup(0, -1, (GLUI_Update_CB)renderScene);
	glui->add_radiobutton_to_group(radioDrawOption, "Show Box Boundary");
	glui->add_radiobutton_to_group(radioDrawOption, "Hide Box Boundary");
	
	glui->add_separator();

  	GLUI_Rotation *view_rot = new GLUI_Rotation(glui, "Rotate", view_rotate );
  	view_rot->set_spin( 1.0 );
   	GLUI_Translation *trans_xy = new GLUI_Translation(glui, "Objects XY",
			GLUI_TRANSLATION_XY, obj_pos );
  	trans_xy->set_speed( 5 );
  	GLUI_Translation *trans_z = 
   	 new GLUI_Translation( glui, "Translate Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
  	trans_z->set_speed( 5 );

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
	Box* root = new Box(boxWidth/2, boxWidth/2, boxWidth/2, boxWidth);
	Box::r0 = R0;

	parseConfigFile(root);
	root->updateStatus();

	if (OT)
	{
		delete(OT);
	}
	OT = new Octree(root, epsilon, QType, seed++);  // Note that seed keeps changing!

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
	alpha[2] = editAlphaZ->get_float_val();
	beta[0] = editBetaX->get_float_val();
	beta[1] = editBetaY->get_float_val();
	beta[2] = editBetaZ->get_float_val();
    QType = radioQType->get_int_val();	

cout<<"inside run:  Qtype= " << QType << "\n";

	Timer t;
	// start timer
	t.start();

	genEmptyTree();

	if (showAnim)
	{
		finishedAnim = 0;
		iPathSeg = 0;		
	}

	noPath = false;	// Confusing use of "noPath"
	int ct = 0;

	if (QType == 0 || QType == 1)
	{
		boxA = OT->getBox(alpha[0], alpha[1], alpha[2]);
		while (boxA && !boxA->isFree())
		{	
			if (!OT->expand(boxA))
			{
				noPath = true; // Confusing use of "noPath"
				break;
			}
			++ct;
			boxA = OT->getBox(boxA, alpha[0], alpha[1], alpha[2]);
		}

		boxB = OT->getBox(beta[0], beta[1], beta[2]);
		while (!noPath && boxB && !boxB->isFree())
		{
			if (!OT->expand(boxB))
			{
				noPath = true;
				break;
			}
			++ct;
			boxB = OT->getBox(boxB, beta[0], beta[1], beta[2]);
		}

		while(!noPath && !OT->isConnect(boxA, boxB))
		{
			if (!OT->expand()) // should ct be passed to expand?
			{
				noPath = true;
			}
			++ct;
		}
	} 
	else if(QType == 2)
	{
		boxA = OT->getBox(alpha[0], alpha[1], alpha[2]);
		while (boxA && !boxA->isFree())
		{
			if (!OT->expand(boxA))
			{
				noPath = true;
				break;
			}
			boxA = OT->getBox(boxA, alpha[0], alpha[1], alpha[2]);
		}

		boxB = OT->getBox(beta[0], beta[1], beta[2]);
		while (!noPath && boxB && !boxB->isFree())
		{
			if (!OT->expand(boxB))
			{
				noPath = true;
				break;
			}
			boxB = OT->getBox(boxB, beta[0], beta[1], beta[2]);
		}

		noPath = !findPath(boxA, boxB, OT, ct);
	}	

	// stop timer
	t.stop();
	// print the elapsed time in millisec
	cout << ">>>>>>>>>>>>>>> > > > > > > >>>>>>>>>>>>>>>>>>\n";
	cout << ">>\n";
	cout << ">>     Time used: " << t.getElapsedTimeInMilliSec() << " ms\n";
	cout << ">>\n";

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
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(5.0);
	glBegin(GL_LINE_STRIP);	
	glVertex3f(beta[0], beta[1], beta[2]);
	for (int i = 0; i < (int)path.size(); ++i)
	{
		glVertex3f(path[i]->x, path[i]->y, path[i]->z);	
	}
	glVertex3f(alpha[0], alpha[1], alpha[2]);
	glEnd();
	glLineWidth(1.0);
}

void drawQuad(Box* b)
{
	glLineWidth(1.0);
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
		if (b->width < epsilon)
		{
			glColor3f(0.5, 0.5, 0.5);
		}
		break;
	case Box::UNKNOWN:
		std::cout << "UNKNOWN in drawQuad" << std::endl;
	}

	if (!hideBoxBoundary)
	{
		glPushMatrix();
		glTranslated(b->x, b->y, b->z);
		glutWireCube(b->width);
		glPopMatrix();

	}	
}

void drawEdges(Box* b)
{
	glColor3f(1.0, 1.0, 1.0);
	for (list<Wall*>::iterator iter = b->Walls.begin(); iter != b->Walls.end(); ++iter)
	{
		Wall* w = *iter;
		double A [3] = {w->b->x - w->a->x, w->b->y - w->a->y, w->b->z - w->a->z};
		double B [3] = {w->c->x - w->a->x, w->c->y - w->a->y, w->c->z - w->a->z};
		double normal [3] = {A[1]*B[2] - B[1]*A[2], B[0]*A[2] - A[0]*B[2], A[0]*B[1] - A[1]*B[0]};
		glBegin(GL_TRIANGLES);
		glNormal3d(normal[0], normal[1], normal[2]);
        	glVertex3f(w->a->x, w->a->y, w->a->z);
        	glVertex3f(w->b->x, w->b->y, w->b->z);
        	glVertex3f(w->c->x, w->c->y, w->c->z);
        	glEnd();
	}
	glLineWidth(1.0);
}

// Recursively get the leaf descendants of Box b
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
	for (int i = 0; i < 8; ++i)
	{
		treeTraverse(b->pChildren[i]);
	}
}

void drawSphere( float Radius, double x, double y, double z, double r, double g, double b)
{
	glColor3d(r,g,b);
	glPushMatrix();	
	glTranslated(x, y, z);
	glutWireSphere(Radius, 100, 100);
	glPopMatrix();
}

void filledSphere( double radius, double x, double y, double z, double r, double g, double b) 
{
	glPushMatrix();
	glColor3d(r,g,b);
	glTranslated(x, y, z);
    	glutSolidSphere(radius, 100, 100);
	glPopMatrix();
}

void drawLine()
{
	if (noPath)
	{
		glColor3f(1, 1, 1);
	} 
	else
	{
		glColor3f(1, 0, 0);
	}
	glLineWidth(3.0);
	glBegin(GL_LINES);
	glVertex3f(alpha[0], alpha[1], alpha[2]);
	glVertex3f(beta[0], beta[1], beta[2]);
	glEnd();
	glLineWidth(1.0);
}

void renderScene(void) 
{
	hideBoxBoundary = radioDrawOption->get_int_val();

	glClearColor(0.6, 0.8, 1.0, 0.0f);  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
  	glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 15.0 );

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	glScalef(2.0/boxWidth, 2.0/boxWidth, 2.0/boxWidth);
	glTranslated (obj_pos[0], obj_pos[1], -obj_pos[2] ); 
  	glMultMatrixf( view_rotate );

	treeTraverse(OT->pRoot);

	double r0 = 5;
	if (r0>R0) r0=R0;
	filledSphere(r0, alpha[0], alpha[1], alpha[2], 0.2, 0.2, 1.0);	//blue start center
	filledSphere(r0, beta[0], beta[1], beta[2], 0.4, 0.4, 0.1);	//yellow goal center

	glPolygonMode(GL_FRONT, GL_LINE);
	drawEdges(OT->pRoot);

	//drawLine();

	if (!noPath)
	{
		vector<Box*> path = Graph::dijkstraShortestPath(boxA, boxB);

	     	if (showAnim)
       		{
           		 if (!finishedAnim)
            		{
                		if (iPathSeg >= path.size())
                		{
                   			 finishedAnim = true;
                		}
               		else
              		{
		            	drawSphere(R0, path[iPathSeg]->x, path[iPathSeg]->y, path[iPathSeg]->z, 0, 0, 1);
		        }
            	}
			
		else drawPath(path);
        }
       
	else drawPath(path);
	}
	filledSphere(R0, alpha[0], alpha[1], alpha[2], 1.0, 0.0, 0.498);	// start
	filledSphere(R0, beta[0], beta[1], beta[2], 0, 0, 1);	// goal

  	glEnable( GL_LIGHTING );
	glutSwapBuffers();
}

void reshape (int width, int height) {  
  int tx, ty, tw, th;
  GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
  glViewport( tx, ty, tw, th );

  xy_aspect = (float)tw / (float)th;

  glutPostRedisplay();
}

void idle (int v)
{
  if ( glutGetWindow() != windowID ) 
    glutSetWindow(windowID); 

    glutTimerFunc(idleTime, idle, 0);
    iPathSeg++;
    glutSetWindow(windowID);
    glutPostRedisplay();
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
	bool isQuad = false; // used to check whether the face is a quadrilateral or triangle
		
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

	int nPt = 0;
	int nPolyhedra = 0;	
	ifs >> nPt;
cout<< "nPt=" << nPt << endl;

	vector<double> pts(nPt*3);
	for (int i = 0; i < nPt; ++i)
	{
		ifs >> pts[i*3] >> pts[i*3+1] >> pts[i*3+2];
		
	}	
	
	//string temp;
	//std::getline(ifs, temp);

	while (true)
	{
		int numFaces;
		ifs >> numFaces;
		
		if (numFaces == 0)
		{
			break;
		}
		else 
		{
			nPolyhedra++;
			for (int i = 0; i < numFaces; ++i)
			{
				vector<Corner*> ptVec;
				
				for (int j = 0; j < 4; ++j)
				{
					int pt;
					ifs >> pt;	
					
					// if the first pt is 0, the face is a triangle, so continue with the next 3 pts
					if (j ==0 && pt == 0) continue; 
					else if (j==0 && pt != 0)
					{
						isQuad = true;
					}
					pt -= 1;
					ptVec.push_back(new Corner(pts[pt*3]*scale+deltaX, 
					pts[pt*3+1]*scale+deltaY, pts[pt*3+2]*scale+deltaZ));
					b -> addCorner(ptVec.back());
				}
			
				if (isQuad){
					// Break up rectangle into 2 triangles

					// First triangle
					Edge* e1 = new Edge (ptVec[0], ptVec[1]);
					Edge* e2 = new Edge (ptVec[1], ptVec[2]);
					Edge* e3 = new Edge (ptVec[2], ptVec[0]);
					b -> addEdge(e1);
					b -> addEdge(e2);
					b -> addEdge(e3);

					Wall* w = new Wall (ptVec[0], ptVec[1], ptVec[2]);
					b -> addWall(w);

					// Second triangle
					Edge* e4 = new Edge (ptVec[0], ptVec[2]);
					Edge* e5 = new Edge (ptVec[2], ptVec[3]);
					Edge* e6 = new Edge (ptVec[3], ptVec[0]);
					b -> addEdge(e4);
					b -> addEdge(e5);
					b -> addEdge(e6);

					Wall* w2 = new Wall (ptVec[0], ptVec[2], ptVec[3]);
					b -> addWall(w2);
				}
				else{
					Edge* e1 = new Edge (ptVec[0], ptVec[1]);
					Edge* e2 = new Edge (ptVec[1], ptVec[2]);
					Edge* e3 = new Edge (ptVec[2], ptVec[0]);
					b -> addEdge(e1);
					b -> addEdge(e2);
					b -> addEdge(e3);

					Wall* w = new Wall (ptVec[0], ptVec[1], ptVec[2]);
					b -> addWall(w);
				}
			}
		}
	}

cout<< "nPolyhedra=" << nPolyhedra << endl;
	ifs.close();
}
