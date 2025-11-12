#ifndef TOKEN_H
# define TOKEN_H

# include <stddef.h>

# include "kinds.h"

typedef struct s_token	t_token;

struct s_token {
	e_kind	kind;
	char	*lexeme;
	t_token	*prev;
	t_token	*next;
};

#endif
