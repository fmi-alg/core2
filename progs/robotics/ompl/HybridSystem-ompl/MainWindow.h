#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

#include "RRT.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    // Print text
    virtual MainWindow& operator<<(const std::string&);
    virtual MainWindow& operator<<(const char*);
    virtual MainWindow& operator<<(int);
    virtual MainWindow& operator<<(long);
    virtual MainWindow& operator<<(float);
    virtual MainWindow& operator<<(double);

private slots:

    void on_run_clicked();
    void on_exit_clicked();

    void on_prm_clicked();
    void on_gauss_clicked();
    void on_rrt_clicked();

    void on_prm_graph_clicked();
    void on_rrt_graph_clicked();

    void on_show_clicked();
    void on_pause_clicked();
    void on_replay_clicked();

    void on_animationSpeed_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
