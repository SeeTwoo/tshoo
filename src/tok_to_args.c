#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"

int	is_redir(t_token *token);

size_t	arg_number(t_token *toks) {
	size_t	i = 0;

	while (toks && (is_redir(toks) || toks->kind == WORD || toks->kind == FILE_NAME)) {
		if (toks->kind == WORD)
			i++;
		toks = toks->next;
	}
	return (i);
}

char	**tok_to_args(t_token *toks) {
	char	**args;
	int		i = 0;

	args = malloc(sizeof(char *) * (arg_number(toks) + 1));
	if (!args)
		return (NULL);
	while (toks && (is_redir(toks) || toks->kind == WORD || toks->kind == FILE_NAME)) {
		if (toks->kind == WORD) {
			args[i] = strdup(toks->lexeme);
			i++;
		}
		toks = toks->next;
	}
	args[i] = NULL;
	return (args);
}
