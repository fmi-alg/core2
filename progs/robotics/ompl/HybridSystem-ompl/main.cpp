#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <limits>

#include <Qdir>

#include <ompl/base/goals/GoalState.h>
#include <ompl/base/spaces/SE2StateSpace.h>
#include <ompl/base/spaces/DiscreteStateSpace.h>
#include <ompl/control/spaces/RealVectorControlSpace.h>
#include <ompl/control/SimpleSetup.h>
#include <ompl/config.h>
#include <boost/math/constants/constants.hpp>

namespace ob = ompl::base;
namespace oc = ompl::control;

//#define mw_out (*window)
//static MainWindow *window;

void propagate(const oc::SpaceInformation *si, const ob::State *state,
               const oc::Control* control, const double duration, ob::State *result);
bool isStateValid(const oc::SpaceInformation *si, const ob::State *state);
void run();

int main(int argc, char** argv) {
    bool foundFiles = false;
    std::string workingDir = QDir::currentPath().toStdString();

    // Test if the build directory is HybridSystem-ompl. If so,
    // the path to the current working directory will
    // include /HybridSystem-ompl
    unsigned long indexOfDesiredDir = workingDir.rfind("/HybridSystem-ompl/");
    if (indexOfDesiredDir != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 5);

        // Set current working directory to HybridSystem-ompl
        QDir::setCurrent(workingDir.c_str());
        foundFiles = true;
    }

    // Test if program was downloaded from Github, and is the build directory.
    // Downloading it from Github will result in the folder having the name
    // /HybridSystem-ompl-master instead of /HybridSystem-ompl
    if (!foundFiles && (indexOfDesiredDir = workingDir.rfind("/HybridSystem-ompl/")) != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 12);

        // Set current working directory to /HybridSystem-ompl-master
        QDir::setCurrent(workingDir.c_str());
        foundFiles = true;
    }

    // Test if a build directory (/build-HybridSystem-ompl-...) was created. This directory
    // will reside in the same directory as /HybridSystem-ompl
    if (!foundFiles && (indexOfDesiredDir = workingDir.rfind("/build-HybridSystem-ompl")) != std::string::npos) {
        QDir dir(workingDir.substr(0, indexOfDesiredDir).c_str());

        if (dir.exists("HybridSystem-ompl/HybridSystem-ompl.pro")) { // Test if /HybridSystem-ompl exists
            workingDir = workingDir.substr(0, indexOfDesiredDir) + "/HybridSystem-ompl";

            // Set current working directory to HybridSystem-ompl
            QDir::setCurrent(workingDir.c_str());
            foundFiles = true;
        }
    }

    // /HybridSystem-ompl could not be found
    if (!foundFiles) {
        std::cerr << std::endl << "!! WARNING !!\n"
        << "The program may not work correctly or at all because the folder "
        "containing the program's files cannot be found.\n"
        "Make sure that the program is inside of a folder named \"HybridSystem-ompl\".\n";
    }

    //QApplication app(argc, argv);
    //window = new MainWindow();
    run();

    //window->show();

    //return app.exec();
    return 0;
}

void propagate(const oc::SpaceInformation *si, const ob::State *state,
               const oc::Control* control, const double duration, ob::State *result) {

    static double timeStep = .01;
    int nsteps = ceil(duration / timeStep);
    double dt = duration / nsteps;
    const double *u = control->as<oc::RealVectorControlSpace::ControlType>()->values;

    ob::CompoundStateSpace::StateType& s = *result->as<ob::CompoundStateSpace::StateType>();
    ob::SE2StateSpace::StateType& se2 = *s.as<ob::SE2StateSpace::StateType>(0);
    ob::RealVectorStateSpace::StateType& velocity = *s.as<ob::RealVectorStateSpace::StateType>(1);
    ob::DiscreteStateSpace::StateType& gear = *s.as<ob::DiscreteStateSpace::StateType>(2);

    si->getStateSpace()->copyState(result, state);
    for(int i=0; i<nsteps; i++) {
        se2.setX(se2.getX() + dt * velocity.values[0] * cos(se2.getYaw()));
        se2.setY(se2.getY() + dt * velocity.values[0] * sin(se2.getYaw()));
        se2.setYaw(se2.getYaw() + dt * u[0]);
        velocity.values[0] = velocity.values[0] + dt * (u[1]*gear.value);

        // 'guards' - conditions to change gears
        if (gear.value > 0) {
            if (gear.value < 3 && velocity.values[0] > 10*(gear.value + 1))
                gear.value++;
            else if (gear.value > 1 && velocity.values[0] < 10*gear.value)
                gear.value--;
        }
        if (!si->satisfiesBounds(result))
            return;
    }
}

// The free space consists of two narrow corridors connected at right angle.
// To make the turn, the car will have to downshift.
bool isStateValid(const oc::SpaceInformation *si, const ob::State *state) {
  const ob::SE2StateSpace::StateType *se2 = state->as<ob::CompoundState>()->as<ob::SE2StateSpace::StateType>(0);
  return si->satisfiesBounds(state) && (se2->getX() < -80. || se2->getY() > 80.);
}


int runCount = 1;
void run() {
    // plan for hybrid car in SE(2) with discrete gears
    ob::StateSpacePtr SE2(new ob::SE2StateSpace());
    ob::StateSpacePtr velocity(new ob::RealVectorStateSpace(1));
    // set the range for gears: [-1,3] inclusive
    ob::StateSpacePtr gear(new ob::DiscreteStateSpace(-1,3));
    ob::StateSpacePtr stateSpace = SE2 + velocity + gear;

    // set the bounds for the R^2 part of SE(2)
    ob::RealVectorBounds bounds(2);
    bounds.setLow(-100);
    bounds.setHigh(100);
    SE2->as<ob::SE2StateSpace>()->setBounds(bounds);

    // set the bounds for the velocity
    ob::RealVectorBounds velocityBound(1);
    velocityBound.setLow(0);
    velocityBound.setHigh(60);
    velocity->as<ob::RealVectorStateSpace>()->setBounds(velocityBound);

    // create start and goal states
    ob::ScopedState<> start(stateSpace);
    ob::ScopedState<> goal(stateSpace);

    // Both start and goal are states with high velocity with the car in third gear.
    // However, to make the turn, the car cannot stay in third gear and will have to
    // shift to first gear.
    start[0] = start[1] = -90.; // position
    start[2] = boost::math::constants::pi<double>()/2; // orientation
    start[3] = 40.; // velocity
    start->as<ob::CompoundState>()->as<ob::DiscreteStateSpace::StateType>(2)->value = 3; // gear

    goal[0] = goal[1] = 90.; // position
    goal[2] = 0.; // orientation
    goal[3] = 40.; // velocity
    goal->as<ob::CompoundState>()->as<ob::DiscreteStateSpace::StateType>(2)->value = 3; // gear

    oc::ControlSpacePtr cmanifold(new oc::RealVectorControlSpace(stateSpace, 2));

    // set the bounds for the control manifold
    ob::RealVectorBounds cbounds(2);
    // bounds for steering input
    cbounds.setLow(0, -1.);
    cbounds.setHigh(0, 1.);
    // bounds for brake/gas input
    cbounds.setLow(1, -20.);
    cbounds.setHigh(1, 20.);
    cmanifold->as<oc::RealVectorControlSpace>()->setBounds(cbounds);

    oc::SimpleSetup setup(cmanifold);
    setup.setStartAndGoalStates(start, goal, 5.);
    setup.setStateValidityChecker(std::bind(
        &isStateValid, setup.getSpaceInformation().get(), std::placeholders::_1));
    setup.setStatePropagator(std::bind(
        &propagate, setup.getSpaceInformation().get(),
        std::placeholders::_1, std::placeholders::_2,
        std::placeholders::_3, std::placeholders::_4));
    setup.getSpaceInformation()->setPropagationStepSize(.1);
    setup.getSpaceInformation()->setMinMaxControlDuration(2, 3);

    // try to solve the problem
    if (setup.solve(30))
    {
        // print the (approximate) solution path: print states along the path
        // and controls required to get from one state to the next
        oc::PathControl& path(setup.getSolutionPath());

        // print out full state on solution path
        // (format: x, y, theta, v, u0, u1, dt)
        for(unsigned int i=0; i<path.getStateCount(); ++i)
        {
            const ob::State* state = path.getState(i);
            const ob::SE2StateSpace::StateType *se2 =
                state->as<ob::CompoundState>()->as<ob::SE2StateSpace::StateType>(0);
            const ob::RealVectorStateSpace::StateType *velocity =
                state->as<ob::CompoundState>()->as<ob::RealVectorStateSpace::StateType>(1);
            const ob::DiscreteStateSpace::StateType *gear =
                state->as<ob::CompoundState>()->as<ob::DiscreteStateSpace::StateType>(2);
            std::cout << se2->getX() << ' ' << se2->getY() << ' ' << se2->getYaw()
                << ' ' << velocity->values[0] << ' ' << gear->value << ' ';
            if (i==0)
                // null controls applied for zero seconds to get to start state
                std::cout << "0 0 0";
            else
            {
                // print controls and control duration needed to get from state i-1 to state i
                const double* u =
                    path.getControl(i-1)->as<oc::RealVectorControlSpace::ControlType>()->values;
                std::cout << u[0] << ' ' << u[1] << ' ' << path.getControlDuration(i-1);
            }
            std::cout << std::endl;
        }
        if (!setup.haveExactSolutionPath())
        {
            std::cout << "Solution is approximate. Distance to actual goal is " << setup.getProblemDefinition()->getSolutionDifference() << std::endl;
        }
    }
}
