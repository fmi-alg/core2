/* file: zhaoji.cpp
 *
 *	yap@tests[./zhaoji
 *	oDigits:10
 *	A:1.230912832e+61
 *	B:1.e-61
 *	A-B:1.230912832e+61
 *	A+B:1.230912832e+61
 *	A*B:1.230912832
 *	not equal!!
 *
 *	yap@tests[568] ./zhaoji 20
 *	oDigits:20
 *	A:1.2309128319231290381e+61
 *	B:1.e-61
 *	A-B:1.2309128319231290381e+61
 *	A+B:1.2309128319231290381e+61
 *	A*B:1.23091283192313
 *	not equal!!
 *
 *	yap@tests[569] ./zhaoji 30
 *	oDigits:30
 *	A:1.23091283192312903812903810924e+61
 *	B:1.e-61
 *	A-B:1.23091283192312903812903810924e+61
 *	A+B:1.23091283192312903812903810924e+61
 *	A*B:1.23091283192313
 *	not equal!!
 *
 *	yap@tests[571] ./zhaoji 70
 *	oDigits:70
 *	A:1.230912831923129038129038109239812903819023819203819023810924e+61
 *	B:1.e-61
 *	A-B:12309128319231290381290381092398129038190238192038190238109238
 *	A+B:12309128319231290381290381092398129038190238192038190238109238
 *	A*B:1.23091283192313
 *	not equal!!
 *	
 * 	Chee: 1/26/18
 *************************************************** */

#include <iostream>

using namespace std;

#include "CORE.h"

int main(int argc, char** argv) {

    int oDigits=10;
    if (argc>1) oDigits = atoi(argv[1]);
    cout<< "oDigits:" << oDigits << endl;

    string a1,b1;

    a1="12309128319231290381290381092398129038190238192038190238109238";
    b1="0.0000000000000000000000000000000000000000000000000000000000001";

    // while(cin>>a1>>b1) {
        Expr A(a1),B(b1);

        cout<< setprecision(oDigits) <<
	    	"A:"<<A<<endl;
        cout<< setprecision(oDigits) <<
	    	"B:"<<B<<endl;
        cout<< setprecision(oDigits) <<
	    	"A-B:"<<A-B<<endl;
        cout<< setprecision(oDigits) <<
        	"A+B:"<<A+B<<endl;
        cout<< setprecision(oDigits) <<
        	"A*B:"<<A*B<<endl;

        if (A-B == A+B)
            cout<<"equal!!!!!!!"<<endl;
        else
            cout<<"not equal!!"<<endl;
    // }
}
