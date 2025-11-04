#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "nodes.h"
#include "token.h"
#include "token_and_node_types.h"

void	free_double_array(char **array);
char	**tok_to_args(t_token **tok_array);
int		get_redirections(t_node *node, t_token **tok_array);

int	command_number(t_token **tok_array) {
	int	number;

	number = 0;
	for (int i = 0; tok_array[i]; i++)
		if (tok_array[i]->type == PIPE)
			number++;
	number++;
	return (number);
}

static t_node	*new_node(t_token **tok_array) {
	t_node	*new;

	new = malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->arg = tok_to_args(tok_array);
	if (!new->arg)
		return (free(new), NULL);
	new->in_redir = NULL;
	new->out_redir = NULL;
	if (get_redirections(new, tok_array) == 1)
		return (free_double_array(new->arg), free(new), NULL);
	new->type = CMD;
	new->pid = -1;
	return (new);
}

//awful memory cleaning DO IT BETTER
t_node	**create_nodes(t_token **tok_array) {
	int		i;
	t_node	**nodes;

	nodes = malloc(sizeof(t_node *) * (command_number(tok_array) + 1));
	if (!nodes)
		return (NULL);
	i = 0;
	while (*tok_array) {
		if ((*tok_array)->type == PIPE) {
			tok_array++;
			continue ;
		}
		nodes[i] = new_node(tok_array);
		if (!nodes[i])
			return (NULL);
		while ((*tok_array) && (*tok_array)->type != PIPE)
			tok_array++;
		i++;
	}
	nodes[i] = NULL;
	return (nodes);
}
