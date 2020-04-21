//
//  main.cpp
//  Sub-div-Solver based on original CORElib
//
//  Created by Zhaoji Wang on 30/01/2018.
//  Copyright © 2018 Zhaoji Wang. All rights reserved.
//

#ifndef CORE_LEVEL
#   define CORE_LEVEL 4  // N=1,2,3.  Defaults to N=3 if omitted
#endif
#include "Curves.h"
#include "box.h"
#include <queue>
#include "src.h"
#include <ctime>
/*
 *********************************************************************
              Sub-div-Solver based on original CORE 2
 *********************************************************************
 */

using namespace std;

int main(int argc, const char *argv[]){
   
    
    clock_t begin,end;
    begin=clock();
    string poly1 = "x^2+y^2-3";
    if(argc >1) poly1 = argv[1];
    string poly2 = "x^3-y";
    if(argc >2) poly2 = argv[2];
    double x_lower = -1;
    if(argc >3) x_lower = atof(argv[3]);
    double x_upper = 1;
    if(argc >4) x_upper = atof(argv[4]);
    double y_lower = -1;
    if(argc >5) y_lower = atof(argv[5]);
    double y_upper = 1;
    if(argc >6) y_upper = atof(argv[6]);
    double epsilon = 0.0001;
    if(argc >7) epsilon = atof(argv[7]);
    
    
    CORE::BiPoly<double>* poly_system = new CORE::BiPoly<double>[2];
    poly_system[0] = poly1;
    poly_system[1] = poly2;
    
    CORE::IntervalT<double> xRegion(x_lower, x_upper),
                    yRegion(y_lower, y_upper); 
    BoxT<double>* B_0=new BoxT<double>(0, xRegion, yRegion);
    
    print_poly_system(poly_system);
    print_box_region(B_0);
    
    Stats stats; 
    Stats_gs stats_gs; 
    queue< BoxT<double> > P, P_gs;

#ifndef GS
    begin=clock();
    subdivision_solver(P, B_0, poly_system, stats);
    end=clock();
    double total_time_subdiv=(double)(end-begin)/CLOCKS_PER_SEC;
    
    output_box(P," isolating ");
    cout << endl;
    cout << "Timing: " << total_time_subdiv << " seconds " << endl;
    cout << endl;
    cout << stats << endl;
#else
    cout << endl;
    begin = clock();
    gs_solver(P_gs, B_0, poly_system, stats_gs);
    end = clock();
    output_box(P_gs," isolating ");
    cout << endl;
    cout << "Timing for gs_solver: " << (double)(end-begin)/CLOCKS_PER_SEC
        << endl;
    cout << endl;
    cout << stats_gs << endl;
#endif

//	system("pause");
    return 0;
}


//Zhaoji at 2018.1.30
//problems:
//1*******. Seem I could only use precision to control precision(1-16)? the other way is using "Expr.r_approx(prec_of_output)", I need to modify the functions ----
//      "toString" in poly.h    and     "<<" in Interval.h
// prec_of_output is things like:
//       "digits2bits(unsigned long digits)"
// see "t_expr.cpp" about the way to print "Expr"


//2*******. my CORE could not execute Level 1! (see tutorial.pdf P7)
