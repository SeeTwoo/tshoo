#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "messages.h"
#include "nodes.h"
#include "redirections.h"

int	heredoc(char *limiter);

static int	setup_pipe(int new, int old) {
	return (dup2(new, old));
}

static int	setup_in_redirs(t_redir *head) {
	int	infile_fd;

	if (!head)
		return (0);
	while (head) {
		if (head->type == IN)
			infile_fd = open(head->name, O_RDONLY);
		else if (head->type == HD)
			infile_fd = heredoc(head->name);
		if (infile_fd == -1)
			return (dprintf(2, "%s%s : %s\n", WARN_HD, head->name, strerror(errno)), 1);
		if (dup2(infile_fd, STDIN_FILENO) == -1)
			return (dprintf(2, "%s%s\n", FATAL_HD, strerror(errno)), 2);
		close(infile_fd);
		head = head->next;
	}
	return (0);
}

static int	setup_out_redirs(t_redir *head) {
	int	flags;
	int	outfile_fd;

	if (!head)
		return (0);
	while (head) {
		flags = O_WRONLY | O_CREAT | head->type;
		outfile_fd = open(head->name, flags, 0644);
		if (outfile_fd == -1)
			return (dprintf(2, "%s%s : %s\n", WARN_HD, head->name, strerror(errno)), 1);
		if (dup2(outfile_fd, STDOUT_FILENO) == -1)
			return (dprintf(2, "%s%s\n", FATAL_HD, strerror(errno)), 2);
		close(outfile_fd);
		head = head->next;
	}
	return (0);
}

int	setup_redirections(t_node *node) {
	if (setup_pipe(node->as.cmd.in, STDIN_FILENO) == -1)
		return (1);
	if (setup_in_redirs(node->as.cmd.in_redir) == 1)
		return (1);
	if (setup_out_redirs(node->as.cmd.out_redir) == 1)
		return (1);
	if (setup_pipe(node->as.cmd.out, STDOUT_FILENO) == -1)
		return (1);
	return (0);
}
