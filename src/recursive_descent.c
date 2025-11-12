#include "recursive_descent.h"

t_token	*skip_cmd_tokens(t_token *toks) {
	while (toks && (toks->type == WORD || is_redir(toks)))
		toks = toks->next;
	return (toks);
}

char	*find_command(t_token *toks) {
	while (toks) {
		if (toks->type == WORD)
			return (toks->lexeme);
		toks = toks->next;
	}
	return (NULL);
}

static builtin	*is_builtin(char *name) {
	if (!name)
		return (NULL);
	if (strcmp(name, "exit") == 0)
		return (tshoo_exit);
	else if (strcmp(name, "echo") == 0)
		return (tshoo_echo);
	else if (strcmp(name, "cd") == 0)
		return (tshoo_cd);
	else if (strcmp(name, "alias") == 0)
		return (tshoo_alias);
	else if (strcmp(name, "unalias") == 0)
		return (tshoo_unalias);
	else if (strcmp(name, "env") == 0)
		return (tshoo_env);
	else if (strcmp(name, "unset") == 0)
		return (tshoo_unset);
	else if (strcmp(name, "style") == 0)
		return (tshoo_style);
	return (NULL);
}

t_node	*recursive_descent(t_token *toks, t_node *tree);

//better frees
t_node	*new_cmd_node(t_token *toks) {
	t_node	*new = malloc(sizeof(t_node));

	if (!new)
		return (NULL);
	new->as.cmd.func = is_builtin(find_command(toks));
	new->kind = new->as.cmd.func ? N_BUILTIN : N_CMD;
	new->subshell = false;
	new->as.cmd.arg = tok_to_args(toks);
	new->as.cmd.in = 0;
	new->as.cmd.out_redir = NULL;
	new->as.cmd.pid = -1;
	new->as.cmd.exit_status = 1;
	get_redirections(new, toks);
	return (recursive_descent(skip_cmd_tokens(toks), new));
}

t_node	*new_binary_node(t_token *toks, t_node *tree, e_node_kind kind) {
	if (!tree || !(toks->next) || toks->next->type != WORD)
		return (dprintf(2, "unclosed pipe"), NULL);

	t_node *new = malloc(sizeof(t_node));

	if (!new)
		return (NULL);
	new->kind = kind;
	new->subshell = false;
	new->as.binary.left = tree;
	new->as.binary.right = recursive_descent(toks->next, NULL);
	return (new);
}

t_node	*recursive_descent(t_token *toks, t_node *tree) {
	if (!toks)
		return (tree);
	if (toks->type == WORD)
		return (new_cmd_node(toks));
	else if (toks->type == PIPE)
		return (new_binary_node(toks, tree, N_PIPE));
	else if (toks->type == AND)
		return (new_binary_node(toks, tree, N_AND));
	return (NULL);
}
