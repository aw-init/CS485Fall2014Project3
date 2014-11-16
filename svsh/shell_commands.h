#ifndef sHELL_COMMANDS
#define sHELL_COMMANDS
#include "llist.h"
void cmd_listjobs();
void var_value(struct token_t *var_token);
char* cmd_defprompt(struct token_t *nprompt);
void cmd_cd(struct token_t *path);
void add_var(char* name, char* value);
void cmd_assign(struct token_t *varname, struct token_t *vardef);
void cmd_bye();
void cmd_run(struct token_t *command, struct llist_t *args, int bg);
void cmd_assignto(struct token_t *varname, struct token_t *command, struct llist_t *args);
void cmd_comment(struct llist_t *args);
//variableList * varList;
#endif
