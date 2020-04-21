#include "display/MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;

extern string cfgName;
extern char cfgNameList[200][200];
extern string fileName;
extern int numCfg;

extern Point3d start;		// start configuration
extern Point3d goal;		// goal configuration
extern Point3d startRot;
extern Point3d goalRot;

extern double R;
extern double epsilon;		// resolution parameter
extern int seed;
extern int searchType;
extern bool showPath;
extern bool showTrace;
extern bool generateTrace;
extern bool showAnim;
extern bool pauseAnim;
extern bool replayAnim;
extern unsigned int iPathSeg;
extern int transparency;
extern double distanceZ;

extern bool showBox;
extern bool showBoxBoundary;

extern int renderSteps;
extern bool step;
extern int animationSpeed;

extern Point3d Bt;
extern Point3d Br;
extern double Btw;
extern double Brw;
extern bool showRing;
extern Triangle3d W;
extern bool showWall;
extern Segment3d E;
extern bool showEdge;
extern bool showCorner;
extern int traceDense;

extern void run();
extern void parseCfgFile();

int incr = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for(int i=0;i<numCfg;++i){
        ui->comboBox->addItem(cfgNameList[i]);
    }
    for(int i=0;i<numCfg;++i) {
        if (strcmp(cfgNameList[i], cfgName.c_str()) == 0) {
            ui->comboBox->setCurrentIndex(i);
        }
    }

    ui->inputFile->setText(QString::fromStdString(fileName.substr(0,fileName.length()-4)));

    ui->aX->setValue(start.X());
    ui->aY->setValue(start.Y());
    ui->aZ->setValue(start.Z());
    ui->aRoll->setValue(startRot.X());
    ui->aPitch->setValue(startRot.Y());
    ui->aYaw->setValue(startRot.Z());

    ui->bX->setValue(goal.X());
    ui->bY->setValue(goal.Y());
    ui->bZ->setValue(goal.Z());
    ui->bRoll->setValue(goalRot.X());
    ui->bPitch->setValue(goalRot.Y());
    ui->bYaw->setValue(goalRot.Z());

    ui->R->setValue(R);
    ui->eps->setValue(epsilon);
    ui->seed->setValue(seed);
    srand(seed);

    ui->transparency->setValue(transparency);
    ui->zoomSlider->setValue(distanceZ);

    switch (searchType) {
      case RANDOM:
          ui->randomButton->setChecked(true);
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

    ui->showBox->setChecked(false);
    ui->steplabel->hide();
    ui->inc->setEnabled(false);
    ui->left->setEnabled(false);
    ui->right->setEnabled(false);

    ui->horizontalSlider->setValue(animationSpeed);

    ui->Btx->setValue(Bt.X());
    ui->Bty->setValue(Bt.Y());
    ui->Btz->setValue(Bt.Z());
    ui->Btw->setValue(Btw);
    ui->Brx->setValue(Br.X());
    ui->Bry->setValue(Br.Y());
    ui->Brz->setValue(Br.Z());
    ui->Brw->setValue(Brw);
    ui->traceDense->setValue(traceDense);

    ui->T1x->setValue(W.V1().X());
    ui->T1x->setValue(W.V1().Y());
    ui->T1x->setValue(W.V1().Z());
    ui->T2x->setValue(W.V2().X());
    ui->T2x->setValue(W.V2().Y());
    ui->T2x->setValue(W.V2().Z());
    ui->T3x->setValue(W.V3().X());
    ui->T3x->setValue(W.V3().Y());
    ui->T3x->setValue(W.V3().Z());

    // Display widget
    connect(ui->openGLWidget, SIGNAL(zDistanceChanged(int)), ui->zoomSlider, SLOT(setValue(int)));
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

void MainWindow::on_run_clicked() {
    char cfgPre[200], cfgCur[200];
    sprintf(cfgPre, "%s", cfgName.c_str());
    sprintf(cfgCur, "%s", ui->comboBox->currentText().toStdString().c_str());
    if (strcmp(cfgPre, cfgCur) == 0) {
        fileName=ui->inputFile->text().toStdString()+".txt";

        start.set(ui->aX->value(), ui->aY->value(), ui->aZ->value());
        startRot.set(ui->aRoll->value(), ui->aPitch->value(), ui->aYaw->value());

        goal.set(ui->bX->value(), ui->bY->value(), ui->bZ->value());
        goalRot.set(ui->bRoll->value(), ui->bPitch->value(), ui->bYaw->value());

        R=ui->R->value();
        epsilon=ui->eps->value();

        int new_seed = ui->seed->value();
        if(new_seed != seed){
            seed = new_seed;
            srand(seed);
        }

        transparency=ui->transparency->value();
        showBox=ui->showBox->isChecked();
    }
    else{
        //cfgName=ui->egFile->text().toStdString();
        cfgName = ui->comboBox->currentText().toStdString();
        parseCfgFile();

        ui->inputFile->setText(QString::fromStdString(fileName.substr(0,fileName.length()-4)));

        ui->aX->setValue(start.X());
        ui->aY->setValue(start.Y());
        ui->aZ->setValue(start.Z());
        ui->aRoll->setValue(startRot.X());
        ui->aPitch->setValue(startRot.Y());
        ui->aYaw->setValue(startRot.Z());

        ui->bX->setValue(goal.X());
        ui->bY->setValue(goal.Y());
        ui->bZ->setValue(goal.Z());
        ui->bRoll->setValue(goalRot.X());
        ui->bPitch->setValue(goalRot.Y());
        ui->bYaw->setValue(goalRot.Z());

        ui->R->setValue(R);

        switch (searchType) {
          case RANDOM:
              ui->randomButton->setChecked(true);
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

        ui->eps->setValue(epsilon);

        int old_seed = ui->seed->value();
        if(old_seed != seed) {
            ui->seed->setValue(seed);
            srand(seed);
        }

        ui->horizontalSlider->setValue(animationSpeed);

        ui->showBox->setChecked(showBox);
    }

    showAnim = true;
    pauseAnim = false;
    iPathSeg = 0;
    incr = 0;
    generateTrace = false;
    run();

    ui->openGLWidget->update();
}


void MainWindow::on_randomButton_clicked()
{
    searchType=RANDOM;
}

void MainWindow::on_bfs_clicked()
{
    searchType=BFS;
}

void MainWindow::on_greedy_clicked()
{
    searchType=GREEDY;
}

void MainWindow::on_bigreedy_clicked()
{
    searchType=BIGREEDY;
}

void MainWindow::on_size_clicked()
{
    searchType=GREEDY_SIZE;
}

void MainWindow::on_bisize_clicked()
{
    searchType=BIGREEDY_SIZE;
}

void MainWindow::on_vor_clicked()
{
    searchType=VORONOI;
}

void MainWindow::on_bivor_clicked()
{
    searchType=BIVORONOI;
}

void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::on_showTrace_clicked() {
    showTrace = !showTrace;
    traceDense = ui->traceDense->value();
    ui->openGLWidget->update();
}
void MainWindow::on_showPath_clicked() {
    showPath = !showPath;
    ui->openGLWidget->update();
}

void MainWindow::on_showAnim_clicked()
{
    showAnim = true;
    iPathSeg = 0;
    ui->openGLWidget->update();
}
void MainWindow::on_pauseAnim_clicked() {
    pauseAnim = !pauseAnim;
    ui->openGLWidget->update();
}
void MainWindow::on_replayAnim_clicked() {
    showAnim = true;
    pauseAnim = false;
    iPathSeg = 0;
    incr = 0;
    ui->openGLWidget->update();
}

void MainWindow::on_showBox_clicked()
{
    showBox=ui->showBox->isChecked();
    ui->openGLWidget->update();
}

void MainWindow::on_showBoxBoundary_clicked()
{
    showBoxBoundary=ui->showBoxBoundary->isChecked();
    ui->openGLWidget->update();
}

void MainWindow::on_showRing_clicked()
{
  Bt.set(ui->Btx->value(),
         ui->Bty->value(),
         ui->Btz->value());
  Btw = ui->Btw->value();
  Br.set(ui->Brx->value(),
         ui->Bry->value(),
         ui->Brz->value());
  Brw = ui->Brw->value();
  showRing = ui->showRing->isChecked();
  ui->openGLWidget->update();
}

void MainWindow::on_showWall_clicked()
{
  W = Triangle3d(Point3d(ui->T1x->value(), ui->T1y->value(), ui->T1z->value()),
                 Point3d(ui->T2x->value(), ui->T2y->value(), ui->T2z->value()),
                 Point3d(ui->T3x->value(), ui->T3y->value(), ui->T3z->value()));
  showWall = ui->showWall->isChecked();
  ui->openGLWidget->update();
}

void MainWindow::on_showEdge_clicked()
{
  E = Segment3d(Point3d(ui->T1x->value(), ui->T1y->value(), ui->T1z->value()),
                Point3d(ui->T2x->value(), ui->T2y->value(), ui->T2z->value()));
  showEdge = ui->showEdge->isChecked();
  ui->openGLWidget->update();
}

void MainWindow::on_showCorner_clicked()
{
  E = Segment3d(Point3d(ui->T1x->value(), ui->T1y->value(), ui->T1z->value()),
                Point3d(ui->T2x->value(), ui->T2y->value(), ui->T2z->value()));
  showCorner = ui->showCorner->isChecked();
  ui->openGLWidget->update();
}

void MainWindow::on_transparency_valueChanged(int value)
{
    transparency=value;
    ui->openGLWidget->update();
}

void MainWindow::on_pushButton_clicked() {
    if(step) {
        step=false;
        ui->steplabel->hide();
        renderSteps=1;
        incr=1;
        ui->inc->setValue(1);
        ui->inc->setEnabled(false);
        ui->left->setEnabled(false);
        ui->right->setEnabled(false);
        ui->openGLWidget->update();
    } else{
        step=true;
        incr=1;
        ui->steplabel->show();
        ui->steplabel->setText("Step: 1");
        ui->inc->setEnabled(true);
        ui->left->setEnabled(true);
        ui->right->setEnabled(true);
    }
}

void MainWindow::on_inc_valueChanged(int arg1)
{
    incr=arg1;
}

void MainWindow::on_left_clicked()
{
    if(renderSteps-incr<1) return;
    renderSteps-=incr;

    ui->steplabel->setText("Step: "+QString::number(renderSteps));
}

void MainWindow::on_right_clicked()
{

    renderSteps += incr;
    ui->steplabel->setText("Step: "+QString::number(renderSteps));
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    animationSpeed = value;
}

void MainWindow::on_zoomSlider_valueChanged()
{
    distanceZ = ui->zoomSlider->value();
    ui->openGLWidget->update();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(event);
    }
}
