#pragma once

// Parser.h
//      We read a text file following the conventions of "inputs/format.txt".
//
//      We must be sure that each triangular face follows the CCW convention
//              when seen from the "outside"

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <QDir>

#include "geometry/Corner.h"
#include "geometry/Edge.h"
#include "geometry/Wall.h"
#include "subdivision/ConfBox3d.h"

extern FILE* stderr;

extern string cfgName;
extern char cfgNameList[200][200];
extern int numCfg;
extern string workingDir;

extern Point3d start;
extern Point3d goal;
extern Point3d startRot;
extern Point3d goalRot;
extern double epsilon;
extern double boxWidth;
extern double boxHeight;
extern double R;
extern int windowPosX;
extern int windowPosY;
extern int searchType;
extern int interactive;
extern int seed;
extern Point3d eye;
extern Point3d at;
extern Vector up;
extern Vector eyeVector;
extern int transparency;
extern double distanceZ;
// extern float view_rotate_angles[3];
extern int animationSpeed;

extern string fileName;  // Input file name
extern string defaultFilePath;
extern string inputDir;  // Path for input files

extern double scale;   // scaling of input environment
extern double deltaX;  // x-translation of input environment
extern double deltaY;  // y-translation of input environment
extern double deltaZ;  // z-translation of input environment

using namespace std;

extern int fileProcessor(string inputfile);

void parseVertices(vector<Vector>& pts, ifstream& ifs);

void parseConfigFile(ConfBox3d* b);
void parseConfigFile(ConfBox3d* b, string workingDir);

void parseVertices(vector<Point3d>& pts, ifstream& ifs) {
  int nPt = 0;
  ifs >> nPt;
  // cout << "nPt=" << nPt << endl;

  for (int i = 0; i < nPt; ++i) {
    double x, y, z;
    ifs >> x >> y >> z;
    pts.push_back(Point3d(x, y, z));
  }
}

void parseCfgList() {
  char sptr[200];
  string cfgDir = workingDir + "/" + inputDir + "/cfg";
  QDir tmpDir(cfgDir.c_str());
  QStringList fileList = tmpDir.entryList();
  while (!fileList.empty()) {
    QString fileName_l = fileList.front();
    strcpy(sptr, fileName_l.toStdString().c_str());
    int len = strlen(sptr);
    if (len > 4 && sptr[len - 1] == 'g' && sptr[len - 2] == 'f' &&
        sptr[len - 3] == 'c' && sptr[len - 4] == '.') {
      strcpy(cfgNameList[numCfg], sptr);
      ++numCfg;
    }
    fileList.pop_front();
  }
}

void parseCfgFile() {
  char cfg_path[200], tmp[200];
  sprintf(cfg_path, "%s/%s", inputDir.c_str(), cfgName.c_str());

  FILE* fptr = fopen(cfg_path, "r");
  if (fptr == nullptr) return;

  while (fgets(tmp, 200, fptr) != nullptr) {
    char* sptr = strtok(tmp, "=: \t");

    // comments
    if (strcmp(sptr, "#") == 0) {
      continue;
    }

    // start configuration
    if (strcmp(sptr, "start_x") == 0) {
      sptr = strtok(nullptr, "=: \t");
      start.setX(atof(sptr));
    }
    if (strcmp(sptr, "start_y") == 0) {
      sptr = strtok(nullptr, "=: \t");
      start.setY(atof(sptr));
    }
    if (strcmp(sptr, "start_z") == 0) {
      sptr = strtok(nullptr, "=: ");
      start.setZ(atof(sptr));
    }
    if (strcmp(sptr, "start_rotation_x") == 0) {
      sptr = strtok(nullptr, "=: \t");
      startRot.setX(atof(sptr));
    }
    if (strcmp(sptr, "start_rotation_y") == 0) {
      sptr = strtok(nullptr, "=: \t");
      startRot.setY(atof(sptr));
    }
    if (strcmp(sptr, "start_rotation_z") == 0) {
      sptr = strtok(nullptr, "=: ");
      startRot.setZ(atof(sptr));
    }

    // goal configuration
    if (strcmp(sptr, "goal_x") == 0) {
      sptr = strtok(nullptr, "=: \t");
      goal.setX(atof(sptr));
    }
    if (strcmp(sptr, "goal_y") == 0) {
      sptr = strtok(nullptr, "=: \t");
      goal.setY(atof(sptr));
    }
    if (strcmp(sptr, "goal_z") == 0) {
      sptr = strtok(nullptr, "=: \t");
      goal.setZ(atof(sptr));
    }
    if (strcmp(sptr, "goal_rotation_x") == 0) {
      sptr = strtok(nullptr, "=: \t");
      goalRot.setX(atof(sptr));
    }
    if (strcmp(sptr, "goal_rotation_y") == 0) {
      sptr = strtok(nullptr, "=: \t");
      goalRot.setY(atof(sptr));
    }
    if (strcmp(sptr, "goal_rotation_z") == 0) {
      sptr = strtok(nullptr, "=: \t");
      goalRot.setZ(atof(sptr));
    }

    if (strcmp(sptr, "epsilon") == 0) {
      sptr = strtok(nullptr, "=: \t");
      epsilon = atof(sptr);
    }

    if (strcmp(sptr, "ring_radius") == 0) {
      sptr = strtok(nullptr, "=: \t");
      R = atof(sptr);
    }

    if (strcmp(sptr, "input_dir") == 0) {
      sptr = strtok(nullptr, "=: #\t");
      inputDir = sptr;
    }
    if (strcmp(sptr, "filename") == 0) {
      sptr = strtok(nullptr, "=: #\t");
      fileName = sptr;
    }

    // box width and height
    if (strcmp(sptr, "box_width") == 0) {
      sptr = strtok(nullptr, "=: \t");
      boxWidth = atof(sptr);
    }

    // windows position
    if (strcmp(sptr, "window_pos_x") == 0) {
      sptr = strtok(nullptr, "=: \t");
      windowPosX = atoi(sptr);
    }
    if (strcmp(sptr, "window_pos_y") == 0) {
      sptr = strtok(nullptr, "=: \t");
      windowPosY = atoi(sptr);
    }

    if (strcmp(sptr, "search_type") == 0) {
      sptr = strtok(nullptr, "=: \t");
      searchType = atoi(sptr);
    }

    if (strcmp(sptr, "random_seed") == 0) {
      sptr = strtok(nullptr, "=: \t");
      seed = atoi(sptr);
    }

    if (strcmp(sptr, "xtrans") == 0) {
      sptr = strtok(nullptr, "=: \t");
      deltaX = atof(sptr);
    }

    if (strcmp(sptr, "ytrans") == 0) {
      sptr = strtok(nullptr, "=: \t");
      deltaY = atof(sptr);
    }

    if (strcmp(sptr, "scale") == 0) {
      sptr = strtok(nullptr, "=: \t");
      scale = atof(sptr);
    }

    if (strcmp(sptr, "animation_speed") == 0) {
      sptr = strtok(nullptr, "=: \t");
      animationSpeed = atoi(sptr);
    }

    if (strcmp(sptr, "transparency") == 0) {
      sptr = strtok(nullptr, "=: \t");
      transparency = atof(sptr);
    }

    if (strcmp(sptr, "verbose") == 0) {
      sptr = strtok(nullptr, "=: \t");
      verbose = atoi(sptr);
    }

	//Yong/Chee Jul'19:
	if (strcmp(sptr, "eye_x") == 0) {
		sptr = strtok(nullptr, "=: \t");
		eye.setX(atof(sptr));
	}
	if (strcmp(sptr, "eye_y") == 0) {
		sptr = strtok(nullptr, "=: \t");
		eye.setY(atof(sptr));
	}
	if (strcmp(sptr, "eye_z") == 0) {
		sptr = strtok(nullptr, "=: \t");
		eye.setZ(atof(sptr));
	}
	if (strcmp(sptr, "lookAt_x") == 0) {
		sptr = strtok(nullptr, "=: \t");
		at.setX(atof(sptr));
	}
	if (strcmp(sptr, "lookAt_y") == 0) {
		sptr = strtok(nullptr, "=: \t");
		at.setY(atof(sptr));
	}
	if (strcmp(sptr, "lookAt_z") == 0) {
		sptr = strtok(nullptr, "=: \t");
		at.setZ(atof(sptr));
	}
  }//while
  fclose(fptr);
}

void parseConfigFile(ConfBox3d* b) {
  stringstream ss;
  ss << inputDir << "/env/" << fileName;  // create full file name
  string s = ss.str();
  FILE* fptr = fopen(s.c_str(), "r");
  if (fptr == NULL) return;

  vector<Corner*> pts;
  vector<Wall*> polys;
  int nPts = 0;
  int nPolys = 0;
  char tmp[200];
  while (fgets(tmp, 200, fptr) != NULL) {
    char* sptr = strtok(tmp, "=: \n\t");

    if (sptr == NULL) {
      continue;
    }
    // comments
    if (strcmp(sptr, "#") == 0) {
      continue;
    }
    if (strcmp(sptr, "End") == 0) {
      break;
    }

    if (strcmp(sptr, "Points") == 0) {
      while (fgets(tmp, 200, fptr) != NULL) {
        sptr = strtok(tmp, "=: \n\t");
        if (sptr == NULL) {
          break;
        }
        if (strcmp(sptr, "Poly") == 0 || strcmp(sptr, "Polyhedron") == 0) {
          break;
        }
        if (strcmp(sptr, "End") == 0) {
          break;
        }

        if (sptr[0] > '9' || sptr[0] < '0') {
          char buffName[200];
          strcpy(buffName, sptr);

          sptr = strtok(NULL, "=: \n\t");
          double x = atof(sptr);
          sptr = strtok(NULL, "=: \n\t");
          double y = atof(sptr);
          sptr = strtok(NULL, "=: \n\t");
          double z = atof(sptr);
          Corner* tmpCorner =
              new Corner(Point3d(x * scale, y * scale, z * scale));

          char* tmpPtr = strtok(buffName, " \"");
          string tmpName = tmpPtr;
          tmpCorner->name = tmpName;
          tmpCorner->id = nPts;
          pts.push_back(tmpCorner);
          ++nPts;
        }
        // no name
        else {
          double x = atof(sptr);
          sptr = strtok(NULL, "=: \n\t");
          double y = atof(sptr);
          sptr = strtok(NULL, "=: \n\t");
          double z = atof(sptr);
          Corner* tmpCorner =
              new Corner(Point3d(x * scale, y * scale, z * scale));

          ss << nPts;
          string tmpName = ss.str();
          tmpCorner->name = tmpName;
          tmpCorner->id = nPts;
          pts.push_back(tmpCorner);
          ++nPts;
        }
      }
    } else if (strcmp(sptr, "Poly") == 0 || strcmp(sptr, "Polyhedron") == 0) {
      // with name
      sptr = strtok(NULL, "=: \n\t");
      string tmpName;

      double sign = 1;
      double tx = 0, ty = 0, tz = 0;  // translation +, -
      double rx = 0, ry = 0, rz = 0;  // rotation @
      double sx = 1, sy = 1, sz = 1;  // scale *

      if (sptr[0] > '9' || sptr[0] < '0') {
        char buffName[200];
        strcpy(buffName, sptr);

        sptr = strtok(NULL, "=: \n\t");
        while (sptr != NULL) {
          if (sptr[0] == '+') {
            sptr = strtok(NULL, "=: \n\t");
            tx = atof(sptr);
            sptr = strtok(NULL, "=: \n\t");
            ty = atof(sptr);
            sptr = strtok(NULL, "=: \n\t");
            tz = atof(sptr);
          } else if (sptr[0] == '-') {
            sign = -1;
            sptr = strtok(NULL, "=: \n\t");
            tx = atof(sptr);
            sptr = strtok(NULL, "=: \n\t");
            ty = atof(sptr);
            sptr = strtok(NULL, "=: \n\t");
            tz = atof(sptr);
          } else if (sptr[0] == '*') {
            sptr = strtok(NULL, "=: \n\t");
            sx = atof(sptr);
            sptr = strtok(NULL, "=: \n\t");
            sy = atof(sptr);
            sptr = strtok(NULL, "=: \n\t");
            sz = atof(sptr);
          } else if (sptr[0] == '@') {
            sptr = strtok(NULL, "=: \n\t");
            rx = atof(sptr);
            sptr = strtok(NULL, "=: \n\t");
            ry = atof(sptr);
            sptr = strtok(NULL, "=: \n\t");
            rz = atof(sptr);
          }

          sptr = strtok(NULL, "=: \n\t");
        }

        char* tmpPtr = strtok(buffName, "\"");
        tmpName = tmpPtr;
      } else {
        ss << nPolys;
        tmpName = ss.str();
      }
      while (fgets(tmp, 200, fptr) != NULL) {
        sptr = strtok(tmp, "=: \n\t");
        if (sptr == NULL) {
          break;
        }
        if (strcmp(sptr, "PutPoly") == 0 ||
            strcmp(sptr, "PutPolyhedron") == 0) {
          break;
        }
        if (strcmp(sptr, "End") == 0) {
          break;
        }

        // triangle
        if (sptr[0] == '0') {
          Corner* aa;
          sptr = strtok(NULL, " \n\t");
          for (int i = 0; (int)i < pts.size(); ++i) {
            if (strcmp(pts[i]->name.c_str(), sptr) == 0) {
              aa = new Corner(Point3d(pts[i]->p.X() * sx + sign * tx,
                                      pts[i]->p.Y() * sy + sign * ty,
                                      pts[i]->p.Z() * sz + sign * tz));
              aa->name = pts[i]->name;
              break;
            }
          }
          b->addCorner(aa);
          Corner* bb;
          sptr = strtok(NULL, " \n\t");
          for (int i = 0; (int)i < pts.size(); ++i) {
            if (strcmp(pts[i]->name.c_str(), sptr) == 0) {
              bb = new Corner(Point3d(pts[i]->p.X() * sx + sign * tx,
                                      pts[i]->p.Y() * sy + sign * ty,
                                      pts[i]->p.Z() * sz + sign * tz));
              bb->name = pts[i]->name;
              break;
            }
          }
          b->addCorner(bb);
          Corner* cc;
          sptr = strtok(NULL, " \n\t");
          for (int i = 0; (int)i < pts.size(); ++i) {
            if (strcmp(pts[i]->name.c_str(), sptr) == 0) {
              cc = new Corner(Point3d(pts[i]->p.X() * sx + sign * tx,
                                      pts[i]->p.Y() * sy + sign * ty,
                                      pts[i]->p.Z() * sz + sign * tz));
              cc->name = pts[i]->name;
              break;
            }
          }
          b->addCorner(cc);

          Edge* e1 = new Edge(aa, bb);
          Edge* e2 = new Edge(bb, cc);
          Edge* e3 = new Edge(cc, aa);
          b->addEdge(e1);
          b->addEdge(e2);
          b->addEdge(e3);

          Wall* w = new Wall(aa, bb, cc);
          b->addWall(w);

          w->name = tmpName;
          w->id = nPolys;
          polys.push_back(w);
        }
        // quad
        else if (sptr[0] == '1') {
        }
      }
      ++nPolys;
    } else if (strcmp(sptr, "PutPoly") == 0 ||
               strcmp(sptr, "PutPolyhedron") == 0) {
      sptr = strtok(NULL, "=: \n\t");
      string tmpName = sptr;
      double sign = 1;
      double tx = 0, ty = 0, tz = 0;  // translation +, -
      double rx = 0, ry = 0, rz = 0;  // rotation @
      double sx = 1, sy = 1, sz = 1;  // scale *

      sptr = strtok(NULL, "=: \n\t");
      while (sptr != NULL) {
        if (sptr[0] == '+') {
          sptr = strtok(NULL, "=: \n\t");
          tx = atof(sptr);
          sptr = strtok(NULL, "=: \n\t");
          ty = atof(sptr);
          sptr = strtok(NULL, "=: \n\t");
          tz = atof(sptr);
        } else if (sptr[0] == '-') {
          sign = -1;
          sptr = strtok(NULL, "=: \n\t");
          tx = atof(sptr);
          sptr = strtok(NULL, "=: \n\t");
          ty = atof(sptr);
          sptr = strtok(NULL, "=: \n\t");
          tz = atof(sptr);
        } else if (sptr[0] == '*') {
          sptr = strtok(NULL, "=: \n\t");
          sx = atof(sptr);
          sptr = strtok(NULL, "=: \n\t");
          sy = atof(sptr);
          sptr = strtok(NULL, "=: \n\t");
          sz = atof(sptr);
        } else if (sptr[0] == '@') {
          sptr = strtok(NULL, "=: \n\t");
          rx = atof(sptr);
          sptr = strtok(NULL, "=: \n\t");
          ry = atof(sptr);
          sptr = strtok(NULL, "=: \n\t");
          rz = atof(sptr);
        }

        sptr = strtok(NULL, "=: \n\t");
      }

      for (int i = 0; i < (int)polys.size(); ++i) {
        if (strcmp(polys[i]->name.c_str(), tmpName.c_str()) == 0) {
          Corner* aa = new Corner(Point3d(polys[i]->a->p.X() * sx + sign * tx,
                                          polys[i]->a->p.Y() * sy + sign * ty,
                                          polys[i]->a->p.Z() * sz + sign * tz));
          aa->name = polys[i]->a->name;
          b->addCorner(aa);
          Corner* bb = new Corner(Point3d(polys[i]->b->p.X() * sx + sign * tx,
                                          polys[i]->b->p.Y() * sy + sign * ty,
                                          polys[i]->b->p.Z() * sz + sign * tz));
          bb->name = polys[i]->b->name;
          b->addCorner(bb);
          Corner* cc = new Corner(Point3d(polys[i]->c->p.X() * sx + sign * tx,
                                          polys[i]->c->p.Y() * sy + sign * ty,
                                          polys[i]->c->p.Z() * sz + sign * tz));
          cc->name = polys[i]->c->name;
          b->addCorner(cc);

          Edge* e1 = new Edge(aa, bb);
          Edge* e2 = new Edge(bb, cc);
          Edge* e3 = new Edge(cc, aa);
          b->addEdge(e1);
          b->addEdge(e2);
          b->addEdge(e3);

          Wall* w = new Wall(aa, bb, cc);
          w->name = polys[i]->name;
          b->addWall(w);
        }
      }
    }
  }

  fprintf(g_fptr, "corner feature\n");
  for (list<Corner*>::iterator iterC = b->Corners.begin();
       iterC != b->Corners.end(); ++iterC) {
    Corner* c = (*iterC);
    fprintf(g_fptr, "corner (%f %f %f)\n", c->p.X(), c->p.Y(), c->p.Z());
  }
  fprintf(g_fptr, "\n\n");

  for (list<Edge*>::iterator iterE = b->Edges.begin(); iterE != b->Edges.end();
       ++iterE) {
    for (list<Wall*>::iterator iterW = b->Walls.begin();
         iterW != b->Walls.end(); ++iterW) {
      if ((*iterE)->same((*iterW)->a, (*iterW)->b) ||
          (*iterE)->same((*iterW)->b, (*iterW)->c) ||
          (*iterE)->same((*iterW)->c, (*iterW)->a)) {
        (*iterE)->rightWall = new Wall((*iterW)->a, (*iterW)->b, (*iterW)->c);
      }
      if ((*iterE)->same((*iterW)->b, (*iterW)->a) ||
          (*iterE)->same((*iterW)->c, (*iterW)->b) ||
          (*iterE)->same((*iterW)->a, (*iterW)->c)) {
        (*iterE)->oppositeWall =
            new Wall((*iterW)->a, (*iterW)->b, (*iterW)->c);
      }
    }
  }

  fprintf(g_fptr, "edge feature\n");
  for (list<Edge*>::iterator iterE = b->Edges.begin(); iterE != b->Edges.end();
       ++iterE) {
    (*iterE)->convexEdge = (*iterE)->isConvex();
    Edge* e = *iterE;
    fprintf(g_fptr, "edge src(%f %f %f) dst(%f %f %f) convex %d\n",
            e->src->p.X(), e->src->p.Y(), e->src->p.Z(), e->dst->p.X(),
            e->dst->p.Y(), e->dst->p.Z(), e->convexEdge);

    Point3d A = e->rightWall->a->p;
    Point3d B = e->rightWall->b->p;
    Point3d C = e->rightWall->c->p;
    fprintf(g_fptr, "rightWall (%f %f %f) (%f %f %f) (%f %f %f)\n", A.X(),
            A.Y(), A.Z(), B.X(), B.Y(), B.Z(), C.X(), C.Y(), C.Z());

    A = e->oppositeWall->a->p;
    B = e->oppositeWall->b->p;
    C = e->oppositeWall->c->p;
    fprintf(g_fptr, "oppositeWall (%f %f %f) (%f %f %f) (%f %f %f)\n", A.X(),
            A.Y(), A.Z(), B.X(), B.Y(), B.Z(), C.X(), C.Y(), C.Z());
  }
  fprintf(g_fptr, "\n\n");

  fprintf(g_fptr, "wall feature\n");
  for (list<Wall*>::iterator iterW = b->Walls.begin(); iterW != b->Walls.end();
       ++iterW) {
    Wall* w = (*iterW);

    Point3d A = w->a->p;
    Point3d B = w->b->p;
    Point3d C = w->c->p;
    fprintf(g_fptr, "wall (%f %f %f) (%f %f %f) (%f %f %f)\n", A.X(), A.Y(),
            A.Z(), B.X(), B.Y(), B.Z(), C.X(), C.Y(), C.Z());

    Vector n = w->tri.normal();
    fprintf(g_fptr, "wall normal %f %f %f\n", n.X(), n.Y(), n.Z());
  }
  fprintf(g_fptr, "\n\n");

  //  FILE* f = fopen("env_posts2.raw", "w");
  //  for (const auto& wall : b->Walls) {
  //    fprintf(f, "1 1 1 %f %f %f %f %f %f %f %f %f\n", wall->a->p.X(),
  //            wall->a->p.Y(), wall->a->p.Z(), wall->c->p.X(), wall->c->p.Y(),
  //            wall->c->p.Z(), wall->b->p.X(), wall->b->p.Y(), wall->b->p.Z());
  //  }
  //  fclose(f);
}

void parseConfigFile(ConfBox3d* b, string workingDir) {
  std::stringstream ss;
  ss << inputDir << "/" << fileName;  // create full file name
  std::string s = ss.str();
  if (FILE* file = fopen(s.c_str(), "r")) {
    fclose(file);
  } else {
    s = defaultFilePath;
  }
  // cout << "input file name = " << s << endl;

  // (Tom)
  char input_file[200];
  sprintf(input_file, "%s/%s", workingDir.c_str(), s.c_str());
  s = input_file;
  fileProcessor(s);  // this will clean the input and put in output_tmp.txt

  // (Tom)
  char sys_call[200];
  sprintf(sys_call, "python %s", workingDir.c_str());
  strcat(sys_call, "/input_interpreter.py");
  system(sys_call);

  ifstream ifs("output-tmp-py.txt");
  if (!ifs) {
    cerr << "cannot open input file" << endl;
    exit(1);
  }

  vector<Point3d> pts;
  parseVertices(pts, ifs);

  int nPolyhedra = 0;
  while (true) {
    int numFaces;
    ifs >> numFaces;
    // cout << numFaces << " faces" << endl;

    if (numFaces == 0) {
      break;
    }
    nPolyhedra++;

    for (int i = 0; i < numFaces; ++i) {
      vector<Corner*> ptVec;

      int objType = 0;  // used to check whether the object is a triangle or a
                        // sphere
                        // objType=0 means a triangle
                        // objType=7 means a sphere
                        // assume now no edge or corner features

      ifs >> objType;  // move this after the for-loop

      if (objType == 0) {
        // The following loop is removed:
        for (int j = 0; j < 3; ++j) {
          unsigned long pt;
          ifs >> pt;
          pt--;  // to get indexing from 0
          ptVec.push_back(new Corner(Point3d(
              pts[pt].X() * scale, pts[pt].Y() * scale, pts[pt].Z() * scale)));
          b->addCorner(ptVec.back());
        }

        Edge* e1 = new Edge(ptVec[0], ptVec[1]);
        Edge* e2 = new Edge(ptVec[1], ptVec[2]);
        Edge* e3 = new Edge(ptVec[2], ptVec[0]);
        b->addEdge(e1);
        b->addEdge(e2);
        b->addEdge(e3);

        Wall* w = new Wall(ptVec[0], ptVec[1], ptVec[2]);
        b->addWall(w);

        for (list<Edge*>::iterator iterE = b->Edges.begin();
             iterE != b->Edges.end(); ++iterE) {
          for (list<Wall*>::iterator iterW = b->Walls.begin();
               iterW != b->Walls.end(); ++iterW) {
            if ((*iterE)->same((*iterW)->a, (*iterW)->b) ||
                (*iterE)->same((*iterW)->b, (*iterW)->c) ||
                (*iterE)->same((*iterW)->c, (*iterW)->a)) {
              (*iterE)->rightWall =
                  new Wall((*iterW)->a, (*iterW)->b, (*iterW)->c);
            }
            if ((*iterE)->same((*iterW)->b, (*iterW)->a) ||
                (*iterE)->same((*iterW)->c, (*iterW)->b) ||
                (*iterE)->same((*iterW)->a, (*iterW)->c)) {
              (*iterE)->oppositeWall =
                  new Wall((*iterW)->a, (*iterW)->b, (*iterW)->c);
            }
          }
        }

      } else if (objType == 7) {
        unsigned long pt;
        ifs >> pt;
        pt--;
        unsigned long radius;
        ifs >> radius;
        b->addSphere(
            new Sphere(Point3d(pts[pt].X() * scale, pts[pt].Y() * scale,
                               pts[pt].Z() * scale),
                       radius));
      }

    }  // for i
  }    // while

  // cout<< "nPolyhedra=" << nPolyhedra << endl;
  ifs.close();
}
