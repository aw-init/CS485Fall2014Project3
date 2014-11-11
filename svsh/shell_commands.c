#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "llist.h"
#include "shell_commands.h"

#define SYS_BUFFERSIZE 1000

// replace this later with environment variable call
int ShowTokens = 1;

void PrintToken(struct token_t *token, char *usage)
{
	switch (token->ttype) {
		
		case STRING:
			char *stype = "string";
			break;
		}
		case VARIABLE:
			char *stype = "variable";
			break;
		case WORD:
			char *stype = "word";
			break;
		case EQ:
		case COMMENT:
			char *stype = "metachar";
			break;
		case DEFPROMPT:
		case ASSIGNTO:
		case CD:
		case LISTJOBS:
		case BYE:
		case RUN:
		case BG:
			char *stype = "keyword";
			break;
	}
}

void cmd_listjobs()
{
	if (ShowTokens) {
		printf("Token Type = %s\tToken = %s\tUsage = %s\n", "keyword", "listjobs", "listjob");     
	}
	printf("listing currently running jobs\n");
}

void cmd_defprompt(struct token_t *nprompt)
{
	printf("set new prompt to %s\n", nprompt->value);
	tk_free(nprompt);
}
void cmd_cd(struct token_t *path)
{
	printf("change directory to %s\n", path->value);
	tk_free(path);
}
void cmd_assign(struct token_t *varname, struct token_t *vardef)
{
	printf("set %s = %s\n", varname->value, vardef->value);
	tk_free(varname);
	tk_free(vardef);
}
void cmd_bye()
{
	printf("bye!\n");
}
void cmd_run(struct token_t *command, struct llist_t *arglist, int bg)
{
	char *argstr = ll_tostring(arglist);
	if (bg) {
		printf("running %s in background with args %s\n", command->value, argstr);
	}
	else {
		printf("running %s with args %s\n", command->value, argstr);
	}
	free(argstr);
	tk_free(command);
	ll_free(arglist);
}

void cmd_assignto(struct token_t *varname, struct token_t *command, struct llist_t *arglist)
{
	char *argstr = ll_tostring(arglist);
	printf("assigning the result of {%s %s} to %s", command->value, argstr, varname->value);
	free(argstr);
	tk_free(varname);
	tk_free(command);
	ll_free(arglist);
}
