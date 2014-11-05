%{
#include <stdio.h>
//#include "y.tab.h"
%}

%%
\n { printf("newline\n"); }

^run { printf("RUN\n"); }

[a-zA-Z+] {
	print("found word \"%s\"\n", yytext);
}
%%
int main(int argc, char**argv) {
	yylex();
}