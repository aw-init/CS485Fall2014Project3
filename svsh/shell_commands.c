#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include "llist.h"
#include "y.tab.h"
#include <unistd.h>
#include "shell_commands.h"
#include <sys/stat.h>
#include <fcntl.h>
#define SYS_BUFFERSIZE 1000
#define SYS_SAVE_VAR 315
#define SYS_GET_VAR 316

// replace this later with environment variable call
int ShowTokens = 1;
struct pidlist_t {
	pid_t pid;
	char *name;
	struct pidlist_t *next;	
};
struct pidlist_t *bg_procs = NULL;
//extern variableList * varList;
 
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
int proc_running(pid_t pid) {
	int status;
	pid_t result = waitpid(pid, &status, WNOHANG);
	if (result == 0) {
		// child still doing useful work
		return 1;
	}
	else if (result == -1) {
		// error case
		return 0;
	}
	else {
		// child is no longer running
		return 0;
	}
}
void cmd_listjobs()
{
	if (ShowTokens) {
		PrintToken(LISTJOBS, "listjobs", "listjobs");
	}
	printf("listing currently running jobs\n");
	struct pidlist_t *newlist = NULL;
	struct pidlist_t *iter = bg_procs;
	struct pidlist_t *tmp = NULL;
	int length = 0;
	while (iter != NULL) {
		if (proc_running(iter->pid)) {
			printf("%d %s\n", iter->pid, iter->name);
			length++;
			struct pidlist_t *node = malloc(sizeof(struct pidlist_t));
			node->pid = iter->pid;
			node->name = iter->name;
			node->next = newlist;
			newlist = node;
		}
		iter = iter->next;
	}
	iter = bg_procs;
	while (iter != NULL) {
		tmp = iter->next;
		free(iter);
		iter = tmp;
	}
	bg_procs = newlist;
	printf("there are currently %d proccesses running\n", length);
}

/*Given a token_t, checks if it's a variable and then replaces the token values with the variable value*/
void var_value(struct token_t *var_token)
{	
	 
	if(var_token->ttype == VARIABLE)
	{
		char* var_definition = (char*) malloc(SYS_BUFFERSIZE);
		syscall(SYS_GET_VAR, var_token->value, var_definition, SYS_BUFFERSIZE);
		free(var_token->value);
		var_token->value = var_definition;
		var_token->ttype = WORD;
	}

}

char* cmd_defprompt(struct token_t *nprompt)
{	char* prompt;
	if (ShowTokens) {
		PrintToken(DEFPROMPT, "defprompt", "defprompt");
		PrintToken(nprompt->ttype, nprompt->value, "arg 1");
	}
	var_value(nprompt);
	prompt = nprompt->value;
	
	//tk_free(nprompt);
	printf("set new prompt to %s\n", prompt);
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

void add_var(char* name, char* value)
{
	char newvalue[SYS_BUFFERSIZE];
	char newname[SYS_BUFFERSIZE];
	strncpy(newname,name,strlen(name));
	strncpy(newvalue,value,strlen(value));

	if(strcmp("$ShowTokens",name)==0)
	{
		ShowTokens = atoi(value);
		
	}
	else{
		syscall(SYS_SAVE_VAR, newname, newvalue);
	}
	
	
}

void cmd_assign(struct token_t *varname, struct token_t *vardef)
{
	if (ShowTokens) {
		PrintToken(varname->ttype, varname->value, "variable_name");
		PrintToken(EQ, "=", "assignment");
		PrintToken(vardef->ttype, vardef->value, "variable_def");
	}
	printf("set %s = %s\n", varname->value, vardef->value);
	add_var(varname->value,vardef->value);
	tk_free(varname);
	tk_free(vardef);
}
void cmd_bye()
{
	if (ShowTokens) {
		PrintToken(BYE, "bye", "bye");
	}
	printf("bye!\n");
	exit(1);
}

char** create_arglist(struct token_t *command, struct llist_t *arglisttokens){
	int len = ll_length(arglisttokens);
	char ** nargv =(char**)  malloc(sizeof(char*) * (len + 2));
	nargv[0] = malloc(sizeof(char) * (strlen(command->value)+1));
	strncpy(nargv[0],command->value, strlen(command->value)+1);
	nargv[len+1] = NULL;
	if(arglisttokens != NULL){
		// construct arglist with arguments
		struct llist_t *iter = arglisttokens;
		int i = 1;
		ll_foreach(iter, {
			int slen = strlen(iter->value->value);
			nargv[i] = malloc(sizeof(char) * (slen+1));
			strcpy(nargv[i], iter->value->value);
			i++;
		});
	}
	//for(int i=0; i<len+2;i++){
	//	printf(
	//}
	return nargv;	
	
}

void cmd_run(struct token_t *command, struct llist_t *arglist, int bg)
{
	
	
	/*
		We still need to add some error checking to exec
		It currently fails silently if it gives an invalid path
		same thing for assignto
	*/
	struct llist_t *iter = arglist;
	if (ShowTokens) {
		PrintToken(RUN, "run", "run");
		PrintToken(command->ttype, command->value, "cmd");
		int count = 0;
		char argN[] = "arg 0000";
		if(iter != NULL)
			ll_foreach(iter, {
				count++;
				sprintf(argN, "arg %4d",  count);
				PrintToken(iter->value->ttype, iter->value->value, argN);
			});
		if(bg)
			PrintToken(BG, "<bg>", "background");
	}
	var_value(command);
	iter = arglist;
	if(iter != NULL)
		ll_foreach(iter, {
			var_value(iter->value);
		});
	if (bg) {
		pid_t pid = fork();
		if (pid == 0) {
			// child is running here
			char ** nargv = create_arglist(command, arglist);
			int retval = execvp(nargv[0],nargv);	
			if(retval == -1)
				printf("%s\n", strerror(errno));
			exit(0);		
		}
		else {
			// parent is running
			
			char *cmd_cpy = strdup(command->value);
			struct pidlist_t *pidnode = malloc(sizeof(struct pidlist_t));
			pidnode->pid = pid;
			pidnode->name = cmd_cpy;
			pidnode->next = bg_procs;
			bg_procs = pidnode;
			
		}
	}
	else {
		pid_t pid = fork();
		if (pid == 0) {
			char ** nargv = create_arglist(command, arglist);
			int retval = execvp(nargv[0],nargv);	
			if(retval == -1)
				printf("%s\n", strerror(errno));
			exit(0);
		}
		else {
			int errormsg = 0;
			pid_t child = wait(&errormsg);
			// child is terminated here
			// listjobs stuff goes here
		}
	}
	//free(argstr);
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
	
		if(iter != NULL)
			ll_foreach(iter, {
				count++;
				sprintf(argN, "arg %4d",  count);
				PrintToken(iter->value->ttype, iter->value->value, argN);
			});
	}
	
//	char *argstr = ll_tostring(arglist);
//	printf("assigning the result of {%s %s} to %s", command->value, argstr, varname->value);
	
	var_value(command);
	struct llist_t *iter = arglist;
	if(iter != NULL)
		ll_foreach(iter, {
			var_value(iter->value);
		});
	
	int saved_stdout = dup(1);
	pid_t pid = fork();
	if (pid == 0) {
		int fd = open(".TEMP.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		dup2(fd, 1);
		dup2(fd, 2);
		close(fd);
		char ** nargv = create_arglist(command, arglist);
		int retval = execvp(nargv[0],nargv);	
		if(retval == -1)
			printf("%s\n", strerror(errno));
		dup2(saved_stdout, 1);
		close(saved_stdout);
		FILE *f = fopen(".TEMP.txt", "rb");
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		char *tempstring = malloc(fsize + 1);
		fread(tempstring, fsize, 1, f);
		fclose(f);
	
		tempstring[fsize] = 0;
		printf("%s\n", tempstring);
		syscall(SYS_SAVE_VAR, varname->value, tempstring);
		exit(0);
	}
	else {
		int errormsg = 0;
		pid_t child = wait(&errormsg);
		// child is terminated here
		// listjobs stuff goes here
	}
	
	//	free(argstr);
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
