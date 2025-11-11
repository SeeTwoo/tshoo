#ifndef NODES_H
# define NODES_H

#include <stdbool.h>
#include <sys/types.h>

#include "redirections.h"
#include "token_and_node_types.h"

typedef struct s_node t_node;

typedef struct s_binary_node {
	t_node		*left;
	t_node		*right;
} binary_node;

typedef struct s_cmd_node {
	char	**arg;
	int		in;
	int		out;
	t_redir	*in_redir;
	t_redir	*out_redir;
	pid_t	pid;
} t_cmd_node;

struct s_node {
	e_kind	kind;
	bool		subshell;
	union {
		t_cmd_node	cmd;
		binary_node	binary;
	} as;
};

#endif
