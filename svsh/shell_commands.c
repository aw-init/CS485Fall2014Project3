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
#include "nametable.h"
#include <fcntl.h>
#define SYS_BUFFERSIZE 1000
#define SYS_SAVE_VAR 315
#define SYS_GET_VAR 316

extern struct nametable_t GLOBAL_NAMETABLE;

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
int is_proc_running(pid_t pid) {
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
		if (is_proc_running(iter->pid)) {
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
int var_value(struct token_t *var_token)
{	 
	/* safely modifies var_token */
	if(var_token->ttype == VARIABLE)
	{
		#ifdef SYSCALL
		char* var_definition = (char*) malloc(SYS_BUFFERSIZE);
		char newvar[SYS_BUFFERSIZE];
		int var_len;
		syscall(SYS_GET_VAR, var_token->value, newvar, &var_len);
		strncpy(var_definition,newvar, var_len);
		var_definition[var_len] = '\0';

		tk_modify(var_token, WORD, var_definition);

		#else
		char *result = nt_get_var(&GLOBAL_NAMETABLE, var_token->value);
		if (result == NULL)
		{
			printf("Could not find value of variable %s\n", var_token->value);
			return -1;
		}
		else
		{
			tk_modify(var_token, WORD, result);
			return 0;
		}
		#endif
	}
	return 0;

}

void cmd_defprompt(char **prompt, struct token_t *nprompt)
{
	/* does not modify nprompt */
	if (ShowTokens) {
		PrintToken(DEFPROMPT, "defprompt", "defprompt");
		PrintToken(nprompt->ttype, nprompt->value, "arg 1");
	}

	if (var_value(nprompt) == 0) {
		(*prompt) = strdup(nprompt->value);
		printf("set new prompt to %s\n", *prompt);
	}
	tk_free(nprompt);
}

void cmd_cd(struct token_t *path)
{
	/* does not modify path */
	if (ShowTokens) {
		PrintToken(CD, "cd", "cd");
		PrintToken(path->ttype, path->value, "arg 1");
	}

	int success = var_value(path);
	if (success < 0) {
		return;
	}

	printf("change directory to %s\n", path->value);
	if(chdir(path->value)<0)
		perror("cd");

	tk_free(path);
}

void add_var(char* name, char* value)
{
	/* does not modify name, value */
	if(strcmp("$ShowTokens",name)==0)
	{
		ShowTokens = atoi(value);
		
	}
	else{
		#ifdef SYSCALL
		char newvalue[SYS_BUFFERSIZE];
		char newname[SYS_BUFFERSIZE];
		strncpy(newname,name,strlen(name));
		newname[strlen(name)] = '\0';
		strncpy(newvalue,value,strlen(value));

		newvalue[strlen(value)] = '\0';

		int retval = syscall(SYS_SAVE_VAR, newname, newvalue);
		if(retval==-1){
			printf("error: The system has reached it max number of allowed variables\n");
		}

		#else
		nt_save_var(&GLOBAL_NAMETABLE, name, value);
		#endif
	}
	
	
}

void cmd_assign(struct token_t *varname, struct token_t *vardef)
{
	if (ShowTokens) {
		PrintToken(varname->ttype, varname->value, "variable_name");
		PrintToken(EQ, "=", "assignment");
		PrintToken(vardef->ttype, vardef->value, "variable_def");
	}

	int success = var_value(vardef);
	if (success < 0) {
		return;
	}
	
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
	/* convert llist into char**, insert command at the beginning */
	int len = ll_length(arglisttokens);

	/*
		number of elements in arglisttokens
		+1 for command at the beginning
		+1 for the NULL at the end (required by execvp)
	*/
	char ** nargv =(char**)  malloc(sizeof(char*) * (len + 2));

	/*
	nargv[0] = malloc(sizeof(char) * (strlen(command->value)+1));
	strncpy(nargv[0],command->value, strlen(command->value)+1);
	*/
	nargv[0] = strdup(command->value);
	nargv[len+1] = NULL;

	if(arglisttokens != NULL){
		// fill arglist with llist arguments

		// prepare iterators
		struct llist_t *iter = arglisttokens;
		int i = 1;

		ll_foreach(iter, {
			nargv[i] = strdup(iter->value->value);
			/*
			int slen = strlen(iter->value->value);
			nargv[i] = malloc(sizeof(char) * (slen+1));
			strcpy(nargv[i], iter->value->value);
			*/
			i++;
		});

	}
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

	// replace variable in command
	int success = var_value(command);
	if (success < 0) {
		return;
	}

	// replace all variables in the arglist
	iter = arglist;
	if(iter != NULL)
		ll_foreach(iter, {
			int success = var_value(iter->value);
			if (success < 0) {
				return;
			}
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
	
	int success = var_value(command);
	if (success < 0) {
		return;
	}
	struct llist_t *iter = arglist;
	if(iter != NULL)
		ll_foreach(iter, {
			int success = var_value(iter->value);
			if (success < 0) {
				return;
			}
		});
	
	int saved_stdout = dup(1);
	pid_t pid = fork();
	if (pid == 0) {
		int fd = open(".TEMP.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		dup2(fd, 1);
		dup2(fd, 2);
		close(fd);
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
		FILE *f = fopen(".TEMP.txt", "rb");
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		char *tempstring = malloc(fsize + 1);
		fread(tempstring, fsize,1 , f);
		fclose(f);
	
		tempstring[fsize] = 0;
		dup2(saved_stdout, 1);
		dup2(saved_stdout, 2);
		close(saved_stdout);
		if((fsize+1) < 1000)
		{
			#ifdef SYSCALL
			syscall(SYS_SAVE_VAR, varname->value, tempstring);

			#else
			nt_save_var(&GLOBAL_NAMETABLE, varname->value, tempstring);
			#endif
		}
		else
		{
			printf("error: command returns more than 1000 characters\n");	
		}
		dup2(saved_stdout, 1);
		dup2(saved_stdout, 2);
		close(saved_stdout);
		free(tempstring);
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
