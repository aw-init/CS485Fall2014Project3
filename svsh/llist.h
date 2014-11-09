#ifndef lLIST_H
#define lLIST_H
struct llist_t {
	char *value;
	struct llist_t *next;
};
// create a new one element list
struct llist_t* ll_new(char *value);

// free the memory owned by the list
// this assumes all strings are malloc'd and not owned by anything else
void ll_free(struct llist_t* head);

// prepend a list with a new value
struct llist_t *ll_cons(char *value, struct llist_t *head);

// get the first value in the list
char *ll_car(struct llist_t *head);

// get the rest of the list
struct llist_t *ll_cdr(struct llist_t *head);

// return the list with the first n elements chopped off
struct llist_t *ll_slice(struct llist_t *head, int n);

//  get the value at index
char *ll_nth(struct llist_t *head, int index);

// get the single element list at the end of the list
struct llist_t *ll_last_node(struct llist_t *head);

// combine two lists
struct llist_t *ll_append(struct llist_t *left, struct llist_t *right);

char *ll_tostring(struct llist_t *head);

#ifndef ll_foreach
#define ll_foreach(X, CODE) do { CODE } while ( (X = X->next) != NULL)
#endif
#endif