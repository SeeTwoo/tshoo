#include <stdlib.h>

#include "token.h"
#include "nodes.h"
#include "token_and_node_types.h"

t_node	*new_operator_node(t_token **tok, int *sublvl, t_node *right, t_node *left) {
	new
}

t_node	*new_command_node(t_token **tok, int *sublvl) {
	t_node	*new = malloc(sizeof(t_node));

	if (!new)
		return (NULL);

}

t_node	*recursive_parser(t_token *tok) {
	int	sublvl = 0;
	
	return (new_command_node(&tok, &sublvl));
}


