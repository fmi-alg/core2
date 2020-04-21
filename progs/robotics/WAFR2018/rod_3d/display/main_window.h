/*
 * main_window.h
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

  // Print text
  virtual MainWindow& operator<<(const std::string&);
  virtual MainWindow& operator<<(const char*);
  virtual MainWindow& operator<<(int);
  virtual MainWindow& operator<<(long);
  virtual MainWindow& operator<<(float);
  virtual MainWindow& operator<<(double);

 private slots:
  void on_random_clicked();
  void on_bfs_clicked();
  void on_greedy_clicked();
  void on_bigreedy_clicked();
  void on_size_clicked();
  void on_bisize_clicked();
  void on_vor_clicked();
  void on_bivor_clicked();

  void on_run_clicked();
  void on_exit_clicked();

  void on_show_path_clicked();
  void on_show_trace_clicked();

  void on_show_animation_clicked();
  void on_pause_animation_clicked();
  void on_replay_animation_clicked();

  void on_show_box_clicked();
  void on_show_box_boundary_clicked();
  void on_show_debug_poly_clicked();
  void on_show_debug_wall_clicked();
  void on_show_debug_edge_clicked();
  void on_show_debug_corner_clicked();

  void on_transparency_slider_valueChanged(int value);
  void on_step_by_step_clicked();
  void on_inc_valueChanged(int arg1);
  void on_left_clicked();
  void on_right_clicked();
  void on_horizontal_slider_valueChanged(int value);
  void on_zoom_slider_valueChanged();

 protected:
  void keyPressEvent(QKeyEvent* event);

 private:
  Ui::MainWindow* ui;
};

#endif  // MAIN_WINDOW_H
