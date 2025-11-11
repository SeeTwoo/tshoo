#include <stdlib.h>
#include <stdio.h>

#include "token.h"
#include "nodes.h"

int		get_redirections(t_node *node, t_token *toks);
int		is_redir(t_token *token);
char	**tok_to_args(t_token *toks);

t_token	*skip_cmd_tokens(t_token *toks) {
	while (toks && (toks->type == WORD || is_redir(toks)))
		toks = toks->next;
	return (toks);
}

t_node	*recursive_descent(t_token *toks, t_node *tree);

//better frees
t_node	*cmd_node(t_token *toks) {
	t_node	*new = malloc(sizeof(t_node));

	if (!new)
		return (NULL);
	new->kind = CMD;
	new->subshell = false;
	new->as.cmd.arg = tok_to_args(toks);
	new->as.cmd.in = 0;
	new->as.cmd.out_redir = NULL;
	new->as.cmd.pid = -1;
	get_redirections(new, toks);
	return (recursive_descent(skip_cmd_tokens(toks), new));
}

t_node	*pipe_node(t_token *toks, t_node *tree) {
	if (!tree || !(toks->next) || toks->next->type != WORD)
		return (dprintf(2, "unclosed pipe"), NULL);

	t_node *new = malloc(sizeof(t_node));

	if (!new)
		return (NULL);
	new->kind = PIPE;
	new->subshell = false;
	new->as.binary.left = tree;
	new->as.binary.right = recursive_descent(toks->next, NULL);
	return (new);
}

t_node	*and_node(t_token *toks, t_node *tree) {
	if (!tree || !(toks->next) || toks->next->type != WORD)
		return (dprintf(2, "unclosed and"), NULL);

	t_node	*new = malloc(sizeof(t_node));

	if (!new)
		return (NULL);
	new->kind = AND;
	new->subshell = false;
	new->as.binary.left = tree;
	new->as.binary.right = recursive_descent(toks->next, NULL);
	return (new);
}

t_node	*recursive_descent(t_token *toks, t_node *tree) {
	if (!toks)
		return (tree);
	if (toks->type == WORD)
		return (cmd_node(toks));
	else if (toks->type == PIPE)
		return (pipe_node(toks, tree));
	else if (toks->type == AND)
		return (and_node(toks, tree));
	return (NULL);
}
