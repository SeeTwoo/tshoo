#include <stdlib.h>

#include "strings.h"

struct string	*st_create()
{
	struct string	*string = malloc(sizeof(struct string));

	if (!string)
		return NULL;
	string->s = malloc(sizeof(char) * 8);
	if (!string->s)
		goto fail1;
	string->capacity = 8;
	string->used = 0;
	return string;
  fail1:
	free(string);
	return NULL;
}
