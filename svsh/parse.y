%{
/*
	Handles parsing.
	Rigth now, argument lists do not work, since I'm hoping I can come up with something better than returning a linked list.

	Maybe we should use this as the entry point? It could simply call functions from another module to run the different commands, which would also improve modularity.
	It also automatically handles multiple line scripts
*/
#include <stdio.h>
#include "shell_commands.h"
#include "llist.h"
%}
%define parse.error verbose
%token EQ COMMENT DEFPROMPT ASSIGNTO CD LISTJOBS BYE RUN BG NEWLINE

%union {
	char *string;
	struct llist_t *llist;
}
%token <string> STRING
%token <string> WORD
%token <string> VARIABLE 
%type <string> arg
%type <llist> arglist;
%%
program:
	line
	| program line
	;
line:
	command NEWLINE
	| error NEWLINE
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
		cmd_defprompt($2);
	}
	;

cd:
	CD arg {
		cmd_cd($2);
	}
	;

assign:
	VARIABLE EQ arg {
		cmd_assign($1, $3);
	}
	;

listjobs:
	LISTJOBS {
		cmd_listjobs();
	}
	;

bye:
	BYE {
		cmd_bye();
	}
	;

arglist:
	arg {
		$$ = ll_new($1);
	}
	| arglist arg {
		struct llist_t *prev = $1;
		struct llist_t *nval = ll_new($2);
		$$ = ll_append(prev, nval);
	}
	;
arg:
	WORD { $$ = $1; }
	| VARIABLE { $$ = $1; }
	| STRING {$$ = $1; }
	;

run:
	RUN WORD arglist {
		cmd_run($2, $3, 0); // not sure yet how to handle arglist
	}
	| RUN WORD arglist BG {
		cmd_run($2, $3, 1);
	}
	;
assignto: ASSIGNTO VARIABLE WORD arglist {
		cmd_assignto($2, $3, $4); // add arglist
	}
	;


%%
int yyerror(char *s)
{
	fprintf(stderr, "%s\n", s);
	return 1;
}
int main(int argc, char **argv) {
	yyparse();
}
