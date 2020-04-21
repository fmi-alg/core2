/*
 *   main.cpp
 *
 *   Author: Ching-Hsiang (Tom) Hsu
 *   Email: chhsu@nyu.edu
 *   Modified: Oct. 31, 2016
 *
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

#include <Qdir>

#include <ompl/geometric/SimpleSetup.h>
#include <ompl/geometric/planners/prm/PRM.h>
#include <ompl/geometric/planners/prm/LazyPRM.h>
#include <ompl/geometric/planners/rrt/RRT.h>
#include <ompl/geometric/planners/rrt/LazyRRT.h>
#include <ompl/geometric/planners/rrt/RRTConnect.h>
#include <ompl/geometric/planners/stride/STRIDE.h>
#include <omplapp/apps/SE2RigidBodyPlanning.h>
#include <omplapp/config.h>

using namespace ompl;

//#define mw_out (*window)
//static MainWindow *window;

FILE *wp;

char WAFR2016cfgName[10][100];

std::string cfgName("T-room3.cfg");
std::string robot("link-70-8_c.raw");
std::string world("bugtrap2_c.raw");

struct CFG{
    double x, y, theta;
};

CFG start, goal;
char cfgPath[1000], tmp[1000];
double s_round = 10;
int succc[100];

void parseExampleFile();
void run(int planner_idx);

std::string workingDir = QDir::currentPath().toStdString();
int main(int argc, char** argv) {
    bool foundFiles = false;

    // Test if the build directory is SE2RigidBody-ompl. If so,
    // the path to the current working directory will
    // include /SE2RigidBody-ompl
    unsigned long indexOfDesiredDir = workingDir.rfind("/SE2RigidBody-ompl/");
    if (indexOfDesiredDir != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 5);

        // Set current working directory to SE2RigidBody-ompl
        QDir::setCurrent(workingDir.c_str());
        foundFiles = true;
    }

    // Test if program was downloaded from Github, and is the build directory.
    // Downloading it from Github will result in the folder having the name
    // /SE2RigidBody-ompl-master instead of /SE2RigidBody-ompl
    if (!foundFiles && (indexOfDesiredDir = workingDir.rfind("/SE2RigidBody-ompl/")) != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 12);

        // Set current working directory to /SE2RigidBody-ompl-master
        QDir::setCurrent(workingDir.c_str());
        foundFiles = true;
    }

    // Test if a build directory (/build-SE2RigidBody-ompl-...) was created. This directory
    // will reside in the same directory as /SE2RigidBody-ompl
    if (!foundFiles && (indexOfDesiredDir = workingDir.rfind("/build-SE2RigidBody-ompl")) != std::string::npos) {
        QDir dir(workingDir.substr(0, indexOfDesiredDir).c_str());

        if (dir.exists("SE2RigidBody-ompl/SE2RigidBody-ompl.pro")) { // Test if /SE2RigidBody-ompl exists
            workingDir = workingDir.substr(0, indexOfDesiredDir) + "/SE2RigidBody-ompl";

            // Set current working directory to SE2RigidBody-ompl
            QDir::setCurrent(workingDir.c_str());
            foundFiles = true;
        }
    }

    // /SE2RigidBody-ompl could not be found
    if (!foundFiles) {
        std::cerr << std::endl << "!! WARNING !!\n"
        << "The program may not work correctly or at all because the folder "
        "containing the program's files cannot be found.\n"
        "Make sure that the program is inside of a folder named \"SE2RigidBody-ompl\".\n";
    }

    //QApplication app(argc, argv);
    //window = new MainWindow();

    sprintf(WAFR2016cfgName[0], "WAFR2016_T-room4");
    sprintf(WAFR2016cfgName[1], "WAFR2016_maze");
    sprintf(WAFR2016cfgName[2], "WAFR2016_rand100");
    sprintf(WAFR2016cfgName[3], "WAFR2016_Hole-in-Wall");
    sprintf(WAFR2016cfgName[4], "WAFR2016_8-ways");
    sprintf(WAFR2016cfgName[5], "WAFR2016_bugtrap2");
    sprintf(WAFR2016cfgName[6], "WAFR2016_bugtrap2_noPath");

    // Easy Passages
    for(int all=0;all<7;++all){
        char ss[200];
        sprintf(ss, "%s.cfg", WAFR2016cfgName[all]);
        cfgName =  ss;
        fprintf(stderr, "%s\n", cfgName.c_str());
        sprintf(ss, "table2_%s.txt", WAFR2016cfgName[all]);
        wp = fopen(ss, "w");
        parseExampleFile();

        fprintf(wp, "start\n");
        fprintf(wp, "%s run %.lf\n", cfgName.c_str(), s_round);
        fprintf(wp, "configuration:\n\trobot: %s\n", robot.c_str());

        for(int i=0;i<5;++i) // planner idx
        {
            if(i%2 == 0) // not try every planner
                run(i);
        }

        fprintf(wp, "finish\n\n");
        fclose(wp);
    }

    // Narrow Passages
    for(int all=0;all<7;++all){
        char ss[200];
        sprintf(ss, "%s_d.cfg", WAFR2016cfgName[all]);
        cfgName =  ss;
        fprintf(stderr, "%s\n", cfgName.c_str());
        sprintf(ss, "table3_%s_d.txt", WAFR2016cfgName[all]);
        wp = fopen(ss, "w");
        parseExampleFile();

        fprintf(wp, "start\n");
        fprintf(wp, "%s run %.lf\n", cfgName.c_str(), s_round);
        fprintf(wp, "configuration:\n\trobot: %s\n", robot.c_str());

        for(int i=0;i<5;++i) // planner idx
        {
            if(i%2 == 0) // not try every planner
                run(i);
        }

        fprintf(wp, "finish\n\n");
        fclose(wp);
    }

    //window->show();

    //return app.exec();
    return 0;
}

double s_roundAngle(double theta){
    while(theta >= 360) theta -= 360;
    while(theta < 0)    theta += 360;
    return theta;
}

double PI = 3.1415926535897f;
double orientation2OMPL(double theta){
    return PI*(s_roundAngle(theta)-180.0f)/180.0f;
}

void parseExampleFile() {

    sprintf(cfgPath, "inputs/%s", cfgName.c_str());
    FILE *fptr = fopen(cfgPath, "r");
    if (fptr == NULL) return ;

    while (fgets(tmp, 200, fptr) != NULL){
        char *sptr = strtok(tmp, "=: \t");

        // comments
        if (strcmp(sptr, "#") == 0) {
            continue;
        }

        // start configuration
        if (strcmp(sptr, "start.x") == 0) {
            sptr = strtok(NULL, "=: \t");
            start.x = atof(sptr);
        }
        if (strcmp(sptr, "start.y") == 0) {
            sptr = strtok(NULL, "=: \t");
            start.y = atof(sptr);
        }
        if (strcmp(sptr, "start.theta") == 0) {
            sptr = strtok(NULL, "=: ");
            start.theta = atof(sptr);
        }

        // goal configuration
        if (strcmp(sptr, "goal.x") == 0) {
            sptr = strtok(NULL, "=: \t");
            goal.x = atof(sptr);
        }
        if (strcmp(sptr, "goal.y") == 0) {
            sptr = strtok(NULL, "=: \t");
            goal.y = atof(sptr);
        }
        if (strcmp(sptr, "goal.theta") == 0) {
            sptr = strtok(NULL, "=: \t");
            goal.theta = atof(sptr);
        }

        if (strcmp(sptr, "robot") == 0) {
            sptr = strtok(NULL, "=: #\t\n");
            robot = sptr;
        }
        if (strcmp(sptr, "world") == 0) {
            sptr = strtok(NULL, "=: #\t\n");
            world = sptr;
        }
    }

    fclose(fptr);
}

int runCount = 1;
void run(int planner_idx) {
    // plan in SE2
    app::SE2RigidBodyPlanning setup;


    // load the robot and the environment
    std::string robot_fname = workingDir + "/inputs/" + robot;
    std::string env_fname = workingDir + "/inputs/" + world;
    fprintf(wp, "%s %s\n", robot_fname.c_str(), env_fname.c_str());
    fprintf(wp, "%lf %lf %lf -> %lf %lf %lf\n", start.x, start.y, start.theta, goal.x, goal.y, goal.theta);
    setup.setRobotMesh(robot_fname.c_str());
    setup.setEnvironmentMesh(env_fname.c_str());

    double tt[100];
    double suc = 0;
    for(int i=0;i<(int)s_round;++i){
        // define starting state
        base::ScopedState<base::SE2StateSpace> m_start(setup.getSpaceInformation());
        m_start->setX(start.x);
        m_start->setY(start.y);
        m_start->setYaw(start.theta);

        // define goal state
        base::ScopedState<base::SE2StateSpace> m_goal(m_start);
        m_goal->setX(goal.x);
        m_goal->setY(goal.y);
        m_goal->setYaw(goal.theta);

        // set the start & goal states
        setup.setStartAndGoalStates(m_start, m_goal);
        // choose different sampling method
        ompl::base::SpaceInformationPtr si_ptr = setup.getSpaceInformation();

        if(planner_idx == 0){
            ompl::geometric::PRM* prm = new ompl::geometric::PRM(si_ptr);
            prm->setMaxNearestNeighbors(15);
            ompl::base::PlannerPtr target_planner_ptr(prm);
            setup.setPlanner(target_planner_ptr);
        }
        else if(planner_idx == 1){
            ompl::base::PlannerPtr target_planner_ptr(new ompl::geometric::LazyPRM(si_ptr));
            setup.setPlanner(target_planner_ptr);
        }
        else if(planner_idx ==2){
            ompl::geometric::RRT* rrt = new ompl::geometric::RRT(si_ptr);
            rrt->setGoalBias(0.01);
            rrt->setRange(0.5);
            ompl::base::PlannerPtr target_planner_ptr(rrt);
            setup.setPlanner(target_planner_ptr);
        }
        else if(planner_idx == 3){
            ompl::geometric::LazyRRT* lazyrrt = new ompl::geometric::LazyRRT(si_ptr);
            ompl::base::PlannerPtr target_planner_ptr(lazyrrt);
            setup.setPlanner(target_planner_ptr);
        }
        else if(planner_idx == 4){
            ompl::geometric::RRTConnect* rrtconnect = new ompl::geometric::RRTConnect(si_ptr);
            rrtconnect->setRange(0.5);
            ompl::base::PlannerPtr target_planner_ptr(rrtconnect);
            setup.setPlanner(target_planner_ptr);
        }


        // Jul. 16 Tom
        // not a good way for evaluating since it is an approximate path ...
        //if(setup.solve()){
        //    suc++;
        //}

        // Jul. 20 Tom
        bool solved = setup.solve(30);
        succc[i] = -1;
        if(solved && setup.haveExactSolutionPath()){
        //if(solved && setup.haveSolutionPath()){
            suc++;
            succc[i] = 1;
        }

        tt[i] = setup.getLastPlanComputationTime()*1000.0f;
    }

    double total = 0;
    double sd = 0, ave = 0;
    for(int i=0;i<(int)s_round;++i){
        total += tt[i];
    }
    ave = total/s_round;
    for(int i=0;i<(int)s_round;++i){
        sd += (tt[i]-ave)*(tt[i]-ave);
    }
    sd = sqrt(sd/s_round);
    double best = FLT_MAX;
    for(int i=0;i<(int)s_round;++i){
        if(tt[i] < best && succc[i] > 0){
            best = tt[i];
        }
    }
    fprintf(wp, "%s %s\n", cfgName.c_str(), setup.getPlanner()->getName().c_str());
    fprintf(wp, "%lf %lf %lf %lf\n", ave, best, sd, suc/s_round);
}
