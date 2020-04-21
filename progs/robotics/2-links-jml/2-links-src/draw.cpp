/*
 * draw.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: zhonghua
 */
#include <unistd.h>
#include <sstream>
using namespace std;

#include "draw.h"

#define SQR(x) ((x)*(x))

extern bool debug;
extern bool check;

extern string filename;
extern RRT* rrt;

float c_theta = 0.0;
uint c_index = 0;
bool animation = false;
bool reverse_order = false;
int current_frame = 0;

int maxFrames = 64*2;
float z_rot = 0.0;

bool showF=true, showE=true, showV=false, showC=true, showT=true, showL=true, showP=true;
double COM[3] = {250, 250, 0};
double R = 250; //center and radius

int path_index = 0;

//copied from meshlab
void DisplayBackground(void)
{
	float topcolor[]={0.3,.3,0.3};
	float bottomcolor[]={0.7,0.7,0.7};

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
			glColor3fv(topcolor);      glVertex2f(-1, 1);
			glColor3fv(bottomcolor);   glVertex2f(-1,-1);
			glColor3fv(topcolor);      glVertex2f( 1, 1);
			glColor3fv(bottomcolor);   glVertex2f( 1,-1);
	glEnd();

	glPopAttrib();
	glPopMatrix(); // restore modelview
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void DrawTree(const RRT_TREE& tree)
{

	glLineWidth(0.5);
	glColor3f(0,0,0);
	glBegin(GL_LINES);
	for(CTIT it = tree.begin(); it != tree.end(); ++it){
		const RRT_NODE* node = *it;
		const RRT_NODE* parent = (*it)->parent;
		if(!parent) continue;
		glVertex2d(parent->cfg.x, parent->cfg.y);
		glVertex2d(node->cfg.x, node->cfg.y);
	}
	glEnd();
}

void DrawPolygons(RRT_OBJS& objs){

	glColor3f(1.0, 0.5, 0.1);
	glLineWidth(1);

	for(RIT it = objs.begin(); it != objs.end(); ++it){
		//const vector<triangle>& tris = it->getTriangulation();
		glBegin(GL_LINE_LOOP);
		size_t size = it->getSize();
		for(size_t i=0;i<size;i++)
		{
			ply_vertex* p = it->operator [](i);

			glVertex2d(p->getPos()[0], p->getPos()[1]);


//			const triangle& tri = tris[i];
//			const ply_vertex* p0 = it->operator [](tri.v[0]);
//			const ply_vertex* p1 = it->operator [](tri.v[1]);
//			const ply_vertex* p2 = it->operator [](tri.v[2]);
//			glVertex2dv(&p0->getPos()[0]);
//			glVertex2dv(&p1->getPos()[0]);
//			glVertex2dv(&p2->getPos()[0]);
		}
		glEnd();
	}

}

void DrawPath(const RRT_PATH& path)
{
	glColor3f(0.0, 0, 1.0);
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	for(CPIT it = path.begin(); it != path.end(); ++it){
		glVertex2d(it->x, it->y);
	}
	glEnd();
}

void DrawRobot(RRT_ROBOT& robot, const RRT_CFG& cfg)
{
	robot.setCFG(cfg);

	glLineWidth(3);
	glBegin(GL_LINES);

	Vector2d v1 = robot.p[1];
	Vector2d v0 = robot.p[0];
	Vector2d v2 = robot.p[2];

	glColor3f(0.0, 1.0, 0.0);
	glVertex2d(v0[0], v0[1]);
	glVertex2d(v1[0], v1[1]);

	glColor3f(1.0, 0.0, 1.0);
	glVertex2d(v0[0], v0[1]);
	glVertex2d(v2[0], v2[1]);
	glEnd();
}

void DrawAll()
{
	DrawPolygons(rrt->getPolygons());
	DrawTree(rrt->getTree());
	DrawPath(rrt->getPath());

	DrawRobot(rrt->getRobot(), rrt->getStart());
	DrawRobot(rrt->getRobot(), rrt->getGoal());

	static const RRT_PATH& path = rrt->getPath();

	if(path_index < 0) path_index = 0;
	if(path_index >= path.size()) path_index = path.size()-1;

	if(!path.empty())
		DrawRobot(rrt->getRobot(), path[path_index]);


}


void DrawLine(const Vector3d& p1, const Vector3d& p2)
{
	GLUquadricObj * IDquadric=gluNewQuadric();      // Create A Pointer To The Quadric Object
	gluQuadricNormals(IDquadric, GLU_SMOOTH);  		// Create Smooth Normals
	gluQuadricTexture(IDquadric, GL_TRUE);    		// Create Texture Coords

	double len = (p2-p1).norm();
	Vector3d v = (p2 - p1).normalize();

	Vector3d z = Vector3d(0,0,1);
	Vector3d t = z%v;					// rotation axis

	double angle = acos (z*v) * 180.0 /PI;		// rotation degree
	glPushMatrix();
	glTranslatef(-p1[0],-p1[1],-p1[2]);
	glRotated(angle,t[0],t[1],t[2]);

	gluCylinder(IDquadric,0.005*R,0.005*R,len,32,32);

	glPopMatrix();

	gluDeleteQuadric(IDquadric);
}

void Display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//DisplayBackground();


	glPushMatrix();
	glLoadIdentity();
	static GLfloat light_position1[] = {  100, 100, 100.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	static GLfloat light_position2[] = { -100, -100, 50.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glPopMatrix();


	glPushMatrix();
	glTranslated(-COM[0],-COM[1],-COM[2]);
	DrawAll();
	glPopMatrix();

	updateWindownTitle();
}

//-----------------------------------------------------------------------------
// other regular openGL callback functions
bool InitGL()
{
	glShadeModel(GL_SMOOTH);
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

	// others
	glEnable( GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	glEnable(GL_NORMALIZE);
	glClearColor(1.0, 1.0, 1.0, 0.0);

	//Let's have light!
	GLfloat Diffuse[] =  { 0.9f, 0.9f, 0.9f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuse);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat WhiteLight[] =  { 0.75f, 0.75f, 0.75f, 1.0f };
	glLightfv(GL_LIGHT0,GL_DIFFUSE,WhiteLight);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,WhiteLight);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

    return true;
}

void Reshape( int w, int h)
{
    if(w>h)
        glViewport( 0, 0, (GLsizei)w, (GLsizei)w );
    else
        glViewport( 0, 0, (GLsizei)h, (GLsizei)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60, 1, 0.1, 10000000000 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

// show keys for controlling the rendering
void printGUIKeys()
{
	printf("GUI keys:\n");
	printf("  Space:  pause/resume animation\n");
	printf("      r:  reset\n");
	printf("      ,:  previous frame\n");
	printf("      .:  next frame\n");
}

void updateWindownTitle()
{
	stringstream ss;

	ss<<"2-Links RRT DEMO "<<filename;

	glutSetWindowTitle(ss.str().c_str());

	glutPostRedisplay();
}

void animate(int value)
{
	if(!animation) return;

	if(path_index + 1 < rrt->getPath().size())
	{
		path_index++;
		glutTimerFunc(50, animate, value);
	}else {
		animation = false;
	}
}



//keyboard event function
void Keyboard( unsigned char key, int x, int y )
{
    switch( key ){
        case 27: exit(0);
        case ' ':
        	animation = !animation;
        	animate(0);

        	break;
        case 'r': path_index = 0; break;
        case ',': path_index --;  break;
        case '.': path_index ++;  break;
    }

    glutPostRedisplay();
}

void resetCamera()
{
    //reset camera
    gli::setScale(1);
    gli::setCameraPosX(0);
    gli::setCameraPosY(0);
    //float R=max( (g_box[1]-g_box[0]), (g_box[3]-g_box[2]) );
    gli::setCameraPosZ(R*3);
    gli::setAzim(0);
    gli::setElev(0);
}


void Draw3D(int argc, char ** argv)
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE|GLUT_DEPTH | GLUT_MULTISAMPLE );
	glutInitWindowSize( 640, 640);
	glutInitWindowPosition( 50, 50 );
	glutCreateWindow( "rrt_demo" );

	InitGL();
	gli::gliInit();
	gli::gliDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);

	/////////////////////////////////////////////////////////////
	//set camera position
	resetCamera();

	animation = true;

	animate(0);

	printGUIKeys();

	updateWindownTitle();

	/////////////////////////////////////////////////////////////
	//printGUIUsage();
	gli::gliMainLoop();

}


