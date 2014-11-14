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
static char* prompt = "svsh";
%}
%define parse.error verbose
%token EQ COMMENT DEFPROMPT ASSIGNTO CD LISTJOBS BYE RUN BG NEWLINE
%union {
	char *string;
	struct token_t *token;
	struct llist_t *llist;
}
%token <string> STRING
%token <string> WORD
%token <string> VARIABLE 
%type <token> arg
%type <llist> arglist
%%
program:
	line
	| program line
	;
line:
	command NEWLINE { printf("%s > ", prompt); }
	| error NEWLINE { printf("error\n%s> ", prompt); }
	;
command:
	defprompt
	| cd
	| assign
	| listjobs
	| bye
	| run
	| assignto
	| comment
	; 
defprompt:
	DEFPROMPT arg {
		//free(prompt);
		prompt = cmd_defprompt($2);
	}
	;

cd:
	CD arg {
		cmd_cd($2);
	}
	;

assign:
	VARIABLE EQ arg {
		struct token_t *var = tk_new(VARIABLE, $1);
		cmd_assign(var, $3);
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
	WORD { $$ = tk_new(WORD, $1); }
	| VARIABLE { $$ = tk_new(VARIABLE, $1); }
	| STRING {$$ = tk_new(STRING, $1); }
	;

run:
	RUN WORD arglist {
		struct token_t *cmd = tk_new(WORD, $2);
		cmd_run(cmd, $3, 0); // not sure yet how to handle arglist
	}

	| RUN WORD arglist BG {
		struct token_t *cmd = tk_new(WORD, $2);
		cmd_run(cmd, $3, 1);
	}
	;
assignto: ASSIGNTO VARIABLE WORD arglist {
		struct token_t *var = tk_new(VARIABLE, $2);
		struct token_t *cmd = tk_new(WORD, $3);
		cmd_assignto(var, cmd, $4); // add arglist
	}
	;

comment:	
	COMMENT arglist{
		cmd_comment($2);
	}
	;

%%
int yyerror(char *s)
{
	fprintf(stderr, "%s\n", s);
	return 1;
}
int main(int argc, char **argv) {
	printf("svsh > ");
	yyparse();
}
