
#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include "glui.h"
#endif

#include <utility>
#include <vector>
#include <stdlib.h>
#include "display.h"
#include <CORE/ExprRep.h>

#include "mk-defs.h"
#include "Point.h"
#include "mk-inl.h"

extern MKPredicates<DT,NT> * pred;

namespace display_funcs
{

//CORE openGL Wrapper
inline void vertex2fWrapper(const double &x, const double &y){ glVertex2f(x, y); }
inline void glVertex2f_core(const double &x, const double &y){ glVertex2f(x, y); }
inline void glVertex2f_core(const Point2d& p){ glVertex2f_core(p[0].doubleValue(),p[1].doubleValue());}
inline void glColor3d_core(const double& r, const double& g,const double& b){ glColor3d(r,g,b);}
inline void glTranslated_core(const double& x, const double& y,const double& z){ glTranslated(x,y,z);}
inline void glRotated_core(const double& a, const double& x, const double& y,const double& z){ glRotated(a,x,y,z);}


// GLUI controls ========================================
//
GLUI_RadioGroup* radioBuildOptions=NULL;
GLUI_EditText* editInput=NULL;
GLUI_EditText* editDir=NULL;
GLUI_EditText* editRadius=NULL;
GLUI_EditText* editEpsilon=NULL;
GLUI_EditText* editMaxEpsilon=NULL;
//    GLUI_EditText* editAlphaX;
//    GLUI_EditText* editAlphaY;
//    GLUI_EditText* editBetaX;
//    GLUI_EditText* editBetaY;

GLUI_Translation * guiTranslate=NULL;
GLUI_Translation * guiZoom=NULL;

//information display
GLUI_StaticText * selectedBoxInfo=NULL; //information about selected box
GLUI_StaticText * vorInfo=NULL;

// GLUI controls ========================================
//
program_params_t PROG_PARAMS;


//
void CreateGUI()
{
    glutInitDisplayMode( GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH );
    glutInitWindowSize(PROG_PARAMS.windowWidth, PROG_PARAMS.windowHeight);
    glutInitWindowPosition(PROG_PARAMS.windowPosX, PROG_PARAMS.windowPosY);

    string title="Soft Curve Arrangement (Press ESC to exit)";
    int windowID = glutCreateWindow((char*)title.c_str());

     //glutCreateWindow( "dude 2d" );

    InitGL();

    glutDisplayFunc(DisplayHandler);
    glutReshapeFunc(ReshapeHandler);
    glutKeyboardFunc(KeyHandler);
    glutSpecialFunc(SpecialKey);
    glutMouseFunc(Mouse);


    //GLUI_Master.set_glutSpecialFunc(SpecialKey);
    GLUI *glui = GLUI_Master.create_glui( title.c_str(), 0, PROG_PARAMS.windowPosX+PROG_PARAMS.windowWidth+10, PROG_PARAMS.windowPosY );

    // SETTING UP THE CONTROL PANEL:
    GLUI_Panel * top_panel=glui->add_panel("Soft Curve Arrangement Controls");
    editInput = glui->add_edittext_to_panel(top_panel, "F(x,y):", GLUI_EDITTEXT_TEXT );
    editInput->set_text((char*)PROG_PARAMS.fxy_str.c_str());
    editInput->set_w(400);

    editDir = glui->add_edittext_to_panel(top_panel, "G(x,y):", GLUI_EDITTEXT_TEXT );
    editDir->set_text((char*)PROG_PARAMS.gxy_str.c_str());
    editDir->set_w(400);

    editEpsilon = glui->add_edittext_to_panel(top_panel, "Epsilon:", GLUI_EDITTEXT_FLOAT );
    editEpsilon->set_float_val(PROG_PARAMS.min_size.doubleValue());
    editEpsilon->set_w(200);

    editMaxEpsilon = glui->add_edittext_to_panel(top_panel, "maxEpsilon:", GLUI_EDITTEXT_FLOAT );
    editMaxEpsilon->set_float_val(PROG_PARAMS.max_size.doubleValue());

//    radioBuildOptions=glui->add_radiogroup_to_panel(top_panel,&vor_build_option);
//    glui->add_radiobutton_to_group(radioBuildOptions, "build Curves CKY");
//    glui->add_radiobutton_to_group(radioBuildOptions, "build Curves VF");
//
//    GLUI_Button* buttonRun = glui->add_button_to_panel(top_panel, "Run", -1, (GLUI_Update_CB)run);
//    buttonRun->set_name("Run me"); // Hack, but to avoid "unused warning" (Chee)

    // New column:
    //glui->add_column_to_panel(top_panel,true);

    glui->add_separator_to_panel(top_panel);
    glui->add_checkbox_to_panel(top_panel,"Box Boundary", &PROG_PARAMS.showBoxBoundary)->set_int_val(PROG_PARAMS.showBoxBoundary);
    glui->add_checkbox_to_panel(top_panel,"Show F", &PROG_PARAMS.showF)->set_int_val(PROG_PARAMS.showF);
    glui->add_checkbox_to_panel(top_panel,"Show G", &PROG_PARAMS.showG)->set_int_val(PROG_PARAMS.showG);
    glui->add_checkbox_to_panel(top_panel,"Show Unit Circle", &PROG_PARAMS.showUnitCircle)->set_int_val(PROG_PARAMS.showUnitCircle);
    glui->add_checkbox_to_panel(top_panel,"Show Axis", &PROG_PARAMS.showAxis)->set_int_val(PROG_PARAMS.showAxis);
    glui->add_checkbox_to_panel(top_panel,"Show Offset Axis", &PROG_PARAMS.showOffsetAxis)->set_int_val(PROG_PARAMS.showOffsetAxis);

    // Save image button
    //glui->add_button_to_panel(top_panel, "Save Image", 0, (GLUI_Update_CB)renderScenePS );

    // Quit button
    glui->add_button_to_panel(top_panel, "Quit", 0, (GLUI_Update_CB)exit );


    //
    GLUI_Panel * selected_box_panel=glui->add_panel("Rendering Options for Selected Box");
//    glui->add_checkbox_to_panel(selected_box_panel,"Circle (clearance+2*Rb)", &sel_circle, -1, (GLUI_Update_CB)renderScene)->set_int_val(sel_circle);
//    glui->add_checkbox_to_panel(selected_box_panel,"Features in the circle", &sel_features, -1, (GLUI_Update_CB)renderScene)->set_int_val(sel_features);
//    glui->add_checkbox_to_panel(selected_box_panel,"Wall Bisectors (purple)", &sel_wall_bisectors, -1, (GLUI_Update_CB)renderScene)->set_int_val(sel_wall_bisectors);
//    glui->add_checkbox_to_panel(selected_box_panel,"Corner Bisectors (green)", &sel_corner_bisectors, -1, (GLUI_Update_CB)renderScene)->set_int_val(sel_corner_bisectors);
//    glui->add_checkbox_to_panel(selected_box_panel,"Parabola (gray)", &sel_parabola, -1, (GLUI_Update_CB)renderScene)->set_int_val(sel_parabola);


    //translate and zoom gui
    GLUI_Panel * bottom_panel=glui->add_panel("View Control");
    guiTranslate=glui->add_translation_to_panel(bottom_panel, "Translate", GLUI_TRANSLATION_XY, PROG_PARAMS.gui_dXY);
    guiTranslate->set_speed(0.01);
    glui->add_column_to_panel(bottom_panel,true);
    guiZoom=glui->add_translation_to_panel(bottom_panel, "Zoom", GLUI_TRANSLATION_Z,&PROG_PARAMS.scale);
    guiZoom->set_speed(0.005);

    // reset button
    glui->add_column_to_panel(bottom_panel,true);
    glui->add_button_to_panel(bottom_panel, "Reset View", 0, (GLUI_Update_CB)reset_move );

    //add some display
    vorInfo=glui->add_statictext("var \n info"); //
    selectedBoxInfo=glui->add_statictext("no selected box"); //information about selected box

    glui->set_main_gfx_window( windowID );
}


inline void drawCXYLineSegments(vector< pair<cxy::Point *, cxy::Point *> >& segs)
{
    int size=segs.size();
    glBegin(GL_LINES);
    for(int i=0;i<size;i++)
    {
        pair<cxy::Point *, cxy::Point *>& line=segs[i];
        glVertex2d(line.first->getX(),line.first->getY());
        glVertex2d(line.second->getX(),line.second->getY());
    }
    glEnd();
}

// The main display routine.
void DisplayHandler()
{
    ClearBackground();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glLoadIdentity();

    //draw the outer boundary
    {
        Point2d UL, UR, LR, LL;
        const_cast<Box*>(PROG_PARAMS.b0)->getCorners(UL, UR, LR, LL);
        glColor3f(1,1,1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_POLYGON);
        glVertex2f_core(UL);
        glVertex2f_core(UR);
        glVertex2f_core(LR);
        glVertex2f_core(LL);
        glEnd();
    }

    glPushMatrix();
    treeTraverse(PROG_PARAMS.b0);
    glPopMatrix();

    drawQuad_selected(PROG_PARAMS.g_selected_PM);

    //draw the outer boundary
    {
        glLineWidth(3);
        Point2d UL, UR, LR, LL;
        const_cast<Box*>(PROG_PARAMS.b0)->getCorners(UL, UR, LR, LL);
        glPushMatrix();
        glTranslated(0,0,0.1);
        glColor3f(0.25, 0.25 , 0.25);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_POLYGON);
        glVertex2f_core(UL);
        glVertex2f_core(UR);
        glVertex2f_core(LR);
        glVertex2f_core(LL);
        glEnd();
        glPopMatrix();
    }

  /*
  glLineWidth(1.5f);        // thick line!
  glColor3f(1.0f,0.0f,0.0f);    // red
  vector<const BoxT<DoubleWrapper> *>::const_iterator it = PROG_PARAMS.n_it.begin();

  // The old mapping for the sake of references
  // v1=a.getX(); v2=b.getX(); v3=a.getY(); v4=b.getY();
  while (it != PROG_PARAMS.n_it.end()) {
    drawQuad(*it);
    ++it;
  }

  glColor3f(0.0f,1.0f,0.0f);    //green
  it = PROG_PARAMS.outer_it.begin();
  //glLineWidth(5.0f);
  while (it != PROG_PARAMS.outer_it.end()) {
    drawQuad(*it);
    ++it;
  }

  glColor3f(0.0f,0.0f,0.0f);    //black
  it = PROG_PARAMS.inner_it.begin();
  //glLineWidth(5.0f);
  while (it != PROG_PARAMS.inner_it.end()) {
    drawQuad(*it);
    ++it;
  }

  glColor3f(0.0f,0.0f,1.0f);    //blue
  it = PROG_PARAMS.amb.begin();
  while (it != PROG_PARAMS.amb.end()) {
    drawQuad(*it);
    ++it;
  }
  */

    if(PROG_PARAMS.showF)
    {
        glLineWidth(1);
        glTranslated(0,0,0.2);
        glColor3d(0,.5,0);
        drawCXYLineSegments(PROG_PARAMS.func_F_cxy_line_segs);
    }

    if(PROG_PARAMS.showG)
    {
        glLineWidth(1);
        glColor3d(0.5,0,0);
        glTranslated(0,0,0.2);
        drawCXYLineSegments(PROG_PARAMS.func_G_cxy_line_segs);
    }

      // Chee(Mar'12): Draw the X- and Y-axes!
    if(PROG_PARAMS.showAxis)
    {
      glTranslated(0,0,0.2);
      glLineWidth(2.0f);        // thick line!
      glColor3f(0.0f, 0.2f, 0.8f);  // bluish line
      glBegin(GL_LINES);
      vertex2fWrapper( PROG_PARAMS.x_min.doubleValue(), 0.0f); // X-axis
      vertex2fWrapper( PROG_PARAMS.x_max.doubleValue(), 0.0f); // X-axis
      vertex2fWrapper( 0.0f, PROG_PARAMS.y_min.doubleValue()); // Y-axis
      vertex2fWrapper( 0.0f, PROG_PARAMS.y_max.doubleValue()); // Y-axis
      glEnd();
    }

      // Chee(Mar'12): Draw the line at unit distance from the axes, to inidicate scale!
      if(PROG_PARAMS.showOffsetAxis)
      {
          glTranslated(0,0,0.2);
          glLineWidth(2.0f);        // thick line!
          glColor3f(0.0f, 0.8f, 0.8f);  // bluish-green
          glBegin(GL_LINES);
          vertex2fWrapper( PROG_PARAMS.x_min.doubleValue(), 1.0f); // X-axis
          vertex2fWrapper( PROG_PARAMS.x_max.doubleValue(), 1.0f); // X-axis
          vertex2fWrapper( 1.0f, PROG_PARAMS.y_min.doubleValue()); // Y-axis
          vertex2fWrapper( 1.0f, PROG_PARAMS.y_max.doubleValue()); // Y-axis
          glEnd();
      }


      if(PROG_PARAMS.showUnitCircle)
      {
          glTranslated(0,0,0.2);
          // Draw circle around origin:
          glLineWidth(2.0f);
          static machine_double width=PROG_PARAMS.b0->width().doubleValue();
          drawCircle(1.0,500.0/width, Point2d(0,0) ,0.0f, 0.2f, 0.8f);
      }

  glLineWidth(1.0f);

  glFlush();

  glutSwapBuffers();
}




// Clear the background for display.
void ClearBackground()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  const machine_double scale = PROG_PARAMS.scale;
  const machine_double x_delta = PROG_PARAMS.gui_dXY[0];
  const machine_double y_delta = PROG_PARAMS.gui_dXY[1];

  gluOrtho2D((PROG_PARAMS.x_min.doubleValue())*scale + x_delta,
             (PROG_PARAMS.x_max.doubleValue())*scale + x_delta,
             (PROG_PARAMS.y_min.doubleValue())*scale + y_delta,
             (PROG_PARAMS.y_max.doubleValue())*scale + y_delta);

  glMatrixMode(GL_MODELVIEW);
  glClearColor(0.9,0.9,0.9,0.0);
  //glClearColor(1,1,1,0.0);
}

// Call back when the window canvas is reshaped.
void ReshapeHandler(const GLsizei w,const GLsizei h)
{
  GLsizei d=min(w,h);


  const machine_double x_width=(PROG_PARAMS.x_max-PROG_PARAMS.x_min).doubleValue();
  const machine_double y_width=(PROG_PARAMS.y_max-PROG_PARAMS.y_min).doubleValue();
  if(x_width>y_width){
    GLsizei dy=d*(y_width/x_width);
    glViewport(0,0,d,dy);  
  }
  else
  {
    GLsizei dx=d*(x_width/y_width);
    glViewport(0,0,dx,d);  
  }

  
  ClearBackground();
}

// Called when a key is pressed in the context of the opengl window
// used to exit when escape is pressed.
void KeyHandler(const unsigned char key, const int x, const int y)
{

  switch (key)
    {
      case 'q':
      case 27:             // ESCAPE key
        exit(0);
        break;
      case 'w':
        PROG_PARAMS.gui_dXY[1]+=0.5*PROG_PARAMS.scale;
        break;
      case 'a':
        PROG_PARAMS.gui_dXY[0]-=0.5*PROG_PARAMS.scale;
        break;
      case 's':
        PROG_PARAMS.gui_dXY[1]-=0.5*PROG_PARAMS.scale;
        break;
      case 'd':
        PROG_PARAMS.gui_dXY[0]+=0.5*PROG_PARAMS.scale;
        break;
      case 'o': // zoom out
        PROG_PARAMS.scale /= 0.9;
        //PROG_PARAMS.x_delta = 0;
        //PROG_PARAMS.y_delta = 0;
        break;
      case 'k':
      case 'i': // zoom in
        PROG_PARAMS.scale *= 0.9;
        break;
      case 'r': // reset
      case ' ':
        reset_move();
        break;
    }

//  switch( key ){
//      case 27: exit(0);
//      case 'w': deltaY_Render+=boxHeight/(uscale_Render*100); break;
//      case 's': deltaY_Render-=boxHeight/(uscale_Render*100); break;
//      case 'd': deltaX_Render+=boxWidth/(uscale_Render*100); break;
//      case 'a': deltaX_Render-=boxWidth/(uscale_Render*100); break;
//      case '=': uscale_Render*=1.5; break;
//      case '-': uscale_Render/=1.5; break;
//      case 'r': reset_move(); break;
//      default: return;
//  }

    //ClearBackground();
    //DisplayHandler();
    GLUI_Master.sync_live_all();
    glutPostRedisplay();
}



// Chee(Mar'12): drawing a circle:
const float DEG2RADX = 3.14159/180;
void drawCircle(float radius)
{
   glBegin(GL_LINE_LOOP);
   	for (int i=0; i < 360; i++) {
      	   float degInRad = i*DEG2RADX;
      	   glVertex2f(cos(degInRad)*radius,sin(degInRad)*radius);
   	}
   glEnd();
}//drawCircle


void drawCircle( double Radius, int numPoints, const Point2d& o, double r, double g, double b)
{
    glColor3d_core(r,g,b);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_LINE_LOOP);
    for( int i = 0; i <= numPoints; ++i )
    {
        machine_double Angle = i * (2.0* 3.1415926 / numPoints);
        machine_double X = cos( Angle )*Radius;
        machine_double Y = sin( Angle )*Radius;
        glVertex2f_core( X + o[0].doubleValue(), Y + o[1].doubleValue());
    }
    glEnd();
}

void filledCircle( double radius, int numPoints, const Point2d& o, double r, double g, double b)
{
    glColor3d_core(r,g,b);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    for( int i = 0; i <= numPoints; ++i )
    {
        machine_double Angle = i * (2.0* 3.1415926 / numPoints);
        machine_double X = cos( (Angle) )*radius;
        machine_double Y = sin( (Angle) )*radius;
        glVertex2f_core( X + o[0].doubleValue(), Y + o[1].doubleValue());
    }
    glEnd();
}


void drawQuad(const Box* b)
{
//    switch(b->status)
//    {
//        case Box::OUT:
//
//            glColor3f(1, 1, 1); //White
//            break;
//
//        case Box::ON:
//
//            glColor3f(0.85, 0.85, 0.85);
//            break;
//
//        case Box::IN:
//
//            glColor3f(1, 1, 0.25);
//
//            if (b->height < epsilon || b->width < epsilon)
//            {
//                glColor3f(0.5, 0.5, 0.5);
//            }
//
//            break;
//
//        case Box::UNKNOWN:
//
//            std::cout << "! Error: found UNKNOWN box in drawQuad" << std::endl;
//            break;
//    }

    glColor3f(1, 1, 1); //White

    Point2d UL, UR, LR, LL;
    const_cast<Box*>(b)->getCorners(UL, UR, LR, LL);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glVertex2f_core(UL);
    glVertex2f_core(UR);
    glVertex2f_core(LR);
    glVertex2f_core(LL);
    glEnd();

    if (PROG_PARAMS.showBoxBoundary)
    {
        glPushMatrix();
        glTranslated(0,0,0.1);
        glColor3f(0.5, 0.5 , 0.5);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_POLYGON);
        glVertex2f_core(UL);
        glVertex2f_core(UR);
        glVertex2f_core(LR);
        glVertex2f_core(LL);
        glEnd();
        glPopMatrix();
    }

    //draw Vor segments
//    if(showVor)
//    {
//        glBegin(GL_LINES);
//        glLineWidth(3);
//        glColor3d_core(.75,0,0);
//        for(list<VorSegment>::iterator i=b->vor_segments.begin();i!=b->vor_segments.end();i++){
//            glVertex2f_core(i->p);
//            glVertex2f_core(i->q);
//        }
//        glEnd();
//    }

    glLineWidth(1);
}

void drawQuad_selected(list<Box*> boxes)
{

    glTranslated(0,0,0.2);

    double radius=PROG_PARAMS.b0->width().doubleValue()*PROG_PARAMS.scale/200;

    filledCircle(radius,50,Point2d(PROG_PARAMS.sel_x,PROG_PARAMS.sel_y),0.5,0.5,0);

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

        DT w2=(b->width()/2)*0.9;
        DT h2=(b->height()/2)*0.9;

        Point2d o=b->center();

        //draw a highlight box
        glBegin(GL_LINE_LOOP);
        glVertex2f_core(Point2d(o[0] - w2, o[1] - h2));
        glVertex2f_core(Point2d(o[0] + w2, o[1] - h2));
        glVertex2f_core(Point2d(o[0] + w2, o[1] + h2));
        glVertex2f_core(Point2d(o[0] - w2, o[1] + h2));
        glEnd();

    }

    //draw details of the last selected box
    //Box * b=boxes.back();

    //draw circle with radius (clearance+2*Rb)
    if(PROG_PARAMS.sel_circle)
    {
//        drawCircle( b->rB*2+b->cl_m, 100, b->o, 1,1,0);
    }

    //draw features in blue
    if(PROG_PARAMS.sel_features)
    {
//        //draw wall features
//        glLineWidth(2);
//        glBegin(GL_LINES);
//        glColor3d_core(0,0,1); //blue
//        for(WIT i=b->walls.begin();i!=b->walls.end();i++){
//            Wall * w=*i;
//            glVertex2f_core(w->src->pos);
//            glVertex2f_core(w->dst->pos);
//        }
//        glEnd();
//
//        //draw corner features
//        glLineWidth(1);
//        for(CIT i=b->corners.begin();i!=b->corners.end();i++){
//            Corner*c=*i;
//            filledCircle(5/uscale_Render,c->pos,0.75,0.75,1);
//            drawCircle(5/uscale_Render,36, c->pos,0,0,1);
//        }
    }

    glLineWidth(1);
}



//-----------------------------------------------------------------------------
// regular openGL callback functions
bool InitGL()
{
    // *Antialias*
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    // others
    glEnable( GL_DEPTH_TEST);
    glClearColor( 1,1,1,0 );

    return true;
}

void treeTraverse(Box* b)
{
    //cout<<"Draw "<<*b<<endl;
    if (b==NULL)
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

// gather information and show it on the GUI diagram
void updateVARinfo()
{
    char info[1024];

    static int leave_size=-1;
    if(leave_size<0)
    {
        list<Box*> leaves;
        const_cast<Box*>(PROG_PARAMS.b0)->getLeaves(leaves);
        leave_size=leaves.size();
    }

    sprintf(info,"Time used: %.2f ms; # of leaves=%d",PROG_PARAMS.timeused,leave_size);
    vorInfo->set_text(info);
}

//
// call this function when the selected box is changed
// show related information on GUI diagram
//
void updateSelectedBoxInfo()
{
    if(PROG_PARAMS.g_selected_PM.empty())
    {
        selectedBoxInfo->set_text("no selected box");
        return;
    }

    Box * selected=PROG_PARAMS.g_selected_PM.back();
    
    //do something about selected here
    
    char info[1024];
    sprintf(info,"Selected box has xxx");
    selectedBoxInfo->set_text(info);
}


void reset_move()
{
    //PROG_PARAMS.uscale_Render=1;
    //PROG_PARAMS.deltaX_Render=PROG_PARAMS.deltaY_Render=0;
    PROG_PARAMS.scale = 1; //1.1;
    PROG_PARAMS.gui_dXY[0] = 0;
    PROG_PARAMS.gui_dXY[1] = 0;
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
        case GLUT_KEY_DOWN:
            if(PROG_PARAMS.g_selected_PM.empty()==false){//not empty
                Box * last=PROG_PARAMS.g_selected_PM.back();
                if(last!=PROG_PARAMS.b0)
                {
                    cout<<"Selected Parent box="<<*(last->pParent)<<endl;
                    bool c0=pred->Exclude(last->pParent);
                    bool jr=pred->JTest(last->pParent);
                    int mkr=pred->MKTest(last->pParent);
                    cout<<"\t c0-test="<<c0<<endl;
                    cout<<"\t J-test="<<((jr)?"has root":"no root")<<endl;
                    cout<<"\t MK-test="<<mkr<<endl;

                    PROG_PARAMS.g_selected_PM.push_back(last->pParent);
                }
            }
            else{
                PROG_PARAMS.gui_dXY[1]+=0.5*PROG_PARAMS.scale;
            }
            break;

        case GLUT_KEY_UP:
            if(PROG_PARAMS.g_selected_PM.size()>1)
                PROG_PARAMS.g_selected_PM.pop_back();
            else
                PROG_PARAMS.gui_dXY[1]-=0.5*PROG_PARAMS.scale;
            break;

        case GLUT_KEY_RIGHT:
            PROG_PARAMS.gui_dXY[0]-=0.5*PROG_PARAMS.scale;
            break;

        case GLUT_KEY_LEFT:
            PROG_PARAMS.gui_dXY[0]+=0.5*PROG_PARAMS.scale;
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

        if( glutGetModifiers()==GLUT_ACTIVE_CTRL )
        {

            //remove selected boxes
            PROG_PARAMS.g_selected_PM.clear();

            static DT width=PROG_PARAMS.b0->width();
            static DT height=PROG_PARAMS.b0->height();
            static DT min_x=PROG_PARAMS.b0->x_range.getL();
            static DT min_y=PROG_PARAMS.b0->y_range.getL();

            int viewport[4];
            glGetIntegerv(GL_VIEWPORT,viewport);
            machine_double v_w=viewport[2];
            machine_double v_h=viewport[3];

            DT nwidth=width * (PROG_PARAMS.windowWidth/v_w);
            DT nheight=height * (PROG_PARAMS.windowHeight/v_h);

            machine_double m_x=(x-PROG_PARAMS.windowWidth/2)*PROG_PARAMS.scale+PROG_PARAMS.windowWidth/2;
            machine_double m_y=(PROG_PARAMS.windowHeight/2-y)*PROG_PARAMS.scale+PROG_PARAMS.windowHeight/2;

            PROG_PARAMS.sel_x=m_x=(nwidth*m_x/PROG_PARAMS.windowWidth+min_x).doubleValue() + PROG_PARAMS.gui_dXY[0];
            PROG_PARAMS.sel_y=m_y=(nheight*m_y/PROG_PARAMS.windowHeight+min_y).doubleValue() + PROG_PARAMS.gui_dXY[1];

            Box * selected = const_cast<Box*>(PROG_PARAMS.b0)->find(Point2d(m_x,m_y));

            if(selected!=NULL)
            {

                cout<<"Selected box="<<*selected<<endl;
                bool c0=pred->Exclude(selected);
                bool jr=pred->JTest(selected);
                int mkr=pred->MKTest(selected);
                cout<<"\t c0-test="<<c0<<endl;
                cout<<"\t J-test="<<((jr)?"has root":"no root")<<endl;
                cout<<"\t MK-test="<<mkr<<endl;
                PROG_PARAMS.g_selected_PM.push_back(selected);
            }//end selected

        }

        updateSelectedBoxInfo();
        glutPostRedisplay();
    }//if pressed the right key/button

}

/*
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
    PS.rect(CORE::Todouble(b->o[0]-b->width / 2), CORE::Todouble(b->o[1] - b->height / 2),
            CORE::Todouble(b->o[0] + b->width / 2), CORE::Todouble(b->o[1] + b->height / 2) );
    PS.setstrokegray(0.5);

    if (showBoxBoundary)
        PS.fillstroke();
    else
        PS.fill();
}
*/

} // namespace display_funcs.
