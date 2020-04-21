%{
#include <iostream>
#include <map>
#include "poly_types.h"

using namespace std;

int yylex();
int yyerror(const char *p) { std::cerr << "error: " << p << std::endl; };

bipoly* poly = new bipoly();

void add_mono(bipoly* poly, mon& mono) {
    map<int, double>* x_entry;
    
    if (poly->find(mono.xpow) == poly->end()) {
      poly->insert(pair<int, map<int, double>*>(mono.xpow, new map<int, double>()));
    }

    x_entry = (*poly)[mono.xpow];
    if (x_entry->find(mono.ypow) == x_entry->end()) {
      x_entry->insert(pair<int, double>(mono.ypow, 0.0));
    }

    (*x_entry)[mono.ypow] += mono.coeff;
}

%}

%union {
    double num;
    int pow;
    mon_inside mon_inside_;
    mon mon_;
    bipoly* bipoly_;
};

%start poly

%token PLUS POW
%token X Y
%token <num> NUM

%type <bipoly_>	    poly
%type <pow>	    x_pow y_pow
%type <mon_>	    mono	
%type <mon_inside_> mono_inside

%%

poly : mono            { add_mono(poly, $1); $$ = poly; }
     | poly PLUS mono  { add_mono($1, $3); $$ = $1; }
;

mono : NUM mono_inside           { $$ = {$1, $2.xpow, $2.ypow};  }
       	| 	mono_inside      { $$ = {1.0, $1.xpow, $1.ypow}; }
       	| 	NUM              { $$ = {$1, 0, 0};              }
;

mono_inside : x_pow            { $$ = {$1, 0}; }
	|     y_pow            { $$ = {0, $1}; }
	|     x_pow y_pow      { $$ = {$1, $2}; }
        |     y_pow x_pow      { $$ = {$2, $1}; }
;

x_pow : X POW NUM     { $$ = (int) $3; }
;

y_pow : Y POW NUM     { $$ = (int) $3; }
;

%%

int main()
{
    yyparse();
    return 0;
}
