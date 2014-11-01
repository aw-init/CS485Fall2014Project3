#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char **argv)
{
	char *line = read_line();
	struct array_t token_arr = lex(line);
	free(line);
	struct token_t *tokens = (struct token_t*) token_arr.contents;
	for (int i = 0; i < token_arr.length; ++i) {
		struct token_t token = tokens[i];
		printf("token(type %d, value %s)\n", token.type, token.value);
	}
	free_token_array(token_arr);
}