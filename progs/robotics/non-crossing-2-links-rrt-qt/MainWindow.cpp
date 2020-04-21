/*
 *  MainWindow.cpp
 *
 *  Author: Ching-Hsiang (Tom) Hsu
 *  Email: chhsu@nyu.edu
 *  Modified: Oct, 31, 2016
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;

extern char cfgNameList[200][200];
extern int numEg;
extern string cfgName;
extern string fileName;
extern Planner *planner;
extern CFG start;
extern CFG goal;
extern int l1,l2;
extern double thickness;
extern int seed;
extern string method;
extern int SearchType;
extern float env_width;
extern float env_height;
extern float env_TR;
extern float env_RR;
extern unsigned int max_sample_size;
extern unsigned int prm_closest_free_k;
extern unsigned int prm_closest_obst_k;
extern float gauss_mean_d;
extern float gauss_std;
extern float rrt_step_size;
extern float rrt_bias;
extern float rrt_close_to_goal;
extern bool showAnim;
extern bool pauseAnim;
extern bool replayAnim;
extern bool finishedAnim;
extern bool prm_graph;
extern int prm_graph_flag;
extern bool prm_graph_mixed;
extern bool prm_graph_free;
extern bool prm_graph_obst;
extern bool prm_graph_edge;
extern bool rrt_graph;
extern bool non_crossing;
extern bool ompl;
extern int animationSpeed;
extern double boxWidth;
extern double boxHeight;
extern bool showTrace;
extern bool showFilledObstacles;
extern bool doTriangulation;

extern int runCount;
extern double timeout;
extern double elapsedTime, elapsedCPUTime;

extern int path_index;

//extern FILE *fptr; // for debugging

extern void run();
extern void parseExampleFile();
extern void parseMapFile();


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    for(int i=0;i<numEg;++i) {
        ui->comboBox->addItem(cfgNameList[i]);
    }
    for(int i=0;i<numEg;++i) {
        if (strcmp(cfgNameList[i], cfgName.c_str()) == 0) {
            ui->comboBox->setCurrentIndex(i);
        }
    }

    //ui->egFile->setText(QString::fromStdString(egName));
    ui->inputFile->setText(QString::fromStdString(fileName.substr(0,fileName.length()-4)));

    ui->aX->setValue(start.x);
    ui->aY->setValue(start.y);
    ui->aT1->setValue(start.t1);
    ui->aT2->setValue(start.t2);

    ui->bX->setValue(goal.x);
    ui->bY->setValue(goal.y);
    ui->bT1->setValue(goal.t1);
    ui->bT2->setValue(goal.t2);

    ui->l1->setValue(l1);
    ui->l2->setValue(l2);
    ui->thickness->setValue(thickness);


    switch (SearchType) {
        case 0:
            ui->prm->setChecked(true);
            break;
        case 1:
            ui->gauss->setChecked(true);
            break;
        case 2:
            ui->rrt->setChecked(true);
            break;
        case 3:
            ui->toggle->setChecked(true);
            break;
        case 4:
            ui->lazytoggle->setChecked(true);
            break;
    }

    ui->max_sample->setValue(max_sample_size);

    ui->prm_closest_free_k->setValue(prm_closest_free_k);
    ui->prm_closest_obst_k->setValue(prm_closest_obst_k);

    ui->gauss_closest_k->setValue(prm_closest_free_k);
    ui->gauss_mean->setValue(gauss_mean_d);
    ui->gauss_std->setValue(gauss_std);

    ui->rrt_step_size->setValue(rrt_step_size);
    ui->rrt_bias->setValue(rrt_bias);
    ui->rrt_close_to_goal->setValue(rrt_close_to_goal);

    ui->prm_graph->setChecked(prm_graph);
    ui->prm_graph_mixed->setChecked(prm_graph_mixed);
    ui->prm_graph_free->setChecked(prm_graph_free);
    ui->prm_graph_obst->setChecked(prm_graph_obst);
    ui->prm_graph_edge->setChecked(prm_graph_edge);

    ui->rrt_graph->setChecked(rrt_graph);
    ui->non_crossing->setChecked(non_crossing);

    ui->ompl->setChecked(ompl);

    ui->animationSpeed->setValue(animationSpeed);
    ui->random->setValue(seed);
    srand(seed);
    doTriangulation = false;

    ui->timeout->setValue(timeout);

    ui->textOutputTime->setText(QString::fromStdString(""));
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
    //sprintf(egCur, "%s", ui->egFile->text().toStdString().c_str());
    sprintf(cfgCur, "%s", ui->comboBox->currentText().toStdString().c_str());

    if (strcmp(cfgPre, cfgCur) == 0) {
        fileName=ui->inputFile->text().toStdString()+".txt";

        start.x=ui->aX->value();
        start.y=ui->aY->value();
        start.t1=ui->aT1->value();
        start.t2=ui->aT2->value();
        start.ws = false;

        goal.x=ui->bX->value();
        goal.y=ui->bY->value();
        goal.t1=ui->bT1->value();
        goal.t2=ui->bT2->value();
        goal.ws = false;

        l1=ui->l1->value();
        l2=ui->l2->value();
        thickness=ui->thickness->value();

        max_sample_size=ui->max_sample->value();

        prm_closest_free_k=ui->prm_closest_free_k->value();
        prm_closest_obst_k=ui->prm_closest_obst_k->value();

        prm_closest_free_k=ui->gauss_closest_k->value();
        gauss_mean_d=ui->gauss_mean->value();
        gauss_std=ui->gauss_std->value();

        rrt_step_size=ui->rrt_step_size->value();
        rrt_bias=ui->rrt_bias->value();
        rrt_close_to_goal=ui->rrt_close_to_goal->value();

        prm_graph=ui->prm_graph->isChecked();
        rrt_graph=ui->rrt_graph->isChecked();
        non_crossing=ui->non_crossing->isChecked();

        ompl = ui->ompl->isChecked();

        // 4/13/2016
        // only initialize the seed when it is changed
        int new_seed = ui->random->value();
        if (seed != new_seed) {
            seed = new_seed;
            srand(seed);
        }

        timeout = ui->timeout->value();
    } else {
        //egName=ui->egFile->text().toStdString();
        cfgName = ui->comboBox->currentText().toStdString();
        parseExampleFile();

        ui->inputFile->setText(QString::fromStdString(fileName.substr(0,fileName.length()-4)));

        ui->aX->setValue(start.x);
        ui->aY->setValue(start.y);
        ui->aT1->setValue(start.t1);
        ui->aT2->setValue(start.t2);
        start.ws = false;

        ui->bX->setValue(goal.x);
        ui->bY->setValue(goal.y);
        ui->bT1->setValue(goal.t1);
        ui->bT2->setValue(goal.t2);
        goal.ws = false;

        ui->l1->setValue(l1);
        ui->l2->setValue(l2);
        ui->thickness->setValue(thickness);

        if(strcmp(method.c_str(), "prm") == 0){
                ui->prm->setChecked(true);
        }
        if(strcmp(method.c_str(), "gauss") == 0){
                ui->gauss->setChecked(true);
        }
        if(strcmp(method.c_str(), "rrt") == 0){
                ui->rrt->setChecked(true);
        }
        if(strcmp(method.c_str(), "toggle") == 0){
                ui->toggle->setChecked(true);
        }
        if(strcmp(method.c_str(), "lazytoggle") == 0){
                ui->lazytoggle->setChecked(true);
        }

        ui->max_sample->setValue(max_sample_size);

        ui->prm_closest_free_k->setValue(prm_closest_free_k);
        ui->prm_closest_obst_k->setValue(prm_closest_obst_k);

        ui->gauss_closest_k->setValue(prm_closest_free_k);
        ui->gauss_mean->setValue(gauss_mean_d);
        ui->gauss_std->setValue(gauss_std);

        ui->rrt_step_size->setValue(rrt_step_size);
        ui->rrt_bias->setValue(rrt_bias);
        ui->rrt_close_to_goal->setValue(rrt_close_to_goal);

        ui->prm_graph->setChecked(prm_graph);
        ui->rrt_graph->setChecked(rrt_graph);
        ui->non_crossing->setChecked(non_crossing);

        ui->ompl->setChecked(ompl);

        int old_seed = ui->random->value();
        if (seed != old_seed) {
            seed = old_seed;
            ui->random->setValue(seed);
            srand(seed);
        }

        ui->timeout->setValue(timeout);
    }

    showAnim = true;
    pauseAnim = false;
    path_index = 0;

    parseMapFile();
    run();
    prm_graph_flag = 1;
    doTriangulation = false;

    ui->openGLWidget->update();
}

void MainWindow::reportTime() {
    string time_info;
    char tmp_buff[200];
    sprintf(tmp_buff, "Run #%d\n", runCount-1);
    time_info.append(tmp_buff);
    sprintf(tmp_buff, "Elapsed CPU time:\n%lf (ms)\n", elapsedCPUTime);
    time_info.append(tmp_buff);
    ui->textOutputTime->append(QString::fromStdString(time_info));
}

void MainWindow::on_prm_clicked() {
    SearchType = 0;
    method = "prm";
}

void MainWindow::on_gauss_clicked() {
    SearchType = 1;
    method = "gauss";
}

void MainWindow::on_rrt_clicked() {
    SearchType = 2;
    method = "rrt";
}

void MainWindow::on_toggle_clicked() {
    SearchType = 3;
    method = "toggle";
}

void MainWindow::on_lazytoggle_clicked() {
    SearchType = 4;
    method = "lazytoggle";
}

void MainWindow::on_prm_graph_clicked(){
    prm_graph = ui->prm_graph->isChecked();
    this->update();
}
void MainWindow::on_prm_graph_mixed_clicked(){
    prm_graph_mixed = ui->prm_graph_mixed->isChecked();
    this->update();
}
void MainWindow::on_prm_graph_free_clicked(){
    prm_graph_free = ui->prm_graph_free->isChecked();
    this->update();
}
void MainWindow::on_prm_graph_obst_clicked(){
    prm_graph_obst = ui->prm_graph_obst->isChecked();
    this->update();
}
void MainWindow::on_prm_graph_edge_clicked(){
    prm_graph_edge = ui->prm_graph_edge->isChecked();
    this->update();
}

void MainWindow::on_rrt_graph_clicked(){
    rrt_graph = ui->rrt_graph->isChecked();
    this->update();
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

void MainWindow::on_animationSpeed_valueChanged(int value) {
    animationSpeed = value;
}
