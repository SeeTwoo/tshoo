#ifndef TOKEN_AND_NODE_TYPES_H
# define TOKEN_AND_NODE_TYPES_H

typedef enum enum_kinds	e_kind;

enum enum_kinds {
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

#endif
