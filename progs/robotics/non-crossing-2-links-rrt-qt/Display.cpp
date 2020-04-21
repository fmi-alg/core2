/*
 * Display.cpp
 *
 *  Author: Ching-Hsiang (Tom) Hsu
 *  Email: chhsu@nyu.edu
 *  Modified: Oct, 31, 2016
 */

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
extern double thickness;
extern int SearchType;
extern double boxWidth;
extern double boxHeight;
extern bool noPath;
extern bool showAnim;
extern bool pauseAnim;
extern int path_index;
extern bool replayAnim;
extern bool prm_graph;
extern int prm_graph_flag;
extern bool prm_graph_mixed;
extern bool prm_graph_free;
extern bool prm_graph_obst;
extern bool prm_graph_edge;
extern bool rrt_graph;

extern bool showTrace;
extern bool showFilledObstacles;

// usleep((99-animationSpeed)*animationSpeedScale);
extern int animationSpeed;         // control the speed on the slider
extern int animationSpeedScale;    // the scale is used when we are not rendering the boxes


double color_table[1000][3];
bool doTriangulation(false);
vector<m_Vector2d> triangles;

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

    for(int i=0;i<1000;++i){
        for(int j=0;j<3;++j){
           color_table[i][j] = drand48();
        }
    }
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

    drawPolygons(planner->getPolygons());


    if(dynamic_cast<PRM*>(planner) != NULL && prm_graph) {
        PRM * prm=dynamic_cast<PRM*>(planner);
        drawGraph(prm, prm->getGraph());
        update();
    }
    if(dynamic_cast<TOGGLEPRM*>(planner) != NULL && prm_graph) {
        TOGGLEPRM * toggleprm=dynamic_cast<TOGGLEPRM*>(planner);
        if(prm_graph_free) drawFreeGraph(toggleprm, toggleprm->getFreeGraph(), toggleprm->getFreeSet());
        if(prm_graph_obst) drawObstGraph(toggleprm, toggleprm->getObstGraph());
        update();
    }
    if(dynamic_cast<LAZYTOGGLEPRM*>(planner) != NULL && prm_graph) {
        LAZYTOGGLEPRM * lazytoggleprm=dynamic_cast<LAZYTOGGLEPRM*>(planner);
        if(prm_graph_mixed) drawMixedGraph(lazytoggleprm, lazytoggleprm->getMixedGraph());
        if(prm_graph_free)  drawFreeGraph(lazytoggleprm, lazytoggleprm->getFreeGraph());
        if(prm_graph_obst)  drawObstGraph(lazytoggleprm, lazytoggleprm->getObstGraph());
        update();
    }
    if(dynamic_cast<RRT*>(planner) !=NULL && rrt_graph) {
        RRT * rrt=dynamic_cast<RRT*>(planner);
        drawTree(rrt, rrt->getTree());
        update();
    }

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
                if(path_index < planner->getPath().size()-1){
                    drawRobot(planner->getRobot(), planner->to_physical(path[path_index]));
                    path_index++;
                } else if(path_index+1 == planner->getPath().size()){
                    drawRobot(planner->getRobot(), planner->to_physical(path[path_index]));

                    showAnim=false;
                    path_index = 0;
                }
            }
            update();
        }
        if(showTrace){
            const PATH& path = planner->getPath();
            for(int i=0;i<planner->getPath().size();++i){
                    drawRobot(planner->getRobot(), planner->to_physical(path[i]));
            }
        }
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

    Vector2d v1 = robot.p[1];
    Vector2d v0 = robot.p[0];
    Vector2d v2 = robot.p[2];

//    if(thickness == 0)
//        glLineWidth(3);
//    else
//        glLineWidth(thickness);

    drawCircle(v1,Vector3d(0,1,1));
    drawLink(v0,v1,Vector3d(0,1,1));
    drawCircle(v0,Vector3d(1,0,1));
    drawLink(v0,v2,Vector3d(1,0,1));
//    glBegin(GL_LINES);
//    glColor3f(0.0, 1.0, 1.0);
//    glVertex2d(v1[0], v1[1]);
//    glVertex2d(v0[0], v0[1]);

//    glColor3f(1.0, 0.0, 1.0);
//    glVertex2d(v0[0], v0[1]);
//    glVertex2d(v2[0], v2[1]);
//    glEnd();

    drawCircle(v2,Vector3d(1,0,1));
}
int cmp(const void * aa, const void * bb) {
    Vector2d a = *(Vector2d*)aa;
    Vector2d b = *(Vector2d*)bb;

    if(a[0] < b[0]) return -1;
    else if(a[0] == b[0]){
        if(a[1] < b[1]) return -1;
        else          return 1;
    }
    else          return 1;
}
void Display::drawLink(Vector2d a, Vector2d b, Vector3d clr) {
//    glColor3fv(clr.rgb);
//    glLineWidth(thickness*2.0f);
//    glBegin(GL_LINE_STRIP);
//        glVertex2f(a.x, a.y);
//        glVertex2f(b.x, b.y);
//    glEnd();
//    glLineWidth(1);

    // remember to do the exchange of the vector
    double vec_x = b[0]-a[0];
    double vec_y = b[1]-a[1];
    double norm2 = sqrt(vec_x*vec_x+vec_y*vec_y);
    vec_x = -(b[1]-a[1])/norm2;
    vec_y = (b[0]-a[0])/norm2;

    Vector2d p[4];

    int dx[4] = {1,-1,1,-1};
    int dy[4] = {1,-1,1,-1};

    thickness *= 0.5;
    for(int i=0;i<2;++i){
        p[i]  = Vector2d(a[0]+dx[i]*thickness*vec_x, a[1]+dy[i]*thickness*vec_y);
    }
    for(int i=2;i<4;++i){
        p[i]  = Vector2d(b[0]+dx[i]*thickness*vec_x, b[1]+dy[i]*thickness*vec_y);
    }
    thickness *= 2;


    qsort(p, 4, sizeof(Vector2d), cmp);

    if(p[0][1] > p[3][1]){
        drawTriangle(p[0], p[3], p[1], clr);
        drawTriangle(p[0], p[2], p[3], clr);
    }
    else{
        drawTriangle(p[0], p[3], p[2], clr);
        drawTriangle(p[0], p[1], p[3], clr);
    }
}
void Display::drawTriangle(Vector2d a, Vector2d b, Vector2d c, Vector3d clr) {
    glColor3f(clr[0],clr[1],clr[2]);
    glBegin(GL_TRIANGLES);
        glVertex2f( a[0], a[1] );
        glVertex2f( b[0], b[1] );
        glVertex2f( c[0], c[1] );
    glEnd();
}
void Display::drawCircle(Vector2d a, Vector3d clr){
    glColor3f(clr[0],clr[1],clr[2]);
    glBegin(GL_TRIANGLE_FAN);
    for(int ii = 0; ii < 360; ii++) {
        GLfloat theta = PI*GLfloat(ii)/180.0f;
        GLfloat x = thickness*0.5f*cosf(theta);
        GLfloat y = thickness*0.5f*sinf(theta);
        glVertex2f(x + a[0], y + a[1]);
    }
    glEnd();
}

void Display::drawPolygons(RRT_OBJS& objs) {

    glLineWidth(2);
    for(RIT it = objs.begin(); it != objs.end(); ++it) {
        size_t size = it->getSize();
        glColor3f(0, 0, 1);
        glBegin(GL_LINE_LOOP);
        for(size_t i=0;i<size;i++) {
            ply_vertex* p = it->operator [](i);
            glVertex2dv(p->getPos().get());
        }
        glEnd();
    }

    if(showFilledObstacles){
        if(!doTriangulation){
            doTriangulation = true;
            triangles.clear();
            for(RIT it = objs.begin(); it != objs.end(); ++it) {
                Vector2dVector a;
                size_t size = it->getSize();
                int bb = -1;
                if((int)size == 4){
                    ply_vertex* pp1 = it->operator [](0);
                    const double *p1 = pp1->getPos().get();
                    ply_vertex* pp2 = it->operator [](1);
                    const double *p2 = pp2->getPos().get();
                    ply_vertex* pp3 = it->operator [](2);
                    const double *p3 = pp3->getPos().get();
                    ply_vertex* pp4 = it->operator [](3);
                    const double *p4 = pp4->getPos().get();
                    if((int)p1[0] == 0 && (int)p1[1] == 0 &&
                       (int)p2[0] == 0 && (int)p2[1] == 512 &&
                       (int)p3[0] == 512 && (int)p3[1] == 512 &&
                       (int)p4[0] == 512 && (int)p4[1] == 0){
                        bb = 1;
                    }
                }
                if(bb > 0) continue;
                for(size_t i=0;i<size-1;i++) {
                    ply_vertex* p = it->operator [](i);
                    const double *pp = p->getPos().get();
                    a.push_back(m_Vector2d(pp[0], pp[1]));
                }
                // allocate an STL vector to hold the answer.
                Vector2dVector result;
                //  Invoke the triangulator to triangulate this polygon.
                m_Triangulate::Process(a,result);
                // print out the results.
                int tcount = result.size()/3;

                for (int i=0; i<tcount; i++){
                    const m_Vector2d &p1 = result[i*3+0];
                    const m_Vector2d &p2 = result[i*3+1];
                    const m_Vector2d &p3 = result[i*3+2];
                    triangles.push_back(p1);
                    triangles.push_back(p2);
                    triangles.push_back(p3);
                }
            }
        }
        else{
            for(int tri=0;tri<(int)triangles.size();tri+=3){
                glColor3f(0.8,0.8,0.8);
                glBegin(GL_TRIANGLES);
                    glVertex2d(triangles[tri].GetX(), triangles[tri].GetY());
                    glVertex2d(triangles[tri+1].GetX(), triangles[tri+1].GetY());
                    glVertex2d(triangles[tri+2].GetX(), triangles[tri+2].GetY());
                glEnd();
            }
        }
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
    glPointSize(2);
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

void Display::drawFreeGraph(TOGGLEPRM* planner, const WDG& graph, vector<struct subset> graph_set) {

    glDisable(GL_LIGHTING);

    ///////////////////////////////////////////////////////////////////////////
    glLineWidth(0.5);
    ///////////////////////////////////////////////////////////////////////////
    //draw nodes
    vector<VID> vids;
    graph.GetVerticesVID(vids);
    typedef vector<VID>::iterator NIT;
    glPointSize(2);
    glBegin(GL_POINTS);
    {for(NIT i=vids.begin();i!=vids.end();i++)
    {
        CFG cfg=planner->to_physical( graph.GetData(*i).getCFG() );
        int n_set = Find(graph_set, (*i));
        glColor3f(color_table[n_set%1000][0],color_table[n_set%1000][1],color_table[n_set%1000][2]);
        glVertex2d(cfg.x,cfg.y);
    }}
    glEnd();

    ///////////////////////////////////////////////////////////////////////////
    //draw edges
    if(prm_graph_edge){
        glPushAttrib(GL_CURRENT_BIT);
        vector< pair<VID,VID> > edges;
        typedef vector< pair<VID,VID> >::iterator EIT;
        graph.GetEdges(edges);
        glBegin(GL_LINES);
        {for(EIT i=edges.begin();i!=edges.end();i++){
            Graph_Node node1 = graph.GetData(i->first);
            Graph_Node node2 = graph.GetData(i->second);
            CFG p1=planner->to_physical(node1.getCFG());
            CFG p2=planner->to_physical(node2.getCFG());
            int n1_set = Find(graph_set, (i->first));
            int n2_set = Find(graph_set, (i->second));
            if(n1_set != n2_set)
                glColor3f(0,0,0);
            else
                glColor3f(color_table[n1_set%1000][0],color_table[n1_set%1000][1],color_table[n1_set%1000][2]);

            glVertex2d(p1.x,p1.y);
            glVertex2d(p2.x,p2.y);
        }}//end for
        glEnd();
    }
}

void Display::drawObstGraph(TOGGLEPRM* planner, const WDG& graph) {

    glDisable(GL_LIGHTING);

    ///////////////////////////////////////////////////////////////////////////
    glLineWidth(0.5);
    ///////////////////////////////////////////////////////////////////////////
    //draw nodes
    vector<VID> vids;
    graph.GetVerticesVID(vids);
    typedef vector<VID>::iterator NIT;
    glPointSize(2);
    glBegin(GL_POINTS);
    {for(NIT i=vids.begin();i!=vids.end();i++)
    {
        CFG cfg=planner->to_physical( graph.GetData(*i).getCFG() );
        glColor3f(1,0,0);
        glVertex2d(cfg.x,cfg.y);
    }}
    glEnd();

    ///////////////////////////////////////////////////////////////////////////
    //draw edges

    if(prm_graph_edge){
      glColor3f(1,0,0);
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
}

void Display::drawMixedGraph(LAZYTOGGLEPRM* planner, const WDG& graph) {

    glDisable(GL_LIGHTING);

    ///////////////////////////////////////////////////////////////////////////
    glLineWidth(0.5);
    ///////////////////////////////////////////////////////////////////////////
    WDG m_graph = graph;
    vector< pair<int,VID> > ccstats;
    int ccsize=GetCCStats(m_graph,ccstats);
    glPointSize(3);
    for(int i=0;i<ccsize;i++)
    {
        int CCvid=ccstats[i].second;
        vector<VID> nodes;
        GetCC(m_graph,CCvid,nodes);

        glColor3f(0.5,0.5,0.5);

        CFG cfg_i=planner->to_physical( m_graph.GetData(CCvid).getCFG() );
        glBegin(GL_POINTS);
            glVertex2d(cfg_i.x,cfg_i.y);
        glEnd();

        typedef vector<VID>::iterator NIT;
        for( NIT j=nodes.begin();j!=nodes.end();j++ )
        {
            CFG cfg_j=planner->to_physical( m_graph.GetData(*j).getCFG() );
            glBegin(GL_POINTS);
                glVertex2d(cfg_j.x,cfg_j.y);
            glEnd();
        }//end for i
    }

    ///////////////////////////////////////////////////////////////////////////
    //draw edges
    if(prm_graph_edge){
        glPushAttrib(GL_CURRENT_BIT);
        vector< pair<VID,VID> > edges;
        typedef vector< pair<VID,VID> >::iterator EIT;
        graph.GetEdges(edges);
        glBegin(GL_LINES);
        {for(EIT i=edges.begin();i!=edges.end();i++){
            Graph_Node node1 = graph.GetData(i->first);
            Graph_Node node2 = graph.GetData(i->second);
            CFG p1=planner->to_physical(node1.getCFG());
            CFG p2=planner->to_physical(node2.getCFG());

            glColor3f(0.5,0.5,0.5);

            glVertex2d(p1.x,p1.y);
            glVertex2d(p2.x,p2.y);
        }}//end for
        glEnd();
    }
}

vector<int> set_idx;
void Display::drawFreeGraph(LAZYTOGGLEPRM* planner, const WDG& graph) {

//    if(prm_graph_flag > 0){
//        prm_graph_flag = -1;
//        WDG m_graph = graph;

//        int ssize = m_graph.GetNextVID();

//        set_idx.clear();
//        for(int i=0;i<ssize;++i){
//            set_idx.push_back(i);
//        }

//        vector< pair<int,VID> > ccstats;
//        int ccsize=GetCCStats(m_graph,ccstats);
//        for(int i=0;i<ccsize;i++)
//        {
//            int CCvid=ccstats[i].second;
//            vector<VID> nodes;
//            GetCC(m_graph,CCvid,nodes);
//            for(int j=0;j<nodes.size();++j){
//                set_idx[nodes[j]] = CCvid;
//            }
//        }

//        vector< pair<VID,VID> > edges;
//        typedef vector< pair<VID,VID> >::iterator EIT;
//        graph.GetEdges(edges);
//        for(EIT i=edges.begin();i!=edges.end();i++){
//            Graph_Node node1 = graph.GetData(i->first);
//            Graph_Node node2 = graph.GetData(i->second);
//            CFG p1=planner->to_physical(node1.getCFG());
//            CFG p2=planner->to_physical(node2.getCFG());
//            int n1_set = set_idx[i->first];
//            int n2_set = set_idx[i->second];
//        }
//    }

    glDisable(GL_LIGHTING);

    ///////////////////////////////////////////////////////////////////////////
    glLineWidth(0.5);
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
        glColor3f(0,1,0);
        glVertex2d(cfg.x,cfg.y);
    }}
    glEnd();

    ///////////////////////////////////////////////////////////////////////////
    //draw edges
    if(prm_graph_edge){
        glPushAttrib(GL_CURRENT_BIT);
        vector< pair<VID,VID> > edges;
        typedef vector< pair<VID,VID> >::iterator EIT;
        graph.GetEdges(edges);
        glBegin(GL_LINES);
        {for(EIT i=edges.begin();i!=edges.end();i++){
            Graph_Node node1 = graph.GetData(i->first);
            Graph_Node node2 = graph.GetData(i->second);
            CFG p1=planner->to_physical(node1.getCFG());
            CFG p2=planner->to_physical(node2.getCFG());
            glColor3f(0,1,0);
            glVertex2d(p1.x,p1.y);
            glVertex2d(p2.x,p2.y);
        }}//end for
        glEnd();
    }
}

void Display::drawObstGraph(LAZYTOGGLEPRM* planner, const WDG& graph) {

    glDisable(GL_LIGHTING);

    ///////////////////////////////////////////////////////////////////////////
    glLineWidth(0.5);
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
        glColor3f(1,0,0);
        glVertex2d(cfg.x,cfg.y);
    }}
    glEnd();

    ///////////////////////////////////////////////////////////////////////////
    //draw edges
    if(prm_graph_edge){
      glColor3f(1,0,0);
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
}
