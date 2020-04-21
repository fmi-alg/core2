/*
SubVor, a program for computing Voronoi diagrams via subdivision.
Copyright (C) 2016 Huck Bennett, Evanthia Papadopoulou, and Chee Yap.
For comments or questions, please contact Huck Bennett at hbennett@cs.nyu.edu.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "vor_qt.h"
#include "vor_box.h"
#include "vor2d.h"
#include "Corner.h"
#include "Edge.h"
#include "Graphics.h"
#include "Object.h"
#include "Point.h"
// #include "CORE.h"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <png.h>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include "glui.h"
#endif
#ifdef __linux__
#include <GL/glui.h>
#endif

// Idea from http://forum.openframeworks.cc/t/scroll-wheel-event/593/3.
#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#endif

namespace po = boost::program_options;
using namespace std;

using vor2d::vor_box;
using vor2d::vor_qt;
using vor2d::Corner;
using vor2d::Edge;
using vor2d::Feature;
using vor2d::Object;

// Global parameters.
const int window_width = 1024;
double abs_eps;
double geom_eps;
bool interactive_mode = false;

int scene_width = window_width;
const double scale_factor = 1.25;
double scale = 1.0;
int sx = 0;
int sy = 0;

// Global variables.
Graphics g(Point2d{0.0, 0.0}, 1.0);
vor_qt* tree;
queue<vor_box*> subdiv;
queue<vor_box*> construct;
vector<vor_box*> vor_edge_boxes;
vector<vor_box*> vor_vert_boxes;
vector<vor_box*> degen_boxes;
vector<Object*> objects;
bool show_grid;
bool save_image;
bool display_image;
string input_file_name;

// Set input options.
// See http://www.boost.org/doc/libs/1_41_0/doc/html/program_options.
void init_options(int argc, char* argv[]) {
  const string input_file_arg = "input_file_name";

  // Set non-positional options.
  po::options_description desc("Voronoi diagram options");
  desc.add_options()
    ("help", "Print this help message.")
    ("geps", po::value<double>(&geom_eps)->default_value(1.0), "Geometric epsilon.")
    ("aeps", po::value<double>(&abs_eps)->default_value(1.0 / (1 << 6)), "Absolute epsilon.")
    ("save", po::value<bool>(&save_image)->default_value(false), "Save an image of the construction.")
    ("display", po::value<bool>(&display_image)->default_value(true), "Display the consturcted Voronoi diagram.")
    ("grid", po::value<bool>(&show_grid)->default_value(true), "Display the quadtree grid.")
    ("input_file_name", po::value<string>(&input_file_name), "Input file name.");

  // Set positional options.
  po::positional_options_description p_desc;
  p_desc.add("input_file_name", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(p_desc).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << "\n";
    exit(1);
  }

  if (!vm.count("input_file_name")) {
    interactive_mode = true;
  }
}

void initialize(string input_file_name) {
  const string title_prefix = "2D subdivision-based Voronoi diagram";

  // Initialize global variables.
  tree = new vor_qt(2 /* dimension */, 2.0 /* width */);
  abs_eps = min(abs_eps, geom_eps);
  
  // Initialize graphics.
  if (display_image) {
    // Set up antialiasing.
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Set up window.
    glutInitWindowSize(window_width, window_width);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutCreateWindow((title_prefix + (input_file_name.length() > 0 ? (" - " + input_file_name) : "")).c_str());
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    // Set up input-output.
    GLUI_Master.set_glutMouseFunc(Mouse);
  }
}

void cleanup() {
  delete tree;
}

void display() {
  if(!display_image) {
    return;
  }
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  g.draw_box_rec(*tree->root(), show_grid);
  
  // Draw all features.
  Corner* c;
  Edge* e;
  vector<Feature*>* features = tree->root()->get_features();
  for (auto it = features->begin(); it < features->end(); ++it) {
    // This dynamic casting is a hack from StackOverflow for handling C++'s lack of "instanceof".
    // TODO: Use typeid or some other functionality?
    c = dynamic_cast<Corner*>(*it);
    if (c != nullptr) {
      g.draw_corner(*c);
    } else {
      e = dynamic_cast<Edge*>(*it);
      g.draw_edge(*e);
    }
  }

  // Save image if applicable.
  if (scale == 1.0 && save_image) {
    save_png();
  }
  
  glutSwapBuffers();
}

// Based on http://www.labbookpages.co.uk/software/imgProc/libPNG.html
// and http://zarb.org/~gc/html/libpng.html.
#define RGBA_COMPS 4
void save_png() {
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  png_bytep* row_ptrs = (png_bytep*) malloc(sizeof(png_bytep*) * window_width);
  png_bytep row_ptr = NULL;
  string filename = input_file_name + (show_grid ? "-grid" : "") + ".png";
  FILE* file = fopen(filename.c_str(), "wb");
  GLubyte* data = (GLubyte*) malloc(sizeof(GLubyte) * RGBA_COMPS * window_width * window_width);

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  info_ptr = png_create_info_struct(png_ptr);
  
  for (int i = 0; i < window_width; i++) {
    row_ptrs[i] = (png_bytep) malloc(sizeof(png_bytep) * RGBA_COMPS * window_width);
  }
  if (!file || !info_ptr || !row_ptrs) {
    cout << "Warning: Unable to write file.\n";
    return;
  }
  png_init_io(png_ptr, file);
  glReadPixels(0, 0, window_width, window_width, GL_RGBA, GL_UNSIGNED_BYTE, data);

  // Write header.
  png_set_IHDR(png_ptr, info_ptr, window_width, window_width,
	       8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
	       PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png_ptr, info_ptr);
  
  // Write image.
  for (int x = 0; x < window_width; x++) {
    row_ptr = row_ptrs[window_width - (x + 1)];
    for (int y = 0; y < window_width * RGBA_COMPS; y++) {
      row_ptr[y] = data[window_width * RGBA_COMPS * x + y];
    }
  }
  png_write_rows(png_ptr, row_ptrs, window_width);
  png_write_end(png_ptr, NULL);

  // Cleanup.
  if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
  if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp) NULL);

  for (int i = 0; i < window_width; i++) {
    free(row_ptrs[i]);
  }
  free(row_ptrs);
  free(data);
  fclose(file);
}

int main(int argc, char* argv[]) {
  init_options(argc, argv);

  if (input_file_name.length() == 0) {
    cout << "No input file specified.\n";
    exit(1);
  }
  
  glutInit(&argc, argv);
  initialize(input_file_name);
  parse(input_file_name);
  glutDisplayFunc(display);
  run();
  glutMainLoop();
  cleanup();
}

string get_line(ifstream& ifs) {
  string s = "";

  do {
    if (ifs.eof()) {
      cout << "End of file encountered.\n";
      exit(1);
    }
    std::getline(ifs, s);
    boost::algorithm::trim(s);
  } while (s.length() == 0 || s[0] == '#'); // Skip comment lines.

  return s;
}

void print_objects(vor_box* box) {
  for (Object* o : *box->get_objects()) {
    for (Feature* f : *o->get_features()) {
      // This is a hack found on StackOverflow to handle C++'s lack of "instanceof".
      // TODO: Use typeid or some other functionality?
      Corner* c = dynamic_cast<Corner*>(f);
      if (c != nullptr) {
	cout << "Point: " << c->position()[0] << " " << c->position()[1];
	// cout << c->dfun_sq()->to_string() << "\n";
	// cout << c->dfun_sq_grad().first.to_string() << "\n";
	// cout << c->dfun_sq_grad().second.to_string() << "\n";
      } else {
	Edge* e = dynamic_cast<Edge*>(f);
	cout << "Edge: "
	     << e->source()->position()[0] << " " << e->source()->position()[1] << ", "
	     << e->dest()->position()[0]   << " " << e->dest()->position()[1];
	// cout << e->dfun_sq()->to_string() << "\n";
	// cout << e->dfun_sq_grad().first.to_string() << "\n";
	// cout << e->dfun_sq_grad().second.to_string() << "\n";
      }
      cout << "\n";
    }
    cout << "\n";
  }
}

void parse(string input) {
  int num_points;
  int point_count;
  int num_objects;
  int vert;
  int x, y;
  Object* o;
  vor_box* root = tree->root();
  ifstream ifs(input, std::ifstream::in);

  if (!ifs) {
    cout << "File " << input << " not found.\n";
    exit(1);
  }

  // 1. Parse points.
  stringstream ss(get_line(ifs));
  ss.seekg(0);
  ss >> num_points;
  vector<double> px(num_points);
  vector<double> py(num_points);
  for (int i = 0; i < num_points; i++) {
    stringstream ss(get_line(ifs));
    ss.seekg(0);
    ss >> px[i] >> py[i];
  }

  // 2. Parse objects.	  
  ss.str(get_line(ifs));
  ss.seekg(0);
  ss >> num_objects;
  for (int i = 0; i < num_objects; i++) {
    double weight = 1.0;
    double a, b, c;
    ss.str(get_line(ifs));
    ss.seekg(0);

    // Parse metric information.
    // Parse multiplicative weight.
    if (ss.peek() == 'w') {
      ss.seekg(1);
      ss >> weight;
      
      // Verify that the weight is positive.
      if (weight <= 0.0) {
	cout << "Error: multiplicative weight must be positive.";
	exit(1);
      }
      o = new Object(weight);
      ss.str(get_line(ifs));
      ss.seekg(0);
    } else if (ss.peek() == 'm') { // Parse anisotropic metric parameters.
      ss.seekg(1);
      // For matrices of the form:
      // [a b]
      // [b c]
      ss >> a >> b >> c;
      o = new Object(a, b, c);
      ss.str(get_line(ifs));
      ss.seekg(0);
      
      // Verify that the matrix is positive definite
      // by checking that its two principal minors are positive.
      if (a <= 0 || (a * c - b * b) <= 0) {
        cout << "Error: metric matrix is not positive definite.\n";
        exit(1);
      }
    } else {
      o = new Object(1.0);
    }

    objects.push_back(o);
    vector<Point2d> points;
    int poly_verts = 0;
    while (!ss.eof()) {
      if (++poly_verts > num_points) {
	cout << "Parse error.\n";
	exit(1);
      }
      ss >> vert;
      Point2d point(2 * px[vert] / window_width - 1, 2 * py[vert] / window_width - 1);
      points.push_back(point);
      assert(points.size() >= 1);
    }

    for (int j = 0; j < points.size(); j++) {
      // Add corner.
      Corner* corner = new Corner(points[j], o);
      o->add_feature(corner);
      root->add_feature(corner);

      // Add edge.
      if (j > 0) {
	Edge* edge = new Edge(points[j - 1], points[j], o);
	o->add_feature(edge);
	root->add_feature(edge);
      }

      // Close polygon (if applicable).
      if (j == points.size() - 1 && points.size() >= 3) {
	Edge* edge = new Edge(points[j], points[0], o);
	o->add_feature(edge);
	root->add_feature(edge);
      }
    }

    root->add_object(o);
  }

  ifs.close();
  print_objects(root);
}

void Mouse(int button, int state, int x, int y) {
  if (state == GLUT_UP) {
    if (button == GLUT_LEFT_BUTTON) {
      show_grid = !show_grid;
    } else if (button == GLUT_WHEEL_UP) {
      scale *= scale_factor;
      Point2d ctr(2.0 * x / window_width - 1, -(2.0 * y / window_width - 1));
      g.update(ctr, scale);
    } else if (button == GLUT_WHEEL_DOWN) {
      scale = (scale > scale_factor) ? (scale / scale_factor) : 1.0;
      g.update(scale);
    }
    display(); // Rerenders based on any click.
  }
}

#define MAX_OBJECTS_FOR_CONSTRUCTION 3 // TODO: Handle degenerate input. I.e., = infinity.
#define MK_SCALE 1.0
#define JC_SCALE 3.0
#define INT_SCALE 5.0

bool inter_root(vor_box* box, double scale) {
  // TODO: Avoid iterating through all vertex boxes.
  for (auto it = vor_vert_boxes.begin(); it != vor_vert_boxes.end(); ++it) {
    if (box->scaled_intersect(**it, scale)) {
      return true;
    }
  }
  return false;
}

bool contained_in_any(vor_box* box, double scale) {
  // TODO: Avoid iterating through all vertex boxes.
  for (auto it = vor_vert_boxes.begin(); it != vor_vert_boxes.end(); ++it) {
    if (box->contained_in(**it, scale)) {
      return true;
    }
  }
  return false;
}

void blame(vor_box* box) {
  if (box->num_objects() > MAX_OBJECTS_FOR_CONSTRUCTION) {
    cout << box->num_objects() << " objects still active.\n";
    return;
  }
  if (!box->few_active_features_per_object()) {
    cout << "One object has many active features.\n";
    for (Feature* f : *(box->get_features())) {
      cout << f << " " << f->parent() << "\n";
    }
    return;
  }
  if (box->max_lipschitz() * box->radius() > box->midpoint_clearance()) {
    cout << "Box clearance insufficent.\n";
    return;
  }
  if (inter_root(box, INT_SCALE)) {
    cout << "Intersects other root boxes.\n";
    cout << vor_vert_boxes.size() << "\n";
    return;
  }
  if (!box->cpv()) {
    cout << "PV failed.\n";
    return;
  }
  if (!box->cjc(JC_SCALE)) {
    cout << "JC failed.\n";
    return;
  }
  if (!box->cmk(MK_SCALE)) {
    cout << "MK failed.\n";
    return;
  }
  // if (2 * box->radius() > geom_eps) {
  //   cout << "Geometric epsilon reached.\n";
  //   cout << box->radius() << " " << geom_eps << "\n";
  //   assert(0);
  //   return;
  // }

  // Programmer error!
  cout << "Programmer error!\n";
  // assert(0);
}

void enqueue_children(vor_box* box) {
  if (box->radius() < abs_eps) {
    cout << "Warning: absolute epsilon reached.\n";
    blame(box);
    box->set_degen(true);
    degen_boxes.push_back(box);
    return;
  }
  
  box->smooth_split();
  vor_box** children = box->children();
  for (int i = 0; i < box->num_children(); i++) {
    subdiv.push(children[i]);
  }
}

void run() {
  assert(abs_eps <= geom_eps);

  // Subdivision phase.
  subdiv.push(tree->root());
  while (!subdiv.empty()) {
    vor_box* box = subdiv.front();
    subdiv.pop();
    double radius = box->radius();
    double num_obj = box->num_objects();
    double lip = box->max_lipschitz();
    assert(num_obj > 0);

    if (num_obj == 1 || box->contained_in_polygon()) {
      continue;
    } else if (num_obj > MAX_OBJECTS_FOR_CONSTRUCTION ||
	       lip * radius > box->midpoint_clearance() || // TODO(*): Check this.
	       radius > 2 * geom_eps ||      // TODO: Make sure this isn't off by a multiplicative factor of 2.
	       !box->few_active_features_per_object() ||
	       !box->cpv()) {
      enqueue_children(box);
    } else if (num_obj == 2 || contained_in_any(box, INT_SCALE)) {
      construct.push(box);                   // TODO: Don't use "construct" queue.
      vor_edge_boxes.push_back(box);
    } else if (!box->cjc(JC_SCALE) ||
	       !box->cmk(MK_SCALE) ||
	       inter_root(box, INT_SCALE)) { // num_obj == 3
      enqueue_children(box);
    } else {
      construct.push(box);
      vor_vert_boxes.push_back(box);
    }
  }

  // Construction phase.
  while (!construct.empty()) {
    vor_box* box = construct.front();
    construct.pop();

    // Ensure that boxes used for construction are leaves.
    // Due to smooth splitting nodes in the construction queue may not be.
    if (box->is_leaf()) {
      box->gen_vertices();
    } else {
      for (int i = 0; i < box->num_children(); i++) {
	construct.push(box->children()[i]);
      }
    }
  }

  // Display.
  display();

  // Print statistics.
  cout << "\n";
  cout << "Total splits: " << tree->splits() << "\n";
  cout << "Vertices confirmed: " << vor_vert_boxes.size() << "\n";
  cout << "Number of degenerate boxes: " << degen_boxes.size() << "\n";
}
