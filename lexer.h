#ifndef lEXER_H
#define lEXER_H
// maximum number of characters in a command line
int MAXLINESIZE;

// token type signifier
enum {
	TT_KEYWORD,
	TT_VARIABLE,
	TT_STRING,
	TT_WORD,
	TT_METACHAR
};

// generic array with length
// used to make returning from lex much cleaner.
// maybe useful other places in code?
// contents are always dynamically allocated - remember to free memory
// some special purpose memory deallocation functions are available below
struct array_t {
	int length;
	void *contents;
};

// represents single token
// value is a dynamically allocated string, and should be free manually.
// see below for deletion function
struct token_t {
	int type;
	char *value;
};

/*
	reads at most MAXLINESIZE characters, stops reading when it reaches EOF or newline
	returns a null terminated string of the characters entered, including newline.
	read_line allocates memory for you, so it should be freed after it has been lexed.
*/
char *read_line();

/*  returns array of tokens
	each token has type and value
	value always ignores signifying syntax (i.e. variable's dollar sign, string's quotes)
	type refers to the enum above.
	lex allocates memory, so you must free it yourself. memory handling functions are provided (see below)*/
struct array_t lex(char*);

// deallocate memory associated with a token. (right now, only the value)
void free_token(struct token_t);

// deallocate memory associated with an array of tokens, as returned by lex
void free_token_array(struct array_t);

// deallocate memory associated with any array.
// Not really relevant right now
// Only does a shallow deletion, if you have an array of dynamically allocated objects,
// it will not delete them correctly
void free_array(struct array_t);

#endif