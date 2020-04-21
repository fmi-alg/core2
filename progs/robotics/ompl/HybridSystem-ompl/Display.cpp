// Header
#include "Display.h"

// Standard Library
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <ctime>

#define SQR(x) ((x)*(x))

// Global Variables in triangle-qt.cpp
extern std::string fileName;
extern Planner * planner;
extern int thickness;
extern int SearchType;
extern double boxWidth;
extern double boxHeight;
extern bool noPath;
extern bool showAnim;
extern bool pauseAnim;
extern int path_index;
extern bool replayAnim;
extern bool prm_graph;
extern bool rrt_graph;

// usleep((99-animationSpeed)*animationSpeedScale);
extern int animationSpeed;         // control the speed on the slider
extern int animationSpeedScale;    // the scale is used when we are not rendering the boxes


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

void Display::initializeGL() {
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    glClearDepth(1.0f);
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

void Display::renderScene() {

    if (!noPath) {
        drawPath(planner, planner->getPath());

        if (replayAnim) {
            replayAnim = false;

            showAnim = true;
            pauseAnim = false;
        }

        if(showAnim){
            usleep((99-animationSpeed)*animationSpeedScale);

            const PATH& path = planner->getPath();

            if(path_index < 0) path_index = 0;
            if(path_index >= path.size()) path_index = path.size()-1;

            if (pauseAnim) {
                drawRobot(planner->getRobot(), planner->to_physical(path[path_index]));
            }
            else {
                if(path_index+1 < planner->getPath().size()){
                    path_index++;
                    drawRobot(planner->getRobot(), planner->to_physical(path[path_index]));
                } else if(path_index+1 == planner->getPath().size()){
                    drawRobot(planner->getRobot(), planner->to_physical(path[path_index]));

                    showAnim=false;
                    path_index = 0;
                }
            }
            update();
        }
    }



    drawPolygons(planner->getPolygons());

    if(dynamic_cast<PRM*>(planner) != NULL && prm_graph) {
        PRM * prm=dynamic_cast<PRM*>(planner);
        drawGraph(prm, prm->getGraph());
        update();
    }
    if(dynamic_cast<RRT*>(planner) !=NULL && rrt_graph) {
        RRT * rrt=dynamic_cast<RRT*>(planner);
        drawTree(rrt, rrt->getTree());
        update();
    }

    drawRobot(planner->getRobot(), planner->to_physical(planner->getStart()));
    drawRobot(planner->getRobot(), planner->to_physical(planner->getGoal()));
}

//===========================================================//
//======            Functions to Draw Shapes           ======//
//===========================================================//
void Display::drawPath(Planner* planner, const PATH& path) {
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    for(CPIT it = path.begin(); it != path.end(); ++it){
        CFG cfg= planner->to_physical(*it);
        glVertex2d(cfg.x, cfg.y);
    }
    glEnd();
}

void Display::drawRobot(RRT_ROBOT& robot, const CFG& cfg) {
    robot.setCFG(cfg);

    if(thickness == 0)
        glLineWidth(3);
    else
        glLineWidth(thickness);

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

void Display::drawPolygons(RRT_OBJS& objs) {
    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(2);

    for(RIT it = objs.begin(); it != objs.end(); ++it) {
        glBegin(GL_LINE_LOOP);
        size_t size = it->getSize();
        for(size_t i=0;i<size;i++) {
            ply_vertex* p = it->operator [](i);

            glVertex2dv(p->getPos().get());
        }
        glEnd();
    }
}

void Display::drawTree(RRT* planner, const RRT_TREE& tree){
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

void Display::drawGraph(PRM* planner, const WDG& graph) {
    glDisable(GL_LIGHTING);

    ///////////////////////////////////////////////////////////////////////////
    glLineWidth(0.5);
    glColor3f(0,0,0);
    ///////////////////////////////////////////////////////////////////////////
    //draw nodes
    vector<VID> vids;
    graph.GetVerticesVID(vids);
    typedef vector<VID>::iterator NIT;
    glPointSize(3);
    glBegin(GL_POINTS);
    {for(NIT i=vids.begin();i!=vids.end();i++)
    {
        CFG cfg=planner->to_physical( graph.GetData(*i).getCFG() );
        glVertex2d(cfg.x,cfg.y);
    }}
    glEnd();

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
