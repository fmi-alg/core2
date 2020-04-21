/********************************************************************************
** Form generated from reading UI file 'NewWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWWINDOW_H
#define UI_NEWWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QPushButton *pushButton_3;
    QLabel *label;
    QTextBrowser *textOutput;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *yes;
    QPushButton *no;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_4;
    QLabel *label_3;
    QSpinBox *spinBox;
    QPushButton *ok;
    QLabel *label_4;
    QRadioButton *interior;
    QRadioButton *exterior;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *note;
    QMenuBar *menubar;

    void setupUi(QMainWindow *NewWindow)
    {
        if (NewWindow->objectName().isEmpty())
            NewWindow->setObjectName(QStringLiteral("NewWindow"));
        NewWindow->resize(925, 535);
        NewWindow->setMaximumSize(QSize(925, 535));
        NewWindow->setMouseTracking(true);
        centralwidget = new QWidget(NewWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(700, 20, 211, 501));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lineEdit = new QLineEdit(verticalLayoutWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 1, 0, 1, 1);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 1, 1, 1);

        pushButton_3 = new QPushButton(verticalLayoutWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 2, 0, 1, 1);

        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        textOutput = new QTextBrowser(verticalLayoutWidget);
        textOutput->setObjectName(QStringLiteral("textOutput"));

        verticalLayout->addWidget(textOutput);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        yes = new QPushButton(verticalLayoutWidget);
        yes->setObjectName(QStringLiteral("yes"));

        horizontalLayout_2->addWidget(yes);

        no = new QPushButton(verticalLayoutWidget);
        no->setObjectName(QStringLiteral("no"));

        horizontalLayout_2->addWidget(no);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_2 = new QPushButton(verticalLayoutWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(530, 20, 160, 281));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_4 = new QPushButton(verticalLayoutWidget_2);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        verticalLayout_2->addWidget(pushButton_4);

        label_3 = new QLabel(verticalLayoutWidget_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_2->addWidget(label_3);

        spinBox = new QSpinBox(verticalLayoutWidget_2);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setMinimum(3);
        spinBox->setMaximum(10);
        spinBox->setValue(3);

        verticalLayout_2->addWidget(spinBox);

        ok = new QPushButton(verticalLayoutWidget_2);
        ok->setObjectName(QStringLiteral("ok"));

        verticalLayout_2->addWidget(ok);

        label_4 = new QLabel(verticalLayoutWidget_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_2->addWidget(label_4);

        interior = new QRadioButton(verticalLayoutWidget_2);
        interior->setObjectName(QStringLiteral("interior"));
        interior->setAutoExclusive(false);

        verticalLayout_2->addWidget(interior);

        exterior = new QRadioButton(verticalLayoutWidget_2);
        exterior->setObjectName(QStringLiteral("exterior"));
        exterior->setAutoExclusive(false);

        verticalLayout_2->addWidget(exterior);

        label_5 = new QLabel(verticalLayoutWidget_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_2->addWidget(label_5);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(530, 310, 161, 71));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(540, 310, 151, 71));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(540, 410, 131, 51));
        note = new QLabel(centralwidget);
        note->setObjectName(QStringLiteral("note"));
        note->setGeometry(QRect(540, 430, 151, 71));
        NewWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(NewWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 925, 22));
        NewWindow->setMenuBar(menubar);

        retranslateUi(NewWindow);

        QMetaObject::connectSlotsByName(NewWindow);
    } // setupUi

    void retranslateUi(QMainWindow *NewWindow)
    {
        NewWindow->setWindowTitle(QApplication::translate("NewWindow", "Make Input File", 0));
        label_2->setText(QApplication::translate("NewWindow", "<html><head/><body><p>.txt</p></body></html>", 0));
        pushButton_3->setText(QApplication::translate("NewWindow", "Create", 0));
        label->setText(QApplication::translate("NewWindow", "<html><head/><body><p align=\"center\">Input File Name:</p></body></html>", 0));
        yes->setText(QApplication::translate("NewWindow", "Yes", 0));
        no->setText(QApplication::translate("NewWindow", "No", 0));
        pushButton_2->setText(QApplication::translate("NewWindow", "Restart", 0));
        pushButton->setText(QApplication::translate("NewWindow", "Cancel", 0));
        pushButton_4->setText(QApplication::translate("NewWindow", "Add New Polygon", 0));
        label_3->setText(QApplication::translate("NewWindow", "<html><head/><body><p align=\"center\">How many vertices?</p></body></html>", 0));
        ok->setText(QApplication::translate("NewWindow", "OK", 0));
        label_4->setText(QApplication::translate("NewWindow", "<html><head/><body><p align=\"center\">Obstacle is the</p></body></html>", 0));
        interior->setText(QApplication::translate("NewWindow", "interior", 0));
        exterior->setText(QApplication::translate("NewWindow", "exterior", 0));
        label_5->setText(QApplication::translate("NewWindow", "<html><head/><body><p align=\"center\">of the polygon.</p></body></html>", 0));
        label_6->setText(QApplication::translate("NewWindow", "<html><head/><body><p align=\"center\">For interior obstacle,</p><p align=\"center\">place the vertices in</p><p align=\"center\"><span style=\" text-decoration: underline;\">clockwise</span> order.</p></body></html>", 0));
        label_7->setText(QApplication::translate("NewWindow", "<html><head/><body><p align=\"center\">For exterior obstacle,</p><p align=\"center\">place the vertices in</p><p align=\"center\"><span style=\" text-decoration: underline;\">counter-clockwise</span> order.</p></body></html>", 0));
        label_8->setText(QApplication::translate("NewWindow", "<html><head/><body><p align=\"center\">Click on the screen</p><p align=\"center\">to place vertices.</p></body></html>", 0));
        note->setText(QApplication::translate("NewWindow", "<html><head/><body><p align=\"center\">Note: In this version,</p><p align=\"center\">you may only choose</p><p align=\"center\">exterior once.</p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class NewWindow: public Ui_NewWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWWINDOW_H
