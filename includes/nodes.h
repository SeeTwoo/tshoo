#ifndef NODES_H
# define NODES_H

#include <stdbool.h>
#include <sys/types.h>

#include "env.h"
#include "redirections.h"


typedef struct s_node			t_node;
typedef struct s_cmd_node		cmd_node;
typedef struct s_binary_node	binary_node;
typedef int	(builtin)(t_node *, t_env *);

struct s_binary_node {
	t_node		*left;
	t_node		*right;
};

struct s_cmd_node {
	char	**arg;
	int		in;
	int		out;
	t_redir	*in_redir;
	t_redir	*out_redir;
	int		pid;
	int		exit_status;
	builtin	*func;
};

struct s_node {
	e_node_kind	kind;
	bool		subshell;
	union {
		cmd_node		cmd;
		binary_node		binary;
	} as;
};

#endif
