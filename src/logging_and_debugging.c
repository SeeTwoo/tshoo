#include <stdio.h>
#include <unistd.h>

#include "nodes.h"
#include "token.h"
#include "redirections.h"
#include "token_and_node_types.h"

/*
void	print_type(t_token *head) {
	if (head->type == FILE_NAME)
		write(1, "FILE_NAME\t", 10);
	else
		write(1, "\t\t", 2);
}
*/

static void	print_type(int type) {
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

	else if (type == WORD)
		printf("WORD");

	else if (type == WRONG)
		printf("WRONG");
}

void	print_colored_line(t_token *head) {
	while (head) {
		printf("\x1b[%dm", head->sublvl + 31);
		fflush(stdout);
		write(1, head->start, head->len);
		fflush(stdout);
		write(1, "\x1b[0m\t\t", 6);
		fflush(stdout);
		print_type(head->type);
		fflush(stdout);
		write(1, "\n", 1);
		fflush(stdout);
		head = head->next;
	}
	write(1, "\n", 1);
}

void	print_tok_list(t_token *head) {
	while (head) {
//		print_type(head);
		write(1, head->start, head->len);
		write(1, "\n", 1);
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
