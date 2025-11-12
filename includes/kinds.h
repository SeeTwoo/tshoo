#ifndef KINDS_H
# define KINDS_H

typedef enum enum_kinds	e_kind;

enum enum_kinds {
	WORD,
	IN,
	HD,
	TRUNC = 512,
	APPEND = 1024,
	FILE_NAME,
	WRONG,
	OPEN_PAR,
	CLOS_PAR,
	CMD,
	BUILTIN,
	LST,
	PIPE,
	AND,
	OR
};

#endif
