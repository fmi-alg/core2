#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;

extern char cfgNameList[200][200];
extern int numEg;
extern string cfgName;
extern string fileName;
extern string inputDir;

extern double alpha[4];		// start configuration
extern double beta[4];		// goal configuration
extern double epsilon;		// resolution parameter

extern double L1;
extern double L2;
extern double thickness;
extern double R0;
extern int seed;
extern int QType;
extern double deltaX;
extern double deltaY;
extern double scale;
extern int inc;
extern bool showTrace;
extern bool showPath;
extern bool showFilledObstacles;
extern bool runAnim;
extern bool replayAnim;
extern bool pauseAnim;
extern bool safeAngle;
extern bool hideBox;
extern bool hideBoxBoundary;
extern int numberForDisplay;
extern bool ompl;

extern int crossingOption;
extern double bandwidth;

extern int renderSteps;
extern bool step;

extern std::vector<int> expansions;
extern bool doTriangulation;
extern vector<Triangle> triangles;

int incr(1);
int animationSpeed(90);
int animationSpeedScale(5000);
int animationSpeedScaleBox(4000);

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
    for(int i=0;i<numEg;++i) {
        if (strcmp(cfgNameList[i], cfgName.c_str()) == 0) {
            ui->comboBox->setCurrentIndex(i);
        }
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
    ui->thickness->setValue(thickness);

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

    ui->horizontalSlider->setValue(animationSpeed);

    ui->hideBox->setChecked(hideBox);
    ui->hideBoxBoundary->setChecked(hideBoxBoundary);
    ui->safe_angle->setChecked(safeAngle);
    ui->ompl_input->setChecked(ompl);

    ui->non_crossing->setChecked(crossingOption);
    ui->bandwidth->setValue(bandwidth);

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

    double mouseX = x;
    double mouseY = 512-y;
    sprintf(tmp_buff, "mouse\n(%.lf, %.lf)\n", mouseX, mouseY);
    box_info.append(tmp_buff);

    for(unsigned int i=0;i<Box::pAllLeaf->size();i++){
        Box *tmp = Box::pAllLeaf->at(i);
        if(!tmp->isLeaf) continue;
        if(mouseX >= tmp->x-tmp->width/2 && mouseX < tmp->x+tmp->width/2 &&
           mouseY >= tmp->y-tmp->height/2 && mouseY < tmp->y+tmp->height/2){
            sprintf(tmp_buff, "box\n%.lf %.lf %.lf %.lf\n", tmp->x, tmp->y, tmp->width, tmp->height);
            box_info.append(tmp_buff);
            sprintf(tmp_buff, "status: %d %d %d %d %d condition %d\n", tmp->status, Box::STUCK, Box::FREE, Box::MIXED, Box::UNKNOWN, tmp->condition);
            box_info.append(tmp_buff);
            break;
        }
    }

    ui->textOutputInfo->setText(QString::fromStdString(box_info));
}

// void MainWindow::showAngleBound(double t1_lowerBound, double t1_upperBound, double t2_lowerBound, double t2_upperBound){
//     char tmp_buff[200];
//     string box_info;
//     sprintf(tmp_buff, "theta 1: [%lf, %lf]\ntheta 2: [%lf, %lf]\n", t1_lowerBound, t1_upperBound, t2_lowerBound, t2_upperBound);
//     ui->angleBoundOutputInfo->setText(QString::fromStdString(box_info));
// }

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
        thickness=ui->thickness->value();

        epsilon=ui->eps->value();

        int new_seed = ui->random->value();
        if(new_seed != seed){
            seed = new_seed;
            srand(seed);
        }

        hideBox=ui->hideBox->isChecked();
        hideBoxBoundary=ui->hideBoxBoundary->isChecked();
        safeAngle=ui->safe_angle->isChecked();
        ompl=ui->ompl_input->isChecked();

        crossingOption=ui->non_crossing->isChecked();
        bandwidth=ui->bandwidth->value();

        // OMPL
        if(ompl){
            double degToRad = PI/180.0f;
            alpha[0] = alpha[0] - L1*cos((alpha[2])*degToRad)*0.5f;
            alpha[1] = alpha[1] - L1*sin((alpha[2])*degToRad)*0.5f;
            beta[0] = beta[0] - L1*cos((beta[2])*degToRad)*0.5f;
            beta[1] = beta[1] - L1*sin((beta[2])*degToRad)*0.5f;
        }
    }
    else {
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
        ui->thickness->setValue(thickness);

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

        ui->horizontalSlider->setValue(animationSpeed);

        ui->hideBox->setChecked(hideBox);
        ui->hideBoxBoundary->setChecked(hideBoxBoundary);
        ui->safe_angle->setChecked(safeAngle);
        ui->ompl_input->setChecked(ompl);

        ui->non_crossing->setChecked(crossingOption);
        ui->bandwidth->setValue(bandwidth);
    }

    runAnim = true;
    pauseAnim = false;
    inc = 0;
    run();

    doTriangulation = false;
    triangles.clear();
    ui->openGLWidget->update();
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

void MainWindow::on_showTrace_clicked() {
    showTrace = !showTrace;
    ui->openGLWidget->update();
}
void MainWindow::on_showPath_clicked() {
    showPath = !showPath;
    ui->openGLWidget->update();
}
void MainWindow::on_showFilledObstacles_clicked() {
    showFilledObstacles = !showFilledObstacles;
    ui->openGLWidget->update();
}
void MainWindow::on_showAnim_clicked() {
    runAnim = true;
    ui->openGLWidget->update();
}
void MainWindow::on_pauseAnim_clicked() {
    pauseAnim = !pauseAnim;
    ui->openGLWidget->update();
}
void MainWindow::on_replayAnim_clicked() {
    runAnim = true;
    pauseAnim = false;
    inc = 0;
    ui->openGLWidget->update();
}

void MainWindow::on_hideBox_clicked()
{
    hideBox=ui->hideBox->isChecked();
    ui->openGLWidget->update();
}
void MainWindow::on_hideBoxBoundary_clicked()
{
    hideBoxBoundary=ui->hideBoxBoundary->isChecked();
    ui->openGLWidget->update();
}

void MainWindow::on_safe_angle_clicked()
{
    safeAngle=ui->safe_angle->isChecked();
    ui->openGLWidget->update();
}

void MainWindow::on_non_crossing_clicked(){
    crossingOption = ui->hideBox->isChecked();
    bandwidth = ui->bandwidth->value();
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
        ui->openGLWidget->update();
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

void MainWindow::on_left_clicked() {
    if(renderSteps-incr<1) return;
    renderSteps-=incr;
    ui->steplabel->setText("Step: "+QString::number(renderSteps));
    ui->openGLWidget->update();
}

void MainWindow::on_right_clicked() {
    renderSteps+=incr;
    if(renderSteps>(int)expansions.size()-1) renderSteps=expansions.size()-1;
    ui->steplabel->setText("Step: "+QString::number(renderSteps));
    ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_valueChanged(int value) {
    animationSpeed=value;
}

string outCfg;
void MainWindow::on_saveCfg_clicked() {
    outCfg = ui->configurationFile->text().toStdString()+".cfg";
    if(strcmp(outCfg.c_str(), ".cfg") != 0){
        outCfg = inputDir + "/" + outCfg;
        FILE *f_out = fopen(outCfg.c_str(), "w");
        if(f_out == NULL){
            cerr << "Cannot create file " << outCfg << "!\n";
        }
        else{
            Timer t;
            fprintf(f_out, "# %s\n", t.currentDateTime().c_str());
            fprintf(f_out, "# Non-crossing Thick 2-link Robot\n");
            fprintf(f_out, "# %s environment\n\n", fileName.c_str());

            fprintf(f_out, "interactive = 0		# 0=interactive, >0 is non-interactive\n\n");

            fprintf(f_out, "startX = %.lf	# start configuration\n", alpha[0]);
            fprintf(f_out, "startY = %.lf\n", alpha[1]);
            fprintf(f_out, "startTheta1 = %.lf	# in degrees\n", alpha[2]);
            fprintf(f_out, "startTheta2 = %.lf	# in degrees\n", alpha[3]);
            fprintf(f_out, "goalX = %.lf		# goal configuration\n", beta[0]);
            fprintf(f_out, "goalY = %.lf\n", beta[1]);
            fprintf(f_out, "goalTheta1 = %.lf	# in degrees\n", beta[2]);
            fprintf(f_out, "goalTheta2 = %.lf	# in degrees\n\n", beta[3]);

            fprintf(f_out, "epsilon = %.lf		# resolution parameter\n\n", epsilon);

            fprintf(f_out, "len1 = %.2lf		# link1 length\n", L1);
            fprintf(f_out, "len2 = %.2lf		# link2 length\n", L2);
            fprintf(f_out, "thickness = %.2lf   # link thickness\n\n", thickness);

            fprintf(f_out, "ompl = 0\n");

            fprintf(f_out, "inputDir = inputs	# where to find input files\n");
            fprintf(f_out, "fileName = %s       # input environment file\n\n", fileName.c_str());

            fprintf(f_out, "boxWidth  = 512		# initial configuration box size\n");
            fprintf(f_out, "boxHeight = 512\n\n");

            fprintf(f_out, "windowPosX = 320	# initial Window position\n");
            fprintf(f_out, "windowPosY = 20\n\n");

            fprintf(f_out, "Qtype = %d		# type of priority queue\n", QType);
            fprintf(f_out, "                #  (0=random, 1=BFS, 2=Greedy Best First, 3=Dist+size, 4=Vor?)\n");
            fprintf(f_out, "seed = %d		# seed for random\n", seed);
            fprintf(f_out, "step = 0		# number of steps to run\n");
            fprintf(f_out, "                # (step=0 means run to completion)\n");
            fprintf(f_out, "xtrans = %lf	# x-translation of input file\n", deltaX);
            fprintf(f_out, "ytrans = %lf	# y-translation of input file\n", deltaY);
            fprintf(f_out, "scale  = %lf	# scaling of input file\n\n", scale);

            fprintf(f_out, "non-crossing = %d\n", crossingOption);
            fprintf(f_out, "bandwidth    = %.lf\n\n", bandwidth);

            fprintf(f_out, "animationSpeed = %d\n", animationSpeed);
            fprintf(f_out, "animationSpeedScale = 5000\n");
            fprintf(f_out, "animationSpeedScaleBox = 4000\n");
            fprintf(f_out, "verbose = 0		# if true, display statistics\n");
        }
        fclose(f_out);
    }
    // empty
    else{
        cerr << "Empty input!\n";
    }
}
