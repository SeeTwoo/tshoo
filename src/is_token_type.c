#include "token.h"

int	is_or(t_token *token) {
	return (token->kind == OR);
}

int	is_and(t_token *token) {
	return (token->kind == AND);
}

int	is_pipe(t_token *token) {
	return (token->kind == PIPE);
}

int	is_in_redir(t_token *token) {
	return (
		token->kind == IN ||
		token->kind == HD
	);
}

int	is_out_redir(t_token *token) {
	return (
		token->kind == APPEND ||
		token->kind == TRUNC
	);
}

int	is_redir(t_token *token) {
	return (
		token->kind == IN ||
		token->kind == HD ||
		token->kind == APPEND ||
		token->kind == TRUNC
	);
}

int	is_wrong_token(t_token *token) {
	return (token->kind == WRONG);
}

int	is_word(t_token *token) {
	return (token->kind == WORD);
}
