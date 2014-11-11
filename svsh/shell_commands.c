#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "llist.h"
#define SYS_BUFFERSIZE 1000
/*
	These will be wrappers around the system calls
	So that we can be sure we don't screw up the calls
	we should use the below vars to pass in args
	to the system calls
*/
static char buf_varname[SYS_BUFFERSIZE];
static char buf_vardef[SYS_BUFFERSIZE];
static char buf_prevname[SYS_BUFFERSIZE];
int SaveVariable(char *varname, char *vardef)
{
	return 0;
}
int GetVariable(char *varname)
{
	if (strcmp(varname, "ShowTokens") == 0) {
		strncpy(buf_vardef, "1", SYS_BUFFERSIZE);
	}
	return 0;
}
int NextVariable(char *prevname)
{
	return 0;
}
void cmd_listjobs()
{
	GetVariable("ShowTokens");
	if (strcmp("1", buf_vardef) == 0) {
		printf("showing tokens\n");
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
