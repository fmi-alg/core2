/* **************************************
   File: main.cpp

   Description:
   This is the entry point for the running the SSS algorithm
   for a rod robot amidst a collection of polyhedrons (obstacles).
   The framework will ultimately support a variety of robots.

   NOTES:
   0. This file originated in the (3d) ball program done by
   	Surin Ahn (June 2013) for the
	2013 Siemens Science & Math Competition under th
	supervision of Professor Yap.  Surin was a
    	Regional Finalist (one out of 100 nationwide) from New York.
        In 2014, she went to Princeton in Computer Science.
   1. See examples of running this program in the Makefile.
   2. Format of input environment: see README FILE
   3. Use WASD keys to rotate the scene
   4. In Fall of 2014, we started to reorgnized all the programs
   	to be consistent with our general SSS framework.

Author:
	Ziyao Wei and Chee Yap (Fall 2014)


   ************************************** */

#include "./main.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

SoftSubdivisionSearch* initializeSSS(double alpha[3], double beta[3]) {
  ConfBox3d* root = new ConfBox3d(boxWidth/2, boxWidth/2, boxWidth/2, boxWidth);
  ConfBox3d::r0 = R0;

  parseConfigFile(root);

  cout<<"inside : initializeSSS Qtype= " << QType << "\n";
  return new SoftSubdivisionSearch(root, epsilon, QType, seed++, alpha, beta);  // Note that seed keeps changing!
}

void animReplay() {
  iPathSeg = 0;
  finishedAnim = 0;
}

void run() {
  //update from glui live variables
  string previousResults = outputStream.str();
  outputStream.str(string());
  outputStream.clear();
  outputStream << "Run " << runCounter++ << ":" << endl;
  fileName = editInput->get_text();
  inputDir = editDir->get_text();
  R0 = editRadius->get_float_val();
  epsilon = editEpsilon->get_float_val();
  alpha[0] = editAlphaX->get_float_val();
  alpha[1] = editAlphaY->get_float_val();
  alpha[2] = editAlphaZ->get_float_val();
  beta[0] = editBetaX->get_float_val();
  beta[1] = editBetaY->get_float_val();
  beta[2] = editBetaZ->get_float_val();
  initialRot[0] = 0;
  initialRot[1] = 1;
  initialRot[2] = 0;
  finalRot[0] = -1;
  finalRot[1] = 0;
  finalRot[2] = 0;
  QType = radioQType->get_int_val();

  outputStream<<"inside run:  Qtype = " << QType << "\n";

  Timer t;
  // start timer
  t.start();

  if (showAnim) {
    animReplay();
  }

  if (sss) {
    delete(sss);
  }

  sss = initializeSSS(alpha, beta);

  path = sss->softSubdivisionSearch();
  int ct = sss->getNumSubdivisions();
  noPath = path.empty();

  // stop timer
  t.stop();
  // print the elapsed time in millisec
  cout << ">>>>>>>>>>>>>>>>>>>>>>>>\n";
  cout << ">>\n";
  cout << ">>     Time used: " << t.getElapsedTimeInMilliSec() << " ms\n";
  cout << ">>\n";

  if (!noPath) {
    cout << ">>     Path found !" << endl;
  } else {
    cout << ">>     No Path !" << endl;
  }
  cout << ">>\n";
  cout << ">>>>>>>>>>>>>>>>>>>>>>>>\n";
  cout << "Expanded " << ct << " times" << endl;
  cout << "total Free boxes: " << sss->freeCount << endl;
  cout << "total Stuck boxes: " << sss->stuckCount << endl;
  cout << "total Mixed boxes < epsilon: " << sss->mixSmallCount << endl;
  cout << "total Mixed boxes > epsilon: " << sss->mixCount - ct - sss->mixSmallCount << endl;
  cout << endl;
  cout << previousResults;
  //output->set_text(outputStream.str().c_str());
}

void printRotationAngles() {
  double x = atan2(view_rotate[9], view_rotate[10]);
  double y = atan2(-view_rotate[8], sqrt(pow(view_rotate[9], 2) + pow(view_rotate[10], 2)));;
  double z = atan2(view_rotate[4], view_rotate[0]);
  cout << "X = " << x << endl;
  cout << "Y = " << y << endl;
  cout << "Z = " << z << endl;
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
  if (!useCustomViewAngles) {
    yAng = 3.14 / 4;
  } else {
    xAng = view_rotate_angles[0];
    yAng = view_rotate_angles[1];
    zAng = view_rotate_angles[2];
  }

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

  viewRot->set_float_array_val(view_rotate);
}

void resetViewPoint() {
  resetRotationMatrix();
  obj_pos[0] = eye[0];
  obj_pos[1] = eye[1];
  obj_pos[2] = eye[2];
}

void resetTopViewPoint() {
  topViewPos[0] = static_cast<float>(boxWidth / 2.0);
  topViewPos[1] = static_cast<float>(boxWidth / 2.0);
  topViewPos[2] = static_cast<float>(boxWidth * 2.5);
}

void logNonInteractiveRun(bool noPath) {
  // do something...
  cout << "Non Interactive Run of Disc Robot" << endl;
  if (noPath) {
    cout << "No Path Found!" << endl;
  } else {
    cout << "Path was Found!" << endl;
  }
}

// Recursively get the leaf descendants of Box b
void drawTree(ConfBox3d* b) {
  if (b) {
    if (b->isLeaf() || b->children.size() != 8) {
      drawQuad(b, epsilon, radioDrawOption->get_int_val());
    } else {
      for (int i = 0; i < 8; ++i) {
        drawTree(b->children[i]);
      }
    }
  }
}

void renderScene(void) {
  drawAxis(boxWidth * 2);

  drawTree(sss->pRoot);        // draws the octree

  glPolygonMode(GL_FRONT, GL_LINE);

  float rot_mat[16];
  if (!noPath) {
    if (showAnim && !finishedAnim) {
      if (iPathSeg >= path.size() - 1) {
        finishedAnim = true;
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
        rod(R0, x, y, z, rot_mat, 0, 1, 0);
      }
    }
    drawPath(path, alpha, beta);
  }
  Rot3dSide::rot2mat(finalRot[0], finalRot[1], finalRot[2], rot_mat);
  rod(R0, alpha[0], alpha[1], alpha[2], rot_mat, 1, 0, 0);  // start
  Rot3dSide::rot2mat(initialRot[0], initialRot[1], initialRot[2], rot_mat);
  rod(R0, beta[0], beta[1], beta[2], rot_mat, 0, 0, 1);  // goal
  drawEdges(sss->pRoot, transparency);
  drawSpheres(sss->pRoot, transparency);

  glEnable(GL_LIGHTING);
  glutSwapBuffers();
}

void renderTopView(void) {
  setUpView(topViewXYAspect, 2.0 / boxWidth);
  gluLookAt(topViewPos[1], topViewPos[2], topViewPos[0], topViewPos[1], topViewPos[2] - 1, topViewPos[0], 1, 0, 0);
  renderScene();
}

void renderCustomView(void) {
  setUpView(customViewXYAspect, 2.0 / boxWidth);
  editEyeX->set_float_val(obj_pos[0]);
  editEyeY->set_float_val(obj_pos[1]);
  editEyeZ->set_float_val(obj_pos[2]);
  editLookAtX->set_float_val(at[0]);
  editLookAtY->set_float_val(at[1]);
  editLookAtZ->set_float_val(at[2]);
  gluLookAt(obj_pos[0], obj_pos[1], obj_pos[2], obj_pos[0] + eyeVector[0], obj_pos[1] + eyeVector[1], obj_pos[2] + eyeVector[2], up[0], up[1], up[2]);
  glMultMatrixf(view_rotate);

  renderScene();
}

void reshapeTopView(int width, int height) {
  reshapeView(topViewWindowID, topViewXYAspect);
}

void reshapeCustomView(int width, int height) {
  reshapeView(customViewWindowID, customViewXYAspect);
}

void idle (int v) {
  glutTimerFunc(1000.0 / frameRate, idle, 0);
  inSegCount++;
  if (inSegCount > segCount) {
    iPathSeg++;
    inSegCount = 1;
  }
  glutSetWindow(customViewWindowID);
  glutPostRedisplay();
  glutSetWindow(topViewWindowID);
  glutPostRedisplay();
}

// MAIN PROGRAM: ========================================
int main(int argc, char* argv[]) {
  if (argc > 1) interactive = atoi(argv[1]);  // Interactive (0) or no (>0)
  if (argc > 2) alpha[0] = atof(argv[2]);    // start x
  if (argc > 3) alpha[1] = atof(argv[3]);    // start y
  if (argc > 4) alpha[2] = atof(argv[4]);    // start z
  if (argc > 5) beta[0] = atof(argv[5]);    // goal x
  if (argc > 6) beta[1] = atof(argv[6]);    // goal y
  if (argc > 7) beta[2] = atof(argv[7]);    // goal z
  if (argc > 8) epsilon = atof(argv[8]);    // epsilon (resolution)
  if (argc > 9) R0      = atof(argv[9]);    // robot radius
  if (argc > 10) fileName = argv[10];     // Input file name
  if (argc > 11) boxWidth = atof(argv[11]);  // boxWidth
  if (argc > 12) windowPosX = atoi(argv[12]);  // window X pos
  if (argc > 13) windowPosY = atoi(argv[13]);  // window Y pos
  if (argc > 14) QType   = atoi(argv[14]);  // PriorityQ Type (random or no)
  if (argc > 15) seed   = static_cast<unsigned int>(atoi(argv[15]));    // for random number generator
  if (argc > 16) {
    inputDir  = argv[16];    // path for input files
    std::stringstream ss;
    ss << inputDir << "/" << fileName;  // create full file name
    defaultFilePath = ss.str();
  }
  if (argc > 17) deltaX  = atof(argv[17]);  // x-translation of input file
  if (argc > 18) deltaY  = atof(argv[18]);  // y-translation of input file
  if (argc > 19) deltaZ = atof(argv[19]);    // z-translation of input file
  if (argc > 20) scale  = atof(argv[20]);    // scaling of input file
  if (argc > 21) eye[0] = atof(argv[21]);
  if (argc > 22) eye[1] = atof(argv[22]);
  if (argc > 23) eye[2] = atof(argv[23]);
  if (argc > 24) at[0] = atof(argv[24]);
  if (argc > 25) at[1] = atof(argv[25]);
  if (argc > 26) at[2] = atof(argv[26]);
  if (argc > 27) up[0] = atof(argv[27]);
  if (argc > 28) up[1] = atof(argv[28]);
  if (argc > 29) up[2] = atof(argv[29]);
  if (argc > 30) transparency = atoi(argv[30]);
  if (argc > 31) frameRate = atoi(argv[31]);
  if (argc > 34) {
    useCustomViewAngles = true;
    view_rotate_angles[0] = atof(argv[32]);
    view_rotate_angles[1] = atof(argv[33]);
    view_rotate_angles[2] = atof(argv[34]);
  }
  // if (argc > 40) {
    // begin_rot
  // }

  cout << "before interactive, Qtype= " << QType << endl;

  if (interactive > 0) {  // non-interactive
    logNonInteractiveRun(noPath);
    return 0;
  }

  // Else, set up for GLUT/GLUI interactive display:
  glutInit(&argc, argv);
  glutInitWindowSize(boxWidth, boxWidth);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowPosition(windowPosX + 800, windowPosY);
  topViewWindowID = glutCreateWindow("Top View");

  glutDisplayFunc(renderTopView);
  glutReshapeFunc(reshapeTopView);

  glutInitWindowPosition(10, windowPosY);
  customViewWindowID = glutCreateWindow("Custom View");

  glutDisplayFunc(renderCustomView);
  glutReshapeFunc(reshapeCustomView);

  glutTimerFunc(50, idle, 0);

  GLUI *glui = GLUI_Master.create_glui("control", 0, windowPosX + 300, windowPosY);

  // SETTING UP THE CONTROL PANEL:
  editInput = glui->add_edittext("Input file:", GLUI_EDITTEXT_TEXT);
  editInput->set_text(const_cast<char*>(fileName.c_str()));
  editDir = glui->add_edittext("Input Directory:", GLUI_EDITTEXT_TEXT);
  editDir->set_text(const_cast<char*>(inputDir.c_str()));

  GLUI_Panel * robot_box_panel = glui->add_panel("Robot Specs");
  editRadius = glui->add_edittext_to_panel(robot_box_panel,
             "Radius:", GLUI_EDITTEXT_FLOAT);
  editRadius->set_float_val(R0);
  editEpsilon = glui->add_edittext_to_panel(robot_box_panel,
              "Epsilon:", GLUI_EDITTEXT_FLOAT);
  editEpsilon->set_float_val(epsilon);

  // NOTE: this is a quick and dirty solution to the bug where Start and Goal
  //    configurations are interchanged!
  //GLUI_Panel * alpha_box_panel = glui->add_panel("Alpha (start configuration)");
  GLUI_Panel * alpha_box_panel = glui->add_panel("Beta (goal configuration)");
  editAlphaX = glui->add_edittext_to_panel(alpha_box_panel,
             "x:", GLUI_EDITTEXT_FLOAT);
  editAlphaX->set_float_val(alpha[0]);
  editAlphaY = glui->add_edittext_to_panel(alpha_box_panel,
             "y:", GLUI_EDITTEXT_FLOAT);
  editAlphaY->set_float_val(alpha[1]);
  editAlphaZ = glui->add_edittext_to_panel(alpha_box_panel, "z:", GLUI_EDITTEXT_FLOAT);
  editAlphaZ->set_float_val(alpha[2]);

  // NOTE: this is a quick and dirty solution to the bug where Start and Goal
  //    configurations are interchanged!
  // GLUI_Panel * beta_box_panel = glui->add_panel("Beta (goal configuration)");
  GLUI_Panel * beta_box_panel = glui->add_panel("Alpha (start configuration)");
  editBetaX = glui->add_edittext_to_panel(beta_box_panel,
            "x:", GLUI_EDITTEXT_FLOAT);
  editBetaX->set_float_val(beta[0]);
  editBetaY = glui->add_edittext_to_panel(beta_box_panel,
            "y:", GLUI_EDITTEXT_FLOAT);
  editBetaY->set_float_val(beta[1]);
  editBetaZ = glui->add_edittext_to_panel(beta_box_panel,
            "z:", GLUI_EDITTEXT_FLOAT);
  editBetaZ->set_float_val(beta[2]);

  GLUI_Panel * eye_box_panel = glui->add_panel("Eye Position");
  editEyeX = glui->add_edittext_to_panel(eye_box_panel,
            "x:", GLUI_EDITTEXT_FLOAT);
  editEyeY = glui->add_edittext_to_panel(eye_box_panel,
            "y:", GLUI_EDITTEXT_FLOAT);
  editEyeZ = glui->add_edittext_to_panel(eye_box_panel,
            "z:", GLUI_EDITTEXT_FLOAT);

  GLUI_Panel * look_at_box_panel = glui->add_panel("Look At Position");
  editLookAtX = glui->add_edittext_to_panel(look_at_box_panel,
            "at x:", GLUI_EDITTEXT_FLOAT);
  editLookAtY = glui->add_edittext_to_panel(look_at_box_panel,
            "at y:", GLUI_EDITTEXT_FLOAT);
  editLookAtZ = glui->add_edittext_to_panel(look_at_box_panel,
            "at z:", GLUI_EDITTEXT_FLOAT);

  editSeed = glui->add_edittext("seed:", GLUI_EDITTEXT_INT);
  editSeed->set_int_val(static_cast<int>(seed));

  GLUI_Panel* animation_panel = glui->add_panel("Animation Control");

  GLUI_Spinner* editFrameRate = glui->add_spinner_to_panel(animation_panel, "Frame Rate", GLUI_SPINNER_INT, &frameRate);
  editFrameRate->set_int_val(frameRate);
  editFrameRate->set_int_limits(1, 100);

  GLUI_Spinner* editTransparency = glui->add_spinner_to_panel(animation_panel, "Transparency", GLUI_SPINNER_INT, &transparency);
  editTransparency->set_int_val(transparency);
  editTransparency->set_int_limits(0, 100);

  GLUI_Button* buttonReplay = glui->add_button_to_panel(animation_panel, "Replay Animation", -1, reinterpret_cast<GLUI_Update_CB>(animReplay));
  buttonReplay->set_name("replay");
  buttonReplay->set_w(1);

  glui->add_separator();
  GLUI_Button* buttonRun = glui->add_button("Run", -1, reinterpret_cast<GLUI_Update_CB>(run));
  buttonRun->set_name("Run me"); // Hack, but to avoid "unused warning" (Chee)

  // New column:
  glui->add_column(true);

  glui->add_separator();
  radioQType = glui->add_radiogroup();
  glui->add_radiobutton_to_group(radioQType, "Random");
  glui->add_radiobutton_to_group(radioQType, "BFS (Breadth First)");
  // glui->add_radiobutton_to_group(radioQType, "A-star");
  glui->add_radiobutton_to_group(radioQType, "Best First Search");
  radioQType->set_int_val(QType);

  glui->add_separator();
  radioDrawOption = glui->add_radiogroup(0, -1, reinterpret_cast<GLUI_Update_CB>(renderCustomView));
  glui->add_radiobutton_to_group(radioDrawOption, "Show Box Boundary");
  glui->add_radiobutton_to_group(radioDrawOption, "Hide Box Boundary");

  glui->add_separator();

  viewRot = new GLUI_Rotation(glui, "Rotate", view_rotate);
  viewRot->set_spin(1.0);

  /****************************************/
  /*       Set up OpenGL lights           */
  /****************************************/
  setLightsForWindow(topViewWindowID);
  setLightsForWindow(customViewWindowID);

  /****************************************/
  /*          Initial Viewing Position     */
  /****************************************/
  eyeVector[0] = at[0] - eye[0];
  eyeVector[1] = at[1] - eye[1];
  eyeVector[2] = at[2] - eye[2];
  resetViewPoint();

  GLUI_Translation *trans_xy = new GLUI_Translation(glui, "Objects XY",
                GLUI_TRANSLATION_XY, obj_pos);
  trans_xy->set_speed(5);
  GLUI_Translation *trans_z =
    new GLUI_Translation(glui, "Translate Z", GLUI_TRANSLATION_Z, &obj_pos[2]);
  trans_z->set_speed(5);
  glui->add_button("Reset Custom", 0, reinterpret_cast<GLUI_Update_CB>(resetViewPoint));
  GLUI_Translation *topViewTransXY = new GLUI_Translation(glui, "Top View XY",
                GLUI_TRANSLATION_XY, topViewPos);
  topViewTransXY->set_speed(5);
  GLUI_Translation *topViewZoom =
    new GLUI_Translation(glui, "Top View Zoom", GLUI_TRANSLATION_Z, &topViewPos[2]);
  trans_z->set_speed(5);
  glui->add_button("Reset Top", 0, reinterpret_cast<GLUI_Update_CB>(resetTopViewPoint));
  glui->add_button("Print Rotation Angles", 0, reinterpret_cast<GLUI_Update_CB>(printRotationAngles));

  // output = new GLUI_TextBox(glui, true);
  // output->set_h(300);
  // output->set_w(200);
  // Quit button
  glui->add_button("Quit", 0, static_cast<GLUI_Update_CB>(exit));

  glui->set_main_gfx_window(customViewWindowID);

  // PERFORM THE INITIAL RUN OF THE ALGORITHM
  //==========================================
  run();   // make it do something interesting from the start!!!

  // SHOULD WE STOP or GO INTERACTIVE?
  //==========================================
  if (interactive > 0) {  // non-interactive
    logNonInteractiveRun(noPath);
    return 0;
  } else {
    glutMainLoop();
  }

  return 0;
}
