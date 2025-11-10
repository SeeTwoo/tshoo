#ifndef NODES_H
# define NODES_H

#include <stdbool.h>
#include <sys/types.h>

#include "redirections.h"

typedef struct s_node	t_node;

struct s_node {
	char	**arg;
	int		type;
	int		in;
	int		out;
	t_redir	*in_redir;
	t_redir	*out_redir;
	pid_t	pid;
	t_node	*left;
	t_node	*right;
	int		sublvl;
	int		prec;
	int		pipe[2];
};

typedef enum {
	CMD,
	PIPE,
	AND,
	OR,
	LST
} node_kind;

typedef struct s_ast AST;

typedef struct {
	AST		*left;
	AST		*right;
} binary_node;

typedef struct {
	char	**arg;
	int		in;
	int		out;
	t_redir	*in_redir;
	t_redir	*out_redir;
	pid_t	pid;
} cmd_node;

typedef struct {
	node_kind	kind;
	bool		subshell;
	union {
		cmd_node	cmd;
		binary_node	operator;
	} as;
} AST;

#endif
