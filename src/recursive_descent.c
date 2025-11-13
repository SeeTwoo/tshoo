#include "recursive_descent.h"

size_t	arg_number(t_token *toks) {
	size_t	i = 0;

	while (toks && (is_redir(toks) || toks->kind == WORD || toks->kind == FILE_NAME)) {
		if (toks->kind == WORD)
			i++;
		toks = toks->next;
	}
	return (i);
}

void	add_redir(t_redir **tail, t_redir **head, t_token **toks) {
	t_redir *new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	if (!(*head)) {
		*head = new;
		*tail = *head;
	} else {
		(*tail)->next = new;
		*tail = (*tail)->next;
	}
	new->name = strdup((*toks)->next->lexeme);
	///better freeeing pls
	new->kind = (*toks)->kind;
	new->next = NULL;
	*toks = (*toks)->next->next;
}

int	consume_tokens(t_node *node, t_token **toks) {
	int		i = 0;
	t_redir	*in_tail = NULL;
	t_redir	*out_tail = NULL;;

	node->as.cmd.in_redir = NULL;
	node->as.cmd.out_redir = NULL;
	node->as.cmd.arg = malloc(sizeof(char *) * (arg_number(*toks) + 1));
	if (!(node->as.cmd.arg))
		return (1);
	while (*toks && (is_redir(*toks) || (*toks)->kind == WORD || (*toks)->kind == FILE_NAME)) {
		if (is_in_redir(*toks)) {
			add_redir(&in_tail, &(node->as.cmd.in_redir), toks);
		} else if (is_out_redir(*toks)) {
			add_redir(&out_tail, &(node->as.cmd.out_redir), toks);
		} else if ((*toks)->kind == WORD) {
			node->as.cmd.arg[i++] = strdup((*toks)->lexeme);
			*toks = (*toks)->next;
		} else {
			*toks = (*toks)->next;
		}
	}
	node->as.cmd.arg[i] = NULL;
	return (0);
}

char	*find_command(t_token *toks) {
	while (toks) {
		if (toks->kind == WORD)
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
	else if (strcmp(name, "unset") == 0)
		return (tshoo_unset);
	else if (strcmp(name, "style") == 0)
		return (tshoo_style);
	return (NULL);
}

t_node	*recursive_descent(t_token **toks, t_node *tree, e_kind kind);

//need better frees
t_node	*new_cmd_node(t_token **toks, e_kind parent) {
	t_node	*new = malloc(sizeof(t_node));

	if (!new)
		return (NULL);
	new->as.cmd.func = is_builtin(find_command(*toks));
	new->kind = new->as.cmd.func ? BUILTIN : CMD;
	new->subshell = false;
	new->as.cmd.in = 0;
	new->as.cmd.out_redir = NULL;
	new->as.cmd.pid = -1;
	new->as.cmd.exit_status = 1;
	consume_tokens(new, toks);
	if (parent == PIPE && *toks && (*toks)->kind != PIPE)
		return (new);
	else
		return (recursive_descent(toks, new, parent));
}

t_node	*new_binary_node(t_token **toks, t_node *tree, e_kind kind) {
	t_node *new = malloc(sizeof(t_node));

	if (!new)
		return (NULL);
	new->kind = kind;
	new->subshell = false;
	new->as.binary.left = tree;
	*toks = (*toks)->next;
	new->as.binary.right = recursive_descent(toks, NULL, kind);
	return (recursive_descent(toks, new, kind));
}

t_node	*recursive_descent(t_token **toks, t_node *tree, e_kind kind) {
	if (!(*toks))
		return (tree);
	if ((*toks)->kind == WORD || is_redir(*toks))
		return (new_cmd_node(toks, kind));
	else if ((*toks)->kind == PIPE)
		return (new_binary_node(toks, tree, PIPE));
	else if ((*toks)->kind == AND)
		return (new_binary_node(toks, tree, AND));
	else if ((*toks)->kind == OR)
		return (new_binary_node(toks, tree, OR));
	return (NULL);
}

t_node	*parser(t_token *toks) {
	t_token	*temp = toks;

	return (recursive_descent(&temp, NULL, CMD));
}
