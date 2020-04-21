/*
 * main.cpp
 *
 * Created on: June 30, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 *
 * TODO: replace all pointers with shared_ptr
 */

#include <algorithm>
#include <string>
#include <vector>

#include <QApplication>
#include <QCoreApplication>
#include <QDir>

#include "main.h"

SoftSubdivisionSearch* initializeSSS(const Point3d& start,
                                     const Point3d& goal) {
  ConfBox3d* root = new ConfBox3d(
      Point3d(box_width / 2.0, box_width / 2.0, box_width / 2.0), box_width);
  ConfBox3d::rod_length = rod_length;

  parseConfigFile(root);

  return new SoftSubdivisionSearch(root, epsilon, search_type, start, goal,
                                   start_rotation, goal_rotation);
}

void run() {
  mw_out << "Run " << ++run_counter << ":\n";

  Timer t;
  t.start();

  if (sss) {
    delete (sss);
  }

  sss = initializeSSS(start, goal);
  path = sss->softSubdivisionSearch();
  int ct = sss->getNumSubdivisions();
  no_path = path.empty();

  t.stop();
  // print the elapsed time in millisec
  mw_out << ">>\tTime used " << t.getElapsedTimeInMilliSec() << " ms\n";
  mw_out << ">>\tExpanded " << ct << " times"
         << "\n";

  mw_out << ">>\n";
  if (!no_path) {
    mw_out << ">>\tPath found !\n";
  } else {
    mw_out << ">>\tNo Path !\n";
  }
  mw_out << ">>\n";

  mw_out << ">>\tSearch Strategy: ";
  switch (search_type) {
    case RANDOM:
      mw_out << "Random\n";
      break;
    case BFS:
      mw_out << "BFS\n";
      break;
    case GREEDY:
      mw_out << "Greedy\n";
      break;
    case BIGREEDY:
      mw_out << "BIGreedy\n";
      break;
    case GREEDY_SIZE:
      mw_out << "Greedy + size heuristic\n";
      break;
    case BIGREEDY_SIZE:
      mw_out << "Bi directional Greedy + size heuristic\n";
      break;
    case VORONOI:
      mw_out << "Voronoi\n";
      break;
    case BIVORONOI:
      mw_out << "Bi-Voronoi\n";
      break;
    default:
      break;
  }

  mw_out << ">>\n";
  mw_out << ">>\ttotal Free boxes: " << sss->free_count << "\n";
  mw_out << ">>\ttotal Stuck boxes: " << sss->stuck_count << "\n";
  mw_out << ">>\ttotal boxes: "
         << sss->free_count + sss->stuck_count + sss->mix_count << "\n\n";
}

void pwdWorkingDir(std::string argv) {
  bool found_dir = false;
  int idx = 0;
  for (int i = argv.size() - 1; i >= 0; --i) {
    if (argv[i] == '/') {
      idx = i + 1;
      break;
    }
  }
  std::string folder_name = "/" + argv.substr(idx, argv.size()) + "/";
  working_dir = QDir::currentPath().toStdString();

  // Test if the build directory is rod_3d. If so,
  // the path to the current working directory will
  // include /rod_3d
  size_t idx_dir = working_dir.rfind(folder_name);
  if (idx_dir != std::string::npos) {
    working_dir = working_dir.substr(0, idx_dir + folder_name.size());
    // Set current working directory to rod_3d
    QDir::setCurrent(working_dir.c_str());
    found_dir = true;
  }

  // Test if a build directory (/build-rod_3d...) was created. This directory
  // will reside in the same directory as /rod_3d
  if (!found_dir && (idx_dir = working_dir.rfind("build-" + folder_name)) !=
                        std::string::npos) {
    QDir profile(working_dir.substr(0, idx_dir).c_str());

    if (profile.exists(
            std::string(folder_name + "/" + folder_name + ".pro").c_str())) {
      working_dir = working_dir.substr(0, idx_dir) + "/" + folder_name;

      // Set current working directory to rod_3d
      QDir::setCurrent(working_dir.c_str());
      found_dir = true;
    }
  }

  // /rod_3d could not be found
  if (!found_dir) {
    std::cerr << std::endl
              << "!! WARNING !!\n"
              << "The program may not work correctly.\n"
              << std::endl;
  }
}

int main(int argc, char* argv[]) {
  pwdWorkingDir(std::string(argv[0]));

  if (argc > 1) start.setX(atof(argv[2]));        // start x
  if (argc > 2) start.setY(atof(argv[3]));        // start y
  if (argc > 3) start.setZ(atof(argv[4]));        // start z
  if (argc > 4) goal.setX(atof(argv[5]));         // goal x
  if (argc > 5) goal.setY(atof(argv[6]));         // goal y
  if (argc > 6) goal.setZ(atof(argv[7]));         // goal z
  if (argc > 7) epsilon = atof(argv[8]);          // epsilon (resolution)
  if (argc > 8) rod_length = atof(argv[9]);       // rod length
  if (argc > 0) filename = argv[10];              // Input file name
  if (argc > 10) box_width = atof(argv[11]);      // box_width
  if (argc > 11) window_pos[0] = atoi(argv[12]);  // window X pos
  if (argc > 12) window_pos[1] = atoi(argv[13]);  // window Y pos
  if (argc > 13)
    search_type = atoi(argv[14]);  // search type (random, BFS, Greedy, ...)
  if (argc > 14)
    random_seed = static_cast<unsigned int>(
        atoi(argv[14]));  // for random number generator
  if (argc > 15) {
    input_dir = argv[15];  // path for input files
    std::stringstream ss;
    ss << input_dir << "/" << filename;  // create full file name
    default_file_path = ss.str();
  }
  if (argc > 17)
    map_translation[0] = atof(argv[17]);  // x-translation of input file
  if (argc > 18)
    map_translation[1] = atof(argv[18]);  // y-translation of input file
  if (argc > 19)
    map_translation[2] = atof(argv[19]);      // z-translation of input file
  if (argc > 20) map_scale = atof(argv[20]);  // scaling of input file
  if (argc > 21) eye.setX(atof(argv[21]));
  if (argc > 22) eye.setY(atof(argv[22]));
  if (argc > 23) eye.setZ(atof(argv[23]));
  if (argc > 24) at.setX(atof(argv[24]));
  if (argc > 25) at.setY(atof(argv[25]));
  if (argc > 26) at.setZ(atof(argv[26]));
  if (argc > 27) up.setX(atof(argv[27]));
  if (argc > 28) up.setY(atof(argv[28]));
  if (argc > 29) up.setZ(atof(argv[29]));
  if (argc > 30) transparency = atoi(argv[30]);
  if (argc > 31) frame_rate = atoi(argv[31]);

  debug_ptr = fopen("debug.txt", "w");

  // Allow creation of Qt GUI
  QApplication app(argc, argv);
  parseCfgList();
  parseCfgFile();
  window = new MainWindow();
  run();
  window->show();

  return app.exec();
}
