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

/*
static void	print_tok_list(t_token *head) {
	while (head) {
//		print_type(head);
		dprintf(2, "%s\n", head->lexeme);
		head = head->next;
	}
}
*/

int	get_depth(t_node *node, int depth) {
	if (node->kind == CMD || node->kind == BUILTIN) {
		return (depth);
	} else {
		int	left = get_depth(node->as.binary.left, depth++);
		int	right = get_depth(node->as.binary.right, depth++);
		return (left > right ? left : right);
	}
}

void	print_level(t_node *node, int i) {
	if (i == 0) {
		print_type(node->kind);
		dprintf(2, "\t");
	} else if (node->kind != CMD || node->kind != BUILTIN) {
		print_level(node->as.binary.left, i - 1);
		print_level(node->as.binary.right, i - 1);
	}
}

void	print_nodes(t_node *node) {
	int	depth = get_depth(node, 0);

	for (int i = 0; i <= depth; i++) {
		print_level(node, i);
		dprintf(2, "\n");
	}
}
