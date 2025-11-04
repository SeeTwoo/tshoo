#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "env.h"
#include "nodes.h"

static char	**set_options(char **command, bool *dash_n) {
	int i;

	while (*command && *command[0] == '-') {
		i = -1;
		while (*command[++i])
			if (*command[i] == 'n')
				*dash_n = true;
		command++;
	}
	return (command);
}

int	tshoo_echo(t_node *command, t_env *env) {
	char	**args;
	bool	dash_n = false;

	(void)env;
	args = set_options(command->arg + 1, &dash_n);
	while (*args) {
		write(1, *args, strlen(*args));
		if (args[1])
			write(1, " ", 1);
		args++;
	}
	if (command->arg[1] && strncmp(command->arg[1], "-n", 2) == 0)
		return (0);
	write(1, "\n", 1);
	return (0);
}
