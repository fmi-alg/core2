/********************************************************************************
** Form generated from reading UI file 'ControlWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLWINDOW_H
#define UI_CONTROLWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include "Display.h"

QT_BEGIN_NAMESPACE

class Ui_ControlWindow
{
public:
    QWidget *centralWidget;
    QTextBrowser *textOutput;
    Display *openGLWidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_globals;
    QLabel *label_r;
    QDoubleSpinBox *doubleSpinBox_r;
    QLabel *label_e;
    QDoubleSpinBox *doubleSpinBox_e;
    QLabel *label_ax;
    QDoubleSpinBox *doubleSpinBox_ax;
    QLabel *label_ay;
    QDoubleSpinBox *doubleSpinBox_ay;
    QLabel *label_bx;
    QDoubleSpinBox *doubleSpinBox_bx;
    QLabel *label_by;
    QDoubleSpinBox *doubleSpinBox_by;
    QLabel *label_s;
    QSpinBox *spinBox_s;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_input;
    QLabel *label_fn;
    QLabel *label_id;
    QLineEdit *lineEdit_id;
    QLineEdit *lineEdit_fn;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout_qtype;
    QRadioButton *radioButton_astr;
    QRadioButton *radioButton_bfs;
    QRadioButton *radioButton_rand;
    QWidget *layoutWidget3;
    QGridLayout *gridLayout_bound;
    QCheckBox *checkBox_bound;
    QLabel *label_boundaries;
    QFrame *line_3;
    QFrame *line_6;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_4;
    QFrame *line_5;
    QWidget *layoutWidget4;
    QGridLayout *gridLayout;
    QRadioButton *radioButton_1step;
    QLabel *label_stepequals;
    QSpinBox *spinBox_steps;
    QRadioButton *radioButton_infsteps;
    QRadioButton *radioButton_varsteps;
    QRadioButton *radioButton_1000steps;
    QLabel *label_render;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *left;
    QPushButton *right;
    QLabel *label;
    QSpinBox *spinBox;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *anim;
    QLabel *label_2;
    QSlider *horizontalSlider;
    QCheckBox *checkBox;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_run;
    QPushButton *pushButton;
    QPushButton *pushButton_exit;
    QButtonGroup *buttonGroup_render;
    QButtonGroup *buttonGroup_qtype;

    void setupUi(QMainWindow *ControlWindow)
    {
        if (ControlWindow->objectName().isEmpty())
            ControlWindow->setObjectName(QStringLiteral("ControlWindow"));
        ControlWindow->resize(1040, 701);
        centralWidget = new QWidget(ControlWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        textOutput = new QTextBrowser(centralWidget);
        textOutput->setObjectName(QStringLiteral("textOutput"));
        textOutput->setGeometry(QRect(620, 290, 411, 401));
        openGLWidget = new Display(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(10, 10, 600, 600));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(890, 10, 143, 230));
        gridLayout_globals = new QGridLayout(layoutWidget);
        gridLayout_globals->setSpacing(6);
        gridLayout_globals->setContentsMargins(11, 11, 11, 11);
        gridLayout_globals->setObjectName(QStringLiteral("gridLayout_globals"));
        gridLayout_globals->setContentsMargins(0, 0, 0, 0);
        label_r = new QLabel(layoutWidget);
        label_r->setObjectName(QStringLiteral("label_r"));
        label_r->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_globals->addWidget(label_r, 0, 0, 1, 1);

        doubleSpinBox_r = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_r->setObjectName(QStringLiteral("doubleSpinBox_r"));
        doubleSpinBox_r->setDecimals(1);
        doubleSpinBox_r->setMaximum(1000);

        gridLayout_globals->addWidget(doubleSpinBox_r, 0, 1, 1, 1);

        label_e = new QLabel(layoutWidget);
        label_e->setObjectName(QStringLiteral("label_e"));
        label_e->setAlignment(Qt::AlignCenter);

        gridLayout_globals->addWidget(label_e, 1, 0, 1, 1);

        doubleSpinBox_e = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_e->setObjectName(QStringLiteral("doubleSpinBox_e"));
        doubleSpinBox_e->setDecimals(1);
        doubleSpinBox_e->setMaximum(1000);

        gridLayout_globals->addWidget(doubleSpinBox_e, 1, 1, 1, 1);

        label_ax = new QLabel(layoutWidget);
        label_ax->setObjectName(QStringLiteral("label_ax"));
        label_ax->setAlignment(Qt::AlignCenter);

        gridLayout_globals->addWidget(label_ax, 2, 0, 1, 1);

        doubleSpinBox_ax = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_ax->setObjectName(QStringLiteral("doubleSpinBox_ax"));
        doubleSpinBox_ax->setDecimals(1);
        doubleSpinBox_ax->setMaximum(1000);

        gridLayout_globals->addWidget(doubleSpinBox_ax, 2, 1, 1, 1);

        label_ay = new QLabel(layoutWidget);
        label_ay->setObjectName(QStringLiteral("label_ay"));
        label_ay->setAlignment(Qt::AlignCenter);

        gridLayout_globals->addWidget(label_ay, 3, 0, 1, 1);

        doubleSpinBox_ay = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_ay->setObjectName(QStringLiteral("doubleSpinBox_ay"));
        doubleSpinBox_ay->setDecimals(1);
        doubleSpinBox_ay->setMaximum(1000);

        gridLayout_globals->addWidget(doubleSpinBox_ay, 3, 1, 1, 1);

        label_bx = new QLabel(layoutWidget);
        label_bx->setObjectName(QStringLiteral("label_bx"));
        label_bx->setAlignment(Qt::AlignCenter);

        gridLayout_globals->addWidget(label_bx, 4, 0, 1, 1);

        doubleSpinBox_bx = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_bx->setObjectName(QStringLiteral("doubleSpinBox_bx"));
        doubleSpinBox_bx->setDecimals(1);
        doubleSpinBox_bx->setMaximum(1000);

        gridLayout_globals->addWidget(doubleSpinBox_bx, 4, 1, 1, 1);

        label_by = new QLabel(layoutWidget);
        label_by->setObjectName(QStringLiteral("label_by"));
        label_by->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_globals->addWidget(label_by, 5, 0, 1, 1);

        doubleSpinBox_by = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_by->setObjectName(QStringLiteral("doubleSpinBox_by"));
        doubleSpinBox_by->setDecimals(1);
        doubleSpinBox_by->setMaximum(1000);

        gridLayout_globals->addWidget(doubleSpinBox_by, 5, 1, 1, 1);

        label_s = new QLabel(layoutWidget);
        label_s->setObjectName(QStringLiteral("label_s"));
        label_s->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_globals->addWidget(label_s, 6, 0, 1, 1);

        spinBox_s = new QSpinBox(layoutWidget);
        spinBox_s->setObjectName(QStringLiteral("spinBox_s"));
        spinBox_s->setMinimum(-100000);
        spinBox_s->setMaximum(100000);

        gridLayout_globals->addWidget(spinBox_s, 6, 1, 1, 1);

        gridLayout_globals->setColumnStretch(1, 100);
        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(630, 10, 241, 61));
        gridLayout_input = new QGridLayout(layoutWidget1);
        gridLayout_input->setSpacing(6);
        gridLayout_input->setContentsMargins(11, 11, 11, 11);
        gridLayout_input->setObjectName(QStringLiteral("gridLayout_input"));
        gridLayout_input->setContentsMargins(0, 0, 0, 0);
        label_fn = new QLabel(layoutWidget1);
        label_fn->setObjectName(QStringLiteral("label_fn"));
        label_fn->setAlignment(Qt::AlignCenter);

        gridLayout_input->addWidget(label_fn, 0, 0, 1, 1);

        label_id = new QLabel(layoutWidget1);
        label_id->setObjectName(QStringLiteral("label_id"));
        label_id->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_input->addWidget(label_id, 1, 0, 1, 2);

        lineEdit_id = new QLineEdit(layoutWidget1);
        lineEdit_id->setObjectName(QStringLiteral("lineEdit_id"));

        gridLayout_input->addWidget(lineEdit_id, 1, 2, 1, 1);

        lineEdit_fn = new QLineEdit(layoutWidget1);
        lineEdit_fn->setObjectName(QStringLiteral("lineEdit_fn"));

        gridLayout_input->addWidget(lineEdit_fn, 0, 1, 1, 2);

        gridLayout_input->setColumnStretch(2, 100);
        layoutWidget2 = new QWidget(centralWidget);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(790, 90, 81, 91));
        gridLayout_qtype = new QGridLayout(layoutWidget2);
        gridLayout_qtype->setSpacing(-1);
        gridLayout_qtype->setContentsMargins(11, 11, 11, 11);
        gridLayout_qtype->setObjectName(QStringLiteral("gridLayout_qtype"));
        gridLayout_qtype->setContentsMargins(0, 0, 0, 0);
        radioButton_astr = new QRadioButton(layoutWidget2);
        buttonGroup_qtype = new QButtonGroup(ControlWindow);
        buttonGroup_qtype->setObjectName(QStringLiteral("buttonGroup_qtype"));
        buttonGroup_qtype->addButton(radioButton_astr);
        radioButton_astr->setObjectName(QStringLiteral("radioButton_astr"));

        gridLayout_qtype->addWidget(radioButton_astr, 2, 0, 1, 1, Qt::AlignLeft|Qt::AlignVCenter);

        radioButton_bfs = new QRadioButton(layoutWidget2);
        buttonGroup_qtype->addButton(radioButton_bfs);
        radioButton_bfs->setObjectName(QStringLiteral("radioButton_bfs"));

        gridLayout_qtype->addWidget(radioButton_bfs, 1, 0, 1, 1, Qt::AlignLeft|Qt::AlignVCenter);

        radioButton_rand = new QRadioButton(layoutWidget2);
        buttonGroup_qtype->addButton(radioButton_rand);
        radioButton_rand->setObjectName(QStringLiteral("radioButton_rand"));

        gridLayout_qtype->addWidget(radioButton_rand, 0, 0, 1, 1, Qt::AlignLeft|Qt::AlignVCenter);

        layoutWidget3 = new QWidget(centralWidget);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(790, 200, 91, 41));
        gridLayout_bound = new QGridLayout(layoutWidget3);
        gridLayout_bound->setSpacing(6);
        gridLayout_bound->setContentsMargins(11, 11, 11, 11);
        gridLayout_bound->setObjectName(QStringLiteral("gridLayout_bound"));
        gridLayout_bound->setHorizontalSpacing(-1);
        gridLayout_bound->setVerticalSpacing(0);
        gridLayout_bound->setContentsMargins(0, 0, 0, 0);
        checkBox_bound = new QCheckBox(layoutWidget3);
        checkBox_bound->setObjectName(QStringLiteral("checkBox_bound"));

        gridLayout_bound->addWidget(checkBox_bound, 0, 0, 1, 1);

        label_boundaries = new QLabel(layoutWidget3);
        label_boundaries->setObjectName(QStringLiteral("label_boundaries"));
        label_boundaries->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_bound->addWidget(label_boundaries, 1, 0, 1, 1);

        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(621, 240, 412, 20));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line_6 = new QFrame(centralWidget);
        line_6->setObjectName(QStringLiteral("line_6"));
        line_6->setGeometry(QRect(612, 10, 20, 331));
        line_6->setFrameShape(QFrame::VLine);
        line_6->setFrameShadow(QFrame::Sunken);
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(870, 9, 20, 241));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(779, 180, 100, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_4 = new QFrame(centralWidget);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setGeometry(QRect(770, 79, 20, 171));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);
        line_5 = new QFrame(centralWidget);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setGeometry(QRect(621, 70, 258, 20));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);
        layoutWidget4 = new QWidget(centralWidget);
        layoutWidget4->setObjectName(QStringLiteral("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(630, 90, 141, 151));
        gridLayout = new QGridLayout(layoutWidget4);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(10);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        radioButton_1step = new QRadioButton(layoutWidget4);
        buttonGroup_render = new QButtonGroup(ControlWindow);
        buttonGroup_render->setObjectName(QStringLiteral("buttonGroup_render"));
        buttonGroup_render->addButton(radioButton_1step);
        radioButton_1step->setObjectName(QStringLiteral("radioButton_1step"));

        gridLayout->addWidget(radioButton_1step, 1, 0, 1, 2);

        label_stepequals = new QLabel(layoutWidget4);
        label_stepequals->setObjectName(QStringLiteral("label_stepequals"));
        label_stepequals->setEnabled(false);
        label_stepequals->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_stepequals, 5, 0, 1, 1);

        spinBox_steps = new QSpinBox(layoutWidget4);
        spinBox_steps->setObjectName(QStringLiteral("spinBox_steps"));
        spinBox_steps->setEnabled(false);
        spinBox_steps->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        spinBox_steps->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        spinBox_steps->setMinimum(1);
        spinBox_steps->setMaximum(100000);
        spinBox_steps->setValue(1);

        gridLayout->addWidget(spinBox_steps, 5, 1, 1, 1);

        radioButton_infsteps = new QRadioButton(layoutWidget4);
        buttonGroup_render->addButton(radioButton_infsteps);
        radioButton_infsteps->setObjectName(QStringLiteral("radioButton_infsteps"));

        gridLayout->addWidget(radioButton_infsteps, 3, 0, 1, 2);

        radioButton_varsteps = new QRadioButton(layoutWidget4);
        buttonGroup_render->addButton(radioButton_varsteps);
        radioButton_varsteps->setObjectName(QStringLiteral("radioButton_varsteps"));

        gridLayout->addWidget(radioButton_varsteps, 4, 0, 1, 2);

        radioButton_1000steps = new QRadioButton(layoutWidget4);
        buttonGroup_render->addButton(radioButton_1000steps);
        radioButton_1000steps->setObjectName(QStringLiteral("radioButton_1000steps"));

        gridLayout->addWidget(radioButton_1000steps, 2, 0, 1, 2);

        label_render = new QLabel(layoutWidget4);
        label_render->setObjectName(QStringLiteral("label_render"));
        label_render->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_render, 0, 0, 1, 2);

        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(640, 250, 371, 36));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        left = new QPushButton(horizontalLayoutWidget);
        left->setObjectName(QStringLiteral("left"));
        left->setMaximumSize(QSize(75, 16777215));

        horizontalLayout_3->addWidget(left);

        right = new QPushButton(horizontalLayoutWidget);
        right->setObjectName(QStringLiteral("right"));
        right->setMaximumSize(QSize(75, 16777215));

        horizontalLayout_3->addWidget(right);

        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        spinBox = new QSpinBox(horizontalLayoutWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setMinimum(1);
        spinBox->setMaximum(1000);

        horizontalLayout_3->addWidget(spinBox);

        horizontalLayoutWidget_2 = new QWidget(centralWidget);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(30, 620, 531, 34));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        anim = new QPushButton(horizontalLayoutWidget_2);
        anim->setObjectName(QStringLiteral("anim"));

        horizontalLayout->addWidget(anim);

        label_2 = new QLabel(horizontalLayoutWidget_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        horizontalSlider = new QSlider(horizontalLayoutWidget_2);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setValue(99);
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider);

        checkBox = new QCheckBox(horizontalLayoutWidget_2);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        horizontalLayout->addWidget(checkBox);

        horizontalLayoutWidget_3 = new QWidget(centralWidget);
        horizontalLayoutWidget_3->setObjectName(QStringLiteral("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(30, 660, 531, 32));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        pushButton_run = new QPushButton(horizontalLayoutWidget_3);
        pushButton_run->setObjectName(QStringLiteral("pushButton_run"));
        pushButton_run->setMinimumSize(QSize(200, 0));
        pushButton_run->setMaximumSize(QSize(125, 16777215));

        horizontalLayout_4->addWidget(pushButton_run);

        pushButton = new QPushButton(horizontalLayoutWidget_3);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_4->addWidget(pushButton);

        pushButton_exit = new QPushButton(horizontalLayoutWidget_3);
        pushButton_exit->setObjectName(QStringLiteral("pushButton_exit"));

        horizontalLayout_4->addWidget(pushButton_exit);

        ControlWindow->setCentralWidget(centralWidget);
        line_6->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        textOutput->raise();
        openGLWidget->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        line_3->raise();
        line->raise();
        line_2->raise();
        line_4->raise();
        line_5->raise();
        layoutWidget->raise();
        horizontalLayoutWidget->raise();
        horizontalLayoutWidget_2->raise();
        horizontalLayoutWidget_3->raise();
#ifndef QT_NO_SHORTCUT
        label_r->setBuddy(doubleSpinBox_r);
        label_e->setBuddy(doubleSpinBox_e);
        label_ax->setBuddy(doubleSpinBox_ax);
        label_ay->setBuddy(doubleSpinBox_ay);
        label_bx->setBuddy(doubleSpinBox_bx);
        label_by->setBuddy(doubleSpinBox_by);
        label_s->setBuddy(spinBox_s);
        label_fn->setBuddy(lineEdit_fn);
        label_id->setBuddy(lineEdit_id);
        label_boundaries->setBuddy(checkBox_bound);
        label_stepequals->setBuddy(spinBox_steps);
#endif // QT_NO_SHORTCUT

        retranslateUi(ControlWindow);

        QMetaObject::connectSlotsByName(ControlWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ControlWindow)
    {
        ControlWindow->setWindowTitle(QApplication::translate("ControlWindow", "Motion Planning", 0));
        label_r->setText(QApplication::translate("ControlWindow", "Radius:", 0));
        label_e->setText(QApplication::translate("ControlWindow", "Epsilon:", 0));
        label_ax->setText(QApplication::translate("ControlWindow", "alpha.x:", 0));
        label_ay->setText(QApplication::translate("ControlWindow", "alpha.y:", 0));
        label_bx->setText(QApplication::translate("ControlWindow", "beta.x:", 0));
        label_by->setText(QApplication::translate("ControlWindow", "beta.y:", 0));
        label_s->setText(QApplication::translate("ControlWindow", "seed:", 0));
        label_fn->setText(QApplication::translate("ControlWindow", "Input file:", 0));
        label_id->setText(QApplication::translate("ControlWindow", "Input Directory:", 0));
        radioButton_astr->setText(QApplication::translate("ControlWindow", "A-star", 0));
        radioButton_bfs->setText(QApplication::translate("ControlWindow", "BFS", 0));
        radioButton_rand->setText(QApplication::translate("ControlWindow", "Random", 0));
        checkBox_bound->setText(QApplication::translate("ControlWindow", "Hide Box", 0));
        label_boundaries->setText(QApplication::translate("ControlWindow", "Boundaries", 0));
        radioButton_1step->setText(QApplication::translate("ControlWindow", "1 step", 0));
        label_stepequals->setText(QApplication::translate("ControlWindow", "step =", 0));
        radioButton_infsteps->setText(QApplication::translate("ControlWindow", "infinity steps", 0));
        radioButton_varsteps->setText(QApplication::translate("ControlWindow", "variable steps", 0));
        radioButton_1000steps->setText(QApplication::translate("ControlWindow", "1000 steps", 0));
        label_render->setText(QApplication::translate("ControlWindow", "Render Options", 0));
        left->setText(QApplication::translate("ControlWindow", "<<", 0));
        right->setText(QApplication::translate("ControlWindow", ">>", 0));
        label->setText(QApplication::translate("ControlWindow", "<html><head/><body><p>Step Increment Size:</p></body></html>", 0));
        anim->setText(QApplication::translate("ControlWindow", "Animation", 0));
        label_2->setText(QApplication::translate("ControlWindow", "<html><head/><body><p align=\"right\">Animation Speed:</p></body></html>", 0));
        checkBox->setText(QApplication::translate("ControlWindow", "Hide Boxes", 0));
        pushButton_run->setText(QApplication::translate("ControlWindow", "Run Me", 0));
        pushButton->setText(QApplication::translate("ControlWindow", "Make Input File", 0));
        pushButton_exit->setText(QApplication::translate("ControlWindow", "Exit", 0));
    } // retranslateUi

};

namespace Ui {
    class ControlWindow: public Ui_ControlWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLWINDOW_H
