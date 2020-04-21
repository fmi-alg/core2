#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;

extern char cfgNameList[200][200];
extern int numEg;
extern string cfgName;
extern string fileName;
extern double alpha[3];		// start configuration
extern double beta[3];		// goal configuration
extern double epsilon;	    // resolution parameter
extern std::vector<int> expansions;
extern bool doTriangulation;
extern vector<Triangle> triangles;
extern double triRobo[2];
extern double R0;
extern int seed;
extern int QType;
extern bool showAnim;
extern bool pauseAnim;
extern bool replayAnim;
extern bool finishedAnim;
extern bool showFilledObstacles;
extern int animationSpeed;
extern bool hideBoxBoundary;
extern bool hideBox;
extern int numberForDisplay;
extern unsigned int renderSteps;
extern bool step;
extern double mouseX, mouseY;
extern double boxWidth;
extern double boxHeight;
extern unsigned int inc;

//extern FILE *fptr; // for debugging

extern void run();
extern void parseExampleFile();


int incr(1);


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for(int i=0;i<numEg;++i) {
        ui->comboBox->addItem(cfgNameList[i]);
    }
    for(int i=0;i<numEg;++i) {
        if (strcmp(cfgNameList[i], cfgName.c_str()) == 0) {
            ui->comboBox->setCurrentIndex(i);
        }
    }

    //ui->egFile->setText(QString::fromStdString(cfgName));
    ui->inputFile->setText(QString::fromStdString(fileName.substr(0,fileName.length()-4)));

    ui->aX->setValue(alpha[0]);
    ui->aY->setValue(alpha[1]);
    ui->aT1->setValue(alpha[2]);

    ui->bX->setValue(beta[0]);
    ui->bY->setValue(beta[1]);
    ui->bT1->setValue(beta[2]);

    ui->l1->setValue(triRobo[0]*180);
    ui->l2->setValue(triRobo[1]*180);
    ui->l3->setValue(R0);


    switch (QType) {
        case 0:
            ui->randomButton->setChecked(true);
            break;
        case 1:
            ui->bfs->setChecked(true);
            break;
        case 2:
            ui->greedy->setChecked(true);
            break;
        case 3:
            ui->dist->setChecked(true);
            break;
        case 4:
            ui->vor->setChecked(true);
            break;
    }

    ui->animationSpeed->setValue(animationSpeed);
    ui->eps->setValue(epsilon);
    ui->random->setValue(seed);
    srand(seed);
    ui->boundary->setChecked(true);
    ui->hidebox->setChecked(true);

    ui->steplabel->hide();
    ui->inc->setEnabled(false);
    ui->left->setEnabled(false);
    ui->right->setEnabled(false);

    doTriangulation = false;
    triangles.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}



//================================//
//     Display Text to Window     //
//================================//
/* Scrolls the text screen to the bottom and prints text */
MainWindow& MainWindow::operator<< (const std::string& str) {
    ui->textOutput->moveCursor (QTextCursor::End);
    ui->textOutput->insertPlainText(str.c_str());
    ui->textOutput->moveCursor (QTextCursor::End);
    return *this;
}

MainWindow& MainWindow::operator<< (const char* text) {
    ui->textOutput->moveCursor (QTextCursor::End);
    ui->textOutput->insertPlainText(text);
    ui->textOutput->moveCursor (QTextCursor::End);
    return *this;
}

MainWindow& MainWindow::operator<< (int i) {
    ui->textOutput->moveCursor (QTextCursor::End);
    ui->textOutput->insertPlainText(QString::number(i));
    ui->textOutput->moveCursor (QTextCursor::End);
    return *this;
}

MainWindow& MainWindow::operator<< (long l) {
    ui->textOutput->moveCursor (QTextCursor::End);
    ui->textOutput->insertPlainText(QString::number(l));
    ui->textOutput->moveCursor (QTextCursor::End);
    return *this;
}

MainWindow& MainWindow::operator<< (float f) {
    ui->textOutput->moveCursor (QTextCursor::End);
    ui->textOutput->insertPlainText(QString::number(f));
    ui->textOutput->moveCursor (QTextCursor::End);
    return *this;
}

MainWindow& MainWindow::operator<< (double d) {
    ui->textOutput->moveCursor (QTextCursor::End);
    ui->textOutput->insertPlainText(QString::number(d));
    ui->textOutput->moveCursor (QTextCursor::End);
    return *this;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x()-90;
    int y = event->y()-30;
    if(x<0||y<0||x>512||y>512) return;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    char tmp_buff[200];
    string box_info;

    int x = event->x()-90;
    int y = event->y()-30;
    if(x<0||y<0||x>512||y>512) return;

    mouseX = x;
    mouseY = 512-y;
    sprintf(tmp_buff, "mouse (%.lf, %.lf)\n", mouseX, mouseY);
    box_info.append(tmp_buff);

    Box* record;
    bool flag = false, flag_free = false, flag_stuck = false;
    double area = boxWidth*boxHeight;
    for(int i=Box::pAllLeaf->size()-1;i>=0;--i){
        Box* tmp = Box::pAllLeaf->at(i);
        if(mouseX >= tmp->x-tmp->width/2 && mouseX < tmp->x+tmp->width/2 &&
           mouseY >= tmp->y-tmp->height/2 && mouseY < tmp->y+tmp->height/2){

            if(!flag) {
                record = tmp;
                flag = true;
            }

            if(tmp->status == Box::FREE){
                flag_free = true;
                if(area > tmp->width*tmp->height){
                    area = tmp->width*tmp->height;
                    record = tmp;
                }
            }
            if(tmp->status == Box::STUCK){
                flag_stuck = true;
            }
        }
    }
    sprintf(tmp_buff, "box (%.lf, %.lf) width %.lf height %.lf\nangle bound %lf %lf\n", record->x, record->y, record->width, record->height, record->xi[0]*180.0f, record->xi[1]*180.0f);
    box_info.append(tmp_buff);

    if(flag_free && flag_stuck) {
        sprintf(tmp_buff, "box status: FREE & STUCK\n");
    } else if(flag_free && !flag_stuck) {
        sprintf(tmp_buff, "box status: FREE\n");
    } else if(!flag_free && flag_stuck) {
        sprintf(tmp_buff, "box status: STUCK\n");
    } else if(record->status == Box::MIXED) {
        sprintf(tmp_buff, "box status: MIXED\n");
    } else {
        sprintf(tmp_buff, "box status: UNKNOWN\n");
    }
    box_info.append(tmp_buff);

    if(record->classify_condition == 1) {
        sprintf(tmp_buff, "1st phase: no parent\n");
    } else if(record->classify_condition == 2) {
        sprintf(tmp_buff, "1st phase: check parents' feature set\n");
    } else if(record->classify_condition == 3) {
        sprintf(tmp_buff, "2nd phase: corner is in triangle\n");
    } else if(record->classify_condition == 4) {
        sprintf(tmp_buff, "2nd phase: wall's end point is in triangle\n");
    } else if(record->classify_condition == 5) {
        sprintf(tmp_buff, "2nd phase: wall intersects triangle\n");
    } else if(record->classify_condition == 6) {
        sprintf(tmp_buff, "2nd phase: no parent\n");
    } else if(record->classify_condition == 7) {
        sprintf(tmp_buff, "2nd phase: check parents' feature set\n");
    } else {
        sprintf(tmp_buff, "not decided\n");
    }
    box_info.append(tmp_buff);

    ui->textBrowser->setText(QString::fromStdString(box_info));
}

void MainWindow::on_run_clicked()
{
    char egPre[200], egCur[200];
    sprintf(egPre, "%s", cfgName.c_str());
    //sprintf(egCur, "%s", ui->egFile->text().toStdString().c_str());
    sprintf(egCur, "%s", ui->comboBox->currentText().toStdString().c_str());
    if (strcmp(egPre, egCur) == 0) {
        fileName=ui->inputFile->text().toStdString()+".txt";

        alpha[0]=ui->aX->value();
        alpha[1]=ui->aY->value();
        alpha[2]=ui->aT1->value();

        beta[0]=ui->bX->value();
        beta[1]=ui->bY->value();
        beta[2]=ui->bT1->value();

        triRobo[0]=ui->l1->value()/180.0f;
        triRobo[1]=ui->l2->value()/180.0f;
        R0=ui->l3->value();

        epsilon=ui->eps->value();


        // 4/13/2016
        // only initialize the seed when it is changed
        int new_seed = ui->random->value();
        if (seed != new_seed) {
            seed = new_seed;
            srand(seed);
        }

        hideBoxBoundary=ui->boundary->isChecked();
    } else {
        //cfgName=ui->egFile->text().toStdString();
        cfgName = ui->comboBox->currentText().toStdString();
        parseExampleFile();

        ui->inputFile->setText(QString::fromStdString(fileName.substr(0,fileName.length()-4)));

        ui->aX->setValue(alpha[0]);
        ui->aY->setValue(alpha[1]);
        ui->aT1->setValue(alpha[2]);

        ui->bX->setValue(beta[0]);
        ui->bY->setValue(beta[1]);
        ui->bT1->setValue(beta[2]);

        ui->l1->setValue(triRobo[0]*180);
        ui->l2->setValue(triRobo[1]*180);
        ui->l3->setValue(R0);

        switch (QType) {
            case 0:
                ui->randomButton->setChecked(true);
                break;
            case 1:
                ui->bfs->setChecked(true);
                break;
            case 2:
                ui->greedy->setChecked(true);
                break;
            case 3:
                ui->dist->setChecked(true);
                break;
            case 4:
                ui->vor->setChecked(true);
                break;
        }

        ui->eps->setValue(epsilon);
        int old_seed = ui->random->value();
        if (seed != old_seed) {
            seed = old_seed;
            ui->random->setValue(seed);
            srand(seed);
        }
    }

    showAnim = true;
    pauseAnim = false;
    inc = 0;
    run();

    doTriangulation = false;
    triangles.clear();

    ui->openGLWidget->update();
    ui->openGLWidget_2->update();
}


void MainWindow::on_randomButton_clicked() {
    QType=0;
}

void MainWindow::on_bfs_clicked() {
    QType=1;
}

void MainWindow::on_greedy_clicked() {
    QType=2;
}

void MainWindow::on_dist_clicked() {
    QType=3;
}

void MainWindow::on_vor_clicked() {
    QType=4;
}

void MainWindow::on_exit_clicked() {
    this->close();
}

void MainWindow::on_show_clicked() {
    showAnim = true;
    this->update();
}

void MainWindow::on_pause_clicked() {
    pauseAnim = !pauseAnim;
    this->update();
}

void MainWindow::on_replay_clicked() {
    replayAnim = true;
    this->update();
}

void MainWindow::on_showFilledObstacles_clicked() {
    showFilledObstacles = !showFilledObstacles;
    this->update();
}

void MainWindow::on_boundary_clicked() {
    hideBoxBoundary=ui->boundary->isChecked();
    this->update();
}

void MainWindow::on_hidebox_clicked() {
    hideBox=ui->hidebox->isChecked();
    this->update();
}

void MainWindow::on_pushButton_clicked() {
    if (step) {
        step=false;
        ui->steplabel->hide();
        renderSteps=1;
        incr=1;
        ui->inc->setValue(1);
        ui->inc->setEnabled(false);
        ui->left->setEnabled(false);
        ui->right->setEnabled(false);
        this->update();
    } else {
        step=true;
        incr=1;
        ui->steplabel->show();
        ui->steplabel->setText("Step: 1");
        ui->inc->setEnabled(true);
        ui->left->setEnabled(true);
        ui->right->setEnabled(true);
    }
}

void MainWindow::on_inc_valueChanged(int arg1) {
    incr=arg1;
}

void MainWindow::on_left_clicked() {
    if(renderSteps-incr<1) return;
    renderSteps-=incr;

    ui->steplabel->setText("Step: "+QString::number(renderSteps));

    ui->openGLWidget->update();
}

void MainWindow::on_right_clicked() {
    renderSteps+=incr;
    if(renderSteps>expansions.size()-1) renderSteps=expansions.size()-1;
    ui->steplabel->setText("Step: "+QString::number(renderSteps));

    ui->openGLWidget->update();
}

void MainWindow::on_animationSpeed_valueChanged(int value) {
    animationSpeed = value;
}
