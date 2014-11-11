#ifndef sHELL_COMMANDS
#define sHELL_COMMANDS
#include "llist.h"
void cmd_listjobs();
void cmd_defprompt(struct token_t *nprompt);
void cmd_cd(struct token_t *path);
void cmd_assign(struct token_t *varname, struct token_t *vardef);
void cmd_bye();
void cmd_run(struct token_t *command, struct llist_t *args, int bg);
void cmd_assignto(struct token_t *varname, struct token_t *command, struct llist_t *args);
#endif
