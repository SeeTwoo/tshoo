#include <stdio.h>
#include <unistd.h>

#include "nodes.h"
#include "token.h"
#include "redirections.h"
#include "token_and_node_types.h"

static void	print_type(int type) {
	if (type == CMD)
		dprintf(2, "CMD");

	else if (type == PIPE)
		dprintf(2, "PIPE");

	else if (type == AND)
		dprintf(2, "AND");

	else if (type == OR)
		dprintf(2, "OR");

	else if (type == SEMI_COL)
		dprintf(2, "SEMI_COL");

	else if (type == WORD)
		dprintf(2, "WORD");

	else if (type == WRONG)
		dprintf(2, "WRONG");
}

void	print_tok_list(t_token *head) {
	while (head) {
//		print_type(head);
		dprintf(2, "%s\n", head->lexeme);
		head = head->next;
	}
}

/*
static void	print_command(char **command) {
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

static void	print_redir(t_redir *redir) {
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
*/

void	print_nodes(t_node *node) {
	if (!node)
		return ;
	dprintf(2, "   --- node ---\n");
	print_type(node->kind);
	if (node->kind == CMD) {
		dprintf(2, "\t\t%s\n", node->as.cmd.arg[0]);
		dprintf(2, "in = %d and out = %d\n", node->as.cmd.in, node->as.cmd.out);
	} else
		dprintf(2, "\n");
	dprintf(2, "\n");
	if (node->kind != CMD) {
		print_nodes(node->as.binary.left);
		print_nodes(node->as.binary.right);
	}
}
