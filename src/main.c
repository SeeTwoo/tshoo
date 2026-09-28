#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "strings.h"

void	welcome_screen()
{
	printf("hello, world !\n");
}


struct string	*line_editor()
{
	struct string	*line;

	write(2, "$> ", 3);
	line = st_create();
	if (!line)
		return NULL;
	while (true) {
		char	c;
		read(0, &c, 1);

		if (c == '\n')
			break ;
		if (st_push(line, c) == -1)
			goto fail;
	}
	return line;
  fail:
	st_destroy(line);
	return NULL;
}

int	main()
{
	bool	should_continue = true;

	welcome_screen();
	while (should_continue) {
		struct string	*line = line_editor();

		if (!line || strncmp(line->buffer, "exit", 4) == 0) {
			should_continue = false;
			continue ;
		}
		write(1, line->buffer, line->len);
		write(1, "\n", 1);
		st_destroy(line);
	}
	return 0;
}
