#include <stdlib.h>
#include <unistd.h>

#include "nodes.h"
#include "token.h"
#include "token_and_node_types.h"

t_node	*create_nodes(t_token *tok_list);
void	free_token_array(t_token **tok_array);
void	free_token_list(t_token *list);
int		is_syntaxically_correct(t_token **tok_array);
t_token	*lexer(char *line);
t_token	**list_to_array(t_token *tok_list);
void	set_token_types(t_token **tok_array);

/*
void	print_type(t_token *head) {
	if (head->type == FILE_NAME)
		write(1, "FILE_NAME\t", 10);
	else
		write(1, "\t\t", 2);
}
*/

void	print_tok_list(t_token *head) {
	while (head) {
//		print_type(head);
		write(1, head->start, head->len);
		write(1, "\n", 1);
		head = head->next;
	}
}

void	print_nodes(t_node *node);
//NEEEEDS BETTER FREEEEES
t_node	**parse_line(char *line) {
	t_node	*nodes;
	t_token	*tok_list;
	t_token	**tok_array;

	tok_list = lexer(line);
	if (!tok_list)
		return (NULL);
	tok_array = list_to_array(tok_list);
	if (!tok_array)
		return (free_token_list(tok_list), NULL);
	if (!is_syntaxically_correct(tok_array))
		return (free_token_array(tok_array), NULL);
	set_token_types(tok_array);
	nodes = create_nodes(tok_list);
//	print_tok_list(tok_list);
	print_nodes(nodes);
	//free_token_array(tok_array);
	if (!nodes)
		return (NULL);

	return (NULL);
}
