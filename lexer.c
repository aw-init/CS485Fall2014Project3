#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int MAXLINESIZE = 100;
enum {
	TT_KEYTT_WORD,
	TT_VARIABLE,
	TT_STRING,
	TT_WORD,
	TT_METACHAR
};

/*
	reads at most MAXLINESIZE characters, stops reading when it reaches EOF or newline
	returns a null terminated string of the characters entered, including newline.
	read_line allocates memory for you, so it should be freed after it has been lexed.
*/
char *read_line()
{
	char *line = malloc(sizeof(char) * MAXLINESIZE);
	char *success = fgets(line, MAXLINESIZE, stdin);
	if (success == NULL) {
		perror("Error while reading line of text");
	}
	return line;
}

// generic array container that carries its own length
struct array_t {
	int length;
	void *contents;
};
struct token_t {
	int type;
	char *value;
};

// checks if a character is valid inside a word.
// Should be just about everything but metachars
// (still need to confirm the grammar to be sure)
static int valid_word_char(char c) {
	if ( isalpha(c) ||
	     c == '/' ||
		 c == '.'
		)
		return 1;
	else
		return 0;
}

/*
	returns array of tokens
	each token has type and value
	value always ignores signifying syntax (i.e. variable's dollar sign, string's quotes)
	type refers to the enum above.
*/
struct array_t lex(char *line) {
	// worst case memory usage is MAXLINESIZE, 1 letter tokens
	// re allocate at end of function,
	// since we almost defintiely don't need this much
	char *buffer = malloc(sizeof(char) * MAXLINESIZE * 2);
	int *tokentype = malloc(sizeof(int) * MAXLINESIZE);
	
	// number of tokens we've
	int token_loc = 0;
	
	//points to the location to write the next char
	// write current to buffer with
	// buffer[write_loc++] = current
	int write_loc = 0;
	
	// points to the char currently being examined
	// read next char using
	// current = line[++read_loc]
	int read_loc = 0;
	
	// Begin the main lex loop
	while (read_loc < MAXLINESIZE)
	{
		// get the first character of the line
		char current = line[read_loc];
		
		// from the first character, we know what token it is.
		// from there, we just branch off and get all the characters
		if (current == '\0')
			break;		
		else if (current == ' ' || current == '\n') {
			read_loc++;
		}
		else if (current == '"') {
			// if it starts with a quotation mark, it is a string.
			// read all characters until we hit another quotation mark
			// (note, we are ignoring escaped and nested strings)
			// skip the opening quotation mark
			current = line[++read_loc];
			while (current != '"') {
				buffer[write_loc++] = current;
				current = line[++read_loc];
			}
			// skip the quotation mark
			// read_loc now points to the following char, ready to be read at start of loop
			read_loc++;
			
			// End the current token
			buffer[write_loc++] = '\0';
			tokentype[token_loc++] = TT_STRING;	
		}
		else if (current == '$') {
			// if it starts with a dollar sign, it is a variable
			// skip the dollar sign
			current = line[++read_loc];
			// a variable is any word of only alpha
			// does not handle words with numbers in them (neccesary?)
			while (isalpha(current)) { 
				buffer[write_loc++] = current;
				current = line[++read_loc];
			}
			
			// finish storing the token
			buffer[write_loc++] = '\0';
			tokentype[token_loc++] = TT_VARIABLE;
			
			// current is updated last in the isalpha loop,
			// so it already points at the next char
		}
		else if (valid_word_char(current)) {
			// this can be either a word or a keyword, we can't tell
			// until we have read the entire thing
			int start = write_loc;
			// add any checks as to what constitutes a word here
			// currently handles alphanumerics with path seperator
			// does not check if it is a validly formatted filename
			while (valid_word_char(current)) {
				buffer[write_loc++] = current;
				current = line[++read_loc];
			}
			
			buffer[write_loc++] = '\0';
			// before we determine the tokentype, we will check if it is a keyword
			// I don't know a better way to do this
			if (strcmp(&buffer[start], "run") == 0) {
				tokentype[token_loc++] = TT_KEYTT_WORD;
			}
			/*
				Add other keywords with similar pattern here
			*/ 
			else {
				// if it wasn't one of those, then it is just a regular word.
				tokentype[token_loc++] = TT_WORD;
			}
		}
		else {
			// otherwise, it is clearly a special character
			buffer[write_loc++] = current;
			tokentype[token_loc++] = TT_METACHAR;
			read_loc++;
		}
	}
	// this code will store the tokens into a convenient data structure
	struct token_t *tokens = malloc(sizeof(struct token_t) * token_loc);
	
	// this is a pointer into buffer, that points to the string for the current token value
	char *bufloc = buffer;
	
	// loop over all tokens and store them into the struct
	for (int i = 0; i < token_loc; ++i) {
		struct token_t token;
		token.type = tokentype[i];
		token.value = strdup(bufloc);
		// advance bufloc to the next string
		while (*bufloc != '\0') {
			bufloc++;
		}
		bufloc++;
		// put token into the token array
		tokens[i] = token;
	}
	
	// Zero the memory. Mostly for debug purposes
	memset(buffer, 0, sizeof(char) * MAXLINESIZE * 2);
	memset(tokentype, 0, sizeof(int) * MAXLINESIZE);
	
	// free the memory we allocated previously
	// I told you it wasn't that bad to allocate huge arrays before
	free(buffer);
	free(tokentype);
	
	// this token just carries the length with the array
	// I feel that it is cleaner than modifying an int pointer argument
	struct array_t tokenArray;
	tokenArray.length = token_loc;
	tokenArray.contents = (void*)tokens;
	return tokenArray;
}

// free memory associated with a token array returned by lex


// free memory associated with token
void free_token(struct token_t tk)
{
	free(tk.value);
}

// does not free elements of arr, only the array itself
void free_array(struct array_t arr) {
	free(arr.contents);
}
void free_token_array(struct array_t tkarr)
{
	struct token_t *contents = (struct token_t*)tkarr.contents;
	for (int i = 0; i < tkarr.length; ++i) {
		free_token(contents[i]);
	}
	free(contents);
}