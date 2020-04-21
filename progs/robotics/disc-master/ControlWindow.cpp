//==============================================================================
// ControlWindow.cpp
// =================
// This class is the central GUI for the disc program.
// ControlWindow feed information to the user and defines what happens when
// a widget on the display is altered.
//
// AUTHOR: Bryant Curto (bryantcurto@gmail.com)
// CREATED: 2015-08-01
//
// Copyright (c) 2015 Bryant Curto
//==============================================================================

// Header
#include "ControlWindow.h"

// Custom
#include "ui_ControlWindow.h"
#include "Display.h"

// Qt
#include <QTextBrowser>
#include <QtWidgets/QOpenGLWidget>

// Standard Library
#include <vector>
#include <QApplication>

// Global Variables in disc.cpp
extern std::string egName;
extern std::string fileName;
extern std::string inputDir;
extern double alpha[2];
extern double beta[2];
extern double epsilon;
extern double R0;
extern int QType;
extern int seed;
extern bool hideBoxBoundary;
extern bool runAnim;

extern void parseExampleFile();

static int defaultVariableStepsValue = 1;

RenderType stepRenderType = INFINITY_STEPS;
long varSteps = 0;

int animationSpeed(99);

bool coloredBoxes(true); //True if the colored boxes are to be shown, false otherwise - John Ryan 08/24/2015


int increment=1;


/* 
 * CONSTRUCTOR
 *
 * Set up window, buttons, and give buttons
 * their initial values.
 */
ControlWindow::ControlWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::ControlWindow)
{
    // Get rid of exit/shrink/maximize buttons on top left
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    
    ui->setupUi(this);

    ui->openGLWidget->setControlWindow(this);

    
    // Apply initial values
    ui->lineEdit_fn->setText(QString::fromStdString(egName));
    ui->lineEdit_id->setText(QString::fromStdString(fileName));
    ui->doubleSpinBox_r->setValue(R0);
    ui->doubleSpinBox_e->setValue(epsilon);
    ui->doubleSpinBox_ax->setValue(alpha[0]);
    ui->doubleSpinBox_ay->setValue(alpha[1]);
    ui->doubleSpinBox_bx->setValue(beta[0]);
    ui->doubleSpinBox_by->setValue(beta[1]);
    ui->spinBox_s->setValue(seed);

    ui->left->hide();
    ui->right->hide();
    ui->label->hide();
    ui->spinBox->hide();

    switch (QType) {
        case Random:
            ui->radioButton_rand->setChecked(true);
            break;
        case BFS:
            ui->radioButton_bfs->setChecked(true);
            break;
        case A_star:
            ui->radioButton_astr->setChecked(true);
            break;
        default:
            char error[] = "Q Type not recognized";
            if (exit)
                exit(QType, error);
            else
                ::exit(QType);
            break;
    }
    
    ui->radioButton_infsteps->setChecked(true);
    ui->spinBox_steps->setValue(defaultVariableStepsValue);
}

/*
 * DESTRUCTOR
 *
 * Destroy 'ui'
 */
ControlWindow::~ControlWindow()
{
    delete ui;
}

/*
 * SET EXIT CALLBACK FUNCTION
 *
 * Function passed will be called when exit button is pressed
 */
void ControlWindow::setExitCallback(void (*e)(int, const char*)) {
    exit = e;
}

/* 
 * SET EXIT CALLBACK FUNCTION
 *
 * Function passed will be called when run button is pressed
 */
void ControlWindow::setRunCallback(void (*r)()) {
    run = r;
}


            //================================//
            //     Display Text to Window     //
            //================================//
/* Scrolls the text screen to the bottom and prints text */
ControlWindow& ControlWindow::operator<< (const std::string& str) {
    ui->textOutput->moveCursor (QTextCursor::End);
    ui->textOutput->insertPlainText(str.c_str());
    ui->textOutput->moveCursor (QTextCursor::End);
    return *this;
}

ControlWindow& ControlWindow::operator<< (const char* text) {
    ui->textOutput->moveCursor (QTextCursor::End);
    ui->textOutput->insertPlainText(text);
    ui->textOutput->moveCursor (QTextCursor::End);
    return *this;
}

ControlWindow& ControlWindow::operator<< (int i) {
    ui->textOutput->moveCursor (QTextCursor::End);
    ui->textOutput->insertPlainText(QString::number(i));
    ui->textOutput->moveCursor (QTextCursor::End);
    return *this;
}

ControlWindow& ControlWindow::operator<< (long l) {
    ui->textOutput->moveCursor (QTextCursor::End);
    ui->textOutput->insertPlainText(QString::number(l));
    ui->textOutput->moveCursor (QTextCursor::End);
    return *this;
}

ControlWindow& ControlWindow::operator<< (float f) {
    ui->textOutput->moveCursor (QTextCursor::End);
    ui->textOutput->insertPlainText(QString::number(f));
    ui->textOutput->moveCursor (QTextCursor::End);
    return *this;
}

ControlWindow& ControlWindow::operator<< (double d) {
    ui->textOutput->moveCursor (QTextCursor::End);
    ui->textOutput->insertPlainText(QString::number(d));
    ui->textOutput->moveCursor (QTextCursor::End);
    return *this;
}


        //======================================//
        //   Control Window Input Functions     //
        //======================================//

/* 
 * CLICK RUN BUTTON
 *
 * Retrieves input from gui, calls run function, and draws to screen.
 * Checks if run and exit functions are set and reports to user if
 * either/both are not
 */
void ControlWindow::on_pushButton_run_clicked()
{
    using namespace std;
    
    if (!run || !exit) {
        if (!run && !exit) {
            std::cerr << "\n\n\n"
            "**************************************************\n"
            "WARNING!!\n\n"
            "ControlWindow exit and run functions not specified\n"
            "**************************************************\n";
            this->close();
        } else if (!exit) {
            std::cerr << "\n\n\n"
            "**************************************************\n"
            "WARNING!!\n\n"
            "ControlWindow exit function not specified\n"
            "**************************************************\n";
            this->close();
        } else
            exit(-1, "Control Window run function not specified");
    } else {
        
        // Store missing arguments
        std::vector<string> missingArgs;

        // Test Example Name
        if (ui->lineEdit_fn->text().isEmpty())
            missingArgs.push_back("Example Name");

        // Test File Name
        if (ui->lineEdit_id->text().isEmpty())
            missingArgs.push_back("File Name");

        char egPre[200], egCur[200];
        sprintf(egPre, "%s", egName.c_str());
        sprintf(egCur, "%s", ui->lineEdit_fn->text().toStdString().c_str());

        // Retrieves values if no arguments are missing
        if (!missingArgs.size()) {

            if (strcmp(egPre, egCur) == 0) {
                // Update global variables
                fileName = ui->lineEdit_id->text().toStdString();
                R0 =       ui->doubleSpinBox_r->value();
                epsilon =  ui->doubleSpinBox_e->value();
                alpha[0] = ui->doubleSpinBox_ax->value();
                alpha[1] = ui->doubleSpinBox_ay->value();
                beta[0] =  ui->doubleSpinBox_bx->value();
                beta[1] =  ui->doubleSpinBox_by->value();
                seed =     ui->spinBox_s->value();

                if (ui->radioButton_rand->isChecked())
                    QType = Random;
                else if (ui->radioButton_bfs->isChecked())
                    QType = BFS;
                else
                    QType = A_star;

                // Update rendering options
                if (ui->radioButton_1step->isChecked())
                    stepRenderType = RenderType::ONE_STEP;

                else if (ui->radioButton_1000steps->isChecked())
                    stepRenderType = RenderType::ONE_THOUSAND_STEPS;

                else if (ui->radioButton_infsteps->isChecked())
                    stepRenderType = RenderType::INFINITY_STEPS;

                else { //ui->radioButton_varsteps->isChecked()
                    stepRenderType = RenderType::VARIABLE_STEPS;
                    varSteps = ui->spinBox_steps->value();
                }

                // Reset "variable steps" if not selected
                if (stepRenderType != RenderType::VARIABLE_STEPS)
                    ui->spinBox_steps->setValue(defaultVariableStepsValue);

            }
            else {
                egName=ui->lineEdit_fn->text().toStdString();
                parseExampleFile();

                ui->lineEdit_id->setText(QString::fromStdString(fileName));
                ui->doubleSpinBox_r->setValue(R0);
                ui->doubleSpinBox_e->setValue(epsilon);
                ui->doubleSpinBox_ax->setValue(alpha[0]);
                ui->doubleSpinBox_ay->setValue(alpha[1]);
                ui->doubleSpinBox_bx->setValue(beta[0]);
                ui->doubleSpinBox_by->setValue(beta[1]);
                ui->spinBox_s->setValue(seed);

                ui->left->hide();
                ui->right->hide();
                ui->label->hide();
                ui->spinBox->hide();

                switch (QType) {
                    case Random:
                        ui->radioButton_rand->setChecked(true);
                        break;
                    case BFS:
                        ui->radioButton_bfs->setChecked(true);
                        break;
                    case A_star:
                        ui->radioButton_astr->setChecked(true);
                        break;
                    default:
                        char error[] = "Q Type not recognized";
                        if (exit)
                            exit(QType, error);
                        else
                            ::exit(QType);
                        break;
                }

                ui->radioButton_infsteps->setChecked(true);
                ui->spinBox_steps->setValue(defaultVariableStepsValue);
            }

            // Output some pace between runs
            *this << "\n\n\n";
            // RUN!!
            run();


            // Output extent of run
            long maxSteps = (!stepRenderType ? varSteps : stepRenderType);
            if (stepRenderType == INFINITY_STEPS || maxSteps >= Box::counter)
                *this << "[ Run to Completion ]\n";
            else
                *this << "[ Ran a Total of " << maxSteps << " Step"
                      << (maxSteps == 1 ? "" : "s") << " ]\n";


            // Update Display
            ui->openGLWidget->genScene();
            ui->openGLWidget->update();
        
        } else {    // Arguments are missing
            *this << "\n\n\n\n\n\n\n"
                     "****************************************************"
                            "************************"
                     "\n\n\n"
                     "WARNING!\n"
                     "Input(s) Not Specified\n";
            for (int i = 0; i < missingArgs.size(); i++)
                *this << "   > " << missingArgs[i] << "\n";
            *this << "\n\n\n"
                     "****************************************************"
                            "************************"
                     "\n\n\n\n";
        }
    }
}

/*
 * CLICK EXIT BUTTON
 *
 * Exits program
 * If exit function is set, that function is called.
 * If no exit function is set, displays warning to user
 * and exits program
 */
void ControlWindow::on_pushButton_exit_clicked()
{
    if (!exit) {
        std::cerr << "\n\n\n"
        "**************************************************\n"
        "WARNING!!\n\n"
        "ControlWindow exit function not specified\n"
        "Program exited unsafely!"
        "**************************************************\n";
        this->close();
    } else
        exit(0, "Exit button pushed.");
}

/*
 * CHECK "Hide Box Boundaries" CHECKBOX
 *
 * Sets 'hideBoxBoundary' to true/false according to
 * if checked/unchecked then calls the screen to be redrawn
 */
void ControlWindow::on_checkBox_bound_clicked(bool checked)
{
    hideBoxBoundary = checked;
    updateDisplay();
}

/*
 * CHECK "1 step" CHECKBOX
 *
 * Disables "steps =" label and adjacent spinbox
 */
void ControlWindow::on_radioButton_1step_clicked(bool checked)
{
    ui->label_stepequals->setDisabled(checked);
    ui->spinBox_steps->setDisabled(checked);
    ui->left->hide();
    ui->right->hide();
    ui->label->hide();
    ui->spinBox->hide();
}

/*
 * CHECK "1000 step" CHECKBOX
 *
 * Disables "steps =" label and adjacent spinbox
 */
void ControlWindow::on_radioButton_1000steps_clicked(bool checked)
{
    ui->label_stepequals->setDisabled(checked);
    ui->spinBox_steps->setDisabled(checked);
    ui->left->hide();
    ui->right->hide();
    ui->label->hide();
    ui->spinBox->hide();
}

/*
 * CHECKED "infinity steps" CHECKBOX
 *
 * Disables "steps =" label and adjacent spinbox
 */
void ControlWindow::on_radioButton_infsteps_clicked(bool checked)
{
    ui->label_stepequals->setDisabled(checked);
    ui->spinBox_steps->setDisabled(checked);
    ui->left->hide();
    ui->right->hide();
    ui->label->hide();
    ui->spinBox->hide();
}

/*
 * CHECK "variable steps" CHECKBOX
 *
 * Enables "steps =" label and adjacent spinbox
 */
void ControlWindow::on_radioButton_varsteps_clicked(bool checked)
{
    ui->label_stepequals->setDisabled(!checked);
    ui->spinBox_steps->setDisabled(!checked);
    ui->left->show();
    ui->right->show();
    ui->label->show();
    ui->spinBox->show();
}

/*
 * UPDATE DISPLAY
 * 
 * Allow other classes to ask for the display to be redrawn
 */
void ControlWindow::updateDisplay() {
    ui->openGLWidget->update();
}

void ControlWindow::on_spinBox_valueChanged(int arg1)
{
    increment=arg1;
}

void ControlWindow::on_radioButton_varsteps_clicked()
{

}

void ControlWindow::on_right_clicked()
{
    ui->spinBox_steps->setValue(varSteps = ui->spinBox_steps->value()+increment);
    stepRenderType = RenderType::VARIABLE_STEPS;
    //run();
    ui->openGLWidget->genScene();
    ui->openGLWidget->update();

}

void ControlWindow::on_left_clicked()
{
    ui->spinBox_steps->setValue(varSteps = ui->spinBox_steps->value()-increment);
    stepRenderType = RenderType::VARIABLE_STEPS;
    //run();
    ui->openGLWidget->genScene();
    ui->openGLWidget->update();
}


void ControlWindow::on_anim_clicked()
{
    runAnim=true;
    ui->openGLWidget->genScene();
    ui->openGLWidget->update();
}

void ControlWindow::on_horizontalSlider_valueChanged(int value)
{
    animationSpeed=value;
}

void ControlWindow::on_checkBox_clicked()
{
    coloredBoxes=!ui->checkBox->isChecked();
    ui->openGLWidget->genScene();
    ui->openGLWidget->update();
}

void ControlWindow::defaultFile(){
}
