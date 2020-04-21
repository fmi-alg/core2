//==============================================================================
// ControlWindow.h
// =================
// This is the central GUI for the disc program.
// ControlWindow feed information to the user and defines what happens when
// a widget on the display is altered.
//
// AUTHOR: Bryant Curto (bryantcurto@gmail.com)
// CREATED: 2015-07-28
//
// Copyright (c) 2015 Bryant Curto
//==============================================================================

#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

// Qt
#include <QMainWindow>
#include <QMouseEvent>

// Standard Library
#include <string>
#include <vector>



enum RenderType {
    ONE_STEP = 1,
    ONE_THOUSAND_STEPS = 1000,
    INFINITY_STEPS = -1,
    VARIABLE_STEPS = 0
};


namespace Ui {
class ControlWindow;
}

class ControlWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    // Constructor & Destructor
    ControlWindow(QWidget* parent = 0);
    ~ControlWindow();

    // Set Callback Functions
    void setExitCallback(void (*e)(int, const char*));
    void setRunCallback(void (*r)());
    void mousePressEvent(QMouseEvent *event);

    // Print text
    virtual ControlWindow& operator<<(const std::string&);
    virtual ControlWindow& operator<<(const char*);
    virtual ControlWindow& operator<<(int);
    virtual ControlWindow& operator<<(long);
    virtual ControlWindow& operator<<(float);
    virtual ControlWindow& operator<<(double);
    
    virtual void updateDisplay();
    void defaultFile();

private slots:
    // Called after button pressed
    void on_pushButton_run_clicked();
    void on_pushButton_exit_clicked();
    void on_checkBox_bound_clicked(bool);

    // Radio Buttons for Render Options
    void on_radioButton_1step_clicked(bool checked);
    void on_radioButton_1000steps_clicked(bool checked);
    void on_radioButton_infsteps_clicked(bool checked);
    void on_radioButton_varsteps_clicked(bool checked);

    void on_spinBox_valueChanged(int arg1);

    void on_radioButton_varsteps_clicked();

    void on_right_clicked();

    void on_left_clicked();


    void on_anim_clicked();
    void on_replay_clicked();
    void on_pause_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_checkBox_clicked();



private:
    Ui::ControlWindow* ui;
        

    enum PriorityQueueType {
        Random = 0, BFS = 1, A_star = 2
    };
    
    void (*run)() = NULL;
    void (*exit)(int, const char*) = NULL;
};

#endif // CONTROLWINDOW_H
