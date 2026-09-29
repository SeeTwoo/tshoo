#ifndef STRING_H
#define STRING_H

struct string {
	char	*buffer;
	size_t	capacity;
	size_t	len;
};

struct string	*st_create();
void		st_destroy(struct string *);
int		st_push(struct string *, char);
int		st_remove(struct string *, size_t);
int		insert(struct string *, size_t, char);

#endif
