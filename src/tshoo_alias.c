#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "env.h"
#include "key_value.h"
#include "messages.h"
#include "nodes.h"

void	modify_kv_list(t_key_value **kv, char *new, ssize_t key_len);

static void	display_aliases(t_key_value *aliases) {
	while (aliases) {
		printf("%s=%s\n", get_key(aliases), get_value(aliases));
		aliases = get_next(aliases);
	}
}

static ssize_t	is_valid_alias(char *new_kv) {
	ssize_t	key_len;

	if (!strchr(new_kv, '='))
		return (dprintf(2, "%s%s\n", MSTK_HD, NEED_EQUAL), -1);
	key_len = strcspn(new_kv, "=");
	if (key_len == 0)
		return (dprintf(2, "%s%s\n", MSTK_HD, NEED_PATTERN), -1);
	return (key_len);
}

int	tshoo_alias(t_node *node, t_env *env) {
	ssize_t	key_len;

	if (!node->as.cmd.arg[1])
		return (display_aliases(env->aliases), 0);
	for (int i = 1; node->as.cmd.arg[i]; i++) {
		key_len = is_valid_alias(node->as.cmd.arg[i]);
		if (key_len == 1)
			continue ;
		modify_kv_list(&(env->aliases), node->as.cmd.arg[i], key_len);
	}
	return (0);
}
