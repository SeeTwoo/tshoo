#include <stdlib.h>

#include "strings.h"

struct string	*st_create()
{
	struct string	*string = malloc(sizeof(struct string));

	if (!string)
		return NULL;
	string->buffer = malloc(sizeof(char) * 8);
	if (!string->buffer)
		goto fail1;
	string->capacity = 8;
	string->len = 0;
	return string;
  fail1:
	free(string);
	return NULL;
}

int	st_push(struct string *s, char c)
{
	char	*new;

	if (s->len >= s->capacity) {
		new = realloc(s->buffer, s->capacity * 2);
		if (!new)
			return -1;
		s->capacity *= 2;
		s->buffer = new;
	}
	s->buffer[s->len] = c;
	s->len++;
	return 0;
}

void	st_destroy(struct string *string)
{
	free(string->buffer);
	free(string);
}
