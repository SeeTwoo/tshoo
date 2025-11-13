#include <stdio.h>
#include <string.h>

#include "env.h"
#include "options.h"
#include "messages.h"

void	set_options(t_env *env, int ac, char **av) {
	for (int i = 1; i < ac ; i++) {
		if (strcmp(av[i], POSIX) == 0)
			env->style = E_POSIX;
		else if (strcmp(av[i], FRIENDLY) == 0)
			env->style = E_FRIENDLY;
		else if (strcmp(av[i], NORC) == 0)
			env->norc = true;
		else
			dprintf(2, "%s%s: %s\n", MSTK_HD, av[i], UNKNOWN_OPTION);
	}
}
