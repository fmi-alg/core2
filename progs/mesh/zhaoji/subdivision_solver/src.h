//
//  src.h
//  Sub-div-Solver based on original CORElib
//
//  Created by Zhaoji Wang on 30/01/2018.
//  Copyright © 2018 Zhaoji Wang. All rights reserved.
//

#ifndef src_h
#define src_h
#ifndef CORE_LEVEL
#   define CORE_LEVEL 3   // N=1,2,3.  Defaults to N=3 if omitted
#endif
#include <CORE/CORE.h>
#include <CORE/poly/Curves.h>
#include <box.h>
#include <mk-inl.h>
#include <queue>

template<class WZJ>
BiPoly<WZJ>* get_poly_system(){
    BiPoly<WZJ>* poly_system= new BiPoly<WZJ>[2];
    cout<<"*************** STEP 1: INPUT the Polynomial System ***************"<<endl;
    string Poly_string_1,Poly_string_2;
    cout<<"   please input the first polynomial of the system:"<<endl;
    cin>>Poly_string_1;
    cout<<"   please input the second polynomial of the system:"<<endl;
    cin>>Poly_string_2;
    poly_system[0]=Poly_string_1;
    poly_system[1]=Poly_string_2;
    return poly_system;
}

template<class WZJ>
inline void print_poly_system(BiPoly<WZJ>* poly_system){
    cout<<"*************** STEP 1 Finish! My polynomial system is ***************"<<endl;
    cout<<"   「"<<poly_system[0].toString()<<"=0"<<endl;
    cout<<"   「"<<poly_system[1].toString()<<"=0"<<endl<<endl;
}

template<class WZJ>
BoxT<WZJ>* get_box_region(){
    cout<<"*************** STEP 2: INPUT the Box region ***************"<<endl;
    string temp1,temp2;
    
    cout<<"    x_lower=";
    cin>>temp1;
    cout<<"    x_upper=";
    cin>>temp2;
    IntervalT<WZJ> XRegion(temp1,temp2);
    cout<<"    y_lower=";
    cin>>temp1;
    cout<<"    y_upper=";
    cin>>temp2;
    IntervalT<WZJ> YRegion(temp1,temp2);
    return ( new BoxT<WZJ>(0,XRegion,YRegion));
    
}

template<class WZJ>
inline void print_box_region(BoxT<WZJ>* Box_region){
    cout<<endl<<"*************** STEP 2 Finish! Box Region is ***************"<<endl;
    cout<<*Box_region<<endl<<endl;
}

Expr get_precision(){
    string precision;
    cout<<"*************** STEP 3 : INPUT precision of OUTPUT refined box region(0~positive infinty) ***************"<<endl;
    cin>>precision;
    cout<<endl<<"*************** STEP 3 Finish! Precision is ***************"<<endl<<precision<<endl<<endl;
    return precision;
}

template<class WZJ>
BoxT<WZJ>* find_the_biggest_box(queue< BoxT<WZJ> > &Q)
{
    //select the biggest box from queue Q and pop it without any change of the queue(we have recovered)
    int size=(int)Q.size(),index=0,i;
    BoxT<WZJ>* result=&Q.front();
    for(i=0;i<size;i++){
        Q.push(Q.front());
        Q.pop();
        if(result->rB < Q.back().rB ){
            result=&Q.back();
            index=i;
        }
    }
    for(i=0;i<index;i++){
        Q.push(Q.front());
        Q.pop();
    }
    BoxT<WZJ>* THE_BIGGEST=new BoxT<WZJ>(Q.front());
    Q.pop();//pop出最大的box
    for(i=index+1;i<size;i++){
        Q.push(Q.front());
        Q.pop();
    }
    return THE_BIGGEST;
}

template<class WZJ>
void subdivision_solver(queue< BoxT<WZJ> > &P,BoxT<WZJ>* B_0,BiPoly<WZJ>* poly_system){
    MKPredicates<WZJ, WZJ> MKP(poly_system[0],poly_system[1],0.00000001,100,100000);
    int i,size_of_Q,size_of_Q1;
    queue< BoxT<WZJ> > Q,Q1;
    BoxT<WZJ>* B;
    BoxT<WZJ>* B_Dilate_3;
    BoxT<WZJ>* B1_Dilate_2;
//initialize the queue, push B_0 to Q
    Q.push(*B_0);
//if Q is not empty, do the following
    while(Q.empty()!=true)
    {
//find and Remove the biggest box from Q
        B=find_the_biggest_box(Q);
//if C0(B) fails then
        if(MKP.Exclude(B)==false)
        {
            B_Dilate_3=B->Dilate(3);
//if JC(3B) succeeds then
            if(MKP.JTest(B_Dilate_3)==true)
            {
//initialize the queue Q1 and push B to Q1
                size_of_Q1=(int)Q1.size();
                for(i=0;i< size_of_Q1;i++)
                {
                    Q1.pop();
                }
                Q1.push(*B);
//if Q1 is not empty, do the following
                while(Q1.empty()!=true){
//pop the first item and assigning it to B1
                    BoxT<WZJ>* B1= new BoxT<WZJ>(Q1.front());
                    Q1.pop();
//if C0(B1) fails then
                    if(MKP.Exclude(B1)==false){
                        B1_Dilate_2=B1->Dilate(2);
//if MK(2B1) succeeds then
                        if(MKP.MKTest(B1_Dilate_2)==true){
//note down 2B1
                            P.push(*B1_Dilate_2);
                            cout<<endl<<"**********Now found   NO."<<P.size()<<" isolated box in output queue-----"<<*B1_Dilate_2<<endl<<endl;
                            cout<<P.front()<<endl;
//discard all the boxes contained in 3B
                            size_of_Q=(int)Q.size();
                            for(i=0;i<size_of_Q; i++){
                                if(lessthan(Q.front(), *B_Dilate_3))
                                    Q.pop();
                                else{
                                    Q.push(Q.front());
                                    Q.pop();
                                }
                            }
                            delete B1_Dilate_2;
                            delete B1;
                            break;
                        }
//split B1 into 4 box and add them to Q1
                        else{
                            if(B1->Split(0)==true){
                                cout<<"INNER::   Split "<<*B1<<endl;
                                for(i=0;i<4;i++){
                                    Q1.push(*(B1->pChildren[i]));
                                }
                            }
                            delete B1_Dilate_2;
                            delete B1;
                        }
                    }
                    else{
                        cout<<"INNER::       Box "<<*B1<<" was excluded" <<endl;
                        delete B1;
                    }
                }
                delete B_Dilate_3;
                delete B;
            }
            else{
                if(B->Split(0)==true){
                    cout<<"OUTER::   Split "<<*B<<endl;
                    for(i=0;i<4;i++){
                        Q.push(*(B->pChildren[i]));
                    }
                }
                delete B_Dilate_3;
                delete B;
                
            }
        }
        else{
            cout<<"OUTER::     Box "<<*B<<" was excluded"<<endl;
            delete B;
        }
    }
    
}

template<class WZJ>
int lessthan(const BoxT<WZJ> &A, const BoxT<WZJ> &other){
    return (Contains(other.x_range, A.x_range) && Contains(other.y_range, A.y_range));
}





template<class WZJ>
void refine_solver_v1(queue< BoxT<WZJ> > P,queue< BoxT<WZJ> > &OUTPUT,BiPoly<WZJ>* poly_system,WZJ &epsilon){
    queue< BoxT<WZJ> > Q;
    int size_of_Q,i;
    WZJ beta= 1+epsilon;
    MKPredicates<WZJ, WZJ> MKP(poly_system[0],poly_system[1],0,0,0);
    
    while(P.empty()!=true)
    {
        BoxT<WZJ>* B=new BoxT<WZJ>(P.front());
        P.pop();
//initial Queue Q
        size_of_Q=(int)Q.size();
        for(i=0;i< size_of_Q;i++)
        {
            Q.pop();
        }
        Q.push(*B);
        delete B;
        while(Q.empty()!=true){
            BoxT<WZJ>* B1=new BoxT<WZJ>(Q.front());
            Q.pop();
            if(MKP.Exclude(B1)==false){
                if((B1->width())>epsilon){
                    if(B1->Split(0)==true){
                        cout<<"V1::Split::  Its too large  : "<<*B1<<endl;
                        for(i=0;i<4;i++){
                            Q.push(*(B1->pChildren[i]));
                        }
                    }
                    delete B1;
                }
                else{
                    if(MKP.MKTest(B1)==true){
                        OUTPUT.push(*B1);
                        cout<<endl<<"**********Now found   NO."<<OUTPUT.size()<<" limited box in output queue-----"<<*B1<<endl<<endl;
                        delete B1;
                        break;
                    }
                    else{
                        if(B1->Split(0)==true){
                            cout<<"V1::Split::  Fail in MKtest   : "<<*B1<<endl;
                            for(i=0;i<4;i++){
                                Q.push(*(B1->pChildren[i])->Dilate(beta));
//It's necessary to execute a "Dilate", terminating the program!
                            }
                        }
                        delete B1;
                    }
                }
            }
            else{
                delete B1;
            }
        }
    }
}

template<class WZJ>
void refine_solver_v2(queue< BoxT<WZJ> > P,queue< BoxT<WZJ> > &OUTPUT,BiPoly<WZJ>* poly_system,WZJ &epsilon){
    queue< BoxT<WZJ> > Q;
    int size_of_Q,i;
    WZJ beta= 1+epsilon;
    MKPredicates<WZJ, WZJ> MKP(poly_system[0],poly_system[1],0,0,0);
    while(P.empty()!=true)
    {
        BoxT<WZJ>* B=new BoxT<WZJ>(P.front());
        P.pop();
//initial Queue Q
        size_of_Q=(int)Q.size();
        for(i=0;i< size_of_Q;i++)
        {
            Q.pop();
        }
        Q.push(*B);
        delete B;
        while(Q.empty()!=true){
            BoxT<WZJ>* B1=new BoxT<WZJ>(Q.front());
            Q.pop();
            if(MKP.Exclude(B1)==false){
                if(MKP.MKTest(B1)==true){
//discard all box in Q1 because we alread found the only roots.
                    size_of_Q=(int)Q.size();
                    for(i=0;i< size_of_Q;i++)
                    {
                        Q.pop();
                    }
// if the size of box match condition, then output it.
                    if(B1->width()<epsilon){
                        OUTPUT.push(*B1);
                        cout<<endl<<"**********Now found   NO."<<OUTPUT.size()<<" limited box in output queue-----"<<*B1<<endl<<endl;
                        delete B1;
                        break;
                    }
                    else{
                        if(B1->Split(0)==true){
                            cout<<"V2::Split::  Its too large, though s.t. MKtest  : "<<*B1<<endl;
                            for(i=0;i<4;i++){
                                Q.push(*(B1->pChildren[i]));
                            }
                            delete B1;
                        }
                    }
                }
                else{
                    if(B1->Split(0)==true){
                        cout<<"V2::Split::  Fail in MKtest   : "<<*B1<<endl;
                        for(i=0;i<4;i++){
                            Q.push(*(B1->pChildren[i])->Dilate(beta));
                        }
                    }
                    delete B1;
                }
            }
            else{
                delete B1;
            }
        }
    }
}

template<class WZJ>
void output_box(queue< BoxT<WZJ> > &P,string c){
    int size_of_P=(int)P.size();
    cout<<endl<<"***************The Solver find "<<size_of_P<<c<<"box finally***************"<<endl;
    for(int i=0;i<size_of_P;i++){
        cout<<i+1<<": ";
        cout<<P.front()<<endl;
        P.push(P.front());
        P.pop();
    }
}


#endif /* src_h */
