#include <stdlib.h>
#include <string.h>

#include "nodes.h"

char	*trim_string(char const *s) {
	char		*dest = malloc(sizeof(char) * (strlen(s) + 1));
	char		*write = dest;
	char const	*read = s;

	if (!dest)
		return (NULL);
	while (*read) {
		if (*read == '\'' || *read == '\"') {
			char	quote = *read;
			read++;
			while (*read && *read != quote)
				*write++ = *read++;
			if (*read == quote)
				read++;
		} else {
			*write++ = *read++;
		}
	}
	*write = '\0';
	return (dest);
}

int	trim_command(t_node *node) {
	char	**cmd;
	char	*temp;

	if (!node || !node->arg|| !node->arg[0])
		return (1);
	cmd = node->arg;
	for (int i = 0; cmd[i]; i++) {
		temp = trim_string(cmd[i]);
		if (!temp)
			continue ;
		free(cmd[i]);
		cmd[i] = temp;
		temp = NULL;
	}
	return (0);
}
