/*
 * main.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: zhonghua, George Mason University
 *  
 *  Modified: Jyh-Ming Lien, Nov. 23, 2013
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

#include <Qdir>

#include <ompl/base/spaces/SO2StateSpace.h>
#include <ompl/geometric/planners/rrt/RRT.h>
#include <ompl/geometric/planners/kpiece/KPIECE1.h>
#include <ompl/geometric/planners/est/EST.h>
#include <ompl/geometric/planners/prm/PRM.h>
#include <ompl/geometric/planners/stride/STRIDE.h>
#include <ompl/tools/benchmark/Benchmark.h>

#include <boost/math/constants/constants.hpp>
#include <boost/format.hpp>

using namespace ompl;

// a 2D line segment
struct Segment
{
    Segment(double p0_x, double p0_y, double p1_x, double p1_y)
        : x0(p0_x), y0(p0_y), x1(p1_x), y1(p1_y)
    {
    }
    double x0, y0, x1, y1;
};

// the robot and environment are modeled both as a vector of segments.
typedef std::vector<Segment> Environment;

// simply use a random projection
class KinematicChainProjector : public ompl::base::ProjectionEvaluator
{
public:
    KinematicChainProjector(const ompl::base::StateSpace *space)
        : ompl::base::ProjectionEvaluator(space)
    {
        int dimension = std::max(2, (int)ceil(log((double) space->getDimension())));
        projectionMatrix_.computeRandom(space->getDimension(), dimension);
    }
    virtual unsigned int getDimension(void) const
    {
        return projectionMatrix_.mat.size1();
    }
    void project(const ompl::base::State *state, ompl::base::EuclideanProjection &projection) const
    {
        std::vector<double> v(space_->getDimension());
        space_->copyToReals(v, state);
        projectionMatrix_.project(&v[0], projection);
    }
protected:
    ompl::base::ProjectionMatrix projectionMatrix_;
};


class KinematicChainSpace : public ompl::base::CompoundStateSpace
{
public:
    KinematicChainSpace(unsigned int numLinks, double linkLength, Environment *env = NULL)
        : ompl::base::CompoundStateSpace(), linkLength_(linkLength), environment_(env)
    {
        for (unsigned int i = 0; i < numLinks; ++i)
            addSubspace(ompl::base::StateSpacePtr(new ompl::base::SO2StateSpace()), 1.);
        lock();
    }

    void registerProjections()
    {
        registerDefaultProjection(ompl::base::ProjectionEvaluatorPtr(new KinematicChainProjector(this)));
    }

    double distance(const ompl::base::State *state1, const ompl::base::State *state2) const
    {
        const StateType *cstate1 = state1->as<StateType>();
        const StateType *cstate2 = state2->as<StateType>();
        double theta1 = 0., theta2 = 0., dx = 0., dy = 0., dist = 0.;

        for (unsigned int i = 0; i < getSubspaceCount(); ++i)
        {
            theta1 += cstate1->as<ompl::base::SO2StateSpace::StateType>(i)->value;
            theta2 += cstate2->as<ompl::base::SO2StateSpace::StateType>(i)->value;
            dx += cos(theta1) - cos(theta2);
            dy += sin(theta1) - sin(theta2);
            dist += sqrt(dx * dx + dy * dy);
        }
        return dist * linkLength_;
    }
    double linkLength() const
    {
        return linkLength_;
    }
    const Environment* environment() const
    {
        return environment_;
    }

protected:
    double linkLength_;
    Environment* environment_;
};


class KinematicChainValidityChecker : public ompl::base::StateValidityChecker
{
public:
    KinematicChainValidityChecker(const ompl::base::SpaceInformationPtr &si)
        : ompl::base::StateValidityChecker(si)
    {
    }

    bool isValid(const ompl::base::State *state) const
    {
        const KinematicChainSpace* space = si_->getStateSpace()->as<KinematicChainSpace>();
        const KinematicChainSpace::StateType *s = state->as<KinematicChainSpace::StateType>();
        unsigned int n = si_->getStateDimension();
        Environment segments;
        double linkLength = space->linkLength();
        double theta = 0., x = 0., y = 0., xN, yN;

        segments.reserve(n + 1);
        for(unsigned int i = 0; i < n; ++i)
        {
            theta += s->as<ompl::base::SO2StateSpace::StateType>(i)->value;
            xN = x + cos(theta) * linkLength;
            yN = y + sin(theta) * linkLength;
            segments.push_back(Segment(x, y, xN, yN));
            x = xN;
            y = yN;
        }
        xN = x + cos(theta) * 0.001;
        yN = y + sin(theta) * 0.001;
        segments.push_back(Segment(x, y, xN, yN));
        return selfIntersectionTest(segments)
            && environmentIntersectionTest(segments, *space->environment());
    }

protected:
    // return true iff env does *not* include a pair of intersecting segments
    bool selfIntersectionTest(const Environment& env) const
    {
        for (unsigned int i = 0; i < env.size(); ++i)
            for (unsigned int j = i + 1; j < env.size(); ++j)
                if (intersectionTest(env[i], env[j]))
                    return false;
        return true;
    }
    // return true iff no segment in env0 intersects any segment in env1
    bool environmentIntersectionTest(const Environment& env0, const Environment& env1) const
    {
        for (unsigned int i = 0; i < env0.size(); ++i)
            for (unsigned int j = 0; j < env1.size(); ++j)
                if (intersectionTest(env0[i], env1[j]))
                    return false;
        return true;
    }
    // return true iff segment s0 intersects segment s1
    bool intersectionTest(const Segment& s0, const Segment& s1) const
    {
        // adopted from:
        // http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/1201356#1201356
        double s10_x = s0.x1 - s0.x0;
        double s10_y = s0.y1 - s0.y0;
        double s32_x = s1.x1 - s1.x0;
        double s32_y = s1.y1 - s1.y0;
        double denom = s10_x * s32_y - s32_x * s10_y;
        if (fabs(denom) < std::numeric_limits<double>::epsilon())
            return false; // Collinear
        bool denomPositive = denom > 0;

        double s02_x = s0.x0 - s1.x0;
        double s02_y = s0.y0 - s1.y0;
        double s_numer = s10_x * s02_y - s10_y * s02_x;
        if ((s_numer < std::numeric_limits<float>::epsilon()) == denomPositive)
            return false; // No collision
        double t_numer = s32_x * s02_y - s32_y * s02_x;
        if ((t_numer < std::numeric_limits<float>::epsilon()) == denomPositive)
            return false; // No collision
        if (((s_numer - denom > -std::numeric_limits<float>::epsilon()) == denomPositive)
            || ((t_numer - denom > std::numeric_limits<float>::epsilon()) == denomPositive))
            return false; // No collision
        return true;
    }
};


Environment createHornEnvironment(unsigned int d, double eps)
{
    std::ofstream envFile("/inputs/environment.dat");
    std::vector<Segment> env;
    double w = 1. / (double)d, x = w, y = -eps, xN, yN, theta = 0.,
        scale = w * (1. + boost::math::constants::pi<double>() * eps);

    envFile << x << " " << y << std::endl;
    for(unsigned int i = 0; i < d - 1; ++i)
    {
        theta += boost::math::constants::pi<double>() / (double) d;
        xN = x + cos(theta) * scale;
        yN = y + sin(theta) * scale;
        env.push_back(Segment(x, y, xN, yN));
        x = xN;
        y = yN;
        envFile << x << " " << y << std::endl;
    }

    theta = 0.;
    x = w;
    y = eps;
    envFile << x << " " << y << std::endl;
    scale = w * (1.0 - boost::math::constants::pi<double>() * eps);
    for(unsigned int i = 0; i < d - 1; ++i)
    {
        theta += boost::math::constants::pi<double>() / d;
        xN = x + cos(theta) * scale;
        yN = y + sin(theta) * scale;
        env.push_back(Segment(x, y, xN, yN));
        x = xN;
        y = yN;
        envFile << x << " " << y << std::endl;
    }
    envFile.close();
    return env;
}


void run();

std::string workingDir = QDir::currentPath().toStdString();
int main(int argc, char** argv) {
    bool foundFiles = false;

    // Test if the build directory is KinematicChain. If so,
    // the path to the current working directory will
    // include /KinematicChain
    unsigned long indexOfDesiredDir = workingDir.rfind("/KinematicChain/");
    if (indexOfDesiredDir != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 5);

        // Set current working directory to KinematicChain
        QDir::setCurrent(workingDir.c_str());
        foundFiles = true;
    }

    // Test if program was downloaded from Github, and is the build directory.
    // Downloading it from Github will result in the folder having the name
    // /KinematicChain-master instead of /KinematicChain
    if (!foundFiles && (indexOfDesiredDir = workingDir.rfind("/KinematicChain/")) != std::string::npos) {
        workingDir = workingDir.substr(0, indexOfDesiredDir + 12);

        // Set current working directory to /KinematicChain-master
        QDir::setCurrent(workingDir.c_str());
        foundFiles = true;
    }

    // Test if a build directory (/build-KinematicChain-...) was created. This directory
    // will reside in the same directory as /KinematicChain
    if (!foundFiles && (indexOfDesiredDir = workingDir.rfind("/build-KinematicChain")) != std::string::npos) {
        QDir dir(workingDir.substr(0, indexOfDesiredDir).c_str());

        if (dir.exists("KinematicChain/KinematicChain.pro")) { // Test if /KinematicChain exists
            workingDir = workingDir.substr(0, indexOfDesiredDir) + "/KinematicChain";

            // Set current working directory to KinematicChain
            QDir::setCurrent(workingDir.c_str());
            foundFiles = true;
        }
    }

    // /KinematicChain could not be found
    if (!foundFiles) {
        std::cerr << std::endl << "!! WARNING !!\n"
        << "The program may not work correctly or at all because the folder "
        "containing the program's files cannot be found.\n"
        "Make sure that the program is inside of a folder named \"KinematicChain\".\n";
    }

    //QApplication app(argc, argv);
    //window = new MainWindow();
    run();

    //window->show();

    //return app.exec();
    return 0;
}

void run() {

    unsigned int numLinks = boost::lexical_cast<unsigned int>(3);
    Environment env = createHornEnvironment(numLinks, log((double)numLinks) / (double)numLinks);
    ompl::base::StateSpacePtr chain(new KinematicChainSpace(numLinks, 1. / (double)numLinks, &env));
    ompl::geometric::SimpleSetup ss(chain);

    ss.setStateValidityChecker(ompl::base::StateValidityCheckerPtr(
        new KinematicChainValidityChecker(ss.getSpaceInformation())));

    ompl::base::ScopedState<> start(chain), goal(chain);
    std::vector<double> startVec(numLinks, boost::math::constants::pi<double>() / (double)numLinks);
    std::vector<double> goalVec(numLinks, 0.);

    startVec[0] = 0.;
    goalVec[0] = boost::math::constants::pi<double>() - .001;
    chain->setup();
    chain->copyFromReals(start.get(), startVec);
    chain->copyFromReals(goal.get(), goalVec);
    ss.setStartAndGoalStates(start, goal);

    // SEKRIT BONUS FEATURE:
    // if you specify a second command line argument, it will solve the
    // problem just once with STRIDE and print out the solution path.
//    if (argc > 2)
//    {
//        ss.setPlanner(ompl::base::PlannerPtr(new ompl::geometric::STRIDE(ss.getSpaceInformation())));
//        ss.setup();
//        ss.print();
//        ss.solve(3600);
//        ss.simplifySolution();

//        ompl::geometric::PathGeometric path = ss.getSolutionPath();
//        std::vector<double> v;
//        for(unsigned int i = 0; i < path.getStateCount(); ++i)
//        {
//            chain->copyToReals(v, path.getState(i));
//            std::copy(v.begin(), v.end(), std::ostream_iterator<double>(std::cout, " "));
//            std::cout << std::endl;
//        }
//        exit(0);
//    }

    // by default, use the Benchmark class
    double runtime_limit = 60, memory_limit = 1024;
    int run_count = 20;
    ompl::tools::Benchmark::Request request(runtime_limit, memory_limit, run_count, 0.5);
    ompl::tools::Benchmark b(ss, "KinematicChain");
    b.addExperimentParameter("num_links", "INTEGER", std::to_string(numLinks));

    b.addPlanner(ompl::base::PlannerPtr(new ompl::geometric::STRIDE(ss.getSpaceInformation())));
    b.addPlanner(ompl::base::PlannerPtr(new ompl::geometric::EST(ss.getSpaceInformation())));
    b.addPlanner(ompl::base::PlannerPtr(new ompl::geometric::KPIECE1(ss.getSpaceInformation())));
    b.addPlanner(ompl::base::PlannerPtr(new ompl::geometric::RRT(ss.getSpaceInformation())));
    b.addPlanner(ompl::base::PlannerPtr(new ompl::geometric::PRM(ss.getSpaceInformation())));
    b.benchmark(request);
    b.saveResultsToFile(boost::str(boost::format("kinematic_%i.log") % numLinks).c_str());

    exit(0);
}
