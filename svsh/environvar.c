/*
	Implements a linked list of key:value pairs to hold the environment variables
	Also contains appropriate interface
	Not as performant as a hash table, but I don't want to implement that.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "environvar.h"
struct variable_t {
	char *key;
	char *value;
	struct variable_t *next;
};

/*
	Environ list assumes responsibility of memory handling.
	free_environ should free all strings contained.
*/
static variable_t *head = NULL;
static variable_t *tail = NULL;

/*
	if the variable already has been initialized, reassign the existing value
	otherwise, create a new node to hold the value
*/
void env_setvar(char *key, char *value)
{
	// check if variable has already been assigned
	struct _variable_t *node = env_search(key);
	if (node != NULL) {
		free(node->value);
		node->value = value;
	}
	else {
		// create new node and populate it
		node = malloc(sizeof(struct variable_t));
		node->key = key;
		node->value = value;
		
		// if no nodes have been added yet, just make them the head
		if (head == NULL) {
			head = node;
			tail = node;
		}
		// otherwise, add them to the end and update the tail
		else {
			tail->next = node;
			tail = node;
		}
	}
}
/*
	return the node that contains the key:value pair named by key
*/
struct variable_t *env_search(char *key)
{
	// create a dummy pointer that we can change
	struct variable_t *current = head;
	
	// keep checking keys until we find the one we want
	while (strcmp(key, current->key) != 0) {
		// if there are no more keys, we know that none exist.
		if (current->next == NULL) {
			return NULL;			
		}
		else {
			current = current->next;
		}
	}
	return current;
}
/*
	return the value associated with the given key
*/
char* env_getvar(char *key)
{
	struct variable_t *node = env_search(key);
	if (node != NULL) {
		return node->value;
	}
	else {
		return NULL;
	}
}

// this is for testing purposes only
int main(int argc, char **argv)
{
	env_setvar("fish", "cake");
	env_setvar("cat", "fish");
}