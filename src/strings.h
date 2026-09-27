#ifndef STRING_H
#define STRING_H

struct string {
	char	*s;
	size_t	capacity;
	size_t	used;
};

struct string	*st_create();
void		st_destroy(struct string *);
int		st_push(struct string *, char);
int		insert(struct string *, char);

#endif
