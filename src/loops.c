#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "nodes.h"
#include "env.h"

#include "tshoo_line.h"

#ifndef PROMPT
# define PROMPT "\x1b[1;32m\x1b[?2004ltrain> \x1b[0m"
#endif

char	*aliasing(char *line, t_key_value *aliases);
void	build_prompt(char *prompt, char *format, t_env *env);
int		exec(t_node *nodes, t_env *env);
void	free_ast(t_node *ast);
char	*get_next_line(int fd);
t_node	*parse_line(char *line);
void	print_nodes(t_node **array);

void	nullifythenewline(char *line) {
	char	*nl;

	nl = strrchr(line, '\n');
	if (!nl)
		return ;
	*nl = '\0';
}

int	process_line(char *line, t_env *env) {
	t_node		*ast;

	if (line[0] == '#')
		return (0);
	ast = parse_line(line);
	if (!ast)
		return (1);
	exec(ast, env);
	free_ast(ast);
	return (0);
}

char	*get_script_line(int fd, t_env *env) {
	char	*line = get_next_line(fd);
	char	*aliased_line;

	if (!line)
		return NULL;
	nullifythenewline(line);
	aliased_line = aliasing(line, env->aliases);
	free(line);
	return (aliased_line);
}

int	script_loop(t_env *env, char *path) {
	char	*line;
	int		fd = open(path, O_RDONLY);

	if (fd == -1)
		return (1);
	while (1) {
		line = get_script_line(fd, env);
		if (!line)
			return (close(fd), 1);
		process_line(line, env);
		free(line);
	}
	close(fd);
	return (0);
}

char	*get_interactive_line(char *prompt, t_env *env) {
	char	*line = tshoo_line(prompt, env->history);
	char	*aliased_line;

	if (!line)
		return NULL;
	tshoo_add_hist(line, env->history);
	aliased_line = aliasing(line, env->aliases);
	free(line);
	return (aliased_line);
}


int	interactive_loop(t_env *env) {
	char		prompt[256];
	char		*line;
	t_node		*ast;

	env->history = tshoo_init_hist();
	while (!env->should_exit) {
		build_prompt(prompt, get_kv_value(env->env_list, "PS1"), env);
		line = get_interactive_line(prompt, env);
		if (!line)
			return (tshoo_free_hist(env->history), 1);
		if (line[0] == '#') {
			free(line);
			continue ;
		}
		ast = parse_line(line);
		free(line);
		if (!ast)
			continue ;
		exec(ast, env);
		free_ast(ast);
	}
	tshoo_free_hist(env->history);
	return (0);
}
