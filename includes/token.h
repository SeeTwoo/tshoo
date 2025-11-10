#ifndef TOKEN_H
# define TOKEN_H

# include <stddef.h>

typedef struct s_token	t_token;

struct s_token {
	char	*lexeme;
	int		type;
	t_token	*prev;
	t_token	*next;
};

#endif
