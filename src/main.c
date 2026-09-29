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

//deal memory error of st_push
int	fill_escape_mask(struct string *escape_mask, struct string *line)
{
	for (size_t i = 0; i < line->len; i++) {
		if (line->buffer[i] == '\\') {
			st_push(escape_mask, 'E');
			st_remove(line, i);
		} else {
			st_push(escape_mask, '-');
		}
	}
	return 0;
}

int	process_line(struct string *line, bool *should_exit)
{
	struct string	*escape_mask = st_create();

	if (!escape_mask)
		return 1;
	fill_escape_mask(escape_mask, line);
	write(1, line->buffer, line->len);
	write(1, "\n", 1);
	write(1, escape_mask->buffer, escape_mask->len);
	write(1, "\n", 1);
	if (strncmp(line->buffer, "exit", 4) == 0)
		*should_exit = true;
	st_destroy(escape_mask);
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
