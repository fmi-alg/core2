#include "MainWindow.h"
#include "ui_MainWindow.h"
using namespace std;

extern char cfgNameList[200][200];
extern int numEg;
extern string cfgName;
extern string fileName;


extern double alpha[4];		// start configuration
extern double beta[4];		// goal configuration
extern double epsilon;			// resolution parameter


extern std::vector<int> expansions;


extern double L1;
extern double L2;
extern double R0;
extern int seed;
extern int QType;

extern int inc;
extern bool runAnim;
extern bool replayAnim;
extern bool pauseAnim;

extern bool hideBox;
extern bool hideBoxBoundary;
extern int numberForDisplay;


extern int renderSteps;
extern bool step;
int incr(1);



int animationSpeed(90);
int animationSpeedScale(5000);
int animationSpeedScaleBox(100);

extern void run();
extern void parseExampleFile();


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for(int i=0;i<numEg;++i){
        ui->comboBox->addItem(cfgNameList[i]);
    }

    ui->inputFile->setText(QString::fromStdString(fileName.substr(0,fileName.length()-4)));

    ui->aX->setValue(alpha[0]);
    ui->aY->setValue(alpha[1]);
    ui->aT1->setValue(alpha[2]);
    ui->aT2->setValue(alpha[3]);

    ui->bX->setValue(beta[0]);
    ui->bY->setValue(beta[1]);
    ui->bT1->setValue(beta[2]);
    ui->bT2->setValue(beta[3]);

    ui->l1->setValue(L1);
    ui->l2->setValue(L2);

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
    ui->random->setValue(seed);
    srand(seed);

    ui->hideBox->setChecked(hideBox);
    ui->hideBoxBoundary->setChecked(hideBoxBoundary);

    ui->steplabel->hide();
    ui->inc->setEnabled(false);
    ui->left->setEnabled(false);
    ui->right->setEnabled(false);
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
        alpha[3]=ui->aT2->value();

        beta[0]=ui->bX->value();
        beta[1]=ui->bY->value();
        beta[2]=ui->bT1->value();
        beta[3]=ui->bT2->value();

        L1=ui->l1->value();
        L2=ui->l2->value();

        epsilon=ui->eps->value();

        int new_seed = ui->random->value();
        if(new_seed != seed){
            seed = new_seed;
            srand(seed);
        }

        hideBox=ui->hideBox->isChecked();
        hideBoxBoundary=ui->hideBoxBoundary->isChecked();
    } else {
        //cfgName=ui->egFile->text().toStdString();
        cfgName = ui->comboBox->currentText().toStdString();
        parseExampleFile();

        ui->inputFile->setText(QString::fromStdString(fileName.substr(0,fileName.length()-4)));

        ui->aX->setValue(alpha[0]);
        ui->aY->setValue(alpha[1]);
        ui->aT1->setValue(alpha[2]);
        ui->aT2->setValue(alpha[3]);

        ui->bX->setValue(beta[0]);
        ui->bY->setValue(beta[1]);
        ui->bT1->setValue(beta[2]);
        ui->bT2->setValue(beta[3]);

        ui->l1->setValue(L1);
        ui->l2->setValue(L2);

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
        if(old_seed != seed) {
            ui->random->setValue(seed);
            srand(seed);
        }
    }

    runAnim = true;
    pauseAnim = false;
    inc = 0;
    run();

    ui->openGLWidget->genScene();
    ui->openGLWidget->update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x()-90;
    int y = event->y()-30;
    if(x<0||y<0||x>512||y>512) return;
}

double mouseX, mouseY;
void MainWindow::mousePressEvent(QMouseEvent *event) {
    int x = event->x()-90;
    int y = event->y()-30;
    if(x<0||y<0||x>512||y>512) return;

    mouseX = x;
    mouseY = 512-y;
    fprintf(stderr, "mouse (%.lf, %.lf)\n", mouseX, mouseY);
}


void MainWindow::on_randomButton_clicked()
{
    QType=0;
}

void MainWindow::on_bfs_clicked()
{
    QType=1;
}

void MainWindow::on_greedy_clicked()
{
    QType=2;
}

void MainWindow::on_dist_clicked()
{
    QType=3;
}

void MainWindow::on_vor_clicked()
{
    QType=4;
}

void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::on_showAnim_clicked() {
    runAnim = true;
    this->update();
}
void MainWindow::on_pauseAnim_clicked() {
    pauseAnim = !pauseAnim;
    this->update();
}
void MainWindow::on_replayAnim_clicked() {
    runAnim = true;
    pauseAnim = false;
    inc = 0;
    this->update();
}
void MainWindow::on_hideBox_clicked()
{
    hideBox=ui->hideBox->isChecked();
    ui->openGLWidget->genScene();
    ui->openGLWidget->update();
}
void MainWindow::on_hideBoxBoundary_clicked()
{
    hideBoxBoundary=ui->hideBoxBoundary->isChecked();
    ui->openGLWidget->genScene();
    ui->openGLWidget->update();
}



void MainWindow::on_pushButton_clicked()
{
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
    }else{
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

    ui->openGLWidget->genScene();
    ui->openGLWidget->update();
}

void MainWindow::on_right_clicked()
{

    renderSteps+=incr;
    if(renderSteps>expansions.size()-1) renderSteps=expansions.size()-1;
    ui->steplabel->setText("Step: "+QString::number(renderSteps));

    ui->openGLWidget->genScene();
    ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_valueChanged(int value) {
    animationSpeed=value;
}
