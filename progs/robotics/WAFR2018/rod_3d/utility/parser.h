/*
 * parser.h
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <QDir>

#include "geometry/corner.h"
#include "geometry/edge.h"
#include "geometry/wall.h"
#include "subdivision/configuration_box3d.h"

extern bool verbose;

// File
extern std::string cfg_name;
extern std::vector<std::string> cfg_name_list;
extern int num_cfg;
extern std::string working_dir;
extern std::string input_dir;
extern std::string filename;
extern std::string default_file_path;

// paramters
extern Point3d start;
extern Point3d goal;
extern Point3d start_rotation;
extern Point3d goal_rotation;
extern double epsilon;
extern double box_width;
extern double rod_length;
extern double thickness;
extern std::vector<int> window_pos;
extern int search_type;
extern int random_seed;

// map parameter
extern double map_scale;  // scaling of input environment
extern std::vector<double> map_translation;

// camera setting
extern Point3d eye;
extern Point3d at;
extern Vector up;
extern Vector eye_vector;

// render setting
extern int transparency;
extern double distance_z;
extern int animation_speed;

void parseVertices(std::vector<Vector>& points, std::ifstream& ifs);
void parseConfigFile(ConfBox3d* box);
void parseConfigFile(ConfBox3d* box, std::string working_dir);

void parseVertices(std::vector<Point3d>& points, std::ifstream& ifs) {
  int nPt = 0;
  ifs >> nPt;
  // cout << "nPt=" << nPt << endl;

  for (int i = 0; i < nPt; ++i) {
    double x, y, z;
    ifs >> x >> y >> z;
    points.push_back(Point3d(x, y, z));
  }
}

void parseCfgList() {
  char sptr[200];
  std::string cfg_dir = working_dir + input_dir;
  std::cerr << cfg_dir << std::endl;
  QDir tmp_dir(cfg_dir.c_str());
  QStringList filelist = tmp_dir.entryList();
  while (!filelist.empty()) {
    QString filename_l = filelist.front();
    strcpy(sptr, filename_l.toStdString().c_str());
    int len = strlen(sptr);
    if (len > 4 && sptr[len - 1] == 'g' && sptr[len - 2] == 'f' &&
        sptr[len - 3] == 'c' && sptr[len - 4] == '.') {
      cfg_name_list.push_back(sptr);
      ++num_cfg;
    }
    filelist.pop_front();
  }
}

void parseCfgFile() {
  char cfg_path[200], tmp[200];
  sprintf(cfg_path, "%s/%s", input_dir.c_str(), cfg_name.c_str());

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
      start_rotation.setX(atof(sptr));
    }
    if (strcmp(sptr, "start_rotation_y") == 0) {
      sptr = strtok(nullptr, "=: \t");
      start_rotation.setY(atof(sptr));
    }
    if (strcmp(sptr, "start_rotation_z") == 0) {
      sptr = strtok(nullptr, "=: ");
      start_rotation.setZ(atof(sptr));
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
      goal_rotation.setX(atof(sptr));
    }
    if (strcmp(sptr, "goal_rotation_y") == 0) {
      sptr = strtok(nullptr, "=: \t");
      goal_rotation.setY(atof(sptr));
    }
    if (strcmp(sptr, "goal_rotation_z") == 0) {
      sptr = strtok(nullptr, "=: \t");
      goal_rotation.setZ(atof(sptr));
    }

    if (strcmp(sptr, "epsilon") == 0) {
      sptr = strtok(nullptr, "=: \t");
      epsilon = atof(sptr);
    }

    if (strcmp(sptr, "rod_length") == 0) {
      sptr = strtok(nullptr, "=: \t");
      rod_length = atof(sptr);
    }
    if (strcmp(sptr, "thickness") == 0) {
      sptr = strtok(nullptr, "=: \t");
      thickness = atof(sptr);
    }

    if (strcmp(sptr, "input_dir") == 0) {
      sptr = strtok(nullptr, "=: #\t");
      input_dir = sptr;
    }
    if (strcmp(sptr, "filename") == 0) {
      sptr = strtok(nullptr, "=: #\t");
      filename = sptr;
    }

    // box width and height
    if (strcmp(sptr, "box_width") == 0) {
      sptr = strtok(nullptr, "=: \t");
      box_width = atof(sptr);
    }

    // windows position
    if (strcmp(sptr, "window_pos_x") == 0) {
      sptr = strtok(nullptr, "=: \t");
      window_pos[0] = atoi(sptr);
    }
    if (strcmp(sptr, "window_pos_y") == 0) {
      sptr = strtok(nullptr, "=: \t");
      window_pos[1] = atoi(sptr);
    }

    if (strcmp(sptr, "search_type") == 0) {
      sptr = strtok(nullptr, "=: \t");
      search_type = atoi(sptr);
    }

    if (strcmp(sptr, "random_seed") == 0) {
      sptr = strtok(nullptr, "=: \t");
      random_seed = atoi(sptr);
    }

    if (strcmp(sptr, "xtrans") == 0) {
      sptr = strtok(nullptr, "=: \t");
      map_translation[0] = atof(sptr);
    }

    if (strcmp(sptr, "ytrans") == 0) {
      sptr = strtok(nullptr, "=: \t");
      map_translation[1] = atof(sptr);
    }

    if (strcmp(sptr, "scale") == 0) {
      sptr = strtok(nullptr, "=: \t");
      map_scale = atof(sptr);
    }

    if (strcmp(sptr, "animation_speed") == 0) {
      sptr = strtok(nullptr, "=: \t");
      animation_speed = atoi(sptr);
    }

    if (strcmp(sptr, "transparency") == 0) {
      sptr = strtok(nullptr, "=: \t");
      transparency = atof(sptr);
    }

    if (strcmp(sptr, "verbose") == 0) {
      sptr = strtok(nullptr, "=: \t");
      verbose = atoi(sptr);
    }
  }
  fclose(fptr);
}

void parseConfigFile(ConfBox3d* b) {
  std::stringstream ss;
  ss << input_dir << "/" << filename;  // create full file name
  std::string s = ss.str();
  FILE* fptr = fopen(s.c_str(), "r");
  if (fptr == nullptr) return;

  std::vector<Corner*> points;
  std::vector<Wall*> polys;
  int num_points = 0;
  int num_polys = 0;
  char tmp[200];
  while (fgets(tmp, 200, fptr) != nullptr) {
    char* sptr = strtok(tmp, "=: \n\t");

    if (sptr == nullptr) {
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
      while (fgets(tmp, 200, fptr) != nullptr) {
        sptr = strtok(tmp, "=: \n\t");
        if (sptr == nullptr) {
          break;
        }
        if (strcmp(sptr, "Poly") == 0 || strcmp(sptr, "Polyhedron") == 0) {
          break;
        }
        if (strcmp(sptr, "End") == 0) {
          break;
        }

        if (sptr[0] > '9' || sptr[0] < '0') {
          char buff_name[200];
          strcpy(buff_name, sptr);

          sptr = strtok(nullptr, "=: \n\t");
          double x = atof(sptr);
          sptr = strtok(nullptr, "=: \n\t");
          double y = atof(sptr);
          sptr = strtok(nullptr, "=: \n\t");
          double z = atof(sptr);
          Corner* tmp_corner =
              new Corner(Point3d(x * map_scale, y * map_scale, z * map_scale));

          char* tmp_ptr = strtok(buff_name, " \"");
          tmp_corner->setName(tmp_ptr);
          tmp_corner->setId(num_points);
          points.push_back(tmp_corner);
          ++num_points;
        }
        // no name
        else {
          double x = atof(sptr);
          sptr = strtok(nullptr, "=: \n\t");
          double y = atof(sptr);
          sptr = strtok(nullptr, "=: \n\t");
          double z = atof(sptr);
          Corner* tmp_corner =
              new Corner(Point3d(x * map_scale, y * map_scale, z * map_scale));

          ss << num_points;
          tmp_corner->setName(ss.str());
          tmp_corner->setId(num_points);
          points.push_back(tmp_corner);
          ++num_points;
        }
      }
    } else if (strcmp(sptr, "Poly") == 0 || strcmp(sptr, "Polyhedron") == 0) {
      // with name
      sptr = strtok(nullptr, "=: \n\t");
      std::string tmp_name;

      double sign = 1;
      double tx = 0, ty = 0, tz = 0;  // translation +, -
      double rx = 0, ry = 0, rz = 0;  // rotation @
      double sx = 1, sy = 1, sz = 1;  // scale *

      if (sptr[0] > '9' || sptr[0] < '0') {
        char buff_name[200];
        strcpy(buff_name, sptr);

        sptr = strtok(nullptr, "=: \n\t");
        while (sptr != nullptr) {
          if (sptr[0] == '+') {
            sptr = strtok(nullptr, "=: \n\t");
            tx = atof(sptr);
            sptr = strtok(nullptr, "=: \n\t");
            ty = atof(sptr);
            sptr = strtok(nullptr, "=: \n\t");
            tz = atof(sptr);
          } else if (sptr[0] == '-') {
            sign = -1;
            sptr = strtok(nullptr, "=: \n\t");
            tx = atof(sptr);
            sptr = strtok(nullptr, "=: \n\t");
            ty = atof(sptr);
            sptr = strtok(nullptr, "=: \n\t");
            tz = atof(sptr);
          } else if (sptr[0] == '*') {
            sptr = strtok(nullptr, "=: \n\t");
            sx = atof(sptr);
            sptr = strtok(nullptr, "=: \n\t");
            sy = atof(sptr);
            sptr = strtok(nullptr, "=: \n\t");
            sz = atof(sptr);
          } else if (sptr[0] == '@') {
            sptr = strtok(nullptr, "=: \n\t");
            rx = atof(sptr);
            sptr = strtok(nullptr, "=: \n\t");
            ry = atof(sptr);
            sptr = strtok(nullptr, "=: \n\t");
            rz = atof(sptr);
          }

          sptr = strtok(nullptr, "=: \n\t");
        }

        char* tmp_ptr = strtok(buff_name, "\"");
        tmp_name = tmp_ptr;
      } else {
        ss << num_polys;
        tmp_name = ss.str();
      }
      while (fgets(tmp, 200, fptr) != nullptr) {
        sptr = strtok(tmp, "=: \n\t");
        if (sptr == nullptr) {
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
          sptr = strtok(nullptr, " \n\t");
          for (const auto& point : points) {
            if (strcmp(point->name().c_str(), sptr) == 0) {
              aa = new Corner(Point3d(point->point().X() * sx + sign * tx,
                                      point->point().Y() * sy + sign * ty,
                                      point->point().Z() * sz + sign * tz));
              aa->setName(point->name());
              break;
            }
          }
          b->addCorner(aa);
          Corner* bb;
          sptr = strtok(nullptr, " \n\t");
          for (const auto& point : points) {
            if (strcmp(point->name().c_str(), sptr) == 0) {
              bb = new Corner(Point3d(point->point().X() * sx + sign * tx,
                                      point->point().Y() * sy + sign * ty,
                                      point->point().Z() * sz + sign * tz));
              bb->setName(point->name());
              break;
            }
          }
          b->addCorner(bb);
          Corner* cc;
          sptr = strtok(nullptr, " \n\t");
          for (const auto& point : points) {
            if (strcmp(point->name().c_str(), sptr) == 0) {
              cc = new Corner(Point3d(point->point().X() * sx + sign * tx,
                                      point->point().Y() * sy + sign * ty,
                                      point->point().Z() * sz + sign * tz));
              cc->setName(point->name());
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

          w->setName(tmp_name);
          w->setId(num_polys);
          polys.push_back(w);
        }
        // quad
        else if (sptr[0] == '1') {
        }
      }
      ++num_polys;
    } else if (strcmp(sptr, "PutPoly") == 0 ||
               strcmp(sptr, "PutPolyhedron") == 0) {
      sptr = strtok(nullptr, "=: \n\t");
      std::string tmp_name = sptr;
      double sign = 1;
      double tx = 0, ty = 0, tz = 0;  // translation +, -
      double rx = 0, ry = 0, rz = 0;  // rotation @
      double sx = 1, sy = 1, sz = 1;  // scale *

      sptr = strtok(nullptr, "=: \n\t");
      while (sptr != nullptr) {
        if (sptr[0] == '+') {
          sptr = strtok(nullptr, "=: \n\t");
          tx = atof(sptr);
          sptr = strtok(nullptr, "=: \n\t");
          ty = atof(sptr);
          sptr = strtok(nullptr, "=: \n\t");
          tz = atof(sptr);
        } else if (sptr[0] == '-') {
          sign = -1;
          sptr = strtok(nullptr, "=: \n\t");
          tx = atof(sptr);
          sptr = strtok(nullptr, "=: \n\t");
          ty = atof(sptr);
          sptr = strtok(nullptr, "=: \n\t");
          tz = atof(sptr);
        } else if (sptr[0] == '*') {
          sptr = strtok(nullptr, "=: \n\t");
          sx = atof(sptr);
          sptr = strtok(nullptr, "=: \n\t");
          sy = atof(sptr);
          sptr = strtok(nullptr, "=: \n\t");
          sz = atof(sptr);
        } else if (sptr[0] == '@') {
          sptr = strtok(nullptr, "=: \n\t");
          rx = atof(sptr);
          sptr = strtok(nullptr, "=: \n\t");
          ry = atof(sptr);
          sptr = strtok(nullptr, "=: \n\t");
          rz = atof(sptr);
        }

        sptr = strtok(nullptr, "=: \n\t");
      }

      for (const auto& poly : polys) {
        if (strcmp(poly->name().c_str(), tmp_name.c_str()) == 0) {
          Corner* aa =
              new Corner(Point3d(poly->a->point().X() * sx + sign * tx,
                                 poly->a->point().Y() * sy + sign * ty,
                                 poly->a->point().Z() * sz + sign * tz));
          aa->setName(poly->a->name());
          b->addCorner(aa);
          Corner* bb =
              new Corner(Point3d(poly->b->point().X() * sx + sign * tx,
                                 poly->b->point().Y() * sy + sign * ty,
                                 poly->b->point().Z() * sz + sign * tz));
          bb->setName(poly->b->name());
          b->addCorner(bb);
          Corner* cc =
              new Corner(Point3d(poly->c->point().X() * sx + sign * tx,
                                 poly->c->point().Y() * sy + sign * ty,
                                 poly->c->point().Z() * sz + sign * tz));
          cc->setName(poly->c->name());
          b->addCorner(cc);

          Edge* e1 = new Edge(aa, bb);
          Edge* e2 = new Edge(bb, cc);
          Edge* e3 = new Edge(cc, aa);
          b->addEdge(e1);
          b->addEdge(e2);
          b->addEdge(e3);

          Wall* w = new Wall(aa, bb, cc);
          w->setName(poly->name());
          b->addWall(w);
        }
      }
    }
  }

  for (auto&& edge : b->edges) {
    for (const auto& wall : b->walls) {
      if (edge->same(wall->a, wall->b) || edge->same(wall->b, wall->c) ||
          edge->same(wall->c, wall->a)) {
        edge->right_wall = new Wall(*wall);
      } else if (edge->same(wall->c, wall->b) || edge->same(wall->b, wall->a) ||
                 edge->same(wall->a, wall->c)) {
        edge->opposite_wall = new Wall(*wall);
      }
    }
  }

  for (auto&& edge : b->edges) {
    edge->setConvexity(edge->isConvex());
  }

  FILE* f = fopen("env_posts_nobound.raw", "w");
  for (const auto& wall : b->walls) {
    fprintf(f, "1 1 1 %f %f %f %f %f %f %f %f %f\n", wall->a->point().X(),
            wall->a->point().Y(), wall->a->point().Z(), wall->c->point().X(),
            wall->c->point().Y(), wall->c->point().Z(), wall->b->point().X(),
            wall->b->point().Y(), wall->b->point().Z());
  }
  fclose(f);
}

#endif  // PARSER_H
