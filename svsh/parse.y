%{
/*

*/
#include <stdio.h>
#include "shell_commands.h"
#include "llist.h"
#define SYS_SAVE_VAR 315
#define SYS_GET_VAR 316
static char* prompt = "svsh";
%}
%define parse.error verbose
%token EQ WHITESPACE COMMENT DEFPROMPT ASSIGNTO CD LISTJOBS BYE RUN BG NEWLINE
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
	| error NEWLINE { printf("error\n%s > ", prompt); }
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
	DEFPROMPT WHITESPACE arg {
		//free(prompt);
		prompt = cmd_defprompt($3);
	}
	;

cd:
	CD WHITESPACE arg {
		cmd_cd($3);
	}
	;

assign:
	VARIABLE WHITESPACE EQ WHITESPACE arg {
		struct token_t *var = tk_new(VARIABLE, $1);
		cmd_assign(var, $5);
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
	| arglist WHITESPACE arg {
		struct llist_t *prev = $1;
		struct llist_t *nval = ll_new($3);
		$$ = ll_append(prev, nval);
	}
	;
arg:
	WORD { $$ = tk_new(WORD, $1); }
	| VARIABLE { $$ = tk_new(VARIABLE, $1); }
	| STRING {$$ = tk_new(STRING, $1); }
	;

run:
	RUN WHITESPACE WORD WHITESPACE arglist {
		struct token_t *cmd = tk_new(WORD, $3);
		cmd_run(cmd, $5, 0);
	}

	| RUN WHITESPACE WORD WHITESPACE arglist WHITESPACE BG {
		struct token_t *cmd = tk_new(WORD, $3);
		cmd_run(cmd, $5, 1);
	}
	| RUN WHITESPACE WORD{
		struct token_t *cmd = tk_new(WORD, $3);
		cmd_run(cmd, NULL, 0);
	}
	| RUN WHITESPACE WORD WHITESPACE BG{
		struct token_t *cmd = tk_new(WORD, $3);
		cmd_run(cmd, NULL, 1);
	}
	| WORD WHITESPACE arglist WHITESPACE BG {
		struct token_t *cmd = tk_new(WORD, $1);
		cmd_run(cmd, $3, 1);
	}
	| WORD WHITESPACE arglist{
		struct token_t *cmd = tk_new(WORD, $1);
		cmd_run(cmd, $3, 0);
	}
	;
assignto: ASSIGNTO WHITESPACE VARIABLE WHITESPACE WORD WHITESPACE arglist {
		struct token_t *var = tk_new(VARIABLE, $3);
		struct token_t *cmd = tk_new(WORD, $5);
		cmd_assignto(var, cmd, $7); // add arglist
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
	atexit(cmd_bye);
	syscall(SYS_SAVE_VAR, "$PATH", "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games");
//	char var_definition[1000];
//	syscall(SYS_GET_VAR, "$PATH", var_definition, 1000);
//	printf("%s\n",var_definition);	
	printf("svsh > ");
	yyparse();
}
