/*
 * main_window.cpp
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#include "display/main_window.h"
#include "ui_main_window.h"

extern FILE* debug_ptr;

// File
extern std::string cfg_name;
extern std::vector<std::string> cfg_name_list;
extern int num_cfg;
extern std::string working_dir;
extern std::string input_dir;  // Path for input files
extern std::string filename;   // Input file name
extern std::string default_file_path;

extern Point3d start;  // start configuration
extern Point3d goal;   // goal configuration
extern Point3d start_rotation;
extern Point3d goal_rotation;

extern double rod_length;
extern double epsilon;  // resolution parameter
extern int random_seed;
extern int search_type;
extern bool show_path;
extern bool show_trace;
extern bool generate_trace;
extern bool show_animation;
extern bool pause_animation;
extern bool replay_animation;
extern unsigned int idx_path;
extern int transparency;
extern double distance_z;

extern bool show_box;
extern bool show_box_boundary;

extern int render_steps;
extern bool step;
extern int animation_speed;

extern Point3d box_translation;
extern double box_translation_width;
extern Point3d box_rotation;
extern double box_rotation_width;
extern bool show_debug_poly;
extern Triangle3d debug_wall;
extern bool show_debug_wall;
extern Segment3d debug_edge;
extern bool show_debug_edge;
extern bool show_debug_corner;
extern int trace_dense;

extern void run();
extern void parseCfgFile();

int incr = 1;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  for (int i = 0; i < num_cfg; ++i) {
    ui->comboBox->addItem(cfg_name_list[i].c_str());
  }
  for (int i = 0; i < num_cfg; ++i) {
    if (strcmp(cfg_name_list[i].c_str(), cfg_name.c_str()) == 0) {
      ui->comboBox->setCurrentIndex(i);
    }
  }

  ui->input_file->setText(
      QString::fromStdString(filename.substr(0, filename.length() - 4)));

  ui->aX->setValue(start.X());
  ui->aY->setValue(start.Y());
  ui->aZ->setValue(start.Z());
  ui->aRoll->setValue(start_rotation.X());
  ui->aPitch->setValue(start_rotation.Y());
  ui->aYaw->setValue(start_rotation.Z());

  ui->bX->setValue(goal.X());
  ui->bY->setValue(goal.Y());
  ui->bZ->setValue(goal.Z());
  ui->bRoll->setValue(goal_rotation.X());
  ui->bPitch->setValue(goal_rotation.Y());
  ui->bYaw->setValue(goal_rotation.Z());

  ui->rod_length->setValue(rod_length);
  ui->epsilon->setValue(epsilon);
  ui->random_seed->setValue(random_seed);
  srand(random_seed);

  ui->transparency_slider->setValue(transparency);
  ui->zoom_slider->setValue(distance_z);

  switch (search_type) {
    case RANDOM:
      ui->random->setChecked(true);
      break;
    case BFS:
      ui->bfs->setChecked(true);
      break;
    case GREEDY:
      ui->greedy->setChecked(true);
      break;
    case BIGREEDY:
      ui->bigreedy->setChecked(true);
      break;
    case GREEDY_SIZE:
      ui->size->setChecked(true);
      break;
    case BIGREEDY_SIZE:
      ui->bisize->setChecked(true);
      break;
    case VORONOI:
      ui->vor->setChecked(true);
      break;
    case BIVORONOI:
      ui->bivor->setChecked(true);
      break;
  }

  ui->show_box->setChecked(false);
  ui->steplabel->hide();
  ui->inc->setEnabled(false);
  ui->left->setEnabled(false);
  ui->right->setEnabled(false);

  ui->horizontal_slider->setValue(animation_speed);

  ui->Btx->setValue(box_translation.X());
  ui->Bty->setValue(box_translation.Y());
  ui->Btz->setValue(box_translation.Z());
  ui->box_translation_width->setValue(box_translation_width);
  ui->Brx->setValue(box_rotation.X());
  ui->Bry->setValue(box_rotation.Y());
  ui->Brz->setValue(box_rotation.Z());
  ui->box_rotation_width->setValue(box_rotation_width);
  ui->trace_dense->setValue(trace_dense);

  // Display widget
  connect(ui->openGLWidget, SIGNAL(zDistanceChanged(int)), ui->zoom_slider,
          SLOT(setValue(int)));
}

MainWindow::~MainWindow() { delete ui; }

//================================//
//     Display Text to Window     //
//================================//
/* Scrolls the text screen to the bottom and prints text */
MainWindow& MainWindow::operator<<(const std::string& str) {
  ui->textOutput->moveCursor(QTextCursor::End);
  ui->textOutput->insertPlainText(str.c_str());
  ui->textOutput->moveCursor(QTextCursor::End);
  return *this;
}

MainWindow& MainWindow::operator<<(const char* text) {
  ui->textOutput->moveCursor(QTextCursor::End);
  ui->textOutput->insertPlainText(text);
  ui->textOutput->moveCursor(QTextCursor::End);
  return *this;
}

MainWindow& MainWindow::operator<<(int i) {
  ui->textOutput->moveCursor(QTextCursor::End);
  ui->textOutput->insertPlainText(QString::number(i));
  ui->textOutput->moveCursor(QTextCursor::End);
  return *this;
}

MainWindow& MainWindow::operator<<(long l) {
  ui->textOutput->moveCursor(QTextCursor::End);
  ui->textOutput->insertPlainText(QString::number(l));
  ui->textOutput->moveCursor(QTextCursor::End);
  return *this;
}

MainWindow& MainWindow::operator<<(float f) {
  ui->textOutput->moveCursor(QTextCursor::End);
  ui->textOutput->insertPlainText(QString::number(f));
  ui->textOutput->moveCursor(QTextCursor::End);
  return *this;
}

MainWindow& MainWindow::operator<<(double d) {
  ui->textOutput->moveCursor(QTextCursor::End);
  ui->textOutput->insertPlainText(QString::number(d));
  ui->textOutput->moveCursor(QTextCursor::End);
  return *this;
}

void MainWindow::on_run_clicked() {
  char cfg_pre[200], cfg_cur[200];
  sprintf(cfg_pre, "%s", cfg_name.c_str());
  sprintf(cfg_cur, "%s", ui->comboBox->currentText().toStdString().c_str());
  if (strcmp(cfg_pre, cfg_cur) == 0) {
    filename = ui->input_file->text().toStdString() + ".txt";

    start.set(ui->aX->value(), ui->aY->value(), ui->aZ->value());
    start_rotation.set(ui->aRoll->value(), ui->aPitch->value(),
                       ui->aYaw->value());

    goal.set(ui->bX->value(), ui->bY->value(), ui->bZ->value());
    goal_rotation.set(ui->bRoll->value(), ui->bPitch->value(),
                      ui->bYaw->value());

    rod_length = ui->rod_length->value();
    epsilon = ui->epsilon->value();

    int new_seed = ui->random_seed->value();
    if (new_seed != random_seed) {
      random_seed = new_seed;
      srand(random_seed);
    }

    transparency = ui->transparency_slider->value();
    show_box = ui->show_box->isChecked();
  } else {
    cfg_name = ui->comboBox->currentText().toStdString();
    parseCfgFile();

    ui->input_file->setText(
        QString::fromStdString(filename.substr(0, filename.length() - 4)));

    ui->aX->setValue(start.X());
    ui->aY->setValue(start.Y());
    ui->aZ->setValue(start.Z());
    ui->aRoll->setValue(start_rotation.X());
    ui->aPitch->setValue(start_rotation.Y());
    ui->aYaw->setValue(start_rotation.Z());

    ui->bX->setValue(goal.X());
    ui->bY->setValue(goal.Y());
    ui->bZ->setValue(goal.Z());
    ui->bRoll->setValue(goal_rotation.X());
    ui->bPitch->setValue(goal_rotation.Y());
    ui->bYaw->setValue(goal_rotation.Z());

    ui->rod_length->setValue(rod_length);

    switch (search_type) {
      case RANDOM:
        ui->random->setChecked(true);
        break;
      case BFS:
        ui->bfs->setChecked(true);
        break;
      case GREEDY:
        ui->greedy->setChecked(true);
        break;
      case BIGREEDY:
        ui->bigreedy->setChecked(true);
        break;
      case GREEDY_SIZE:
        ui->size->setChecked(true);
        break;
      case BIGREEDY_SIZE:
        ui->bisize->setChecked(true);
        break;
      case VORONOI:
        ui->vor->setChecked(true);
        break;
      case BIVORONOI:
        ui->bivor->setChecked(true);
        break;
    }

    ui->epsilon->setValue(epsilon);

    int old_seed = ui->random_seed->value();
    if (old_seed != random_seed) {
      ui->random_seed->setValue(random_seed);
      srand(random_seed);
    }

    ui->horizontal_slider->setValue(animation_speed);

    ui->show_box->setChecked(show_box);
  }

  show_animation = true;
  pause_animation = false;
  idx_path = 0;
  incr = 0;
  generate_trace = false;
  run();

  ui->openGLWidget->update();
}

void MainWindow::on_random_clicked() { search_type = RANDOM; }

void MainWindow::on_bfs_clicked() { search_type = BFS; }

void MainWindow::on_greedy_clicked() { search_type = GREEDY; }

void MainWindow::on_bigreedy_clicked() { search_type = BIGREEDY; }

void MainWindow::on_size_clicked() { search_type = GREEDY_SIZE; }

void MainWindow::on_bisize_clicked() { search_type = BIGREEDY_SIZE; }

void MainWindow::on_vor_clicked() { search_type = VORONOI; }

void MainWindow::on_bivor_clicked() { search_type = BIVORONOI; }

void MainWindow::on_exit_clicked() {
  fclose(debug_ptr);
  this->close();
}

void MainWindow::on_show_trace_clicked() {
  show_trace = !show_trace;
  trace_dense = ui->trace_dense->value();
  ui->openGLWidget->update();
}
void MainWindow::on_show_path_clicked() {
  show_path = !show_path;
  ui->openGLWidget->update();
}

void MainWindow::on_show_animation_clicked() {
  show_animation = true;
  idx_path = 0;
  ui->openGLWidget->update();
}
void MainWindow::on_pause_animation_clicked() {
  pause_animation = !pause_animation;
  ui->openGLWidget->update();
}
void MainWindow::on_replay_animation_clicked() {
  show_animation = true;
  pause_animation = false;
  idx_path = 0;
  incr = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_show_box_clicked() {
  show_box = ui->show_box->isChecked();
  ui->openGLWidget->update();
}

void MainWindow::on_show_box_boundary_clicked() {
  show_box_boundary = ui->show_box_boundary->isChecked();
  ui->openGLWidget->update();
}

void MainWindow::on_show_debug_poly_clicked() {
  box_translation.set(ui->Btx->value(), ui->Bty->value(), ui->Btz->value());
  box_translation_width = ui->box_translation_width->value();
  box_rotation.set(ui->Brx->value(), ui->Bry->value(), ui->Brz->value());
  box_rotation_width = ui->box_rotation_width->value();
  show_debug_poly = ui->show_debug_poly->isChecked();
  ui->openGLWidget->update();
}

void MainWindow::on_show_debug_wall_clicked() {
  debug_wall =
      Triangle3d(Point3d(ui->T1x->value(), ui->T1y->value(), ui->T1z->value()),
                 Point3d(ui->T2x->value(), ui->T2y->value(), ui->T2z->value()),
                 Point3d(ui->T3x->value(), ui->T3y->value(), ui->T3z->value()));
  show_debug_wall = ui->show_debug_wall->isChecked();
  ui->openGLWidget->update();
}

void MainWindow::on_show_debug_edge_clicked() {
  debug_edge =
      Segment3d(Point3d(ui->T1x->value(), ui->T1y->value(), ui->T1z->value()),
                Point3d(ui->T2x->value(), ui->T2y->value(), ui->T2z->value()));
  show_debug_edge = ui->show_debug_edge->isChecked();
  ui->openGLWidget->update();
}

void MainWindow::on_show_debug_corner_clicked() {
  debug_edge =
      Segment3d(Point3d(ui->T1x->value(), ui->T1y->value(), ui->T1z->value()),
                Point3d(ui->T2x->value(), ui->T2y->value(), ui->T2z->value()));
  show_debug_corner = ui->show_debug_corner->isChecked();
  ui->openGLWidget->update();
}

void MainWindow::on_transparency_slider_valueChanged(int value) {
  transparency = value;
  ui->openGLWidget->update();
}

void MainWindow::on_step_by_step_clicked() {
  if (step) {
    step = false;
    ui->steplabel->hide();
    render_steps = 1;
    incr = 1;
    ui->inc->setValue(1);
    ui->inc->setEnabled(false);
    ui->left->setEnabled(false);
    ui->right->setEnabled(false);
    ui->openGLWidget->update();
  } else {
    step = true;
    incr = 1;
    ui->steplabel->show();
    ui->steplabel->setText("Step: 1");
    ui->inc->setEnabled(true);
    ui->left->setEnabled(true);
    ui->right->setEnabled(true);
  }
}

void MainWindow::on_inc_valueChanged(int arg1) { incr = arg1; }

void MainWindow::on_left_clicked() {
  if (render_steps - incr < 1) return;
  render_steps -= incr;

  ui->steplabel->setText("Step: " + QString::number(render_steps));
}

void MainWindow::on_right_clicked() {
  render_steps += incr;
  ui->steplabel->setText("Step: " + QString::number(render_steps));
}

void MainWindow::on_horizontal_slider_valueChanged(int value) {
  animation_speed = value;
}

void MainWindow::on_zoom_slider_valueChanged() {
  distance_z = ui->zoom_slider->value();
  ui->openGLWidget->update();
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Escape) {
    close();
  } else {
    QWidget::keyPressEvent(event);
  }
}
