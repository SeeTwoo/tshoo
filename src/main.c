#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "strings.h";

void	welcome_screen()
{
	printf("hello, world !\n");
}

struct string	*line_editor()
{
	struct string	*line = st_create();
	size_t		cursor;
	
	if (!line)
		return NULL;
	return line;
}

int	main()
{
	welcome_screen();
	while (true) {
		struct string	line = line_editor();
		string_destroy(&line);
	}
	return 0;
}
