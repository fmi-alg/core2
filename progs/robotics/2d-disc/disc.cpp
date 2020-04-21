// robotics2D.cpp : Defines the entry point for the console application.
//
#include "QuadTree.h"
#include "PriorityQueue.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.h"

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif

#include <gl/glut.h>
//#include <GL/glui.h>
//#include "GL/glui.h"
#include "glui.h"
#include <set>

using namespace std;

QuadTree* QT;
//double alpha[2] = {50, 312};
//double beta[2] = {256, 62};
double alpha[2] = {10, 360};
double beta[2] = {500, 20};
double epsilon = 1;
Box* boxA;
Box* boxB;
bool noPath = true;
double boxWidth = 512;
double boxHeight = 512;
double R0 = 30;
int windowPosX = 400;
int windowPosY = 200;
string fileName("input2.txt"); 
int QType = 0;
//glui controls
GLUI_RadioGroup* radioQType;
GLUI_EditText* editInput;
GLUI_EditText* editRadius;
GLUI_EditText* editEpsilon;
GLUI_EditText* editAlphaX;
GLUI_EditText* editAlphaY;
GLUI_EditText* editBetaX;
GLUI_EditText* editBetaY;

void renderScene(void);
void parseConfigFile(Box*);
void run();
void genEmptyTree();
void drawPath(vector<Box*>&);


int main(int argc, char* argv[])
{
	//vector<Box*> bv;
	//for (int i = 0; i < 10; ++i)
	//{
	//	bv.push_back(new Box(0, 0, 0, 0));
	//}
	//for (int i = 0; i < 10; ++i)
	//{
	//	bv[i]->dist2Source = 9-i;
	//}
	//distHeap::makeHeap(bv);
	//Box* bbb = distHeap::extractMin(bv);
	//bbb->dist2Source = 0.5;
	//distHeap::insert(bv, bbb);
	//distHeap::decreaseKey(bv, bv[3], 0.5);


	genEmptyTree();

	glutInit(&argc, argv);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutInitWindowSize(boxWidth, boxWidth);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	int windowID = glutCreateWindow("Motion Planning");
	glutDisplayFunc(renderScene);
	GLUI_Master.set_glutIdleFunc( NULL );
	GLUI *glui = GLUI_Master.create_glui( "control", 0, windowPosX + boxWidth + 20, windowPosY );
	
	editInput = glui->add_edittext( "Input file:", GLUI_EDITTEXT_TEXT );
	editInput->set_text((char*)fileName.c_str());
	editRadius = glui->add_edittext( "Radius:", GLUI_EDITTEXT_FLOAT );
	editRadius->set_float_val(R0);
	editEpsilon = glui->add_edittext( "Epsilon:", GLUI_EDITTEXT_FLOAT );
	editEpsilon->set_float_val(epsilon);
	editAlphaX = glui->add_edittext( "alpha.x:", GLUI_EDITTEXT_FLOAT );
	editAlphaX->set_float_val(alpha[0]);
	editAlphaY = glui->add_edittext( "alpha.y:", GLUI_EDITTEXT_FLOAT );
	editAlphaY->set_float_val(alpha[1]);
	editBetaX = glui->add_edittext( "Beta.x:", GLUI_EDITTEXT_FLOAT );
	editBetaX->set_float_val(beta[0]);
	editBetaY = glui->add_edittext( "Beta.y:", GLUI_EDITTEXT_FLOAT );
	editBetaY->set_float_val(beta[1]);

	glui->add_separator();
	radioQType = glui->add_radiogroup();
	glui->add_radiobutton_to_group(radioQType, "Sequential");
	glui->add_radiobutton_to_group(radioQType, "Random");
	glui->add_separator();

	GLUI_Button* buttonRun = glui->add_button( "Run", -1, (GLUI_Update_CB)run);

	glui->set_main_gfx_window( windowID );
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
	QT = new QuadTree(root, epsilon, QType);
}

void run()
{
	//update from glui live variables
	fileName = editInput->get_text();
	R0 = editRadius->get_float_val();
	epsilon = editEpsilon->get_float_val();
	alpha[0] = editAlphaX->get_float_val();
	alpha[1] = editAlphaY->get_float_val();
	beta[0] = editBetaX->get_float_val();
	beta[1] = editBetaY->get_float_val();
    QType = radioQType->get_int_val();

	genEmptyTree();

	noPath = false;

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

	int ct = 0;
	while(!noPath && !QT->isConnect(boxA, boxB))
	{
		++ct;
		if (!QT->expand())
		{
			noPath = true;
		}

	}

	glutPostRedisplay();

	cout << "path found is " << !noPath << endl;
	cout << "expended " << ct << " times" << endl;
}

void drawPath(vector<Box*>& path)
{
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);	
	glVertex2f(beta[0], beta[1]);
	for (int i = 0; i < path.size(); ++i)
	{
		glVertex2f(path[i]->x, path[i]->y);
	}
	glVertex2f(alpha[0], alpha[1]);
	glEnd();
}

void drawQuad(Box* b)
{
	switch(b->status)
	{
	case Box::FREE:
		glColor3f(0, 1, 0);
		break;
	case Box::STUCK:
		glColor3f(1, 0, 0);
		break;
	case Box::MIXED:
		glColor3f(1, 1, 0);
		if (b->height < epsilon || b->width < epsilon)
		{
			glColor3f(0.5, 0.5, 0.5);
		}

		break;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex2f(b->x - b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y + b->height / 2);
	glVertex2f(b->x - b->width / 2, b->y + b->height / 2);
	glEnd();

	glColor3f(0, 0 , 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	glVertex2f(b->x - b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y - b->height / 2);
	glVertex2f(b->x + b->width / 2, b->y + b->height / 2);
	glVertex2f(b->x - b->width / 2, b->y + b->height / 2);
	glEnd();
}

void drawWalls(Box* b)
{
	glColor3f(1, 1, 1);
	for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end(); ++iter)
	{
		Wall* w = *iter;
		glBegin(GL_LINES);
		glVertex2f(w->src->x, w->src->y);
		glVertex2f(w->dst->x, w->dst->y);
		glEnd();
	}
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

void drawCircle( float Radius, int numPoints, double x, double y)
{
	glBegin( GL_LINE_STRIP );
	for( int i = 0; i <= numPoints; ++i )
	{
		float Angle = i * (2.0* 3.1415926 / numPoints);  
		float X = cos( Angle )*Radius;  
		float Y = sin( Angle )*Radius;
		glVertex2f( X + x, Y + y);
	}
	glEnd();
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glScalef(2.0/boxWidth, 2.0/boxHeight, 0);
	glTranslatef(-boxWidth/2, -boxHeight/2, 0);

	treeTraverse(QT->pRoot);

	glPolygonMode(GL_FRONT, GL_LINE);

	glColor3f(0, 0, 1);
	//glPointSize(R0*2);
	//glEnable( GL_POINT_SMOOTH );
	//glBegin(GL_POINTS);
	//glVertex2f(alpha[0], alpha[1]);
	//glVertex2f(beta[0], beta[1]);
	//glEnd();
	drawCircle(R0, 100, alpha[0], alpha[1]);
	drawCircle(R0, 100, beta[0], beta[1]);

	drawWalls(QT->pRoot);

	if (!noPath)
	{
		vector<Box*> path = Graph::findPath(boxA, boxB);
		drawPath(path);
	}

	glutSwapBuffers();
}

void parseConfigFile(Box* b)
{	
	ifstream ifs(fileName.c_str());
	if (!ifs)
	{
		cerr<< "cannot open input file" << endl;
		exit(1);
	}

	int nPt, nFeature;
	ifs >> nPt;
	vector<double> pts(nPt*2);
	for (int i = 0; i < nPt; ++i)
	{
		ifs >> pts[i*2] >> pts[i*2+1];
	}
	ifs >> nFeature;
	string temp;
	std::getline(ifs, temp);
	for (int i = 0; i < nFeature; ++i)
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
				ptVec.push_back(new Corner(pts[pt*2], pts[pt*2+1]));
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
