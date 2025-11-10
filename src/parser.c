#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>

#include "nodes.h"
#include "token.h"
#include "token_and_node_types.h"

void	free_double_array(char **array);
int		get_redirections(t_node *node, t_token *toks);
int		is_redir(t_token *token);
char	**tok_to_args(t_token *toks);

void	print_type(int type);

static AST	*new_cmd_node(t_token **toks) {
	AST	*new = malloc(sizeof(t_node));

	if (!new)
		return (NULL);
	new->kind = CMD;
	new->subshell = false;
	new->as.cmd.arg = tok_to_args(*toks);
	if (!new->as.cmd.arg)
		return (free(new), NULL);
	if (get_redirections(&(as.cmd), *toks) == 1)
		return (free_double_array(new->arg), free(new), NULL);
	while (*toks && (is_redir(*toks) || (*toks)->type == WORD || (*toks)->type == FILE_NAME))
		*toks = (*toks)->next;
	return (new);
}

static t_node	*new_pipe_node(t_token **toks, t_node *left) {
	AST	*new = malloc(sizeof(t_node));

	if (!new)
		return (NULL);
	new->kind = PIPE;
	new->subshell = false;
	new->as.operator.left = left;
	new->as.operator.right = NULL;
	*toks = (*toks)->next;
	return (new);
}

t_node	*create_nodes(t_token *toks) {
	t_node	*ast;
	t_node	*leaf;

	ast = new_cmd_node(&toks);
	leaf = ast;
	if (!ast)
		return (NULL);
	for (; toks;) {
		if (ast->prec > toks->prec || ast->sublvl > toks->sublvl) {
			ast = new_operator_node(&toks, ast);
			leaf = ast;
		} else if (is_redir(toks) || toks->type == WORD || toks->type == FILE_NAME) {
			leaf->right = new_cmd_node(&toks);
		} else {
			leaf->right = new_operator_node(&toks, leaf->right);
			leaf = leaf->right;
		}
	}
	return (ast);
}
