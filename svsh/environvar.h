#ifndef eNVIRONVAR_H
#define eNVIRONVAR_H
struct variable_t {
	char *key;
	char *value;
	struct variable_t *next;
};
char* env_getvar(char *key);
struct variable_t *env_search(char *key);
void env_setvar(char *key, char *value);
#endif