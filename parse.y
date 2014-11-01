%{
#include <stdio.h>
char **lastLine = NULL;
%}

%token EQ COMMENT DEFPROMPT ASSIGNTO CD LISTJOBS BYE RUN BG

%union {
	char *str;
}
%token <str> STRING
%token <str> WORD
%token <str> VARIABLE 
%type <str> arg
%%
command:
	defprompt
	| cd
	| assign
	| listjobs
	| bye
	| run
	| assignto
	; 
defprompt:
	DEFPROMPT arg {
		printf("command<defprompt>(%s)\n", $2);
	}
	;

cd:
	CD arg {
		printf("command<cd>(%s)\n", $2);
	}
	;

assign:
	VARIABLE EQ arg {
		printf("set %s = %s\n", $1, $3);
	}
	;

listjobs:
	LISTJOBS {
		printf("command<listjobs>()\n");
	}
	;

bye:
	BYE {
		printf("command<bye>()\n");
	}
	;

arglist:
	arg
	| arglist arg
	;
arg:
	WORD { $$ = $1; }
	| VARIABLE { $$ = $1; }
	| STRING {$$ = $1; }
	;

run:
	RUN WORD arglist {
		printf("command<run>(%s, args)\n", $2);
	}
	| RUN WORD arglist BG {
		printf("command<run>(%s, args, bg)\n", $2);
	}
	;
assignto: ASSIGNTO VARIABLE WORD arglist {
		printf("assign %s to %s args", $2, $3);
	}
	;


%%
int yyerror(char *s)
{
	fprintf(stderr, "err %s\n", s);
	return 1;
}
int main(int argc, char **argv) {
	yyparse();
}
