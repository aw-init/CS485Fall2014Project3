#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "llist.h"
#include "y.tab.h"
#include "shell_commands.h"

#define SYS_BUFFERSIZE 1000

// replace this later with environment variable call
int ShowTokens = 1;

void PrintToken(int ttype, char *value, char *usage)
{
	char stype[9];
	switch (ttype) {
		case STRING:
			strncpy(stype, "string", 7);
			break;
		case VARIABLE:
			strncpy(stype, "variable", 9);
			break;
		case WORD:
			strncpy(stype, "word", 5);
			break;
		case EQ:
		case COMMENT:
			strncpy(stype, "metachar", 9);
			break;
		case DEFPROMPT:
		case ASSIGNTO:
		case CD:
		case LISTJOBS:
		case BYE:
		case RUN:
		case BG:
			strncpy(stype, "keyword", 8);
			break;
	}
	printf("Token Type = %s\tToken = %s\tUsage = %s\n", stype, value, usage);
}

void cmd_listjobs()
{
	if (ShowTokens) {
		PrintToken(LISTJOBS, "listjobs", "listjobs");
	}
	printf("listing currently running jobs\n");
}

char* cmd_defprompt(struct token_t *nprompt)
{
	if (ShowTokens) {
		PrintToken(DEFPROMPT, "defprompt", "defprompt");
		PrintToken(nprompt->ttype, nprompt->value, "arg 1");
	}
	printf("set new prompt to %s\n", nprompt->value);
	char* prompt = nprompt->value;
	//tk_free(nprompt);
	return prompt;
}
void cmd_cd(struct token_t *path)
{
	if (ShowTokens) {
		PrintToken(CD, "cd", "cd");
		PrintToken(path->ttype, path->value, "arg 1");
	}
	printf("change directory to %s\n", path->value);
	if(chdir(path->value)<0)
		perror("cd");
	tk_free(path);
}
void cmd_assign(struct token_t *varname, struct token_t *vardef)
{
	if (ShowTokens) {
		PrintToken(varname->ttype, varname->value, "variable_name");
		PrintToken(EQ, "=", "assignment");
		PrintToken(vardef->ttype, vardef->value, "variable_def");
	}
	printf("set %s = %s\n", varname->value, vardef->value);
	tk_free(varname);
	tk_free(vardef);
}
void cmd_bye()
{
	if (ShowTokens) {
		PrintToken(BYE, "bye", "bye");
	}
	printf("bye!\n");
	exit(0);
}
void cmd_run(struct token_t *command, struct llist_t *arglist, int bg)
{
	if (ShowTokens) {
		PrintToken(RUN, "run", "run");
		int count = 0;
		char argN[] = "arg 0000";
		struct llist_t *iter = arglist;
		ll_foreach(iter, {
			count++;
			sprintf(argN, "arg %4d",  count);
			PrintToken(iter->value->ttype, iter->value->value, argN);
		});
	}
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
	if (ShowTokens) {
		PrintToken(ASSIGNTO, "assignto", "assignto");
		PrintToken(varname->ttype, varname->value, "variable_name");
		PrintToken(command->ttype, command->value, "cmd");
		int count = 0;
		char argN[] = "arg 0000";
		struct llist_t *iter = arglist;
		ll_foreach(iter, {
			count++;
			sprintf(argN, "arg %4d",  count);
			PrintToken(iter->value->ttype, iter->value->value, argN);
		});
	}
	char *argstr = ll_tostring(arglist);
	printf("assigning the result of {%s %s} to %s", command->value, argstr, varname->value);
	free(argstr);
	tk_free(varname);
	tk_free(command);
	ll_free(arglist);
}

void cmd_comment(struct llist_t *arglist)
{
	if(ShowTokens)
	{
		PrintToken(COMMENT, "#", "comment");
	
		int count = 0;
		char argN[] = "arg 0000";
		struct llist_t *iter = arglist;
		ll_foreach(iter, {
			count++;
			sprintf(argN, "arg %4d",  count);
			PrintToken(iter->value->ttype, iter->value->value, argN);
		});
	}
}
