#include <string.h>
#include <unistd.h>

#include "env.h"
#include "nodes.h"

int	expand_command(char **cmd, t_key_value *env);
int	setup_redirections(t_node *node);
int	trim_command(t_node *node);
int	tshoo_alias(t_node *cmd, t_env *env);
int	tshoo_cd(t_node *cmd, t_env *env);
int	tshoo_env(t_node *cmd, t_env *env);
int	tshoo_exit(t_node *cmd, t_env *env);
int	tshoo_echo(t_node *cmd, t_env *env);
int	tshoo_style(t_node *cmd, t_env *env);
int	tshoo_unalias(t_node *cmd, t_env *env);
int	tshoo_unset(t_node *cmd, t_env *env);

typedef int	(*t_builtin)(t_node *cmd, t_env *env);

t_builtin	is_builtin(char *name) {
	if (!name)
		return (NULL);
	if (strcmp(name, "exit") == 0)
		return (tshoo_exit);
	else if (strcmp(name, "echo") == 0)
		return (tshoo_echo);
	else if (strcmp(name, "cd") == 0)
		return (tshoo_cd);
	else if (strcmp(name, "alias") == 0)
		return (tshoo_alias);
	else if (strcmp(name, "unalias") == 0)
		return (tshoo_unalias);
	else if (strcmp(name, "env") == 0)
		return (tshoo_env);
	else if (strcmp(name, "unset") == 0)
		return (tshoo_unset);
	else if (strcmp(name, "style") == 0)
		return (tshoo_style);
	return (NULL);
}

void	exec_builtin(t_builtin func, t_node *node, t_env *env) {
	int	saved_stdin = dup(STDIN_FILENO);
	int	saved_stdout = dup(STDOUT_FILENO);

	expand_command(node->command, env->env_list);
	trim_command(node);
	setup_redirections(node);
	func(node, env);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
