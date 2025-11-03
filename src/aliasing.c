#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "env.h"
#include "messages.h"

static size_t	skip_whitespaces(char *line) {
	size_t	i;

	i = 0;
	while (line[i] && isspace(line[i]))
		i++;
	return (i);
}

static size_t	skip_to_next_command_block(char *line) {
	size_t	i;

	i = 0;
	while (line[i] && line[i] != '|')
		i++;
	if (line[i] == '|')
		i++;
	return (i);
}

static char *replace(char *old_line, char *current, char *alias, size_t key_len) {
	char	*line;

	line = malloc(sizeof(char) * (strlen(old_line) - key_len + strlen(alias) + 1));
	if (!line)
		return (dprintf(2, "%s%s: %s\n", WARN_HD, ERR_MALLOC, NOT_ALIASED), old_line);
	*current = '\0';
	sprintf(line, "%s%s%s", old_line, alias, current + key_len);
	free(old_line);
	return (line);
}

static char	*search_and_replace(char *line, char *current, t_key_value *aliases) {
	char	*alias;
	size_t	key_len;

	key_len = strcspn(current, " \r\t\n<>|");
	alias = get_kv_n_value(aliases, current, key_len);
	if (!alias)
		return (line);
	return (replace(line, current, alias, key_len));
}

char	*aliasing(char *line, t_key_value *aliases) {
	size_t	current;

	if (!aliases)
		return (line);
	current = 0;
	while (*(line + current)) {
		current += skip_whitespaces(line + current);
		line = search_and_replace(line, (char *)(line + current), aliases);
		current += skip_to_next_command_block(line + current);
	}
	return (line);
}
