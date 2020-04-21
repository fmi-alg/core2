/*
 * main.h
 *
 * Created on: June 30, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "CORE/geom3d/point3d.h"
#include "CORE/geom3d/polygon3d.h"
#include "display/color.h"
#include "display/main_window.h"
#include "path/path.h"
#include "sss/graph.h"
#include "sss/soft_subdivision_search.h"
#include "strategy/box_priority_queue.h"
#include "utility/Timer.h"
#include "utility/parser.h"

// External Routines ========================================

void run();
SoftSubdivisionSearch *sss;

// GLOBAL INPUT Parameters ========================================

#define mw_out (*window)
MainWindow *window;

FILE *debug_ptr;

// File
std::string cfg_name("WAFR18_rand40_2.cfg");
std::vector<std::string> cfg_name_list;
int num_cfg = 0;
std::string working_dir;
std::string input_dir("inputs/WAFR18");  // Path for input files
std::string filename("empty.txt");       // Input file name
std::string default_file_path("inputs/empty.txt");

// Paramters
double EPS = 1e-10;               // check 0
Point3d start(300, 450, 260);     // start configuration
Point3d goal(175, 50, 360);       // goal configuration
Point3d start_rotation(0, 1, 0);  // x hat, y hat, z hat
Point3d goal_rotation(-1, 0, 0);
double epsilon = 4;                        // resolution parameter
double box_width = 512;                    // Initial box width
double rod_length = 36;                    // Rod length
double thickness = 0;                      // Rod thickness
std::vector<int> window_pos = {260, 150};  // Position of Window
int search_type = 2;                       // random, BFS, Greedy, ...
int random_seed =
    999;  // seed for random number generator (Could also be used for BFS, etc)

// map parameter
double map_scale = 1;  // scaling of input environment
std::vector<double> map_translation = {0, 0,
                                       0};  // translation of input environment

// camera setting
Point3d eye(512, 512, 512);
Point3d at(0, 0, 0);
Vector up(0, 1, 0);
Vector eye_vector(0, 0, 0);

// render setting
int transparency = 80;
double box_transparency = 0.2f;
double distance_z = 900;
unsigned int idx_path = 0;
float idx_seg = 1;
float seg_count = 32;
bool show_path = true;
bool show_trace = false;
bool generate_trace = false;
bool show_box = false;
bool show_box_boundary = false;
bool show_animation = true;
bool pause_animation = false;
bool replay_animation = false;
int animation_speed = 99;

std::vector<ConfBox3d *> path;
std::vector<std::vector<float>> path_display;
static int run_counter = 0;
bool no_path = true;
bool move_away = true;
int frame_rate = 50;
bool verbose = false;

// debug information
Point3d box_translation(0, 0, 0);
double box_translation_width = 8;
Point3d box_rotation(0, 1, 0);
double box_rotation_width = 2;
bool show_debug_poly = false;
Triangle3d debug_wall;
bool show_debug_wall = false;
Segment3d debug_edge;
bool show_debug_edge = false;
bool show_debug_corner = false;
int trace_dense = 16;

// Global Variables for openGL ========================================
//
Point3d obj_pos(static_cast<float>(-box_width / 2.),
                static_cast<float>(-box_width / 2.),
                static_cast<float>(2. * box_width));
float view_rotate[16] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                         0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
float view_rotate_x[16] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
float view_rotate_y[16] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
float view_rotate_z[16] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
Point3d top_view_pos(static_cast<float>(box_width / 2.0),
                     static_cast<float>(box_width / 2.0),
                     static_cast<float>(box_width * 2.5));
