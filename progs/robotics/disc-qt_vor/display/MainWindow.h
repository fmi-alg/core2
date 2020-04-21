#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

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
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

    void on_run_clicked();

    void on_rand_clicked();
    void on_bfs_clicked();
    void on_greedy_clicked();
    void on_vor_clicked();
    void on_bivor_clicked();

    void on_route_bfs_clicked();
    void on_route_safe_clicked();
    void on_route_astar_clicked();

    void on_exit_clicked();

    void on_show_clicked();
    void on_pause_clicked();
    void on_replay_clicked();
    void on_trace_clicked();
    void on_showFilledObstacles_clicked();

    void on_boundary_clicked();
    void on_hidebox_clicked();


    void on_pushButton_clicked();

    void on_inc_valueChanged(int arg1);
    void on_left_clicked();
    void on_right_clicked();

    void on_animationSpeed_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
