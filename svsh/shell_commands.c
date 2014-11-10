#include <stdlib.h>
#include <stdio.h>
#include "llist.h"
void cmd_listjobs()
{
	printf("listing currently running jobs\n");
}

void cmd_defprompt(char *nprompt)
{
	printf("set new prompt to %s\n", nprompt);
}
void cmd_cd(char *path)
{
		printf("change directory to %s\n", path);
}
void cmd_assign(char *varname, char *vardef)
{
	printf("set %s = %s\n", varname, vardef);
}
void cmd_bye()
{
	printf("bye!\n");
}
void cmd_run(char *command, struct llist_t *arglist, int bg)
{
	char *argstr = ll_tostring(arglist);
	if (bg) {
		printf("running %s in background with args %s\n", command, argstr);
	}
	else {
		printf("running %s with args %s\n", command, argstr);
	}
	free(argstr);
}

void cmd_assignto(char *varname, char *command, struct llist_t *arglist)
{
	char *argstr = ll_tostring(arglist);
	printf("assigning the result of {%s %s} to %s", command, argstr, varname);
	free(argstr);
}