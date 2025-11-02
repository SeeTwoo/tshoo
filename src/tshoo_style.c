#include <string.h>
#include <stdio.h>

#include "env.h"
#include "nodes.h"
#include "options.h"
#include "messages.h"

int	display_style(t_env *env) {
	if (env->style == E_FRIENDLY)
		printf("error messages are in friendly mode right now !\n");
	else
		printf("error messages are blunt and unhelpful\n");
	return (0);
}

int	tshoo_style(t_node *cmd, t_env *env) {
	char	**args;

	if (!(cmd->command[1]))
		return (display_style(env));
	args = cmd->command + 1;
	for (int i = 0; args[i]; i++) {
		if (strcmp(args[i], FRIENDLY) == 0)
			env->style = E_FRIENDLY;
		else if (strcmp(args[i], POSIX) == 0)
			env->style = E_POSIX;
		else
			dprintf(2, "%s%s: %s\n", MSTK_HD, args[i], UNKNOWN_OPTION);
	}
	return (0);
}
