#include <stdlib.h>
#include <string.h>
#include "llist.h"
struct token_t *tk_new(int tt, char *val)
{
	struct token_t *tok = malloc(sizeof(struct token_t));
	tok->value = val;
	tok->ttype = tt;
	return tok;
}
void tk_free(struct token_t *tok)
{
	free(tok->value);
	free(tok);
}
struct llist_t *ll_new(struct token_t *value)
{
	struct llist_t *node = malloc(sizeof(struct llist_t));
	node->value = value;
	node->next = NULL;
	return node;
}
void ll_free(struct llist_t *head)
{
	struct llist_t *tmp;
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		tk_free(tmp->value);
		free(tmp);
	}
}
struct llist_t *ll_cons(struct token_t *value, struct llist_t *head) {
	struct llist_t *newnode = malloc(sizeof(struct llist_t));
	newnode->value = value;
	newnode->next = head;
	return newnode;
}
struct token_t *ll_car(struct llist_t *head)
{
	return head->value;
}
struct llist_t *ll_cdr(struct llist_t *head)
{
	return head->next;
}



struct llist_t *ll_slice(struct llist_t *head, int index)
{
	while (index > 0 && head->next != NULL)
	{
		head = head->next;
		index--;
	}
	if (index == 0) {
		return head;
	}
	else {
		return NULL;
	}
}
struct token_t *ll_nth(struct llist_t *head, int index)
{
	struct llist_t *tmp = ll_slice(head, index);
	if (tmp == NULL) return NULL;
	else return tmp->value;
}


int ll_length(struct llist_t *head)
{
	int i = 1;
	while (head->next != NULL) {
		head = head->next;
		i++;
	}
	return i;
}

struct llist_t *ll_last_node(struct llist_t *head)
{
	while (head->next != NULL)
	{
		head = head->next;
	}
	return head;
}
struct llist_t *ll_append(struct llist_t *left, struct llist_t *right)
{
	struct llist_t *last = ll_last_node(left);
	last->next = right;
	return left;
}

char *ll_tostring(struct llist_t *head)
{
	int string_size = 0;
	struct llist_t *iter = head;
	ll_foreach(iter,
		string_size += strlen(iter->value->value) + 2;
	);
	char *final_string = malloc(sizeof(char) * (string_size + 3));
	strcat(final_string, "[");
	
	iter = head;
	ll_foreach(iter,
		strcat(final_string, iter->value->value);
		if (iter->next != NULL) {
			strcat(final_string, ", ");
		}
	);
	strcat(final_string, "]");
	return final_string;
}
