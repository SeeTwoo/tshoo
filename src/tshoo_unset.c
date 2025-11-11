#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "nodes.h"

int	tshoo_unset(t_node *cmd, t_env *env) {
	if (!cmd->as.cmd.arg[1])
		return (1);
	for (int i = 1; cmd->as.cmd.arg[i]; i++) {
		delete_kv_node(&(env->env_list), cmd->as.cmd.arg[i]);
	}
	return (0);
}
