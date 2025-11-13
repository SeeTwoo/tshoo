#include <stdlib.h>
#include <unistd.h>

#include "nodes.h"
#include "token.h"

void	free_token_list(t_token *list);
int		is_syntaxically_correct(t_token *toks);
t_token	*lexer(char *line);
t_node	*parser(t_token *toks);
void	set_token_types(t_token *toks);

void	print_nodes(t_node *node);
void	print_colored_line(t_token *head);

t_node	*parse_line(char *line) {
	t_token	*toks;
	t_node	*ast;

	toks = lexer(line);
	if (!toks)
		return (NULL);
	ast = parser(toks);
	free_token_list(toks);
	return (ast);
}
