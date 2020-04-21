//
//  src.h
//  Sub-div-Solver based on original CORElib
//
//  Created by Zhaoji Wang on 30/01/2018.
//  Copyright © 2018 Zhaoji Wang. All rights reserved.
//

#ifndef src_h
#define src_h
#include "mk-inl.h"
#include "gs-inl.h"
#include <queue>
#include <ctime>

template<class numtype>
inline void print_poly_system(CORE::BiPoly<numtype>* poly_system){
    cout<<"The polynomial system is: " << endl;
    cout<<"   f1 = "<< poly_system[0].toString() << "=0" <<endl;
    cout<<"   f2 = "<< poly_system[1].toString() << "=0" <<endl
        <<endl;
}

template<class numtype>
inline void print_box_region(BoxT<numtype>* Box_region){
    cout<<"The box region is:" << endl;
    cout<< *Box_region << endl << endl;
}

struct Stats
{
    unsigned long int C0_count, MK_count, JC_count;
    clock_t C0_timing, MK_timing, JC_timing;
    Stats():C0_count(0), MK_count(0),JC_count(0),
            C0_timing(0), MK_timing(0), JC_timing(0){};
};

ostream& operator<< (ostream& out, Stats & stats)
{
    out << "Counts of C0 test: " << stats.C0_count << endl;
    out << "Counts of JC test: " << stats.JC_count << endl;
    out << "Counts of MK test: " << stats.MK_count << endl;
    out << "Timing for C0 test: " << (double)stats.C0_timing/CLOCKS_PER_SEC
        << " seconds " << endl;
    out << "Timing for JC test: " << (double)stats.JC_timing/CLOCKS_PER_SEC
        << " seconds " << endl;
    out << "Timing for MK test: " << (double)stats.MK_timing/CLOCKS_PER_SEC
        << " seconds " << endl;
    out << "Average timing per C0 test: " 
        << (double)stats.C0_timing/stats.C0_count/CLOCKS_PER_SEC
        << " seconds " << endl;
    out << "Average timing per JC test: " 
        << (double)stats.JC_timing/stats.JC_count/CLOCKS_PER_SEC
        << " seconds " << endl;
    out << "Average timing per MK test: " 
        << (double)stats.MK_timing/stats.MK_count/CLOCKS_PER_SEC
        << " seconds " << endl;
    return out;
}

struct Stats_gs
{
    unsigned long int reduce_count, split_count, unique_count,
                      reject_count;
    Stats_gs():reduce_count(0), split_count(0),unique_count(0),
               reject_count(0){};
};

ostream& operator<< (ostream& out, Stats_gs& stats_gs)
{
    out << "Number of box produced: " 
        << stats_gs.reduce_count + stats_gs.split_count
           + stats_gs.reject_count + stats_gs.unique_count
        << endl;
    out << "Number of box reduced: " << stats_gs.reduce_count
        << endl;
    out << "Number of box split: " << stats_gs.split_count
        << endl;
    out << "Number of box rejected: " << stats_gs.reject_count
        << endl;
    out << "Number of box containing a root: " << stats_gs.unique_count
        << endl;
    return out;
}


template<class numtype>
void subdivision_solver(queue< BoxT<numtype> > &P,BoxT<numtype>* B_0,CORE::BiPoly<numtype>* poly_system, Stats& stats){
    MKPredicates<numtype, numtype> MKP(poly_system[0],poly_system[1],0.00000001,100,100000);
    int i,size_of_Q;
    queue< BoxT<numtype> > Q;
    BoxT<numtype>* B;
    BoxT<numtype>* B_Dilate_3;
//initialize the queue
    Q.push(*B_0);
    while(Q.empty()!=true)
    {
//take the first box from Q (it is also the largest box)
        B= new BoxT<numtype>(Q.front());
        Q.pop();
        cout << "--------- new box -----------" << endl;
        cout << *B << endl;
        cout << "Q.size() = " << Q.size() << endl;
//if C0(B) fails 
        stats.C0_count++;
        clock_t C0_start = clock();
        bool exclude_test = MKP.Exclude(B);
        clock_t C0_end = clock();
        stats.C0_timing += (C0_end - C0_start);
        if(exclude_test==false)
        {
            cout << "exclude_test==false " << std::endl;
//            if(B->rB < 0.1){
                B_Dilate_3=B->Dilate(3);
    //check whether jacobian condition has previously succeeded,
    //if not, perform the jacobian test            
                if(B->flag_jc==false)
                {
    //if JC(3B) fails then split the box
                    stats.JC_count++;
                    clock_t JC_start = clock();
                    bool JC_test = MKP.JTest(B_Dilate_3);
                    clock_t JC_end = clock();
                    stats.JC_timing += (JC_end - JC_start);
                    if(JC_test==false)
                    {
                        cout << "jacobian condition fails" << std::endl;
                        if(B->Split(0)==true)
                            for(i=0;i<4;i++)
                                Q.push(*(B->pChildren[i]));
                        continue;
                    }
    //if JC(3B) succeeds then record it 
                    B->flag_jc = true;
                }
    //if MK(3B) succeeds then
                stats.MK_count++;
                clock_t MK_start = clock();
                int MK_test = MKP.MKTest(B_Dilate_3);
                clock_t MK_end = clock();
                stats.MK_timing += (MK_end - MK_start);
                if(MK_test==1){
    //put 3B in the output queue
                    cout << "MK test succeeds" << std::endl;
                    P.push(*B_Dilate_3);
    //discard all the boxes contained in 3B
                    size_of_Q=(int)Q.size();
                    for(i=0;i<size_of_Q; i++){
                        if(Contains(*B_Dilate_3, Q.front()))
                            Q.pop();
                        else{
                            Q.push(Q.front());
                            Q.pop();
                        }
                    }
                    continue;
                }   
    //split B into 4 children and add them into Q
//            }
            cout << "MK test succeeds" << std::endl;
            if(B->Split(0)==true)
                for(i=0;i<4;i++)
                    Q.push(*(B->pChildren[i]));
            delete B;
        }
        else{
            delete B;
        }
    }
    
}

// check whether box1 contains box2
template<class numtype>
bool Contains(const BoxT<numtype> & box1, const BoxT<numtype> & box2){
    return (Contains(box1.x_range, box2.x_range) && Contains(box1.y_range, box2.y_range));
}

template<class numtype>
void output_box(queue< BoxT<numtype> > &P,string c){
    int size_of_P=(int)P.size();
    cout << size_of_P << " isolating boxes are founded: " << endl;
    for(int i=0; i<size_of_P; i++){
        cout<< "Box " << i+1 << ": ";
        cout<< P.front() << endl;
        P.push(P.front());
        P.pop();
    }
}


template<class numtype>
void gs_solver(queue< BoxT<numtype> > &P,BoxT<numtype>* B_0,
                        CORE::BiPoly<numtype>* poly_system, Stats_gs& stats)
{
    GSPredicates<numtype, numtype> GSP(poly_system[0],poly_system[1],0.00000001,100,100000);
    int size_of_Q;
    queue<BoxT<numtype> > Q;
    BoxT<numtype>* B;
//initialize the queue
    Q.push(*B_0);
    while(Q.empty()!=true)
    {
        B= new BoxT<numtype>(Q.front());
        cout << "--------- new box -----------" << endl;
        cout << B->x_range << endl;
        cout << B->y_range << endl;
        Q.pop();
        vector<BoxT<numtype>* >* next = new vector<BoxT<numtype>* >;
        cout << "Q.size() = " << Q.size() << endl;
        int result = GSP.GS_iteration(B, next);
        // if result == 1, there is one box in "next", and the uniqueness
        // of root in this box has been confirmed
        if(result == 1){
            stats.unique_count++;
            cout << "size of next (with result == 1) is: " 
                << next -> size() << endl;
            for(size_t j = 0; j < next->size(); j++) 
                P.push(*(*next)[j]);     
        }
        // if result == 0, there may be one or more box in "next"
        else if (result == 0){
            stats.reduce_count++;
            cout << " ==============================================" << endl
                << "size of next (with result == 0) is: " 
                << next -> size() << endl;
            for(size_t j = 0; j < next->size(); j++) 
                Q.push(*(*next)[j]);     
        }
        // if result == -2, split is suggested
        else if (result == -2){
            stats.split_count++;
            cout << " ==============================================" << endl
                << "size of next (with result == -2) is: " 
                << next -> size() << endl;
            if(B->Split(0)==true)
                for(int i=0;i<4;i++)
                    Q.push(*(B->pChildren[i]));
        }
        else 
            stats.reject_count++;
        delete next;
        delete B;
    }
}


#endif
