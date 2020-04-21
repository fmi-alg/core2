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
#include <CORE/CORE.h>
#include <CORE/poly/Curves.h>
#include <box.h>
#include <queue>
#include "src.h"
#include <ctime>
/*
 *********************************************************************
              Sub-div-Solver based on original CORE 2
 *********************************************************************
 */


int main(){
    
    clock_t begin,end;
    begin=clock();
    
    using namespace std;
    queue< BoxT<Expr> > P,Refine_P_v1,Refine_P_v2;
    BiPoly<Expr>* poly_system=get_poly_system<Expr>();
    print_poly_system(poly_system);
    BoxT<Expr>* B_0=get_box_region<Expr>();
    print_box_region(B_0);
    Expr epsilon=get_precision();
    
    
    begin=clock();
    subdivision_solver(P, B_0, poly_system);
    end=clock();
    Expr total_time_subdiv=(Expr)(end-begin)/CLOCKS_PER_SEC;
    
    cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Procedure of Refinement V1 %%%%%%%%%%%%%%%%%%%%%%%"<<endl;
    begin=clock();
    refine_solver_v1(P, Refine_P_v1, poly_system, epsilon);
    end=clock();
    Expr total_time_refine_v1=(Expr)(end-begin)/CLOCKS_PER_SEC;
    
    
    cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Procedure of Refinement V2 %%%%%%%%%%%%%%%%%%%%%%%"<<endl;
    begin=clock();
    refine_solver_v2(P, Refine_P_v2, poly_system, epsilon);
    end=clock();
    Expr total_time_refine_v2=(Expr)(end-begin)/CLOCKS_PER_SEC;
    
    
    output_box(P," isolated ");
    output_box(Refine_P_v1," refined(v1) ");
    output_box(Refine_P_v2," refined(v2) ");
    
    delete[] poly_system;
    delete B_0;

    cout<<endl<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl<<"   "<<total_time_subdiv<<" seconds used for subdivision"<<endl<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
    cout<<endl<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl<<"   "<<total_time_refine_v1<<" seconds used for refinement V1"<<endl<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
    cout<<endl<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl<<"   "<<total_time_refine_v2<<" seconds used for refinement V2"<<endl<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
}


//Zhaoji at 2018.1.30
//problems:
//1*******. Seem I could only use precision to control precision(1-16)? the other way is using "Expr.r_approx(prec_of_output)", I need to modify the functions ----
//      "toString" in poly.h    and     "<<" in Interval.h
// prec_of_output is things like:
//       "digits2bits(unsigned long digits)"
// see "t_expr.cpp" about the way to print "Expr"


//2*******. my CORE could not execute Level 1! (see tutorial.pdf P7)
