#ifndef sHELL_COMMANDS
#define sHELL_COMMANDS
#include "llist.h"
void cmd_listjobs();
void cmd_defprompt(char *nprompt);
void cmd_cd(char *path);
void cmd_assign(char *varname, char *vardef);
void cmd_bye();
void cmd_run(char *command, struct llist_t *args, int bg);
void cmd_assignto(char *varname, char *command, struct llist_t *args);
#endif