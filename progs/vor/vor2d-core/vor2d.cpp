/* **************************************
   File: vor2d.cpp
   $Id: vor2d.cpp 1304 2012-07-27 14:56:24Z jmlien $

   Description: 
	This is the entry point for our Subdivision Algorithm for
	computing a Voronoi complex of a polygonal set.

	To run, call with these positional arguments:
               > ./vor2d $(interactive) \
                       $(pseudo) $(interior) $(fileName) \
                       $(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
                       $(inputDir) \
                       $(xtrans) $(ytrans) $(scale) \
                       $(closePoly) $(c1) $(c2) 

	where these arguments (some representative values are shown) 
		interactive = 0		# 0=interactive, >0 is non-interactive
		epsilon = 2		# resolution parameter
		pseudo = 0		# boolean: 0 or true => show pseudo Vor objects
					# boolean: 1 or false => don't show pseudo Vor obj
		interior = 0		# boolean: 0 or true => show interior Vor objects
					# boolean: 1 or false => don't show 
		inputDir = inputs	# directory to find input files
		fileName = bugtrap.txt	# input environment file
		boxWidth = 512		# initial configuration box size
		boxHt = 512
		windowPosX = 200	# initial Window position
		windowPosY = 200	
		seed = 11		# ransom seed 
		step = 0		# number of steps to run
					#	(step=0 means run to completion)
		xtrans = 5		# x-translation of the input data 
		ytrans = 5		# y-translation of the input data
		scale  = 1		# scaling of input data 
		closePoly  = 1  	# if true (non zero), don't close input polygons
		c1  = 0			# if 0 (false) then do not use c1 filter
		c2  = 0			# if 0 (false) then do not use c2 filter

	Format of input environment: see README FILE

   HISTORY: May, 2012: Jyh-Ming adapted this from the disc.cpp code of Wang/Chiang/Yap.
   Since Core Library  Version 2.1

 ************************************** */


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include "glui.h"
#endif


double boxWidth = 512;          // Initial box width
double boxHeight = 512;         // Initial box height

#include "Timer.h"
#include "QuadTree.h"
#include "PriorityQueue.h"
#include "SimplePSinC.h"

int Feature::LEVEL=0;		// Initializing a static feature

#include <set>
using namespace std;

QuadTree* QT;

// GLOBAL INPUT Parameters ========================================
//

	double epsilon = 10;			// resolution parameter
	double deltaX=0;			// Translate input file in x-direction
	double deltaY=0;			// Translate input file in y-direction
	double uscale=1;			// Scale the input file

	float deltaX_Render=0;            // Translate display in x-direction
    float deltaY_Render=0;            // Translate display in y-direction
    float uscale_Render=1;            // Scale the display

	int windowPosX = 400;			// X Position of Window
	int windowPosY = 200;			// Y Position of Window
	string fileName("test.poly"); 	// Input file name
	string inputDir("inputs"); 		// Path for input files 

	int interactive = 0;			// Run interactively?
						        //    Yes (0) or No (1)
	int seed = 111;				// seed for random number generator
						// (Could also be used for BFS, etc)

	bool pseudo = false;   // show pseudo Voronoi vertices/curves
	bool interior = false; // show Voronoi interior to the polygons
	bool closing_poly=true;
	int  showBoxBoundary = 1;  //draw box boundary
	bool c1=false; //c1 predicate (true is new version, false is old version)
	bool c2=false; //c2 predicate

	string title="Subdivision Voronoi 2D";	// display title
	double cutoff = 10;			// cutoff value (or "delta") for expansion
	double maxEpsilon = 1000;		// maximum epsilon for an IN box

	int freeCount = 0;
	int stuckCount = 0;
	int mixCount = 0;
	int mixSmallCount = 0;
	double timeused=0;

	list<Box*> g_selected_PM;

// GLUI controls ========================================
//
	GLUI_RadioGroup* radioQType;
	GLUI_RadioGroup* radioDrawOption;
	GLUI_EditText* editInput;
	GLUI_EditText* editDir;
	GLUI_EditText* editRadius;
	GLUI_EditText* editEpsilon;
	GLUI_EditText* editMaxEpsilon;
	GLUI_EditText* editAlphaX;
	GLUI_EditText* editAlphaY;
	GLUI_EditText* editBetaX;
	GLUI_EditText* editBetaY;
	GLUI_EditText* editSeed;

	GLUI_Translation * guiTranslate;
	GLUI_Translation * guiZoom;

	//information display
	GLUI_StaticText * selectedBoxInfo; //information about selected box
	GLUI_StaticText * vorInfo;

// External Routines ========================================
//

void run();
void genEmptyTree();

//IO
void parseConfigFile(Box*);
extern int fileProcessor(string inputfile);

//GL
void renderScene(void);
void drawPath(vector<Box*>&);
void drawCircle( double Radius, int numPoints, double x, double y, double r, double g, double b);
void filledCircle( double radius, double x, double y, double r, double g, double b);
void Keyboard( unsigned char key, int x, int y );
void SpecialKey( int key, int x, int y );
void Mouse(int button, int state, int x, int y);

//Postscript
void renderScenePS();
void treeTraversePS(SimplePSinC& PS, Box* b);
void drawVorPS(SimplePSinC& PS, Box* b);
void drawQuadPS(SimplePSinC& PS, Box* b);
void drawWallsPS(SimplePSinC& PS, Box* b);

//GUI related functions
void updateVARinfo();
void updateSelectedBoxInfo();
void reset_move();

//CORE
void glVertex2f_core(double x, double y){ glVertex2f(CORE::Todouble(x),CORE::Todouble(y));}
void glColor3d_core(double r, double g,double b){ glColor3d(CORE::Todouble(r),CORE::Todouble(g),CORE::Todouble(b));}



//tmp
Wall * closest_wall=NULL;
Corner * closest_corner=NULL;


float gui_dXY[2];
float gui_dZ;

// MAIN PROGRAM: ========================================
int main(int argc, char* argv[])
{
	if (argc > 1) interactive = atoi(argv[1]);	// Interactive (0) or no (>0)
	if (argc > 2) epsilon = atof(argv[2]);	// show pseudo Voronoi vertices/curves
	if (argc > 3) pseudo = atoi(argv[3]); 	// show Voronoi interior to the polygons
	if (argc > 4) interior = atoi(argv[4]);	// epsilon (resolution)
	if (argc > 5) fileName = argv[5]; 		// Input file name
	if (argc > 6) boxWidth = atof(argv[6]);		// boxWidth
	if (argc > 7) boxHeight = atof(argv[7]);	// boxHeight
	if (argc > 8) windowPosX = atoi(argv[8]);	// window X pos
	if (argc > 9) windowPosY = atoi(argv[9]);	// window Y pos
	if (argc > 10) inputDir  = argv[10];		// path for input files
	if (argc > 11) deltaX  = atof(argv[11]);        // translate x
	if (argc > 12) deltaY  = atof(argv[12]);        // translate y
	if (argc > 13) uscale  = atof(argv[13]);        // translate y
	if (argc > 14) closing_poly = atoi(argv[14]);    // control closing polygons
	if (argc > 15) c1 = atoi(argv[15]);             // control c1 predicate
	if (argc > 16) c2 = atoi(argv[16]);             // control c2 predicate
	if (argc > 17) maxEpsilon = atof(argv[17]);     // max eps for any Vor (IN) box
	if (argc > 18) title = argv[18];               	// title of this demo
	if (argc > 19) cutoff = atof(argv[19]);        	// cutoff for subdivision


	// Else, set up for GLUT/GLUI interactive display:
	glutInit(&argc, argv);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutInitWindowSize(boxWidth, boxWidth);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	int windowID = glutCreateWindow((char*)title.c_str());
	glutDisplayFunc(renderScene);
	GLUI_Master.set_glutIdleFunc( NULL );
	GLUI_Master.set_glutKeyboardFunc(Keyboard);
	GLUI_Master.set_glutMouseFunc(Mouse);
	GLUI_Master.set_glutSpecialFunc(SpecialKey);

	char level_string[36];
	sprintf(level_string,"Vor2D Level %d",Feature::corelevel());
	GLUI *glui = GLUI_Master.create_glui( level_string, 0, windowPosX + boxWidth + 20, windowPosY );

	glClearColor(0.5,0,0,0);

    // *Antialias*
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

	// SETTING UP THE CONTROL PANEL:
    GLUI_Panel * top_panel=glui->add_panel("VOR2D Controls");
	editInput = glui->add_edittext_to_panel(top_panel, "Input file:", GLUI_EDITTEXT_TEXT );
	editInput->set_text((char*)fileName.c_str());
	editDir = glui->add_edittext_to_panel(top_panel, "Input Directory:", GLUI_EDITTEXT_TEXT );
	editDir->set_text((char*)inputDir.c_str());
	editEpsilon = glui->add_edittext_to_panel(top_panel, "Epsilon:", GLUI_EDITTEXT_FLOAT );
	editEpsilon->set_float_val(CORE::Todouble(epsilon));
	editMaxEpsilon = glui->add_edittext_to_panel(top_panel, "maxEpsilon:", GLUI_EDITTEXT_FLOAT );
	editMaxEpsilon->set_float_val(CORE::Todouble(maxEpsilon));

	GLUI_Button* buttonRun = glui->add_button_to_panel(top_panel, "Run", -1, (GLUI_Update_CB)run);
	buttonRun->set_name("Run me"); // Hack, but to avoid "unused warning" (Chee)

	// New column:
	glui->add_column_to_panel(top_panel,true);

	glui->add_separator_to_panel(top_panel);
	glui->add_checkbox_to_panel(top_panel,"Box Boundary", &showBoxBoundary, -1, (GLUI_Update_CB)renderScene)->set_int_val(1);
	glui->add_separator_to_panel(top_panel);

	// Save image button
    glui->add_button_to_panel(top_panel, "Save Image", 0, (GLUI_Update_CB)renderScenePS );

	// Quit button
	glui->add_button_to_panel(top_panel, "Quit", 0, (GLUI_Update_CB)exit );

	//translate and zoom gui
	GLUI_Panel * bottom_panel=glui->add_panel("View Control");
	guiTranslate=glui->add_translation_to_panel(bottom_panel, "Translate", GLUI_TRANSLATION_XY,gui_dXY);
	glui->add_column_to_panel(bottom_panel,true);
	guiZoom=glui->add_translation_to_panel(bottom_panel, "Zoom", GLUI_TRANSLATION_Z,&gui_dZ);
	// reset button
	glui->add_column_to_panel(bottom_panel,true);
	glui->add_button_to_panel(bottom_panel, "Reset View", 0, (GLUI_Update_CB)reset_move );

    //add some display
	vorInfo=glui->add_statictext("var \n info"); //
	selectedBoxInfo=glui->add_statictext("no selected box"); //information about selected box


	glui->set_main_gfx_window( windowID );

	// PERFORM THE INITIAL RUN OF THE ALGORITHM
	//==========================================
	run(); 	// make it do something interesting from the start!!!

	// SHOULD WE STOP or GO INTERACTIVE?
	//==========================================
	if (interactive > 0) {	// non-interactive
	    // do something...
	    return 0;
	}
	else
		glutMainLoop();

	return 0;
}

//create an empty quadtree
void genEmptyTree()
{
	Box* root = new Box(boxWidth/2, boxHeight/2, boxWidth, boxHeight);

	parseConfigFile(root);
	//Chee: root->updateStatus(maxEpsilon);
	root->updateStatus();


	if (QT)
	{
		delete(QT);
	}
	QT = new QuadTree(root, epsilon, maxEpsilon);

}

void run()
{
	//update from glui live variables
	fileName = editInput->get_text();
	inputDir = editDir->get_text();
	epsilon = editEpsilon->get_float_val();
	maxEpsilon= editMaxEpsilon->get_float_val();

	::Timer t;

	// start timer
	t.start();

	genEmptyTree();

	do
	{
	    //subdivison phase
	    QT->subdividePhase();

	    //balance
	    QT->balancePhase();
	}
	while(QT->PQ->empty()==false);

    //construct
    QT->constructPhase();

	// stop timer
	t.stop();
	timeused=t.getElapsedTimeInMilliSec();

	updateVARinfo();

	glutPostRedisplay();

	freeCount = stuckCount = mixCount = mixSmallCount = 0;
}

//void drawPath(vector<Box*>& path)
//{
//	glColor3f(0.5, 0, 0.25);
//	glLineWidth(3.0);
//	glBegin(GL_LINE_STRIP);
//	glVertex2f_core(beta[0], beta[1]);
//	for (int i = 0; i < (int)path.size(); ++i)
//	{
//		glVertex2f_core(path[i]->x, path[i]->y);
//	}
//	glVertex2f_core(alpha[0], alpha[1]);
//	glEnd();
//	glLineWidth(1.0);
//}


void drawQuad(Box* b)
{
	switch(b->status)
	{
        case Box::OUT:

            glColor3f(1, 1, 1); //White
            break;

        case Box::ON:

            glColor3f(0.85, 0.85, 0.85);
            break;

        case Box::IN:

            glColor3f(1, 1, 0.25);

            if (b->height < epsilon || b->width < epsilon)
            {
                glColor3f(0.5, 0.5, 0.5);
            }

            break;

        case Box::UNKNOWN:

            std::cout << "! Error: found UNKNOWN box in drawQuad" << std::endl;
            break;
	}

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glVertex2f_core(b->x - b->width / 2, b->y - b->height / 2);
    glVertex2f_core(b->x + b->width / 2, b->y - b->height / 2);
    glVertex2f_core(b->x + b->width / 2, b->y + b->height / 2);
    glVertex2f_core(b->x - b->width / 2, b->y + b->height / 2);
    glEnd();

	if (showBoxBoundary)
	{
		glColor3f(0.5, 0.5 , 0.5);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		glVertex2f_core(b->x - b->width / 2, b->y - b->height / 2);
		glVertex2f_core(b->x + b->width / 2, b->y - b->height / 2);
		glVertex2f_core(b->x + b->width / 2, b->y + b->height / 2);
		glVertex2f_core(b->x - b->width / 2, b->y + b->height / 2);
		glEnd();
	}	

	//draw segments
	glBegin(GL_LINES);
	glLineWidth(3);
	glColor3d_core(.75,0,0);

	for(list<VorSegment>::iterator i=b->vor_segments.begin();i!=b->vor_segments.end();i++){
	    glVertex2f_core(i->p[0],i->p[1]);
	    glVertex2f_core(i->q[0],i->q[1]);
	}
	glEnd();
	glLineWidth(1);
}

double Ax,Ay;
double Bx,By;
double Cx,Cy;
double Dx,Dy;

void drawQuad_selected(list<Box*> boxes)
{
    for(list<Box*>::iterator i=boxes.begin();i!=boxes.end();i++)
    {
        Box * b=*i;

        if(b==boxes.back())
        {
            glLineWidth(3);
            glColor3f(1, 0 , 0);
        }
        else
        {
            glLineWidth(1);
            glColor3f(.5, 0 , 0);
        }

        double w2=(b->width/2)*0.9;
        double h2=(b->height/2)*0.9;


        //draw a highlight box
        glBegin(GL_LINE_LOOP);
        glVertex2f_core(b->x - w2, b->y - h2);
        glVertex2f_core(b->x + w2, b->y - h2);
        glVertex2f_core(b->x + w2, b->y + h2);
        glVertex2f_core(b->x - w2, b->y + h2);
        glEnd();

    }

    Box * b=boxes.back();

    //draw circle with radius (clearance+2*Rb)
    drawCircle( b->rB*2+b->cl_m, 100, b->x, b->y, 1,1,0);
    //filledCircle( b->rB*2+b->cl_m, b->x, b->y, 1,1,0);

    //draw features in blue
    typedef list<Corner*>::iterator CIT;
    typedef list<Wall*>::iterator WIT;

    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3d_core(0,0,1);
    for(WIT i=b->walls.begin();i!=b->walls.end();i++){
        Wall * w=*i;
        glVertex2f_core(w->src->x,w->src->y);
        glVertex2f_core(w->dst->x,w->dst->y);
    }
    glEnd();

    //------------------------
    //tmp
    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3d_core(0,1,0);
    if(closest_wall!=NULL)
    {
        Wall * w=closest_wall;
        glVertex2f_core(w->src->x,w->src->y);
        glVertex2f_core(w->dst->x,w->dst->y);
    }
    glEnd();

    if(closest_corner!=NULL)
    {
        drawCircle(5/uscale_Render,36, closest_corner->x,closest_corner->y,0,1,0);
    }
    //------------------------

    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3d(1,1,0);
    glVertex2f_core(Ax,Ay);
    glVertex2f_core(Bx,By);

    glColor3d(0,1,0);
    glVertex2f_core(Cx,Cy);
    glVertex2f_core(Dx,Dy);

    glEnd();

    glLineWidth(1);
    for(CIT i=b->corners.begin();i!=b->corners.end();i++){
        Corner*c=*i;
        filledCircle(5/uscale_Render,c->x,c->y,0.75,0.75,1);
        drawCircle(5/uscale_Render,36, c->x,c->y,0,0,1);
    }

    glLineWidth(1);
}

void drawWalls(Box* b)
{
	glColor3f(0, 0, 0);
	glLineWidth(2.0);
	for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end(); ++iter)
	{
		Wall* w = *iter;
		glBegin(GL_LINES);
		glVertex2f_core(w->src->x, w->src->y);
		glVertex2f_core(w->dst->x, w->dst->y);
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

void drawCircle( double Radius, int numPoints, double x, double y, double r, double g, double b)
{
    glColor3d_core(r,g,b);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_LINE_LOOP);
    for( int i = 0; i <= numPoints; ++i )
    {
        double Angle = i * (2.0* 3.1415926 / numPoints);
        double X = cos( CORE::Todouble(Angle) )*Radius;
        double Y = sin( CORE::Todouble(Angle) )*Radius;
        glVertex2f_core( X + x, Y + y);
    }
    glEnd();
}

void filledCircle( double radius, double x, double y, double r, double g, double b) 
{
    int numPoints = 100;
    glColor3d_core(r,g,b);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    for( int i = 0; i <= numPoints; ++i )
    {
        double Angle = i * (2.0* 3.1415926 / numPoints);
        double X = cos( CORE::Todouble(Angle) )*radius;
        double Y = sin( CORE::Todouble(Angle) )*radius;
        glVertex2f_core( X + x, Y + y);
    }
    glEnd();
}

void renderScene(void) 
{
    uscale_Render=(gui_dZ>0)?pow(1.05f,gui_dZ):1.0f/pow(1.05f,-gui_dZ);
    deltaX_Render=gui_dXY[0]*1.0f/uscale_Render;
    deltaY_Render=gui_dXY[1]*1.0f/uscale_Render;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glScalef(2.0*uscale_Render/boxWidth, 2.0*uscale_Render/boxHeight, 0);
	glTranslatef(-boxWidth/2+deltaX_Render, -boxHeight/2+deltaY_Render, 0);

	//draw quad tree
	treeTraverse(QT->pRoot);

    //draw obstacles
    glPolygonMode(GL_FRONT, GL_LINE);
    drawWalls(QT->pRoot);

    //draw selected feature
    if(g_selected_PM.empty()==false)
        drawQuad_selected(g_selected_PM);

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
	cout << "\n- Input file name = " << s << endl;

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
	cout<< "- nPt=" << nPt << endl;

	//skip_comment_line ( ifs );	// again, clear white space
	vector<double> pts(nPt*2);
	for (int i = 0; i < nPt; ++i)
	{
		ifs >> pts[i*2] >> pts[i*2+1];
	}

	//skip_comment_line ( ifs );	// again, clear white space
	ifs >> nPolygons;
	//skip_comment_line ( ifs );	// again, clear white space

	cout<< "- nPolygons=" << nPolygons << endl;

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
			ss >> pt;
			pt -= 1; //1 based array
			if (ptSet.find(pt) == ptSet.end())
			{
				ptVec.push_back(new Corner(pts[pt*2]*uscale+deltaX, pts[pt*2+1]*uscale+deltaY));
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
			    if(closing_poly)
			    {
                    if (ptVec.size() > 1)
                    {
                        Wall* w = new Wall(ptVec[ptVec.size()-1], ptVec[0]);
                        b->addWall(w);
                        break;
                    }
			    }//end closing_poly
			}
		}
	}
	ifs.close();

}

// gather information and show it on the GUI diagram
void updateVARinfo()
{

    char info[1024];

    static int leave_size=-1;
    if(leave_size<0)
    {
        list<Box*> leaves;
        QT->pRoot->getLeaves(leaves);
        leave_size=leaves.size();
    }


    sprintf(info,"Time used: %.2f ms; # of leaves=%d",CORE::Todouble(timeused),leave_size);
    vorInfo->set_text(info);
}

//
// call this function when the selected box is changed
// show related information on GUI diagram
//
void updateSelectedBoxInfo()
{
    if(g_selected_PM.empty())
    {
        selectedBoxInfo->set_text("no selected box");
        return;
    }

    Box * selected=g_selected_PM.back();
    char info[1024];
    sprintf(info,"Selected box has %d corner and %d wall features", (int)selected->corners.size(), (int)selected->walls.size());
    selectedBoxInfo->set_text(info);
}


void reset_move()
{
    uscale_Render=1; deltaX_Render=deltaY_Render=0;
    guiTranslate->set_x(0);
    guiTranslate->set_y(0);
    guiZoom->set_z(0);
}

//
// handle keyboard events
//
void Keyboard( unsigned char key, int x, int y )
{
    // find closest colorPt3D if ctrl is pressed...
    switch( key ){
        case 27: exit(0);
        case 'w': deltaY_Render+=boxHeight/(uscale_Render*100); break;
        case 's': deltaY_Render-=boxHeight/(uscale_Render*100); break;
        case 'd': deltaX_Render+=boxWidth/(uscale_Render*100); break;
        case 'a': deltaX_Render-=boxWidth/(uscale_Render*100); break;
        case '=': uscale_Render*=1.5; break;
        case '-': uscale_Render/=1.5; break;
        case 'r': reset_move(); break;
        default: return;
    }

    glutPostRedisplay();
}



//
//
// move up and down in the Qtree hierarchy
//
//

void SpecialKey(int key, int x, int y)
{
    // find closest colorPt3D if ctrl is pressed...
    switch( key ){
        case GLUT_KEY_UP:
            if(g_selected_PM.empty()==false){//not empty
                Box * last=g_selected_PM.back();
                if(last!=QT->pRoot){
                    g_selected_PM.push_back(last->pParent);
                }
            }
            break;

        case GLUT_KEY_DOWN:
            if(g_selected_PM.size()>1)
                g_selected_PM.pop_back();
            break;
        default: return;
    }

    updateSelectedBoxInfo();
    glutPostRedisplay();
}


//
//
// selecting a pocket minimum...
//
//

void Mouse(int button, int state, int x, int y)
{
    //control needs to be pressed to selelect nodes
    if( state == GLUT_UP )
    {
        g_selected_PM.clear();
        if( glutGetModifiers()==GLUT_ACTIVE_CTRL )
        {
            int viewport[4];
            glGetIntegerv(GL_VIEWPORT,viewport);
            double m_x=(x-boxWidth/2)/uscale_Render-deltaX_Render+boxWidth/2;
            double m_y=(viewport[3]-y-boxHeight/2)/uscale_Render-deltaY_Render+boxHeight/2;

            Box * selected = QT->pRoot->find(m_x,m_y);


            if(selected!=NULL)
            {
                //selected->pParent->distribute_features2box(selected);
                selected->buildVor();

                BoxNode mid;
                mid.x=selected->x;
                mid.y=selected->y;
                selected->pParent->determine_clearance(mid);
                if(dynamic_cast<Wall*>(mid.nearest_feature)!=NULL)
                {
                    closest_wall=(Wall*)mid.nearest_feature;
                    closest_corner=NULL;
                }
                else{
                    closest_wall=NULL;
                    closest_corner=(Corner*)mid.nearest_feature;
                }


                g_selected_PM.push_back(selected);
            }//end selected

        }

        updateSelectedBoxInfo();
        glutPostRedisplay();
    }//if pressed the right key/button

}


//Postscript
void renderScenePS()
{
    string ps_filename="vor2d_screen_dump.ps"; //fileName+".ps";

    SimplePSinC PS;
    PS.open(ps_filename,0,0,boxWidth,boxHeight);
    PS.setlinejoin(1);

    //draw tree
    treeTraversePS(PS,QT->pRoot);

    //draw obstacles
    drawWallsPS(PS,QT->pRoot);

    //draw selected item(s)

    //////////////////////////////////////////////////
    //Chee: bounding box:
    PS.setlinewidth(4);
    Box* r = QT->pRoot;
    PS.rect(CORE::Todouble(r->x-r->width / 2), CORE::Todouble(r->y - r->height / 2),
            CORE::Todouble(r->x + r->width / 2), CORE::Todouble(r->y + r->height / 2) );
    PS.setstrokergb(0,0,0);
    PS.stroke();
    //////////////////////////////////////////////////

    PS.close();

    cout<<"- Saved screen to "<<ps_filename<<endl;
}

void treeTraversePS(SimplePSinC& PS, Box* b)
{
    if (!b)
    {
        return;
    }
    if (b->isLeaf)
    {
        drawQuadPS(PS, b);
        drawVorPS(PS,b);
        return;
    }
    for (int i = 0; i < 4; ++i)
    {
        treeTraversePS(PS, b->pChildren[i]);
    }
}

void drawQuadPS(SimplePSinC& PS, Box* b)
{
    switch(b->status)
    {
        case Box::OUT:
            PS.setfilledrgb(1,1,1);
            break;

        case Box::ON:
            PS.setfilledrgb(0.85, 0.85, 0.85);
            break;

        case Box::IN:

            PS.setfilledrgb(1, 1, 0.25);

            if (b->height < epsilon || b->width < epsilon)
            {
                PS.setfilledrgb(0.5, 0.5, 0.5);
            }
            break;

        case Box::UNKNOWN:

            std::cout << "! Error: found UNKNOWN box in drawQuad" << std::endl;

            break;
    }

    PS.setlinewidth(1);
    PS.rect(CORE::Todouble(b->x-b->width / 2), CORE::Todouble(b->y - b->height / 2),
            CORE::Todouble(b->x + b->width / 2), CORE::Todouble(b->y + b->height / 2) );
    PS.setstrokegray(0.5);

    if (showBoxBoundary)
        PS.fillstroke();
    else
        PS.fill();
}


void drawVorPS(SimplePSinC& PS, Box* b)
{

    PS.setlinewidth(2);
    PS.setstrokergb(1,0,0);

    for(list<VorSegment>::iterator i=b->vor_segments.begin();i!=b->vor_segments.end();i++)
    {
        PS.line(CORE::Todouble(i->p[0]),CORE::Todouble(i->p[1]), CORE::Todouble(i->q[0]), CORE::Todouble(i->q[1]) );
    }

    PS.stroke();
}

void drawWallsPS(SimplePSinC& PS, Box* b)
{
    PS.setlinewidth(2);
    PS.setstrokergb(0,0,0);

    for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end(); ++iter)
    {
        Wall* w = *iter;
        PS.line( CORE::Todouble(w->src->x), CORE::Todouble(w->src->y),CORE::Todouble(w->dst->x), CORE::Todouble(w->dst->y) );
    }

    PS.stroke();
}

