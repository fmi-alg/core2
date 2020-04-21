//==============================================================================
// Display.cpp
// =================
// This class is the widget on ControlWindow that renders the scene.
// Display generates and paints boxes, walls, and circles using OpenGL
// to allow the user to get a full understanding of the program's output.
//
// AUTHOR: Bryant Curto (bryantcurto@gmail.com)
// CREATED: 2015-08-02
//
// Copyright (c) 2015 Bryant Curto
//==============================================================================

// Header
#include "Display.h"

#include <ctime>

// Custom
#include "ShaderException.h"

// Standard Library
#include <iostream>
#include <math.h>

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
extern bool runAnim;
extern bool pauseAnim;
extern bool replayAnim;

extern int animationSpeed;
extern int animationSpeedScale;
extern int animationSpeedScaleBox;

extern double bandwidth;

extern vector<Box*> PATH;
extern std::vector<int> expansions;


int filledCircleResolution = 32;   // Num vertices in filled circle
int inc(0);



// Global Variables in MainWindow.cpp


// Member Variables
static int numQuads = 0;
static bool regenShapes = true;
// Stores Vertices of Shapes
static std::vector<double>  filledCircles;
static std::vector<double>  lines;
static std::vector<double>  quads;
static std::vector<double>  quadOutlines;
static std::vector<double>  pathLineVertices;



int renderSteps(1);
bool step(false);



#include <unistd.h>

int pSize;
int lim;


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
Display::~Display() {
    makeCurrent();
    delete program;
    // Pointer to Control Window is not
    //   handled by Display
    doneCurrent();
}


/*
 * NEW SCENE
 *
 * Set 'reGenShapes' to true
 * Improves performance by preventing
 * objects in scene from being regenerated
 * every time paintGL is called
 */
void Display::genScene() {
    regenShapes = true;
}

/*
 * INITIALIZE GL
 *
 * Called once to prepare display window for
 * rendering.
 * Creates and initializes shader program
 */
void Display::initializeGL() {
   initializeOpenGLFunctions();

    // Set background color
    glClearColor(0.7, 0.7, 0.7, 1.0);

    program = new shader("simple.vert", "simple.frag");

    try {
        program->init();
    } catch (ShaderException& exc) {
        *window << "\n\n\n\n\n\n\n\n\n"
                "**************************************************\n"
                "ERROR!!\n\n" << exc.what() << "\n\n"
                "**************************************************\n"
                "\n\n\n\n";
    }
}

/*
 * PAINT GL
 *
 * Regenerates (if necessary) and draws scene to
 * dispaly screen.
 */
void Display::paintGL() {


    // Set Background Color and Prep for Drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program->use();


    if (regenShapes) {

        /*******************
         *     Cleanup     */

        glBindVertexArray(0);

        quads.clear();
        quadOutlines.clear();
        filledCircles.clear();
        lines.clear();
        pathLineVertices.clear();

        numQuads = 0;

        /*******************
         * Generate Shapes */

        // Quads and Quad Outlines
        if(!hideBox || !hideBoxBoundary) putQuads();

        //Lines Representing Robot at Start (alpha) and End (beta)
        double degToRad = PI/180.0f;
        genLine(thickness, alpha[0], alpha[1], alpha[0]+L1*cos(alpha[2]*degToRad), alpha[1]+L1*sin(alpha[2]*degToRad), 1,0,1);
        genFilledCircle(thickness/2.0f, alpha[0], alpha[1], 1,0,1);
        genFilledCircle(thickness/2.0f, alpha[0]+L1*cos(alpha[2]*degToRad), alpha[1]+L1*sin(alpha[2]*degToRad), 1,0,1);
        genLine(thickness, alpha[0], alpha[1], alpha[0]+L2*cos(alpha[3]*degToRad), alpha[1]+L2*sin(alpha[3]*degToRad), 0,1,0);
        genFilledCircle(thickness/2.0f, alpha[0], alpha[1], 0,1,0);
        genFilledCircle(thickness/2.0f, alpha[0]+L2*cos(alpha[3]*degToRad), alpha[1]+L2*sin(alpha[3]*degToRad), 0,1,0);


        genLine(thickness, beta[0], beta[1], beta[0]+L1*cos(beta[2]*degToRad), beta[1]+L1*sin(beta[2]*degToRad), 0.25,0,0.25);
        genFilledCircle(thickness/2.0f, beta[0], beta[1], 0.25,0,0.25);
        genFilledCircle(thickness/2.0f, beta[0]+L1*cos(beta[2]*degToRad), beta[1]+L1*sin(beta[2]*degToRad), 0.25,0,0.25);
        genLine(thickness, beta[0], beta[1], beta[0]+L2*cos(beta[3]*degToRad), beta[1]+L2*sin(beta[3]*degToRad), 0,0.25,0);
        genFilledCircle(thickness/2.0f, beta[0], beta[1], 0,0.25,0);
        genFilledCircle(thickness/2.0f, beta[0]+L2*cos(beta[3]*degToRad), beta[1]+L2*sin(beta[3]*degToRad), 0,0.25,0);


        // If a Path Exists, Generate the Line Representing It
        if (!noPath) {
           genPath(PATH, alpha, beta, R0);
        }


        // Lines Representing Obsticals
        genWalls(QT->pRoot);

        // Line From Start (alpha) to End (beta)
        //genLine(1.6, alpha[0], alpha[1], beta[0], beta[1],
        //        (noPath ? 0.0 : 1.0), 0.5, 0.75, false);

        regenShapes = false;
    }

    if(runAnim && !noPath){
        if(pauseAnim) {
            drawRobot(PATH.at(pSize-inc-1));
        }
        else {
            regenShapes=false;
            pSize=PATH.size();
            if(inc<pSize){
                drawRobot(PATH.at(pSize-inc-1));
                inc++;

                if(hideBox) usleep(  (99-animationSpeed) * animationSpeedScale);
                else        usleep(  (99-animationSpeed) * animationSpeedScaleBox);
            }
            else{
                inc=0;
                runAnim=false;
            }
        }
        genScene();
        update();
    }


    /*******************
     *   Draw Shapes   */

    // Green, Yellow, Red, & Grey Squares
    drawCircles();
    drawLines();
    // Path Line
    if(!noPath) drawPath();
    if(!hideBox) drawQuads();
    if(!hideBoxBoundary) drawQuadsBoundary();
}


double theta1;
double theta2;
double oldAngleForRender1 = -1;
double oldAngleForRender2 = -1;
void Display::drawRobot(Box* b){

    theta1 = b->xi[Box::LOWER1];
    theta2 = b->xi[Box::UPPER2];
    if (crossingOption) {
        double max = 0;
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

    if(runAnim)
        fprintf(fp, "x:%.lf y:%.lf  t1:%lf t2:%lf\n", b->x, b->y, theta1, theta2);

    double deg2rad = PI/180.0f;
    genLine(thickness, b->x, b->y, b->x+L1*cos(theta1*deg2rad), b->y+L1*sin(theta1*deg2rad), 0.5,0,0.5);
    genFilledCircle(thickness/2.0f, b->x, b->y, 0.5,0,0.5);
    genFilledCircle(thickness/2.0f, b->x+L1*cos(theta1*deg2rad), b->y+L1*sin(theta1*deg2rad), 0.5,0,0.5);

    genLine(thickness, b->x, b->y, b->x+L2*cos(theta2*deg2rad) , b->y+L2*sin(theta2*deg2rad), 0,0.5,0);
    genFilledCircle(thickness/2.0f, b->x, b->y, 0,0.5,0);
    genFilledCircle(thickness/2.0f, b->x+L2*cos(theta2*deg2rad) , b->y+L2*sin(theta2*deg2rad), 0,0.5,0);
}


void Display::putQuads(){
    Box* tmp;
    if(step){
        for(int i=0;  i<(int)Box::pAllLeaf->size()&&i<expansions.at(renderSteps);i++){
            tmp=Box::pAllLeaf->at(i);
            genQuad(tmp,epsilon);
            numQuads++;
        }
    }//NOTE: This draws ALL quads, including non leaves - this is overkill, and wastes time and space.
    //Can we do this better?
    else{
        for(unsigned int i=0;i<Box::pAllLeaf->size();i++){
            tmp = Box::pAllLeaf->at(i);
            if(!tmp->isLeaf) continue;
            genQuad(tmp,epsilon);
            numQuads++;
        }
    }
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
}

/*
 *  SET CONTROL WINDOW
 *
 * Pass pointer that holds Display widget
 * so that Display can print information
 * to the user via the text display screen.
 */
void Display::setControlWindow(MainWindow* cw) {
    window = cw;
}


//===========================================================//
//======          Functions to Generate Shapes         ======//
//=== To generate shapes, OpenGL requires you to store    ===//
//=== the data for the vertices of each shape.            ===//
//=== I.e. the (x, y) coordinate of each vertex.          ===//
//=== However, in OpenGL, each vertex has its own color.  ===//
//=== Therefore, to draw one triangle, 15 values must be  ===//
//=== stored:                                             ===//
//=== x1 y1 r1 g1 b1 | x2 y2 r2 g2 b2 | x3 y3 r3 g3 b3    ===//
//===========================================================//
/*
 * TO NORMAL DEVICE COORDINATES
 *
 * Convert coordinates in the ranges 0 <= x <= boxWidth
 * and 0 <= y <= boxHeight to the Normalized Device
 * Coordinate Range used by OpenGL -1 <= x < 1 and
 * -1 <= y <= 1
 */
void Display::toNormalDeviceCoords(double& x, double& y) {
    x = ((2.0 * x) / boxWidth) - 1;
    y = ((2.0 * y) / boxHeight) - 1;
}

/*
 * SET VERTEX COLOR
 *
 * Inline function to easily set the color of a vertex
 * since OpenGL requires each vertex to have its own color
 */
inline void Display::setVertexColor(vector<double>& array, float r, float g, float b) {
    array.push_back(r);
    array.push_back(g);
    array.push_back(b);
}

/*
 * PERPENDICULAR VERTICES
 *
 * Used to give lines thickness.
 * The function accepts two points ('a' and 'b') and computes the vertices
 * of a rectangle, where 'a' and 'b' are the midpoints of the sides and
 * 'thickness' is the height.
 *
 *     [p1]-----[p2] \
 *      |         |   \
 *     [a]       [b]   > thickness
 *      |         |   /
 *     [p3]-----[p4] /
 *
 * 'thickness' is with respect to the 'boxWidth' and 'boxHeight' not the
 * normalized device coordinate range
 *
 * Precondition: 'a' and 'b' cannot have already been normalized
 * Postcondition: Perpendicular vertices have been added to specified
 *                vector and normalized
 *
 * This function uses vecters to find the vertices by converting 'a'
 * and 'b' into a vector, finding a perpendicular vector, giving it
 * the length of 'thickness', and adding/subtracting the values of
 * this vector to 'a' and 'b'
 */
void Display::perpVertices(float thickness_l, vector<double>& storage, double a_x, double a_y,
                       double b_x, double b_y, float red, float green, float blue) {
    thickness_l /= 2.0f;
    double x = a_x - b_x;
    double y = a_y - b_y;
    //(*controlWin) << "\nSTAGE 1: x= " << x << ",  y= " << y << "\n";
    //double tempx = x, tempy = y;

    // Used to prevent overflow error
    float essentuallyZero = 0.001;

    if ((-essentuallyZero < x && x < essentuallyZero) &&
        (-essentuallyZero < y && y < essentuallyZero)) {
        x = 0; y = 0;
    } else if (-essentuallyZero < x && x < essentuallyZero) {
        x = 1 * thickness_l;
        y = 0;
    } else if (-essentuallyZero < y && y < essentuallyZero) {
        y = 1 * thickness_l;
        x = 0;
    } else {
        x = 1/x;
        y = 1/y;

        double temp_x = x, temp_y = y;
        x = thickness_l * (temp_x / sqrt(pow(temp_x, 2) + pow(temp_y, 2)));
        y = thickness_l * (temp_y / sqrt(pow(temp_x, 2) + pow(temp_y, 2)));
    }
    //(*controlWin) << "STAGE 2: x= " << x << ",  y= " << y << "\n";
    //(*controlWin) << "DOT PRODUCT= " << x * tempx - y * tempy << "\n\n";

    // Save vertices to 'storage'
    storage.push_back(a_x + x);
    storage.push_back(a_y - y);
    setVertexColor(storage, red, green, blue);

    storage.push_back(a_x - x);
    storage.push_back(a_y + y);
    setVertexColor(storage, red, green, blue);

    storage.push_back(b_x + x);
    storage.push_back(b_y - y);
    setVertexColor(storage, red, green, blue);

    storage.push_back(b_x - x);
    storage.push_back(b_y + y);
    setVertexColor(storage, red, green, blue);

    // Set previous 4 vertices to normal device coordinates
    for (int i = 0; i < 4; i++)
        toNormalDeviceCoords(storage[(storage.size()-5) - i*5], storage[(storage.size()-4) - i*5]);
}

/*
 * GENERATE PATH
 *
 * Generates the shapes that constitute the path, a smooth line from
 * 'alpha' to 'beta' avoiding all obsticals
 */
void Display::genPath(vector<Box*>& path, double* alpha, double* beta, double R0)
{
    double red = 0.5, green = 0.0, blue = 0;
    float thickness_l = R0 < 1.5/2 ? 2*R0 : 1.5;

    vector<double> vertexHolder;

    vertexHolder.push_back(beta[0]);        // beta x
    vertexHolder.push_back(beta[1]);        // beta y

    for (int i = 0; i < (int)path.size(); i++) {
        vertexHolder.push_back(path[i]->x);
        vertexHolder.push_back(path[i]->y);
    }

    vertexHolder.push_back(alpha[0]);       // alpha x
    vertexHolder.push_back(alpha[1]);       // alpha y

    // Generate vertices for path line
    for (int n = 0; n < (int)vertexHolder.size() / 2 - 1; n++)
        perpVertices(thickness_l, pathLineVertices,
                   vertexHolder[(n)*2], vertexHolder[(n)*2+1],
                   vertexHolder[(n+1)*2], vertexHolder[(n+1)*2+1],
                   red, green, blue);
}

/*
 * GENERATE LINES
 *
 * Uses to draw obsticals, this function generates the shapes necessary
 * to create a line (with thickness)
 */
void Display::genLine(float thickness_l, double a_x, double a_y, double b_x, double b_y, float red, float green, float blue){
    // Calculate perpendicular vertices to give line thickness
    perpVertices(thickness_l, lines, a_x, a_y, b_x, b_y, red, green, blue);
}

/*
 * GENERATE QUADRILATERAL
 *
 * Generates a box and with the color of its status and
 * a black outline around the box
 */
void Display::genQuad(Box* b, double epsilon)
{

    // Get color based on Box's Status
    double red, green, blue;

    switch(b->status)
    {
        case Box::FREE:
            // color is dark green, representing completed free angular ranges for both links
            // Note: the angular range of link i is the full circle iff LOWERi=0 and UPPERi=360 (i=1 or 2).
            if (b->xi[Box::LOWER1] != 0 || b->xi[Box::UPPER1] != 360 || b->xi[Box::LOWER2] != 0 || b->xi[Box::UPPER2] != 360) {
                red = 0x66 / 255.0;
                green = 0xCC / 255.0;
                blue = 0x99 / 255.0;
            }
            // light green
            else {
                red = 0x33 / 255.0;
                green = 0x99 / 255.0;
                blue = 0x33 / 255.0;
            }
            break;
        case Box::STUCK:  //color is red
            red = 0xFF/255.0;
            green = 0/255.0;
            blue = 0/255.0;
            break;
        case Box::MIXED: //color is yellow (if box is epsilon-large)
            red = 0xFF / 255.0;
            green = 0xFF / 255.0;
            blue = 0 / 255.0;
            if (b->height < 2 * epsilon || b->width < 2 * epsilon) { // color is gray (if box is epsilon-small)
                red = 0.5;
                green = 0.5;
                blue = 0.5;
            }
            break;
        case Box::UNKNOWN:
            red = 0xFF / 255.0;
            green = 0xFF / 255.0;
            blue = 0 / 255.0;
            (*window) << "UNKNOWN box status in genQuad\n";
            break;
    }

    /*
    switch(b->status)
    {
        case Box::FREE:
            red = 0x00 / 255.0;
            green = 0xFF / 255.0;
            blue = 0x00 / 255.0;
            break;
        case Box::STUCK:
            red = 0xFF/255.0;
            green = 0x00/255.0;
            blue = 0x00/255.0;
            break;
        case Box::MIXED:
            red = 0xFF / 255.0;
            green = 0xFF / 255.0;
            blue = 0.0 / 255.0;
            break;
        case Box::UNKNOWN:
            (*window) << "UNKNOWN box status in genQuad\n";
    }
    */

    // 1st Corner: lower left
    quads.push_back(b->x - b->width / 2);
    quads.push_back(b->y - b->height / 2);
    setVertexColor(quads, red, green, blue);

    // 2nd Corner: lower right
    quads.push_back(b->x + b->width / 2);
    quads.push_back(b->y - b->height / 2);
    setVertexColor(quads, red, green, blue);

    // 3rd Corner: upper right
    quads.push_back(b->x + b->width / 2);
    quads.push_back(b->y + b->height / 2);
    setVertexColor(quads, red, green, blue);

    // 4th Corner: upper left
    quads.push_back(b->x - b->width / 2);
    quads.push_back(b->y + b->height / 2);
    setVertexColor(quads, red, green, blue);

    // Convert vertices to normalized divece coordinates
    for (int i = 0; i < 4; i++)
        toNormalDeviceCoords(quads[(quads.size()-5) - i*5], quads[(quads.size()-4) - i*5]);

    // Add vertices of the quads' outlines
    for (int j = 3; j>=0 ; j--) {
        quadOutlines.push_back(quads[(quads.size()-5) - j*5]);
        quadOutlines.push_back(quads[(quads.size()-4) - j*5]);
        setVertexColor(quadOutlines, 0.0, 0.0, 0.0);
    }
}

/*
 * GENERATE WALLS
 *
 * Generate white lines representing obsticals
 */
void Display::genWalls(Box* b)
{
    // With all values set to 1.0, the color is white
    double red = 0, green = 0, blue = 1.0;

    for (list<Wall*>::iterator iter = b->walls.begin(); iter != b->walls.end(); ++iter)
    {
        Wall* w = *iter;
        genLine(1, w->src->x, w->src->y, w->dst->x, w->dst->y, red, green, blue);
    }
}

/*
 * GENERATE FILLED CIRCLE
 *
 * Generates a filled circle with a given radius and centered
 * at ('x', 'y')
 *
 * A perfectly smooth circle cannot be drawn, so a series of
 * lines must be used instead. The smoothness (number of
 * vertices) is determined by 'filledCircleResolution'
 */
void Display::genFilledCircle(double radius, double x, double y, float red, float green, float blue){
    // Convert to Normal Device Coordinates [-1.0, 1.0]
    toNormalDeviceCoords(x, y);
    double radius_x = radius / (boxWidth/2.0);
    double radius_y = radius / (boxHeight/2.0);


    // Set Vertices
    filledCircles.push_back(x);               // Center x
    filledCircles.push_back(y);               // Center y
    setVertexColor(filledCircles, red, green, blue);

    // Set manually to prevent floating point
    // error => incomplete circle
    filledCircles.push_back(x + radius_x);    // theta = 0
    filledCircles.push_back(y);
    setVertexColor(filledCircles, red, green, blue);

    for (int div = 1; div < filledCircleResolution; ++div) {
        double theta = div * (2 * M_PI) / filledCircleResolution;

        filledCircles.push_back(cos(theta) * radius_x + x);
        filledCircles.push_back(sin(theta) * radius_y + y);
        setVertexColor(filledCircles, red, green, blue);
    }

    filledCircles.push_back(x + radius_x);    // theta = 2*pi
    filledCircles.push_back(y);
    setVertexColor(filledCircles, red, green, blue);
}

//===========================================================//
//======            Functions to Draw Shapes           ======//
//=== To draw shapes, OpenGL requires the vertex data of  ===//
//=== each shape, the number of vertices, the way         ===//
//=== this data will be stored on the GPU, how it will    ===//
//=== be drawn to the screen, and much more.              ===//
//=== See http://learnopengl.com to get a better          ===//
//=== understanding of this section of code.              ===//
//===========================================================//




/*
 * DRAW LINES
 *
 * Uses vertices stored in 'lines' vector
 * to draw lines to the screen.
 */
void Display::drawLines() {
    if (!lines.size()) return;

    // Array of all vertices
    // Currently, values in vectors are stored in
    // contiguous memory slots
    double* coord_color = &lines[0];

    // Set Up
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Transfer data from CPU to GPU
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, lines.size()*sizeof(GLdouble), coord_color, GL_STATIC_DRAW);

    // Tell OpenGL where in the list of values
    // the coordinate values are stored
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Tell OpenGL where in the list of values
    // the color values are stored
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)(2 * sizeof(GLdouble)));
    glEnableVertexAttribArray(1);

    // Draw Setup
    int numLines = lines.size()/20;
    int* count = new int[numLines];
    for (int i = 0; i < numLines; i++)
        count[i] = 4;
    int* indexOfFirstVert = new int[numLines];
    for (int j = 0; j < numLines; j++)
        indexOfFirstVert[j] = j * 4;

    // Draw!!
    /*
     * Note: look up 'GL_TRIANGLE_STRIP' to get a
     * better understanding of how lines are given
     * thickness.
     */
    glMultiDrawArrays(GL_TRIANGLE_STRIP, indexOfFirstVert, count, numLines);

    // Clean up
    delete[] count;
    delete[] indexOfFirstVert;
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

/*
 * DRAW QUADRILATERALS
 *
 * Use the vertices stored in 'quads' vector
 * to draw quadrilaterals to the screen.
 * If 'hideBoxBoundary' == false, the outlines
 * stored in 'quadOutlines' vector are drawn.
 */
void Display::drawQuads() {
    if (!quads.size()) return;

    //== Draw Solid Boxes ==//

    // Array of all vertices
    double* coord_color = &quads[0];

    /*
     * A quadrilateral can't be made outright;
     * two triangles must be drawn instead.
     * For the vertices shared by the triangles,
     * we could list them in 'quads' multiple times
     * or we can use indices.
     *
     * What is happening in the following loop:
     *  Each 5 values in 'quads' correlates with one vertex
     *    (2 coordinate values & 3 color values).
     *  Each 20 values in 'quads' correlates with 1 quadrilateral.
     *  Every 4 vertices (20 values) are used to create
     *    2 triangles, which has a total of 6 vertices.
     *  Therefore, 6 indices are needed for every 4 vertices
     *  in 'quads'.
     */
    int* indices = new int[quads.size()/20 * 6];
    for (int i = 0; i < (int)quads.size()/20; i++) {
        indices[i*6]     = i*4 + 1;
        indices[i*6 + 1] = i*4 + 3;
        indices[i*6 + 2] = i*4 + 0;
        indices[i*6 + 3] = i*4 + 1;
        indices[i*6 + 4] = i*4 + 2;
        indices[i*6 + 5] = i*4 + 3;
    }

    // Set Up
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Transfer data from CPU to GPU
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ARRAY_BUFFER, quads.size()*sizeof(GLdouble), coord_color, GL_STATIC_DRAW);

    // Tell OpenGL where in the list of values
    // the coordinate values are stored
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Tell OpenGL where in the list of values
    // the color values are stored
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)(2 * sizeof(GLdouble)));
    glEnableVertexAttribArray(1);

    // Transfer more data from CPU to GPU
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, quads.size()/20*6*sizeof(GLint), indices, GL_STATIC_DRAW);

    // Draw!
    glDrawElements(GL_TRIANGLES, quads.size()/20*6, GL_UNSIGNED_INT, (GLvoid*)0);

    // Cleanup
    glDeleteBuffers(1, &ebo);
    delete[] indices;

    // More Cleanup
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Display::drawQuadsBoundary() {
    if (!quadOutlines.size()) return;

    // Set Up
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Transfer data from CPU to GPU
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // Tell OpenGL where in the list of values
    // the coordinate values are stored
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Tell OpenGL where in the list of values
    // the color values are stored
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)(2 * sizeof(GLdouble)));
    glEnableVertexAttribArray(1);

    // Array of all vertices
    double *coord_color = &quadOutlines[0];

    // Transfer data from CPU to GPU
    glBufferData(GL_ARRAY_BUFFER, quadOutlines.size()*sizeof(GLdouble), coord_color, GL_STATIC_DRAW);

    // Draw Setup
    int* count = new int[numQuads];
    for (int i = 0; i < numQuads; i++)
        count[i] = 4;
    int* indexOfFirstVert = new int[numQuads];
    for (int j = 0; j < numQuads; j++)
        indexOfFirstVert[j] = 4*j;

    // Draw!!
    glMultiDrawArrays(GL_LINE_LOOP, indexOfFirstVert, count, numQuads);

    // Cleanup
    delete[] count;
    delete[] indexOfFirstVert;

    // Cleanup
    glDeleteBuffers(1, &ebo);

    // More Cleanup
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

/*
 * DRAW PATH
 *
 * Uses the vertices stored in 'pathLineVertices'
 * vector to draw path lines to screen.
 */
void Display::drawPath() {
    if (!pathLineVertices.size()) return;

    // Array of all vertices
    double* coord_color = &pathLineVertices[0];

    // Set Up
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Transfer data from CPU to GPU
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, pathLineVertices.size() * sizeof(GLdouble),
                 coord_color, GL_STATIC_DRAW);

    // Tell OpenGL where in the list of values
    // the coordinate values are stored
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Tell OpenGL where in the list of values
    // the color values are stored
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble),
                          (GLvoid*)(2 * sizeof(GLdouble)));
    glEnableVertexAttribArray(1);

    // Draw Setup
    int* count = new int[pathLineVertices.size() / 20];
    for (int i = 0; i < (int)pathLineVertices.size()/20; i++)
        count[i] = 4;

    int* indexOfFirstVert = new int[pathLineVertices.size() / 20];
    for (int j = 0; j < (int)pathLineVertices.size()/20; j++)
        indexOfFirstVert[j] = 4*j;

    // Draw!!!!!
    /*
     * Note: look up 'GL_TRIANGLE_LOOP' to get a
     * better understanding of how the path is drawn
     */
    glMultiDrawArrays(GL_TRIANGLE_STRIP, indexOfFirstVert, count, pathLineVertices.size()/20);

    // Clean up
    delete[] count;
    delete[] indexOfFirstVert;
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

/*
 * DRAW CIRLCES
 *
 * Uses the vertices stored in 'filledCircles'
 * vector to draw solid circles and 'hollowCircles'
 * vector to draw circle outlines to screen.
 */
void Display::drawCircles() {

    //== Solid Circles ==//

    if (filledCircles.size()) {

        // Array of all vertices
        double* coord_color = &filledCircles[0];

        GLuint vao, vbo;
        // Generate VAO
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // Transfer data from CPU to GPU
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, filledCircles.size()*sizeof(GLdouble), coord_color, GL_STATIC_DRAW);

        // Tell OpenGL where in the list of values
        // the coordinate values are stored
        glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Tell OpenGL where in the list of values
        // the color values are stored
        glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 5*sizeof(GLdouble), (GLvoid*)(2 * sizeof(GLdouble)));
        glEnableVertexAttribArray(1);

        // Draw Setup
        int numCircles = filledCircles.size() / (5 * (filledCircleResolution + 2));

        int* count = new int[numCircles];
        for (int i = 0; i < numCircles; i++)
            count[i] = filledCircleResolution+2;

        int* indexOfFirstVert = new int[numCircles];
        for (int j = 0; j < numCircles; j++)
            indexOfFirstVert[j] = j * (filledCircleResolution+2);

        // Draw!!
        /*
         * Note: look up 'GL_TRIANGLE_FAN' to get a
         * better understanding of how solid circles
         * are drawn
         */
        glMultiDrawArrays(GL_TRIANGLE_FAN, indexOfFirstVert, count, numCircles);

        // Clean up
        delete[] count;
        delete[] indexOfFirstVert;
        glBindVertexArray(0);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}
