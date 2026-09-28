#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "strings.h"

struct string	*line_editor();

void	welcome_screen()
{
	printf("hello, world !\n");
}

int	process_line(struct string *line, bool *should_exit)
{
	write(1, line->buffer, line->len);
	write(1, "\n", 1);
	if (strncmp(line->buffer, "exit", 4) == 0)
		*should_exit = true;
	return 0;
}

int	main()
{
	bool	should_exit = false;

	welcome_screen();
	while (!should_exit) {
		struct string	*line = line_editor();

		if (!line)
			break ;
		process_line(line, &should_exit);
		st_destroy(line);
	}
	return 0;
}
