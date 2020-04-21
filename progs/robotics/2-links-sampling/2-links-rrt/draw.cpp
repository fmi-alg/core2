/*
 * draw.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: zhonghua
 */
//#include <unistd.h>
#include <sstream>
using namespace std;

#include "draw.h"

#define SQR(x) ((x)*(x))

extern bool debug;
extern bool check;

extern string filename;
extern Planner * planner;

extern float env_TR;				// TRANSLATIONAL RESOLUTION
extern float env_RR;				// Rotational RESOLUTION (deg)

float c_theta = 0.0;
uint c_index = 0;
bool animation = false;
bool reverse_order = false;
bool show_roadmap_node_as_robot=false;
bool show_path_node_as_robot=false;
bool show_roadmap=true;
bool show_path=true;
int current_frame = 0;

int maxFrames = 64*2;
float z_rot = 0.0;

bool showF=true, showE=true, showV=false, showC=true, showT=true, showL=true, showP=true;
double COM[3] = {250, 250, 0};
double R = 250; //center and radius

int path_index = 0;

void DrawRobot(RRT_ROBOT& robot, const CFG& cfg);


static PATH path;

void drawCircle(float radius, float angle, bool fill)

{

#if GL_ON

    float delta=0.2f;

    angle=2*PI-angle;

    float s=PI+angle/2; float e=3*PI-angle/2;



    /////////////////////////////////////////////////////////////

    if(!fill)

        glBegin(GL_LINE_LOOP);

    else 

        glBegin(GL_POLYGON);



    {if( angle>0 ) glVertex3f( 0, 0.2f, 0 );

    for(float theta=s; theta<e ; theta += delta )

        glVertex3f( radius*sin(theta), 0.2f, radius*cos(theta) );

    glVertex3f( radius*sin(e), 0.2f, radius*cos(e) );}

    glEnd();

#endif

}



void drawArrow(float radius)

{

#if GL_ON

    glBegin(GL_TRIANGLES);

    glVertex3d(0.4,0.2,radius);

    glVertex3d(-0.4,0.2,radius);

    glVertex3d(0,0.2,0.6+radius);

    glEnd();



    glBegin(GL_LINES);

    glVertex3d(0,0.2,0);

    glVertex3d(0,0.2,radius);

    glEnd();

#endif//GL_ON

}



void drawLine(const Point2d & p1,const Point2d & p2, float * color)

{   

#if GL_ON

    glPushAttrib(GL_CURRENT_BIT);

    glBegin(GL_LINES);

    glColor3fv(color);

        glVertex3d(p1[0],0.2f,p1[1]);

        glVertex3d(p2[0],0.2f,p2[1]);

    glEnd();

    glPopAttrib();

#endif

}

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


void DrawTree(RRT* planner, const RRT_TREE& tree)
{
	glPointSize(3);
	glColor3f(0,0,0);
	for(CTIT it = tree.begin(); it != tree.end(); ++it)
	{
		const RRT_NODE* node = *it;
		CFG cfg=planner->to_physical(node->cfg);
		if(show_roadmap_node_as_robot)
			DrawRobot(planner->getRobot(), cfg);
		else
		{
			glBegin(GL_POINTS);
			glVertex2d(cfg.x,cfg.y);
			glEnd();
		}
    }
    
	glLineWidth(0.5);
	glColor3f(0,0,0);
	glBegin(GL_LINES);
	for(CTIT it = tree.begin(); it != tree.end(); ++it){
		const RRT_NODE* node = *it;
		const RRT_NODE* parent = (*it)->parent;
		if(!parent) continue;

		CFG ncfg=planner->to_physical(node->cfg);
		CFG pcfg=planner->to_physical(parent->cfg);
		glVertex2d(ncfg.x, ncfg.y);
		glVertex2d(pcfg.x, pcfg.y);
	}
	glEnd();
}


void DrawGraph(PRM* planner, const WDG& graph)
{
    float radius=10;

    glDisable(GL_LIGHTING);



    ///////////////////////////////////////////////////////////////////////////

	glLineWidth(0.5);
	glColor3f(0,0,0);

    //glColor3fv(m_color.get());

    ///////////////////////////////////////////////////////////////////////////

    //draw nodes

	vector<VID> vids;

	graph.GetVerticesVID(vids);

    typedef vector<VID>::iterator NIT;

	glPointSize(3);

	

    {for(NIT i=vids.begin();i!=vids.end();i++)

	{

		CFG cfg=planner->to_physical( graph.GetData(*i).getCFG() );
		
		if(show_roadmap_node_as_robot)
			DrawRobot(planner->getRobot(), cfg);
		else
		{
			glBegin(GL_POINTS);
			glVertex2d(cfg.x,cfg.y);
			glEnd();
		}
    }}

	



    ///////////////////////////////////////////////////////////////////////////

    //draw edges



	glColor4f(0.5,0.5,0.5,0.5);

    glPushAttrib(GL_CURRENT_BIT);

    vector< pair<VID,VID> > edges;

    typedef vector< pair<VID,VID> >::iterator EIT;

    graph.GetEdges(edges);

    glBegin(GL_LINES);

    {for(EIT i=edges.begin();i!=edges.end();i++){

		CFG p1=planner->to_physical(graph.GetData(i->first).getCFG());

		CFG p2=planner->to_physical(graph.GetData(i->second).getCFG());

		glVertex2d(p1.x,p1.y);

        glVertex2d(p2.x,p2.y);

    }}//end for

    glEnd();
}

void DrawPolygons(RRT_OBJS& objs)
{

	//glColor3f(1.0, 0.5, 0.1);
	glColor3f(0,0,0);
	glLineWidth(1);

	for(RIT it = objs.begin(); it != objs.end(); ++it){
		//const vector<triangle>& tris = it->getTriangulation();
		glBegin(GL_LINE_LOOP);
		size_t size = it->getSize();
		for(size_t i=0;i<size;i++)
		{
			ply_vertex* p = it->operator [](i);

			glVertex2dv(p->getPos().get());


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

void DrawPath(Planner* planner, const PATH& path)
{
	glColor3f(0.0, 0, 1.0);
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	for(CPIT it = path.begin(); it != path.end(); ++it){
		CFG cfg= planner->to_physical(*it);
		glVertex2d(cfg.x, cfg.y);
	}
	glEnd();
	
	if(show_path_node_as_robot)
	{
		for(CPIT it = path.begin(); it != path.end(); ++it)
		{
			CFG cfg= planner->to_physical(*it);
			DrawRobot(planner->getRobot(), cfg);
		}
	}
}

void DrawRobot(RRT_ROBOT& robot, const CFG& cfg)
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

void interpolate_path(PATH& path)
{
	PATH tmp;
	for(PATH::iterator i=path.begin();i!=path.end();i++)
	{
		PATH::iterator n=i;
		n++;
		if(n==path.end()){ tmp.push_back(*i); break; }
		CFG vec=((*n)-(*i));
		double dT=vec.normT();
		double dR=vec.normR();
		int steps=(int)ceil(max(dT/env_TR,dR/env_RR));
		
		CFG step=vec/steps;
		
		for(int s=0;s<steps;s++)
		{
			CFG cfg=(*i)+step*s;
			tmp.push_back(cfg);
		}
		
	}//end for
	
	//
	tmp.swap(path);
}

void DrawAll()
{
	DrawPolygons(planner->getPolygons());

	if(show_roadmap)
	{
		glPushMatrix();
		glTranslated(0,0,-.2);
		if(dynamic_cast<RRT*>(planner)!=NULL)
		{
			RRT * rrt=dynamic_cast<RRT*>(planner);
			DrawTree(rrt, rrt->getTree());
		}

		if(dynamic_cast<PRM*>(planner)!=NULL)
		{
			PRM * prm=dynamic_cast<PRM*>(planner);
			DrawGraph(prm, prm->getGraph());
		}
		glPopMatrix();
	}

	
	if(path.empty())
	{
		path=planner->getPath();
		interpolate_path(path);
	}
	
	if(show_path)
	{
		glPushMatrix();
		glTranslated(0,0,-.1);
		DrawPath(planner, path);
		glPopMatrix();
	}

	DrawRobot(planner->getRobot(), planner->to_physical(planner->getStart()));
	DrawRobot(planner->getRobot(), planner->to_physical(planner->getGoal()));

	if(path_index < 0) path_index = 0;
	if(path_index >= path.size()) path_index = path.size()-1;

	if(!path.empty())
		DrawRobot(planner->getRobot(), planner->to_physical(path[path_index]) );


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
	printf("      1:  show/hide roadmap\n");
	printf("      2:  show/hide path\n");
	printf("      3:  show/hide roadmap node as robot\n");
	printf("      4:  show/hide path node as robot\n");
}

void updateWindownTitle()
{
	stringstream ss;

	ss<<"2-Links RRT/PRM DEMO "<<filename;

	glutSetWindowTitle(ss.str().c_str());

	glutPostRedisplay();
}

void animate(int value)
{
	if(!animation) return;

	if(path_index + 1 < path.size())
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
        case '1': show_roadmap=!show_roadmap; break;
        case '2': show_path=!show_path; break;
        case '3': show_roadmap_node_as_robot=!show_roadmap_node_as_robot; break;
        case '4': show_path_node_as_robot=!show_path_node_as_robot; break;
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
	glutInitWindowSize( 800, 800);
	glutInitWindowPosition( 50, 50 );
	glutCreateWindow( "rrt_demo" );

	InitGL();
	gli::gliInit();
	gli::gliDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);

	/////////////////////////////////////////////////////////////
	//set camera position
	gli::set2DMode(true);
	resetCamera();

	animation = true;

	animate(0);

	printGUIKeys();

	updateWindownTitle();

	/////////////////////////////////////////////////////////////
	//printGUIUsage();
	gli::gliMainLoop();

}


