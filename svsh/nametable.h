#ifndef nametable_h
#define nametable_h
#include "llist.h"
/* a simple key:value map with linear search time*/
struct nametable_t {
	struct llist_t *keys;
	struct llist_t *values;
	unsigned int size;
};
int nt_save_var(struct nametable_t* table, char *key, char *value);
char* nt_get_var(struct nametable_t* table, char *key);
#endif
