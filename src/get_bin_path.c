#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "nodes.h"

static char	*build_temp_path(char *path, char *name) {
	size_t	path_len = strlen(path);
	size_t	name_len = strlen(name);
	char	*temp = malloc(sizeof(char) * (path_len + name_len + 2));

	if (!temp)
		return (NULL);
	strcpy(temp, path);
	strcpy(temp + path_len, "/");
	strcpy(temp + path_len + 1, name);
	return (temp);
}

static char	*find_command_full_path(char *paths, char *name) {
	char	*path_dup = strdup(paths);
	char	*tok;
	char	*temp;

	if (!path_dup)
		return (NULL);
	tok = strtok(path_dup, ":");
	while (tok) {
		temp = build_temp_path(tok, name);
		if (access(temp, X_OK) == 0) {
			free(path_dup);
			return (temp);
		}
		free(temp);
		tok = strtok(NULL, ":");
	}
	free(path_dup);
	return (NULL);
}

int	get_bin_path(t_node *node, char *path_variable) {
	char	*command_path;

	if (!node || !(node->as.cmd.arg) || !(node->as.cmd.arg[0]))
		return (1);
	if (access(node->as.cmd.arg[0], X_OK) == 0)
		return (0);
	if (!path_variable)
		return (1);
	command_path = find_command_full_path(path_variable, node->as.cmd.arg[0]);
	if (!command_path)
		return (1);
	free(node->as.cmd.arg[0]);
	node->as.cmd.arg[0] = command_path;
	return (0);
}
