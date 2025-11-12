#include <stdio.h>
#include <unistd.h>

#include "nodes.h"
#include "token.h"
#include "redirections.h"

static void	print_type(int type) {
	if (type == CMD)
		dprintf(2, "CMD");
	else if (type == BUILTIN)
		dprintf(2, "BUILTIN");
	else if (type == PIPE)
		dprintf(2, "PIPE");
	else if (type == AND)
		dprintf(2, "AND");
	else if (type == OR)
		dprintf(2, "OR");
	else if (type == LST)
		dprintf(2, "LST");
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

void	print_nodes(t_node *node) {
	if (!node)
		return ;
	dprintf(2, "   --- node ---\n");
	print_type(node->kind);
	if (node->kind == CMD || node->kind == BUILTIN) {
		dprintf(2, "\t\t%s\n", node->as.cmd.arg[0]);
		dprintf(2, "in = %d and out = %d\n", node->as.cmd.in, node->as.cmd.out);
	} else
		dprintf(2, "\n");
	dprintf(2, "\n");
	if (node->kind != CMD && node->kind != BUILTIN) {
		print_nodes(node->as.binary.left);
		print_nodes(node->as.binary.right);
	}
}
