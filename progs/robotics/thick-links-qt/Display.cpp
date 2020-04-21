/*
 *  Display.cpp
 *  
 *  Author: Ching-Hsiang (Tom) Hsu
 *  Email: chhsu@nyu.edu
 *  Modified: Ching-Hsiang (Tom) Hsu, Oct. 31, 2016
 */

// Header
#include "Display.h"

// Standard Library
#include <iostream>
#include <math.h>
#include <ctime>

extern FILE *fp;

// Global Variables in 2-links.cpp
extern QuadTree* QT;
extern std::string fileName;
//extern std::string inputDir;
extern double alpha[4];
extern double beta[4];
extern double epsilon;
extern double L1;
extern double L2;
extern double R0;
extern double thickness;
extern int QType;
extern int seed;
extern double boxWidth;
extern double boxHeight;
extern Box* boxA;
extern Box* boxB;
extern bool noPath;

extern bool hideBox;
extern bool hideBoxBoundary;
extern bool showTrace;
extern bool showPath;
extern bool showFilledObstacles;
extern bool showRobot;
extern bool safeAngle;
extern bool runAnim;
extern bool pauseAnim;
extern bool replayAnim;
extern bool inPoly;

extern int animationSpeed;
extern int animationSpeedScale;
extern int animationSpeedScaleBox;

extern double bandwidth;

extern vector<Box*> PATH;
extern vector<int> expansions;
extern vector<Polygon> polygons;

// color coding variable ========================================
extern Color clr_totalFREE;
extern Color clr_partialFREE;
extern Color clr_MIXED;
extern Color clr_STUCK;
extern Color clr_EPS;
extern Color clr_UNKNOWN;
extern Color clr_start;
extern Color clr_goal;
extern Color clr_path;
extern Color clr_robot;
extern Color clr_obstacle;
extern Color clr_boundary;

unsigned int inc(0);

// Global Variables in MainWindow.cpp

// Member Variables
static int numQuads = 0;

int renderSteps(1);
bool step(false);

#include <unistd.h>

unsigned int pSize;
int lim;

bool doTriangulation(false);
vector<Triangle> triangles;

/*
 * CONSTRUCTOR
 *
 * Define data members
 */
Display::Display(QWidget* parent):
    QOpenGLWidget(parent)
{}

/*
 * DESTRUCTOR
 *
 * Destroy shader program
 */
Display::~Display()
{}

/*
 * INITIALIZE GL
 *
 * Called once to prepare display window for
 * rendering.
 * Creates and initializes shader program
 */
void Display::initializeGL() {
    //initializeOpenGLFunctions();

    // Set background color
    glClearColor(1, 1, 1, 1.0);
    //else       glClearColor(0.6,0.6,0.6,1);
    glClearDepth(1.0f);
    //glEnable(GL_DOUBLEBUFFER);
}

/*
 * RESIZE GL
 *
 * Sets size of viewport.
 * If 'Display' is the wooden frame of a painting,
 * the viewport is the canvas.
 */
void Display::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*
 * PAINT GL
 *
 * Regenerates (if necessary) and draws scene to
 * dispaly screen.
 */
void Display::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    renderScene();
    glFlush();
}

void Display::renderScene(){

    if (!hideBox) {
        putBoxes();
    }

    Pose start = Pose(alpha[0], alpha[1], alpha[2], alpha[3]);
    Pose goal = Pose(beta[0], beta[1], beta[2], beta[3]);

    drawObstacles();

    drawRobot(start, 0.4);
    drawRobot(goal, 1);


    if (!noPath) {
        if(showPath) drawPath();

        if(showTrace){
            for(int i=0;i<(int)PATH.size();i+=40){
                double gradient = i;
                gradient = 1-gradient*0.6/(double)PATH.size();
                drawRobot(PATH.at(i), gradient);
            }
        }

        if (replayAnim) {
            replayAnim = false;

            runAnim = true;
            pauseAnim = false;
            inc = 0;
        }

        if(runAnim){
            if (pauseAnim) {
                double gradient = pSize-inc-1;
                gradient = 1-gradient*0.6/(double)PATH.size();
                drawRobot(PATH.at(pSize-inc-1), gradient);
            }
            else {
                pSize=PATH.size();
                if(inc<pSize){
                    double gradient = pSize-inc-1;
                    gradient = 1-gradient*0.6/(double)PATH.size();
                    drawRobot(PATH.at(pSize-inc-1), gradient);
                    inc++;

                    if(hideBox) usleep(  (99-animationSpeed) * animationSpeedScale);
                    else        usleep(  (99-animationSpeed) * animationSpeedScaleBox);
                }
                else{
                    inc=0;
                    runAnim=false;
                }
            }
            update();
        }
    }
}


double calcSafeAngle(double lowerBound, double upperBound){
    // degree
    if(lowerBound < upperBound){
        return (lowerBound+upperBound)*0.5f;
    }
    else{
        double diff_angle = upperBound+(360-lowerBound);
        double mid_angle = lowerBound+diff_angle*0.5f;
        if(mid_angle >= 360) mid_angle -= 360.0f;
        return mid_angle;
    }
}

double theta1;
double theta2;
double oldAngleForRender1 = -1;
double oldAngleForRender2 = -1;
void Display::drawRobot(Box* b, double gradient){
    if(!showRobot) return;

//    if(!showTrace){
//        window->showAngleBound(b->xi[Box::LOWER1], b->xi[Box::UPPER1], b->xi[Box::LOWER2], b->xi[Box::UPPER2]);
//    }

    //fprintf(stderr, "%lf %lf   %lf %lf\n", b->xi[Box::LOWER1], b->xi[Box::UPPER1], b->xi[Box::LOWER2], b->xi[Box::UPPER2]);

    theta1 = b->xi[Box::LOWER1];
    theta2 = b->xi[Box::UPPER2];

    if(safeAngle){
        theta1 = calcSafeAngle(b->xi[Box::LOWER1], b->xi[Box::UPPER1]);
        theta2 = calcSafeAngle(b->xi[Box::LOWER2], b->xi[Box::UPPER2]);
    }

    if (crossingOption) {
        double max = 0;
        // may have problems to do this bound check
        if (oldAngleForRender1 <= b->xi[1] && oldAngleForRender1 >= b->xi[0] &&
            oldAngleForRender2 <= b->xi[3] && oldAngleForRender2 >= b->xi[2] &&
            angleDistance(oldAngleForRender1, oldAngleForRender2) > bandwidth) {
            theta1 = oldAngleForRender1;
            theta2 = oldAngleForRender2;
        } else if (b->order == Box::LT) {
            for (int j = b->xi[0]; j <= b->xi[1]; j += 3) {
                for (int k = b->xi[2]; k <= b->xi[3]; k += 3) {
                    if (angleDistance(j, k) > max && j < k) {
                        max = angleDistance(j, k);
                        theta1 = j;
                        theta2 = k;
                        if (max == 180) {
                            break;
                        }
                    }
                }
            }
        } else if (b->order == Box::GT) {
            for (int j = b->xi[0]; j <= b->xi[1]; j += 3) {
                for (int k = b->xi[2]; k <= b->xi[3]; k += 3) {
                    if (angleDistance(j, k) > max && j > k) {
                        max = angleDistance(j, k);
                        theta1 = j;
                        theta2 = k;
                        if (max == 180) {
                            break;
                        }
                    }
                }
            }
        }
        oldAngleForRender1 = theta1;
        oldAngleForRender2 = theta2;
    }

//    fprintf(fp, "%lf %lf theta %lf %lf  %lf %lf  %lf %lf\n", b->x, b->y,
//            b->xi[Box::LOWER1], b->xi[Box::UPPER1],
//            b->xi[Box::LOWER2], b->xi[Box::UPPER2],
//            theta1, theta2);

    double deg2rad = PI/180.0f;
    Pose o(b->x, b->y);
    Pose p1(b->x+L1*cos(theta1*deg2rad), b->y+L1*sin(theta1*deg2rad));
    Pose p2(b->x+L2*cos(theta2*deg2rad), b->y+L2*sin(theta2*deg2rad));

    drawLink(o, p2, Color(0,gradient,0));
    drawCircle(p2, Color(0,gradient,0));
    drawLink(o, p1, Color(gradient,0,gradient));
    drawCircle(p1, Color(gradient,0,gradient));
    drawCircle(o, Color(gradient,0,gradient));
}

void Display::drawRobot(Pose p, double gradient){
    if(!showRobot) return;
    double deg2rad = PI/180.0f;
    Pose o(p.x, p.y);
    Pose p1(p.x+L1*cos(p.t1*deg2rad), p.y+L1*sin(p.t1*deg2rad));
    Pose p2(p.x+L2*cos(p.t2*deg2rad), p.y+L2*sin(p.t2*deg2rad));

    drawLink(o, p2, Color(0,gradient,0));
    drawCircle(p2, Color(0,gradient,0));
    drawLink(o, p1, Color(gradient,0,gradient));
    drawCircle(p1, Color(gradient,0,gradient));
    drawCircle(o, Color(gradient,0,gradient));
}


int cmp(const void * aa, const void * bb) {
    Pose a = *(Pose*)aa;
    Pose b = *(Pose*)bb;

    if(a.x < b.x) return -1;
    else if(a.x == b.x){
        if(a.y < b.y) return -1;
        else          return 1;
    }
    else          return 1;
}
void Display::drawLink(Pose a, Pose b, Color clr) {
//    glColor3fv(clr.rgb);
//    glLineWidth(thickness*2.0f);
//    glBegin(GL_LINE_STRIP);
//        glVertex2f(a.x, a.y);
//        glVertex2f(b.x, b.y);
//    glEnd();
//    glLineWidth(1);

    // remember to do the exchange of the vector
    double vec_x = b.x-a.x;
    double vec_y = b.y-a.y;
    double norm2 = sqrt(vec_x*vec_x+vec_y*vec_y);
    vec_x = -(b.y-a.y)/norm2;
    vec_y = (b.x-a.x)/norm2;

    Pose p[4];

    int dx[4] = {1,-1,1,-1};
    int dy[4] = {1,-1,1,-1};

    thickness *= 0.5;
    for(int i=0;i<2;++i){
        p[i]  = Pose(a.x+dx[i]*thickness*vec_x, a.y+dy[i]*thickness*vec_y);
    }
    for(int i=2;i<4;++i){
        p[i]  = Pose(b.x+dx[i]*thickness*vec_x, b.y+dy[i]*thickness*vec_y);
    }
    thickness *= 2;


    qsort(p, 4, sizeof(Pose), cmp);

    if(p[0].y > p[3].y){
        drawTriangle(p[0], p[3], p[1], clr);
        drawTriangle(p[0], p[2], p[3], clr);
    }
    else{
        drawTriangle(p[0], p[3], p[2], clr);
        drawTriangle(p[0], p[1], p[3], clr);
    }
}

void Display::drawCircle(Pose a, Color clr){
    glColor3fv(clr.rgb);
    glBegin(GL_TRIANGLE_FAN);
    for(int ii = 0; ii < 360; ii++) {
        GLfloat theta = PI*GLfloat(ii)/180.0f;
        GLfloat x = thickness*0.5f*cosf(theta);
        GLfloat y = thickness*0.5f*sinf(theta);
        glVertex2f(x + a.x, y + a.y);
    }
    glEnd();
}

void Display::putBoxes(){
    Box* tmp;
    if(step){
        for(unsigned int i=0;  i<(unsigned int)Box::pAllLeaf->size()&&i<(unsigned int)expansions.at(renderSteps);i++){
            tmp=Box::pAllLeaf->at(i);
            drawBoxes(tmp, epsilon);
            numQuads++;
        }
    }
    //NOTE: This draws ALL quads, including non leaves - this is overkill, and wastes time and space.
    //Can we do this better?
    else{
        for(unsigned int i=0;i<(unsigned int)Box::pAllLeaf->size();i++){
            tmp = Box::pAllLeaf->at(i);
            if(!tmp->isLeaf) continue;
            drawBoxes(tmp, epsilon);
            numQuads++;
        }
    }
}

void Display::drawBoxes(Box* b, double epsilon) {
    Color clr_box = clr_MIXED;

    switch(b->status)
    {
        case Box::FREE:
            // color is dark green, representing completed free angular ranges for both links
            // Note: the angular range of link i is the full circle iff LOWERi=0 and UPPERi=360 (i=1 or 2).
            if (b->xi[Box::LOWER1] != 0 || b->xi[Box::UPPER1] != 360 || b->xi[Box::LOWER2] != 0 || b->xi[Box::UPPER2] != 360) {
                clr_box = clr_totalFREE;
            }
            // light green
            else {
                clr_box = clr_partialFREE;
            }
            break;
        case Box::STUCK:  //color is red
            clr_box = clr_STUCK;
            break;
        case Box::MIXED: //color is yellow (if box is epsilon-large)
            if (b->height < epsilon || b->width < epsilon) { // color is gray (if box is epsilon-small)
                clr_box = clr_EPS;
            }
            break;
        default:
            //(*window) << "UNKNOWN box status in genQuad\n";
            break;
    }
    // 1st Corner: lower left
    Pose lower_left  = Pose(b->x - b->width / 2, b->y - b->height / 2);
    // 2nd Corner: lower right
    Pose lower_right = Pose(b->x + b->width / 2, b->y - b->height / 2);
    // 3rd Corner: upper right
    Pose upper_right = Pose(b->x + b->width / 2, b->y + b->height / 2);
    // 4th Corner: upper left
    Pose upper_left  = Pose(b->x - b->width / 2, b->y + b->height / 2);

    drawQuadrilateral(lower_left, lower_right, upper_right, upper_left, clr_box);

    // 1 2 3 4
    if (!hideBoxBoundary) {
        drawQuadrilateralOutline(lower_left, lower_right, upper_right, upper_left, Color(0,0,0,1));
    }
}

void Display::drawQuadrilateral(Pose a, Pose b, Pose c, Pose d, Color clr){
    glColor4fv(clr.rgba);
    glBegin(GL_POLYGON);
        glVertex2f( a.x, a.y );
        glVertex2f( b.x, b.y );
        glVertex2f( c.x, c.y );
        glVertex2f( d.x, d.y );
        glVertex2f( a.x, a.y );
    glEnd();
}

void Display::drawQuadrilateralOutline(Pose a, Pose b, Pose c, Pose d, Color clr){
    glColor3fv(clr.rgb);
    glLineWidth(2.0);
    glBegin(GL_LINE_STRIP);
        glVertex2f( a.x, a.y );
        glVertex2f( b.x, b.y );
        glVertex2f( c.x, c.y );
        glVertex2f( d.x, d.y );
        glVertex2f( a.x, a.y );
    glEnd();
    glLineWidth(1.0);
}

void Display::drawObstacles() {
    Box* b = QT->pRoot;
    glLineWidth(3.0);
    for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end(); ++iter) {
        Wall* w = *iter;
        glColor4fv(clr_obstacle.rgba);
        glBegin(GL_LINE_STRIP);
            glVertex2f(w->src->x, w->src->y);
            glVertex2f(w->dst->x, w->dst->y);
        glEnd();
    }
    glLineWidth(1.0);

    if(showFilledObstacles){        
        if(!doTriangulation){
            doTriangulation = true;
            for(int p=0;p<(int)polygons.size();++p){
                Vector2dVector a;
                for(int j=0;j<(int)polygons[p].corners.size()-1;++j){
                    a.push_back(Vector2d(polygons[p].corners[j]->x, polygons[p].corners[j]->y));
                }
                // allocate an STL vector to hold the answer.
                Vector2dVector result;
                //  Invoke the triangulator to triangulate this polygon.
                Triangulate::Process(a,result);
                // print out the results.
                int tcount = result.size()/3;

                for (int i=0; i<tcount; i++){
                    const Vector2d &p1 = result[i*3+0];
                    const Vector2d &p2 = result[i*3+1];
                    const Vector2d &p3 = result[i*3+2];
                    Triangle tmp(Pose(p1.GetX(),p1.GetY()), Pose(p2.GetX(),p2.GetY()), Pose(p3.GetX(),p3.GetY()));
                    triangles.push_back(tmp);
                }
            }
        }
        else{
            for(int tri=0;tri<(int)triangles.size();++tri){
                drawTriangle(triangles[tri].a, triangles[tri].b, triangles[tri].c, Color(0.8,0.8,0.8));
            }
        }
    }
}

void Display::drawTriangle(Pose a, Pose b, Pose c, Color clr) {
    glColor3fv(clr.rgb);
    glBegin(GL_TRIANGLES);
        glVertex2f( a.x, a.y );
        glVertex2f( b.x, b.y );
        glVertex2f( c.x, c.y );
    glEnd();
}

void Display::drawPath() {
    glColor3fv(clr_path.rgb);
    glLineWidth(3.0);
    glBegin(GL_LINE_STRIP);
        for (int i = (int)PATH.size()-1; i >=0; --i) {
            glVertex2f(PATH[i]->x, PATH[i]->y);
        }
    glEnd();
    glLineWidth(1.0);
}
