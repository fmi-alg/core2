/*****************************************************************************
 *
 *  Verify decimal digits of evaluation
 *
 *      verify-digits   [prec]  [loops]
 *
 ****************************************************************************/

#include "agm.h"

using namespace CORE;
using namespace std;

// functors
struct Exp {
    Exp(const NT& v) : value(v) 
    { }
    NT operator() (int prec) 
    { return exp(value, prec); }
private:
    NT  value;
};

struct Log {
    Log(const NT& v) : value(v) 
    { }
    NT operator() (int prec) 
    { return log(value, prec); }
private:
    NT  value;
};

struct Sin {
    Sin(const NT& v) : value(v) 
    { }
    NT operator() (int prec) 
    { return sin(value, prec); }
private:
    NT  value;
};

struct Cos {
    Cos(const NT& v) : value(v) 
    { }
    NT operator() (int prec) 
    { return cos(value, prec); }
private:
    NT  value;
};

struct Tan {
    Tan(const NT& v) : value(v) 
    { }
    NT operator() (int prec) 
    { return tan(value, prec); }
private:
    NT  value;
};

// verify routine
template <class EvalFunction>
void verify(int prec, int loops, EvalFunction f) {
	int bprec = digit2bit(prec);

    NT old_f, new_f;
    extLong msb;
    old_f = f(bprec);
        
	for (int i=1; i<=loops; i++) {
        cout << bprec*i << ":\t" << setprecision(prec*i) << old_f << endl;
		new_f = f(bprec*(i+1));
        msb = (new_f - old_f).uMSB();
        if (-msb < bprec*i)
            cout << "error!\t";
        else
            cout << "correct!\t";
           
        cout << "del.uMSB=" << msb << "\n" << endl;
        old_f = new_f;
	}
}

int main(int args, char** argv) {
	int prec = 100;		// precision in digits
	int loops = 5;     // loops for self-check
    
    if (args > 1)
		prec = atoi(argv[1]);
    if (args > 2)
        loops = atoi(argv[2]);

    cout << setprecision(prec);

    /*
    cout << "test exp(): " << endl;
    verify(prec, loops, Exp(-0.5));
    verify(prec, loops, Exp(0.5));
    verify(prec, loops, Exp(1.0));
    verify(prec, loops, Exp(1.1));
    verify(prec, loops, Exp(1.2));
    verify(prec, loops, Exp(1.3));
    verify(prec, loops, Exp(1.4));
    verify(prec, loops, Exp(1.5));
    verify(prec, loops, Exp(1.6));
    verify(prec, loops, Exp(1.7));
    verify(prec, loops, Exp(1.8));
    verify(prec, loops, Exp(1.9));
    verify(prec, loops, Exp(2.0));
    verify(prec, loops, Exp(100));    
    
     
    cout << "test log(): " << endl;
    verify(prec, loops, Log(0.5)); 
    verify(prec, loops, Log(1.0));
    verify(prec, loops, Log(1.5));
    verify(prec, loops, Log(2.0));
    cout << "log(3)=";
    verify(prec, loops, Log(3.0));
    cout << "log(6)=";
    verify(prec, loops, Log(6.0));    
    cout << "log(9)=";
    verify(prec, loops, Log(9.0));
    cout << "log(100)=";
    verify(prec, loops, Log(100));
    */
   

    verify(prec, loops, Log(4.004));  // failed
    cout << "test sin(): " << endl;
//    verify(prec, loops, Sin(-0.5));
    verify(prec, loops, Sin(100));
    cout << "test cos(): " << endl;
    verify(prec, loops, Cos(-0.5));
    verify(prec, loops, Cos(100));
    cout << "test tan(): " << endl;
    verify(prec, loops, Tan(-0.5));
    verify(prec, loops, Tan(100));
    
    //verify(prec, loops, Sin(0.5));
    //verify(prec, loops, Sin(1.0));
    //verify(prec, loops, Sin(1.5));
    //verify(prec, loops, Sin(2.0));
    //verify(prec, loops, Sin(100));    
    
    return 0;
}
