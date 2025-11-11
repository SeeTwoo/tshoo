#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env.h"
#include "messages.h"
#include "nodes.h"

struct s_key_value {
	char		*key;
	char		*value;
	t_key_value *next;
};

int	array_len(char **array) {
	int	i = 0;

	while (array[i])
		i++;
	return (i);
}

char	*get_destination(t_node *node, t_key_value *list, int ac) {
	char	*destination;

	if (ac == 1) {
		destination = get_kv_value(list, "HOME");
		if (!destination)
			dprintf(2, "%s%s\n", WARN_HD, NO_HOME);
	} else if (ac == 2 && strcmp("-", node->as.cmd.arg[1]) == 0) {
		destination = get_kv_value(list, "OLDPWD");
		if (!destination)
			dprintf(2, "%s%s\n", WARN_HD, NO_OLDPWD);
	} else {
		destination = node->as.cmd.arg[1];
	}
	return (destination);
}

int	tshoo_cd(t_node *node, t_env *env) {
	t_key_value	*old_pwd = kv_chr(env->env_list, "OLDPWD");
	t_key_value	*pwd = kv_chr(env->env_list, "PWD");
	char		*path;
	int			ac = array_len(node->as.cmd.arg);

	if (ac > 2)
		return (dprintf(2, "%s%s : %s\n", WARN_HD, "cd", TOO_MANY), 1);
	path = get_destination(node, env->env_list, ac);
	if (!path)
		return (1);
	if (access(path, F_OK) != 0)
		return (1);
	if (chdir(path) == -1)
		return (dprintf(2, "%s%s : %s\n", WARN_HD, node->as.cmd.arg[1], strerror(errno)), 1);
	free(old_pwd->value);
	old_pwd->value = strdup(pwd->value);
	free(pwd->value);
	pwd->value = getcwd(NULL, 0);
	if (!(pwd->value))
		dprintf(2, "%s%s: %s\n", WARN_HD, ERR_MALLOC, NO_ENV);
	return (0);
}
