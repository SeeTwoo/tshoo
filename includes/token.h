#ifndef TOKEN_H
# define TOKEN_H

# include <stddef.h>

typedef enum enum_token_kinds	e_token_kind;

enum enum_token_kinds {
	CMD,
	WORD,
	PIPE,
	IN,
	HD,
	TRUNC = 512,
	APPEND = 1024,
	FILE_NAME,
	WRONG,
	AND,
	OR,
	OPEN_PAR,
	CLOS_PAR,
	SEMI_COL
};

typedef struct s_token	t_token;

struct s_token {
	e_token_kind	type;
	char			*lexeme;
	t_token			*prev;
	t_token			*next;
};

#endif
