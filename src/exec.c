#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "env.h"
#include "messages.h"
#include "nodes.h"

typedef int	(*t_builtin)(t_node *cmd, t_env *env);

void		assign_variable(t_env *env, char *new);
void		close_every_fd(t_node *ast);
char		**list_to_env(t_key_value *env);
int			expand_command(char **cmd, t_key_value *env);
void		free_ast(t_node *ast);
void		free_double_array(char **array);
void		free_kv_list(t_key_value*);
int			get_bin_path(t_node *node, char *path);
char		*get_kv_value(t_key_value *list, char *key);
void		print_nodes(t_node *nodes);
void		safer_close(int *fd);
int			setup_redirections(t_node *command);
int			trim_command(t_node *node);

int			expand_command(char **cmd, t_key_value *env);
int			setup_redirections(t_node *node);
int			trim_command(t_node *node);
int			tshoo_alias(t_node *cmd, t_env *env);
int			tshoo_cd(t_node *cmd, t_env *env);
int			tshoo_env(t_node *cmd, t_env *env);
int			tshoo_exit(t_node *cmd, t_env *env);
int			tshoo_echo(t_node *cmd, t_env *env);
int			tshoo_style(t_node *cmd, t_env *env);
int			tshoo_unalias(t_node *cmd, t_env *env);
int			tshoo_unset(t_node *cmd, t_env *env);

static t_builtin	is_builtin(char *name) {
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

static void	exec_builtin(t_builtin func, t_node *node, t_env *env) {
	int	saved_stdin = dup(STDIN_FILENO);
	int	saved_stdout = dup(STDOUT_FILENO);

	expand_command(node->as.cmd.arg, env->env_list);
	trim_command(node);
	setup_redirections(node);
	func(node, env);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	safer_close(&saved_stdin);
	safer_close(&saved_stdout);
}

static int	get_pipes(t_node *ast, int in, int out) {
	int	fds[2];

	if (ast->kind == PIPE) {
		pipe(fds);
		get_pipes(ast->as.binary.left, in, fds[1]);
		get_pipes(ast->as.binary.right, fds[0], out);
	} else if (ast->kind == CMD) {
		ast->as.cmd.in = in;
		ast->as.cmd.out = out;
	} else {
		get_pipes(ast->as.binary.left, STDIN_FILENO, out);
		get_pipes(ast->as.binary.right, in, STDOUT_FILENO);
	}
	return (0);
}

static int	wait_ast(t_node *ast) {
	int	status = -1;

	if (!ast)
		return (-1);
	if (ast->kind == CMD) {
		waitpid(ast->as.cmd.pid, &status, 0);
		return (status);
	}
	wait_ast(ast->as.binary.left);
	status = wait_ast(ast->as.binary.right);
	return (status);
}

static void	clean_child_process(t_node *nodes, t_env *env) {
	close_every_fd(nodes);
	free_ast(nodes);
	tshoo_free_hist(env->history);
	free_kv_list(env->aliases);
	free_kv_list(env->env_list);
}

static void	error_child_process(t_node *nodes, t_env *env) {
	clean_child_process(nodes, env);
	exit(EXIT_FAILURE);
}

static int	exec_command(t_node *command, t_env *env, t_node *ast_root) {
	char	**cmd_args;
	char	**cmd_env;

	command->as.cmd.pid = fork();
	if (command->as.cmd.pid == -1)
		return (dprintf(2, "%s%s\n", FATAL_HD, ERR_FORK), 1);
	if (command->as.cmd.pid != 0)
		return (0);
	sigaction(SIGINT, &(struct sigaction){.sa_handler = SIG_DFL}, NULL);
	expand_command(command->as.cmd.arg, env->env_list);
	trim_command(command);
	if (setup_redirections(command) == 1)
		error_child_process(ast_root, env);
	cmd_args = command->as.cmd.arg;
	command->as.cmd.arg = NULL;
	cmd_env = list_to_env(env->env_list);
	clean_child_process(ast_root, env);
	execve(cmd_args[0], cmd_args, cmd_env);
	free_double_array(cmd_args);
	free_double_array(cmd_env);
	exit(EXIT_FAILURE);
}

int	exec_cmd_node(t_node *ast, t_env *env, t_node *ast_root) {
	t_builtin	func;
	int			ret = 0;

	func = is_builtin(ast->as.cmd.arg[0]);
	if (func)
		exec_builtin(func, ast, env);
	else if (ast->as.cmd.arg[0] && strchr(ast->as.cmd.arg[0], '='))
		assign_variable(env, ast->as.cmd.arg[0]);
	else if (get_bin_path(ast, get_kv_value(env->env_list, "PATH")) == 0)
		ret = exec_command(ast, env, ast_root);
	else
		dprintf(2, "%s%s : %s\n", MSTK_HD, ast->as.cmd.arg[0], CMD_FND);
	if (ret == 1)
		env->should_exit = true;
	return (0);
}

int	exec_ast(t_node *ast, t_env *env, t_node *ast_root);

int	or_exec(t_node *ast, t_env *env, t_node *ast_root) {
	exec_ast(ast->as.binary.left, env, ast_root);
	if (wait_ast(ast->as.binary.left) == 0)
		return (0);
	exec_ast(ast->as.binary.right, env, ast_root);
	return (0);
}

int	and_exec(t_node *ast, t_env *env, t_node *ast_root) {
	exec_ast(ast->as.binary.left, env, ast_root);
	if (wait_ast(ast->as.binary.left) != 0)
		return (0);
	exec_ast(ast->as.binary.right, env, ast_root);
	return (0);
}

int	semi_col_exec(t_node *ast, t_env *env, t_node *ast_root) {
	exec_ast(ast->as.binary.left, env, ast_root);
	wait_ast(ast->as.binary.left);
	exec_ast(ast->as.binary.right, env, ast_root);
	return (0);
}

int	subshell(t_node *ast, t_env *env, t_node *ast_root) {
	int	pid = fork();

	if (pid != 0)
		return (wait_ast(ast), pid);
	exec_ast(ast, env, ast_root);
	wait_ast(ast);
	clean_child_process(ast_root, env);
	exit(EXIT_SUCCESS);
}

int	exec_ast(t_node *ast, t_env *env, t_node *ast_root) {
	if (ast->kind == PIPE) {
		exec_ast(ast->as.binary.left, env, ast_root);
		exec_ast(ast->as.binary.right, env, ast_root);
	} else if (ast->kind == CMD) {
		exec_cmd_node(ast, env, ast_root);
		safer_close(&ast->as.cmd.in);
		safer_close(&ast->as.cmd.out);
	}
	return (0);
}

int	exec(t_node *ast, t_env *env) {
	get_pipes(ast, STDIN_FILENO, STDOUT_FILENO);
	exec_ast(ast, env, ast);
	wait_ast(ast);
	return (0);
}
