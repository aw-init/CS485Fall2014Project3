#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pidlist.h"
struct pidlist_t *pl_new(pid_t value)
{
	struct pidlist_t *node = malloc(sizeof(struct pidlist_t));
	node->pid = value;
	node->next = NULL;
	return node;
}
void pl_free(struct pidlist_t *head)
{
	struct pidlist_t *tmp;
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}
struct pidlist_t *pl_cons(pid_t value, struct pidlist_t *head) {
	if (head != NULL) {
		struct pidlist_t *newnode = malloc(sizeof(struct pidlist_t));
		newnode->pid = value;
		newnode->next = head;
		return newnode;
	}
	else {
		return pl_new(value);
	}
}

// remove an element from the pid list.
// handles memory deallocation, and returns the updated list
struct pidlist_t *pl_remove(struct pidlist_t *head, int index) {
	struct pidlist_t *last;
	struct pidlist_t *current;
	struct pidlist_t *next;
	if (head == NULL) {
		return NULL;
	}
	if (index == 0) {
		next = head->next;
		
		head->next = NULL;
		pl_free(current);
		return next;
	}
	else {
	
		// get the nodes before, at, and after the index
		last = pl_slice(head, index - 1);
		if (last == NULL) return NULL;
		
		current = last->next;
		if (current == NULL) return NULL;
		
		// we don't care if next is null
		next = current->next;
		
		// last skips over the current
		last->next = next;
		
		// current is separated from the list, so we can free it
		current->next = NULL;
		
		// free current
		pl_free(current);
		return next;
	}
	
}
pid_t pl_car(struct pidlist_t *head)
{
	return head->pid;
}
struct pidlist_t *pl_cdr(struct pidlist_t *head)
{
	return head->next;
}



struct pidlist_t *pl_slice(struct pidlist_t *head, int index)
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
pid_t pl_nth(struct pidlist_t *head, int index)
{
	struct pidlist_t *tmp = pl_slice(head, index);
	if (tmp == NULL) return 0;
	else return tmp->pid;
}


int pl_length(struct pidlist_t *head)
{
	if (head == NULL) return 0;
	int i = 1;
	while (head->next != NULL) {
		head = head->next;
		i++;
	}
	return i;
}

struct pidlist_t *pl_last_node(struct pidlist_t *head)
{
	while (head->next != NULL)
	{
		head = head->next;
	}
	return head;
}
struct pidlist_t *pl_append(struct pidlist_t *left, struct pidlist_t *right)
{
	struct pidlist_t *last = pl_last_node(left);
	last->next = right;
	return left;
}
