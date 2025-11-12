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
