#include "GL/gliLight.h"
#include <GL/gli.h>
#include "Euler_Ops.h"

/*The Rotation, Movement, Zoom in&out degree*/
double rotDeg=1;
double movDeg=10;
double zoomDeg=1;

double startX=0;                    	//start postion of mouse press 
double startY=0;
double startZ=0;

double base=0.0;                   	//Initialize the origin rotation arguments
double theta=0.0;
double rad=0.0;

double rotX=0.0;                    	//Initialize the rotation vector
double rotY=0.0;
double rotZ=0.0;

bool centered=false;
double centerX=0.0;                 	//Initialize the center of the polyhedron
double centerY=0.0;
double centerZ=0.0;

double scalar=1.0;                  	//Initialize the scalar of the polyhedron
double thetaX=0;
double thetaY=0;

double initRotDeg=45;			//Initialize the rotation of the polyhedron
double initRotAxisX=1;
double initRotAxisY=1;
double initRotAxisZ=1;


/* Chee: new parameters */
double radius=100.0;		  
double radius2=10.0;		 
double height=50.0;		
double height2=20.0;	
int nsegments=12;
int nsections=20;

/*Kai the second Part ( Like the legs of the chair )*/
int nsegments2=5;
int nsections2=1;
int nparts=10;
int nparts2=20;
double position2=80;

/*The third part ( Like the back of the chair ) */
double position3=90;
double thickness3=10;
double height3=50;

/*Kai: Rotation Amplitude*/
int rotUnitX=5;
int rotUnitY=5;
int rotUnitZ=5;

int comLen=1000;
/*View Point*/
GLdouble  	eyeX=0;
GLdouble  	eyeY=0;
GLdouble  	eyeZ=0;
GLdouble  	cX=0;
GLdouble  	cY=0;
GLdouble  	cZ=0;
GLdouble  	upX=0;
GLdouble  	upY=0;
GLdouble  	upZ=0;

GLsizei windowWidth = 512;	    // initial configuration box size
GLsizei windowHeight = 512;
GLsizei windowPosX = 200;           // initial Window position
GLsizei windowPosY = 200;	

/*Declaration of mouse operations*/
void pressMouse(int button,int state,int x,int y);
void holdMouse(int x,int y);

string inputDir("inputs"); 	    // Path for input files 
string fileName("cube.gwb"); 	  // Input file name (this is not used currently)
string model("chain");	        // model can be "cylinder", "sphere", "trefoil", etc.
string mode("mult_surface");		// mode can be "wire" or "face"


bool drawWire=true;
bool drawSolid=true;

extern Euler_Ops *eo;

/************************************************
 * GUI STUFF:
 ************************************************/

/*Initialize the window paint*/

void MyInit(void)
{
  glClearColor(1,1,1,0);
  glColor3f(1,0,0);
  glPointSize(10);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  //gluOrtho2D(0,(GLdouble)ww,0.0,(GLdouble)wh);
  glOrtho(-100.0,(GLdouble)windowWidth,-100.0,(GLdouble)windowHeight,-1000,1000);
  //gluLookAt(eyeX,eyeY,eyeZ,cX,cY,cZ,upX,upY,upZ);
}//MyInit

void displayPoly(Solid *s)
{
  /*Get Edges */
  Vec<Edge *> *es=s->sedges;

  Vec<Face *> *fs=s->sfaces;

  if (!centered)
  {
    /*center of the polyhedron*/
    Vec<double> *c=s->center();

    /*center coordinates of the polyhedron*/

    centerX=(*c)[0];
    centerY=(*c)[1];
    centerZ=(*c)[2];
    centered=true;
  }//if we do not have a center

  //draw solid model
  if (drawSolid)
  {
  
    /*Let us add a light*/
    glEnable(GL_LIGHTING);

    //offset of polygons so the wires are easier to see
    glEnable( GL_POLYGON_OFFSET_FILL );
    glPolygonOffset( 2.0, 2.0 );

    //default color. gray
    glColor3f(0.5,0.5,0.5);

    //loop through each face
    for (unsigned int i=0;i<fs->size();i++)
    {
      /*Now we get the face*/
      Face *f=(*fs)[i];

      /*Now we have the loops*/
      Vec< Loop *> *ls=f->floops;
      
      //compute face normal
    	{
        Loop * loop=(*ls)[0];
    		Vertex *v1=loop->ledg->start;
    		Vertex *v2=loop->ledg->nxthe->start;
    		Vertex *v3=loop->ledg->nxthe->nxthe->start;
    		double p1[3]={v1->getX(),v1->getY(),v1->getZ()}, 
    		 	     p2[3]={v2->getX(),v2->getY(),v2->getZ()}, 
    			     p3[3]={v3->getX(),v3->getY(),v3->getZ()};
    	
    		double vec1[3]={p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2]};
    		double vec2[3]={p3[0]-p1[0],p3[1]-p1[1],p3[2]-p1[2]};
    		double * n=crossProduct(vec2,vec1);
    		double d=n[0]*n[0]+n[1]*n[1]+n[2]*n[2];
    		if(d>0)
    		{
    			d=sqrt(d);
    			glNormal3d(n[0]/d,n[1]/d,n[2]/d);
    			//cout<<"n="<<n[0]/d<<","<<n[1]/d<<","<<n[2]/d<<endl;
    		}
    		delete [] n;
    	}

      //draw face
      for (unsigned int j=0;j<ls->size();j++)
      {
        
        /*Leading HalfEdge*/
        HalfEdge *lhe=(*ls)[j]->ledg;
        HalfEdge *the=lhe->prvhe;

        /*Start vertex*/
        Vertex *v=lhe->start;

     	  //draw polygon
        glBegin(GL_POLYGON);
  	      glVertex3f(v->getX(),v->getY(),v->getZ());
      	  while(the!=lhe){
          	    v=the->start;
              	glVertex3f(v->getX(),v->getY(),v->getZ());
  	            the=the->prvhe;
      	  }//while
        glEnd();

      }//for j
    }//for i

    //set things back
    glDisable( GL_POLYGON_OFFSET_FILL );
    glDisable(GL_LIGHTING);
  }//end draw solid


  //Wire Solids
  if (drawWire)
  {
    glColor3d(0,0,1);
    glBegin(GL_LINES);
    for (unsigned int i=0;i<es->size();i++){
      Edge *e=(*es)[i];
      Vertex *v1=e->he1->start;
      Vertex *v2=e->he2->start;
      glVertex3f(v1->getX(),v1->getY(),v1->getZ());
      glVertex3f(v2->getX(),v2->getY(),v2->getZ());

    }
    glEnd();
  }//wire

}//displaypoly

/*Display the polyhedron*/
void display(void)
{
  //Init Draw
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_MODELVIEW);

  for (unsigned int i=0;i<eo->solids->size();i++)
  {
  	Solid *s=(*(eo->solids))[i];
    displayPoly(s);
  }

}//display without scalar

void resetCamera()
{
	double model_radus=1000;
	
    //reset camera
    gli::setScale(1.25);
    gli::setCameraPosZ(2*model_radus);
    gli::setCameraPosX(0);
    gli::setCameraPosY(0);
    gli::setAzim(0);
    gli::setElev(0);
    
	glutPostRedisplay();
}


/*Key Pressed*/
void keyPressed(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 'r': resetCamera(); break;
    case 'w': drawWire=!drawWire; break;
    case 's': drawSolid=!drawSolid; break;  
  }
	
	glutPostRedisplay();
}

void setupLight()
{
    //Let's have light!
    double dist=100;
    light0_position[0]=dist;
    light0_position[1]=50+dist*2;
    light0_position[2]=dist;

    light1_position[0]=-dist;
    light1_position[1]=dist*2;
    light1_position[2]=dist*2;


    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glEnable(GL_LIGHT1);


    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_black);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_low_shininess);

}

bool InitGL()
{
    // *Antialias*
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST );

    glShadeModel(GL_SMOOTH);

    // others
    glEnable( GL_DEPTH_TEST);
    glClearColor( 1,1,1,1.0 );


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //let's have light
    setupLight();

    return true;
}

void Reshape( int w, int h)
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluPerspective(60, w*1.0f/h, 0.1, 10000 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void createGUI(int argc,char **argv)
{
   /*Prepare for the window*/
   glutInit(&argc,argv);
   glutInitDisplayMode( GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH );
   glutInitWindowSize(windowWidth,windowHeight);
   glutInitWindowPosition(windowPosX,windowPosY);
   glutCreateWindow(argc>2?model.c_str():"GWB");

   InitGL();
   gli::gliInit();
   gli::gliDisplayFunc(display);
   gli::gliKeyboardFunc(keyPressed);
   glutReshapeFunc(Reshape);
   
   resetCamera();
   
   glutMainLoop(); 
}

