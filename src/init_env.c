#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "messages.h"

struct s_key_value {
	char		*key;
	char		*value;
	t_key_value	*next;
};

void	display_kv(t_key_value *list) {
	while (list) {
		printf("%s and %s\n", list->key, list->key);
		list = list->next;
	}
}

static t_key_value *env_to_list(char **env) {
	t_key_value	*head;
	t_key_value	*tail;

	if (!(*env))
		return (NULL);
	head = new_kv_node(*env);
	if (!head)
		return (dprintf(2, "%s%s, %s\n", WARN_HD, ERR_MALLOC, NO_ENV), NULL);
	tail = head;
	env++;
	while (*env) {
		tail->next = new_kv_node(*env);
		if (!(tail->next))
			return (dprintf(2, "%s%s, %s\n", WARN_HD, ERR_MALLOC, NO_ENV), head);
		tail = tail->next;
		env++;
	}
	return (head);
}

int	init_env(t_env *env, char **arg_env) {
	env->env_list = env_to_list(arg_env);
	env->env_list = NULL;
	env->history = NULL;
	env->aliases = NULL;
	env->should_exit = false;
	env->style = E_FRIENDLY;
	return (0);
}
