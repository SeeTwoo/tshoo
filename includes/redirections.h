#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

#include "kinds.h"

typedef struct s_redir	t_redir;

struct s_redir {
	char	*name;
	e_kind	kind;
	t_redir	*next;
};

#endif
