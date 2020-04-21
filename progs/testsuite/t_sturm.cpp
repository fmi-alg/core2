#define CORE_NO_AUTOMATIC_NAMESPACE
#ifdef CORE_LEVEL
#undef CORE_LEVEL
#define CORE_LEVEL 4
#endif

#include <CORE/CORE.h>
#include <CORE/poly/Poly.h>
#include <iostream>

using namespace std;

int main()
{
    CORE::BigRat c[9];

    c[0]=CORE::BigRat("1225969745589853940699882447880155625/664613997892457936451903530140172288");

    c[1]=CORE::BigRat("-149192919905533219658325090294590900625/2658455991569831745807614120560689152");

    c[2]=CORE::BigRat("-176473560706138356181046066127379288725/2658455991569831745807614120560689152");

    c[3]=CORE::BigRat("-59323957331081368745121550919931250845/10633823966279326983230456482242756608");

    c[4]=CORE::BigRat("213976196148401335217623674372070736157/10633823966279326983230456482242756608");

    c[5]=CORE::BigRat("10107182444809889626671506586504631263/5316911983139663491615228241121378304");

    c[6]=CORE::BigRat("-12967429119484147645038507190192518603/10633823966279326983230456482242756608");

    c[7]=CORE::BigRat("-894074013945673684338239220768251181/10633823966279326983230456482242756608");

    c[8]=CORE::BigRat("963347446801307512325971073199501/5316911983139663491615228241121378304");

    CORE::Polynomial<CORE::BigRat> poly(8, c);

    cout << "Polynomial of degree 8 : " << poly << endl;

    poly.contract();

    CORE::Sturm<CORE::BigRat> my_sturm(poly);
    int nb_roots=my_sturm.numberOfRoots(-1000, 1000);

    cout << "Number of Real Roots = " << nb_roots<< endl;

    for (int k=1;k<=nb_roots;++k){
        CORE::BFInterval bfi_sturm = my_sturm.isolateRoot(k, -1000, 1000);
        cout << "the " + CORE::numberToString(k)
	    		+ "-th isolating interval is ["
	    << bfi_sturm.first << ", " << bfi_sturm.second << "]" << endl;

        CORE::Expr res=CORE::rootOf(poly, bfi_sturm);
        cout <<  "   -- this root is "<< res << endl;
    }
   
    return 0;
}
