#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;

// Global Variables in disc-qt.cpp
extern QuadTree* QT;

extern char cfgNameList[200][200];
extern int numEg;
extern string cfgName;
extern string fileName;
extern Point2d alpha;		// start configuration
extern Point2d beta;		// goal configuration
extern double epsilon;	    // resolution parameter
extern std::vector<int> expansions;
extern bool doTriangulation;
extern double R0;
extern int seed;
extern int SearchType;
extern int RouteType;
extern bool showAnim;
extern bool pauseAnim;
extern bool replayAnim;
extern bool showTrace;
extern bool finishedAnim;
extern bool showFilledObstacles;
extern int animationSpeed;
extern bool hideBoxBoundary;
extern bool hideBox;
extern int numberForDisplay;
extern unsigned int renderSteps;
extern bool step;
extern double boxWidth;
extern double boxHeight;
extern unsigned int inc;

//extern FILE *fptr; // for debugging
extern double d_mouseX, d_mouseY;

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

    ui->aX->setValue(alpha.X());
    ui->aY->setValue(alpha.Y());

    ui->bX->setValue(beta.X());
    ui->bY->setValue(beta.Y());

    ui->R0->setValue(R0);


    switch(SearchType){
    case PriorityQ::RANDOM:
      ui->rand->setChecked(true);
      break;
    case PriorityQ::BFS:
      ui->bfs->setChecked(true);
      break;
    case PriorityQ::GREEDY:
      ui->greedy->setChecked(true);
      break;
    case PriorityQ::VORONOI:
      ui->vor->setChecked(true);
      break;
    case PriorityQ::BIVORONOI:
      ui->bivor->setChecked(true);
      break;
    default:
      break;
  }

  switch (RouteType) {
    case Route::R_BFS:
      ui->route_bfs->setChecked(true);
      break;
    case Route::R_SAFE:
      ui->route_safe->setChecked(true);
      break;
    case Route::R_ASTAR:
      ui->route_astar->setChecked(true);
      break;
    default:
      break;
  }

  ui->animationSpeed->setValue(animationSpeed);
  ui->eps->setValue(epsilon);
  ui->seed->setValue(seed);
  srand(seed);
  ui->boundary->setChecked(true);
  ui->hidebox->setChecked(true);

  ui->steplabel->hide();
  ui->inc->setEnabled(false);
  ui->left->setEnabled(false);
  ui->right->setEnabled(false);

  doTriangulation = false;
  //triangles.clear();
}

MainWindow::~MainWindow(){
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

void MainWindow::mouseMoveEvent(QMouseEvent *event){
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

  d_mouseX = x;
  d_mouseY = 512-y;
  sprintf(tmp_buff, "mouse click (%.lf, %.lf)\n", d_mouseX, d_mouseY);
  box_info.append(tmp_buff);

  Box* d_record = NULL;
  bool flag = false, flag_free = false, flag_stuck = false;
  double area = boxWidth*boxHeight;
  for(int i=Box::pAllLeaf->size()-1;i>=0;--i){
      Box* tmp = Box::pAllLeaf->at(i);
      if(d_mouseX >= tmp->mB.X()-tmp->width/2  && d_mouseX < tmp->mB.X()+tmp->width/2 &&
         d_mouseY >= tmp->mB.Y()-tmp->height/2 && d_mouseY < tmp->mB.Y()+tmp->height/2){

          if(!flag) {
              d_record = tmp;
              flag = true;
          }

          if(tmp->status == FREE){
              flag_free = true;
              if(area > tmp->width*tmp->height){
                  area = tmp->width*tmp->height;
                  d_record = tmp;
              }
          }
          if(tmp->status == STUCK){
              flag_stuck = true;
          }
      }
  }
  if(d_record != NULL){
    sprintf(tmp_buff, "box center (%.lf, %.lf)\nwidth %.lf height %.lf\n",
            d_record->mB.X(), d_record->mB.Y(), d_record->width, d_record->height);
    box_info.append(tmp_buff);

    if(d_record->status == FREE){
        sprintf(tmp_buff, "box status: FREE\n");
    } else if(d_record->status == STUCK){
        sprintf(tmp_buff, "box status: STUCK\n");
    } else if(d_record->status == MIXED) {
        sprintf(tmp_buff, "box status: MIXED\n");
    } else {
        sprintf(tmp_buff, "box status: UNKNOWN\n");
    }
    box_info.append(tmp_buff);


    sprintf(tmp_buff, "feature numbers\n  corner %d wall %d\n  vor corner %d wall %d  bi %d\n",
            d_record->corners.size(), d_record->walls.size(),
            d_record->corners_v.size(), d_record->walls_v.size(),
            d_record->bidirection);
    box_info.append(tmp_buff);

    if(d_record->d_classify_condition == 0) {
        sprintf(tmp_buff, "corner inner domain\n");
    } else if(d_record->d_classify_condition == 1) {
        sprintf(tmp_buff, "wall inner domain\n");
    } else if(d_record->d_classify_condition == 2) {
        sprintf(tmp_buff, "no feature check child status\n");
    } else if(d_record->d_classify_condition == 3) {
        sprintf(tmp_buff, "1/2 feature: no wall\n");
    } else if(d_record->d_classify_condition == 4) {
        sprintf(tmp_buff, "1/2 feature: no corner dup wall\n");
    } else if(d_record->d_classify_condition == 5) {
        sprintf(tmp_buff, "1/2 feature: no corner\n");
    } else if(d_record->d_classify_condition == 6) {
        sprintf(tmp_buff, "1/2 feature: closer wall dup wall\n");
    } else if(d_record->d_classify_condition == 7) {
        sprintf(tmp_buff, "1/2 feature: closer wall\n");
    } else if(d_record->d_classify_condition == 8) {
        sprintf(tmp_buff, "1/2 feature: closer corner\n");
    } else {
        sprintf(tmp_buff, "not decided\n");
    }
    box_info.append(tmp_buff);
  }
  else{
    sprintf(tmp_buff, "error\n");
    box_info.append(tmp_buff);
  }

  ui->textBrowser->setText(QString::fromStdString(box_info));
}

void MainWindow::on_run_clicked(){
    char egPre[200], egCur[200];
    sprintf(egPre, "%s", cfgName.c_str());
    //sprintf(egCur, "%s", ui->egFile->text().toStdString().c_str());
    sprintf(egCur, "%s", ui->comboBox->currentText().toStdString().c_str());
    if (strcmp(egPre, egCur) == 0) {
        fileName=ui->inputFile->text().toStdString()+".txt";

        alpha.setX(ui->aX->value());
        alpha.setY(ui->aY->value());

        beta.setX(ui->bX->value());
        beta.setY(ui->bY->value());

        R0=ui->R0->value();

        epsilon=ui->eps->value();


        // 4/13/2016
        // only initialize the seed when it is changed
        int new_seed = ui->seed->value();
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

        ui->aX->setValue(alpha.X());
        ui->aY->setValue(alpha.Y());

        ui->bX->setValue(beta.X());
        ui->bY->setValue(beta.Y());

        ui->R0->setValue(R0);

        switch (SearchType) {
          case PriorityQ::RANDOM:
            ui->rand->setChecked(true);
            break;
          case PriorityQ::BFS:
            ui->bfs->setChecked(true);
            break;
          case PriorityQ::GREEDY:
            ui->greedy->setChecked(true);
            break;
          case PriorityQ::VORONOI:
            ui->vor->setChecked(true);
            break;
          case PriorityQ::BIVORONOI:
            ui->bivor->setChecked(true);
            break;
          default:
            break;
        }

        switch (RouteType) {
          case Route::R_BFS:
            ui->route_bfs->setChecked(true);
            break;
          case Route::R_SAFE:
            ui->route_safe->setChecked(true);
            break;
          case Route::R_ASTAR:
            ui->route_astar->setChecked(true);
            break;
          default:
            break;
        }

        ui->eps->setValue(epsilon);
        int old_seed = ui->seed->value();
        if (seed != old_seed) {
            seed = old_seed;
            ui->seed->setValue(seed);
            srand(seed);
        }
    }

    showAnim = true;
    pauseAnim = false;
    inc = 0;
    run();

    doTriangulation = false;
    //triangles.clear();

    ui->openGLWidget->update();
}


void MainWindow::on_rand_clicked() {
  SearchType=0;
}

void MainWindow::on_bfs_clicked() {
  SearchType=1;
}

void MainWindow::on_greedy_clicked() {
  SearchType=2;
}

void MainWindow::on_vor_clicked() {
  SearchType=3;
}

void MainWindow::on_bivor_clicked() {
  SearchType=4;
}

void MainWindow::on_route_bfs_clicked() {
  RouteType=0;
}

void MainWindow::on_route_safe_clicked() {
  RouteType=1;
}

void MainWindow::on_route_astar_clicked() {
  RouteType=2;
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

void MainWindow::on_trace_clicked() {
  showTrace = !showTrace;
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
  if(step){
    step=false;
    ui->steplabel->hide();
    renderSteps=1;
    incr=1;
    ui->inc->setValue(1);
    ui->inc->setEnabled(false);
    ui->left->setEnabled(false);
    ui->right->setEnabled(false);
    this->update();
  }
  else{
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
