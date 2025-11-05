#include <stdio.h>

#include "nodes.h"
#include "redirections.h"
#include "token_and_node_types.h"

void	print_command(char **command) {
	if (!command)
		return ;
	dprintf(2, "command: ");
	for (int i = 0; command[i]; i++) {
		dprintf(2, "%s", command[i]);
		if (command[i + 1])
			dprintf(2, ", ");
	}
	dprintf(2, "\n");
}

void	print_redir(t_redir *redir) {
	if (!redir)
		return ;
	while (redir) {
		dprintf(2, "%s", redir->name);
		if (redir->next)
			dprintf(2, ", ");
		redir = redir->next;
	}
	dprintf(2, "\n");
}

void	print_type(int type) {
	if (type == CMD)
		printf("CMD");

	else if (type == PIPE)
		printf("PIPE");

	else if (type == AND)
		printf("AND");

	else if (type == OR)
		printf("OR");

	else if (type == SEMI_COL)
		printf("SEMI_COL");
}

void	print_nodes(t_node *node) {
	if (!node)
		return ;
	printf("   --- node ---\n");
	print_type(node->type);
	if (node->type == CMD)
		printf("\t\t%s\n", node->arg[0]);
	else
		printf("\n");
	printf("\n");
	print_nodes(node->left);
	print_nodes(node->right);
}
