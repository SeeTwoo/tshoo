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

void	print_nodes(t_node **nodes) {
	for (int i = 0; nodes[i]; i++) {
		dprintf(2, "    --- node ---\n");
		print_command(nodes[i]->arg);
		if (nodes[i]->in_redir)
			dprintf(2, "\x1b[33min redirs: \x1b[0m");
		print_redir(nodes[i]->in_redir);
		if (nodes[i]->out_redir)
			dprintf(2, "\x1b[34mout redirs: \x1b[0m");
		print_redir(nodes[i]->out_redir);
		dprintf(2, "\n");
	}
}
