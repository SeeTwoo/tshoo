#ifndef TOKEN_H
# define TOKEN_H

# include <stddef.h>

# include "token_and_node_types.h"

typedef struct s_token	t_token;

struct s_token {
	char	*lexeme;
	e_kind	type;
	t_token	*prev;
	t_token	*next;
};

#endif
