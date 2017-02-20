#include "nametable.h"
#include "llist.h"
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



struct nametable_t* nt_new()
{
	struct nametable_t *tmp = malloc(sizeof(struct nametable_t));
	tmp->size = 0;
	return tmp;
}

int nt_save_var(struct nametable_t* table, char *key, char *value)
{
	/* does not modify key, value */
	printf("Saving variable %s = %s to Table[%d]\n", key, value, table->size);



	if (table->size == 0)
	{
		struct token_t *keytk = tk_new(STRING, strdup(key));
		struct token_t *valtk = tk_new(STRING, strdup(value));
		table->keys = ll_new(keytk);
		table->values = ll_new(valtk);
		table->size += 1;
	}
	else
	{

		int found = 0;
		struct llist_t *iter = table->keys;
		struct llist_t *par_iter = table->values;
		do {
			if (strcmp(iter->value->value, key) == 0) {
				found = 1;
				break;
			}
			iter = iter->next;
			par_iter = par_iter->next;
		} while (iter != NULL);


		if (found > 0)
		{
			tk_modify(par_iter->value, par_iter->value->ttype, strdup(value));
		}
		else
		{
			
			struct token_t *keytk = tk_new(STRING, strdup(key));
			struct token_t *valtk = tk_new(STRING, strdup(value));

			struct llist_t *new_keys = ll_cons(keytk, table->keys);
			struct llist_t *new_vals = ll_cons(valtk, table->values);

			table->keys = new_keys;
			table->values = new_vals;
			table->size += 1;
		}
	}
	return 0;
}

char* nt_get_var(struct nametable_t* table, char *key)
{
	/* does not modify key, does not modify table */
	if (table->size == 0)
	{
		return NULL;
	}

	struct llist_t *iter = table->keys;
	struct llist_t *par_iter = table->values;
	do {
		if (strcmp(key, iter->value->value) == 0) {
			return strdup(par_iter->value->value);
		}

		iter = iter->next;
		par_iter = par_iter->next;
	} while (iter != NULL);


	return NULL;
}
