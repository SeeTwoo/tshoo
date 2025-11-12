#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "nodes.h"

int		get_redirections(t_node *node, t_token *toks);
int		is_redir(t_token *token);
char	**tok_to_args(t_token *toks);

int			tshoo_alias(t_node *cmd, t_env *env);
int			tshoo_cd(t_node *cmd, t_env *env);
int			tshoo_env(t_node *cmd, t_env *env);
int			tshoo_exit(t_node *cmd, t_env *env);
int			tshoo_echo(t_node *cmd, t_env *env);
int			tshoo_style(t_node *cmd, t_env *env);
int			tshoo_unalias(t_node *cmd, t_env *env);
int			tshoo_unset(t_node *cmd, t_env *env);
