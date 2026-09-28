#include <stdbool.h>
#include <stdio.h>
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
	char		*prompt = "\x1b[38;5;214m%> \x1b[0m";

	write(2, prompt, strlen(prompt));
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
	welcome_screen();
	while (true) {
		struct string	*line = line_editor();

		if (!line)
			break ;
		if (strncmp(line->buffer, "exit", 4) == 0) {
			st_destroy(line);
			break ;
		}
		write(1, line->buffer, line->len);
		write(1, "\n", 1);
		st_destroy(line);
	}
	return 0;
}
