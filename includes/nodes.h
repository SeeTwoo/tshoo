#ifndef NODES_H
# define NODES_H

#include <stdbool.h>
#include <sys/types.h>

#include "redirections.h"

typedef enum enum_node_kinds	e_node_kind;

typedef struct s_node			t_node;
typedef struct s_built			t_built;
typedef struct s_binary_node	t_binary_node;
typedef struct s_cmd_node		t_cmd_node;
typedef int	(*t_builtin_function)(t_node *, t_env *);

struct s_binary_node {
	t_node		*left;
	t_node		*right;
};

struct s_built {
	t_builtin_function	*func;
	int					exit_status;
};

struct s_cmd_node {
	char	**arg;
	int		in;
	int		out;
	t_redir	*in_redir;
	t_redir	*out_redir;
	union {
		int		pid;
		t_built	builtin;
	} as;
};

struct s_node {
	e_node_kind	kind;
	bool		subshell;
	union {
		t_cmd_node	cmd;
		binary_node	binary;
	} as;
};

enum enum_node_kinds {
	CMD,
	BUILTIN,
	LST,
	PIPE,
	AND,
	OR
};

#endif
