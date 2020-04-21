%{
#include <iostream>
#include <stdlib.h>
#include "poly_types.h"
#include "poly.tab.h"
%}

%option noyywrap

%%

                        /* 'val' in 'yylval.val' is the field declared in
                           calc2.y with the %union directive. */
([0-9]+(\.?)[0-9]*)|([0-9]+(\.?)[0-9]*)     { yylval.num = atof(yytext); return NUM; }
"x"                                         { return X;    }
"y"                                         { return Y;    }
"+"                                         { return PLUS; }
"^"                                         { return POW;  }
[ \t\r\n]+                                  { /* Ignore white space. */ }
.                                           { std::cerr << "Invalid token" << std::endl; exit(1); }

%%
