#include <stdlib.h>
#include <string.h>

#include "strings.h"

struct string	*st_create()
{
	struct string	*s= malloc(sizeof(struct string));

	if (!s)
		return NULL;
	s->buffer = malloc(sizeof(char) * 8);
	if (!s->buffer)
		goto fail1;
	s->capacity = 8;
	s->len = 0;
	return s;
  fail1:
	free(s);
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

void	st_destroy(struct string *s)
{
	free(s->buffer);
	free(s);
}

int	st_remove(struct string *s, size_t index)
{
	if (index > s->len)
		return 1;
	memmove(&s->buffer[index], &s->buffer[index + 1], s->len - index);
	s->len--;
	return 0;
}

int	st_insert(struct string *s, size_t index, char c)
{
	char	*new;

	if (index > s->len)
		return 1;
	if (s->len >= s->capacity) {
		new = realloc(s->buffer, s->capacity * 2);
		if (!new)
			return -1;
		s->capacity *= 2;
		s->buffer = new;
	}
	memmove(&s->buffer[index + 1], &s->buffer[index], s->len - index);
	s->buffer[index] = c;
	s->len++;
	return 0;
}
