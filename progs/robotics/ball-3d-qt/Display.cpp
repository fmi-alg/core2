//==============================================================================
// Display.cpp
// =================
// This class is the widget on ControlWindow that renders the scene.
// Display generates and paints boxes, walls, and circles using OpenGL
// to allow the user to get a full understanding of the program's output.
//
// AUTHOR: Bryant Curto (bryantcurto@gmail.com)
//         Ching-Hsiang Hsu (chh415@nyu.edu)
// CREATED: 2015-08-02
// UPDATE:  2015-10-20
// Copyright (c) 2015 Bryant Curto, Ching-Hsiang Hsu
//==============================================================================

#include "Display.h"

#include <ctime>
#include <iostream>
#include <math.h>
#include <unistd.h>

//Global Variables
extern char running_dir[300];
extern std::string fileName;
extern double alpha[3];
extern double beta[3];
extern double epsilon;
extern double R;
extern double L;
extern int QType;
extern int seed;
extern bool noPath;
extern bool showBox;
extern bool finishedAnim;
extern int animationSpeed;
extern double boxWidth;
extern SoftSubdivisionSearch* sss;
extern int transparency;
extern double distanceZ;
extern double eye[3];
extern double at[3];
extern double up[3];
extern double eyeVector[3];
extern float view_rotate_angles[3];
extern float initialRot[3];
extern float finalRot[3];
extern float obj_pos[3];
extern float view_rotate[16];
extern float view_rotate_x[16];
extern float view_rotate_y[16];
extern float view_rotate_z[16];
extern vector<ConfBox3d*> path;
extern bool showAnim;
extern unsigned int iPathSeg;
extern unsigned int inSegCount;
extern float segCount;


//local Variables
static GLuint sphereList;
static bool regenShapes = true;
int renderSteps = 1;
bool step = false;


/*
 * CONSTRUCTOR
 *
 * Define data members
 */
Display::Display(QWidget* parent):
    QOpenGLWidget(parent)
{
    xRot = 0;
    yRot = -45*16;
    zRot = 0;

    distanceX = 0.0;
    distanceY = 300.0;
    distanceZ = 900.0;

    // focus on key press event
    setFocusPolicy(Qt::StrongFocus);
}

/*
 * DESTRUCTOR
 */
Display::~Display()
{
}

void Display::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    event->accept();
}

void Display::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);

        update();
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);

        update();
    }

    lastPos = event->pos();
    event->accept();
}

void Display::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();

    if (event->orientation() == Qt::Vertical) {
        if (delta < 0) {
            distanceZ *= 1.01;
        } else if (delta > 0) {
            distanceZ *= 0.99;
        }

        update();
    }

    emit zDistanceChanged(distanceZ);

    event->accept();
}

void Display::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {
        distanceY += 5.0f;
        update();
    } else if (event->key() == Qt::Key_Down) {
        distanceY -= 5.0f;
        update();
    } else if (event->key() == Qt::Key_Left) {
        distanceX += 5.0f;
        update();
    } else if (event->key() == Qt::Key_Right) {
        distanceX -= 5.0f;
        update();
    }

    event->accept();
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void Display::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        //emit xRotationChanged(angle);
        update();
    }
}

void Display::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        //emit yRotationChanged(angle);
        update();
    }
}

void Display::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        //emit zRotationChanged(angle);
        update();
    }
}

void resetViewRotate(float* view_rotate) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            view_rotate[i * 4 + j] = 0;
        }
        view_rotate[i * 4 + i] = 1;
    }
}

void multiplyRotationMatrix(float* view_rotate_x) {
    float tmp[16];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tmp[i * 4 + j] = 0;
            for (int k = 0; k < 4; k++) {
                tmp[i * 4 + j] += view_rotate[i * 4 + k] * view_rotate_x[k * 4 + j];
            }
        }
    }

    for (int i = 0; i < 16; i++) {
        view_rotate[i] = tmp[i];
    }
}

void resetRotationMatrix() {
    resetViewRotate(view_rotate);

    float xAng = 0;
    float yAng = 0;
    float zAng = 0;

    xAng = view_rotate_angles[0];
    yAng = view_rotate_angles[1];
    zAng = view_rotate_angles[2];

    // row 2:
    view_rotate_x[5] = cos(xAng);
    view_rotate_x[6] = -sin(xAng);
    // row 3:
    view_rotate_x[9] = sin(xAng);
    view_rotate_x[10] = cos(xAng);

    view_rotate_y[0] = cos(yAng);
    view_rotate_y[2] = sin(yAng);
    // row 3:
    view_rotate_y[8] = -sin(yAng);
    view_rotate_y[10] = cos(yAng);

    // row 1:
    view_rotate_z[0] = cos(zAng);
    view_rotate_z[1] = -sin(zAng);
    // row 2:
    view_rotate_z[4] = sin(zAng);
    view_rotate_z[5] = cos(zAng);

    multiplyRotationMatrix(view_rotate_z);
    multiplyRotationMatrix(view_rotate_y);
    multiplyRotationMatrix(view_rotate_x);
}

void Display::initializeGL()
{
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_FLAT);

    GLfloat light0_ambient[] =  {0.0, 0.0, 0.0};
    GLfloat light0_diffuse[] =  {1.0, 1.0, 1.0};
    GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};
    GLfloat light0_specular[] = {1.0, 1.0, 1.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glEnable(GL_DEPTH_TEST);

    eyeVector[0] = at[0] - eye[0];
    eyeVector[1] = at[1] - eye[1];
    eyeVector[2] = at[2] - eye[2];
    resetRotationMatrix();
    obj_pos[0] = eye[0];
    obj_pos[1] = eye[1];
    obj_pos[2] = eye[2];
}

void Display::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //gluLookAt(obj_pos[0], obj_pos[1], obj_pos[2]+distance, obj_pos[0] + eyeVector[0], obj_pos[1] + eyeVector[1], obj_pos[2] + eyeVector[2], up[0], up[1], up[2]);
    gluLookAt(distanceX, distanceY, distanceZ, 0.0f, 0.0f, 0.0, 0.0f, 1.0f, 0.0f);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    renderScene();

    glFlush();
}

void Display::resizeGL(int w, int h)
{
    if(h == 0) h = 1;

    glViewport(0, 0, w, h);             //reset the current view port
    glMatrixMode(GL_PROJECTION);        //select the model view matrix
    glLoadIdentity();                   // reset the model view matrix

    //calculate the aspect ratio of the window
    gluPerspective(60.0f, (GLfloat)w/h, 0.001f, 1000.0f); //fovy : view angle, (0.1f, 100.0f) : drawing area
    glMatrixMode(GL_MODELVIEW); // select model view matrix
    glLoadIdentity();   // reset the model view matrix

    //glViewport(0, 0, w, h);

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();

    //GLdouble dWt;
    //GLdouble dHt;

    //if(w > h) {
    //    dHt = 1;
    //    dWt = ((double)w) / h;
    //} else {
    //    dHt = ((double)h) / w;
    //    dWt = 1;
    //}

    //glFrustum(-dWt*0.04, dWt*0.04, -0.04, 0.04, 0.1, 15.0);
    //glOrtho(-dWt, dWt, -dHt, dHt, 5.0, 100.0);

    //gluLookAt(5.0, 2.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //gluLookAt(obj_pos[0], obj_pos[1], obj_pos[2], obj_pos[0] + eyeVector[0], obj_pos[1] + eyeVector[1], obj_pos[2] + eyeVector[2], up[0], up[1], up[2]);
    //glMultMatrixf(view_rotate);

    //double scale = 2.0f/boxWidth;
    //glScalef(scale, scale, scale);
    //gluLookAt(obj_pos[0], obj_pos[1], obj_pos[2], obj_pos[0] + eyeVector[0], obj_pos[1] + eyeVector[1], obj_pos[2] + eyeVector[2], up[0], up[1], up[2]);
    //glMultMatrixf(view_rotate);
}

void Display::renderScene()
{
    drawAxis(boxWidth * 2);
    drawTree(sss->pRoot);

    //glPolygonMode(GL_FRONT, GL_LINE);

    float rot_mat[16];
    if (!noPath) {
        if (showAnim && !finishedAnim) {
            if (iPathSeg >= path.size() - 1) {
                finishedAnim = true;
                showAnim = false;
            } else {
                float dx = path[iPathSeg + 1]->x - path[iPathSeg]->x;
                float dy = path[iPathSeg + 1]->y - path[iPathSeg]->y;
                float dz = path[iPathSeg + 1]->z - path[iPathSeg]->z;
                float segLength = sqrt(dx * dx + dy * dy + dz * dz);
                segCount = segLength / epsilon;
                float x = path[iPathSeg]->x + dx / segCount * inSegCount;
                float y = path[iPathSeg]->y + dy / segCount * inSegCount;
                float z = path[iPathSeg]->z + dz / segCount * inSegCount;
                path[iPathSeg]->getRot(initialRot);
                float nextRot[3] = {initialRot[0], initialRot[1], initialRot[2]};
                if (path.size() == iPathSeg + 2) {
                    nextRot[0] = finalRot[0];
                    nextRot[1] = finalRot[1];
                    nextRot[2] = finalRot[2];
                } else {
                    path[iPathSeg + 1]->getRot(nextRot);
                }
                float rot[3];
                Rot3dSide::interpolateRot(initialRot, nextRot, inSegCount, segCount, rot);
                Rot3dSide::rot2mat(rot[0], rot[1], rot[2], rot_mat);
                rod(R, x, y, z, rot_mat, 0, 1, 0);

                usleep((99-animationSpeed)*500);
                inSegCount++;
                if (inSegCount > segCount) {
                    iPathSeg++;
                    inSegCount = 1;
                }
                update();
            }
        }
        drawPath(path, alpha, beta);
    }
    Rot3dSide::rot2mat(finalRot[0], finalRot[1], finalRot[2], rot_mat);
    rod(R, alpha[0], alpha[1], alpha[2], rot_mat, 1, 0, 0);  // start
    Rot3dSide::rot2mat(initialRot[0], initialRot[1], initialRot[2], rot_mat);
    rod(R, beta[0], beta[1], beta[2], rot_mat, 0, 0, 1);  // goal


    // Draw Obstacles
    drawEdges(sss->pRoot, transparency);
    drawSpheres(sss->pRoot, transparency);
}

//===========================================================//
//======            Functions to Draw Shapes           ======//
//===========================================================//
/*
 * DRAW AXIS
 */
void Display::drawAxis(double length) {
    // Disable lighting to let the axis show color all the time
    glDisable(GL_LIGHTING);
    glLineWidth(3.0);
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(length, 0, 0);

        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, length, 0);

        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, length);
    glEnd();
    glEnable(GL_LIGHTING);
}

/*
 * DRAW Edge
 */
void Display::drawEdges(ConfBox3d* b, int transparency) {
    glEnable(GL_BLEND);
    glLineWidth(3.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (list<Wall*>::iterator iter = b->Walls.begin(); iter != b->Walls.end(); ++iter) {
        Wall* w = *iter;
        double A[3] = {w->b->x - w->a->x, w->b->y - w->a->y, w->b->z - w->a->z};
        double B[3] = {w->c->x - w->a->x, w->c->y - w->a->y, w->c->z - w->a->z};
        double normal[3] = {A[1]*B[2] - B[1]*A[2], B[0]*A[2] - A[0]*B[2], A[0]*B[1] - A[1]*B[0]};
        glColor4f(1.0, 1.0, 1.0, transparency / 100.0);
        glBegin(GL_TRIANGLES);
            glNormal3d(normal[0], normal[1], normal[2]);
            glVertex3f(w->a->x, w->a->y, w->a->z);
            glVertex3f(w->b->x, w->b->y, w->b->z);
            glVertex3f(w->c->x, w->c->y, w->c->z);
        glEnd();
    }
    glLineWidth(1.0);
}

/*
 * DRAW Cube
 */
void Display::drawCube(double length) {
    glBegin (GL_LINE_LOOP);
        glVertex3f(length, length, 0);
        glVertex3f(-length,length,0);
        glVertex3f(-length,-length,0);
        glVertex3f(length,-length,0);
    glEnd();
    glBegin (GL_LINE_LOOP);
        glVertex3f(length, length, length);
        glVertex3f(-length, length, length);
        glVertex3f(-length, -length, length);
        glVertex3f(length, -length, length);
    glEnd();
    glBegin (GL_LINES);
        glVertex3f(length,length,0);
        glVertex3f(length,length,length);
        glVertex3f(-length,length,0);
        glVertex3f(-length,length,length);
        glVertex3f(-length,-length,0);
        glVertex3f(-length,-length,length);
        glVertex3f(length,-length,0);
        glVertex3f(length,-length,length);
    glEnd();
}

/*
 * DRAW Quads
 */
void Display::drawQuads(ConfBox3d* b, double epsilon) {
    switch (b->status) {
        case FREE:
        glLineWidth(1);
        glColor3f(0.25, 1, 0.25);
            break;
        case STUCK:
        glLineWidth(1);
        glColor3f(1, 0.25, 0.25);
            break;
        case MIXED:
        glLineWidth(1);
        glColor3f(1, 1, 0.25);
        if (b->width < epsilon) {
            glColor3f(0.5, 0.5, 0.5);
        }
            break;
        case UNKNOWN:
        std::cout << "UNKNOWN in drawQuad" << std::endl;
            break;
        default:
        std::cout << b->status << std::endl;
        std::cerr << "Wrong Box Type" << std::endl;
            exit(1);
    }

    if (showBox) {
        glDisable(GL_LIGHTING);
        glPushMatrix();
            glTranslated(b->x, b->y, b->z);
            drawCube(b->width);
        glPopMatrix();
        glEnable(GL_LIGHTING);
    }
}

/*
 * DRAW Tree
 */
void Display::drawTree(ConfBox3d* b) {
    if (b) {
        if (b->isLeaf() || b->children.size() != 8) {
            drawQuads(b, epsilon);
        } else {
            for (int i = 0; i < 8; ++i) {
                drawTree(b->children[i]);
            }
        }
    }
}

/*
 * DRAW PATH
 */
void Display::drawPath(vector<ConfBox3d*>& path, double alpha[3], double beta[3])
{
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(3.0);
    glBegin(GL_LINE_STRIP);
        glVertex3f(beta[0], beta[1], beta[2]);
        for (unsigned int i = 0; i < path.size(); ++i) {
            glVertex3f(path[i]->x, path[i]->y, path[i]->z);
        }
        glVertex3f(alpha[0], alpha[1], alpha[2]);
    glEnd();
}

/*
 * DRAW Sphere
 */
void Display::drawSpheres(ConfBox3d* b, int transparency) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (list<Sphere*>::iterator iter = b->spheres.begin(); iter != b->spheres.end(); ++iter) {
        Sphere* s = *iter;
        filledSphere(s->radius, s->x, s->y, s->z, 1.0, 1.0, 1.0, transparency / 100.0);
    }
}
void Display::filledSphere(double radius, double x, double y, double z, double r, double g, double b, double transparency = 1) {
    sphereList = glGenLists(1);
    glNewList(sphereList, GL_COMPILE);
        GLUquadricObj *quadObj = gluNewQuadric ();
        gluQuadricDrawStyle(quadObj, GLU_FILL);
        gluQuadricNormals(quadObj, GLU_SMOOTH);
        gluSphere(quadObj, radius, 100, 100);
    glEndList();

    glPushMatrix();
        glColor4f(r, g, b, transparency);
        glTranslated(x, y, z);
        glCallList(sphereList);
    glPopMatrix();
}

/*
 * DRAW Rod ???
 */
void Display::rod(double length, double x, double y, double z, float rot_mat[16], double r = 1, double g = 1, double b = 1) {
    glDisable(GL_LIGHTING);
    glLineWidth(5.0);
    glPushMatrix();
        glTranslatef(x, y, z);
        glMultMatrixf(rot_mat);
        glBegin(GL_LINES);
            glColor3f(r, g, b);
            glVertex3f(0, 0, 0);
            glVertex3f(0, length, 0);
        glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}
