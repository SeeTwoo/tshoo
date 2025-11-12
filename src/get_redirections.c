#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "nodes.h"

int	is_in_redir(t_token *token);
int	is_out_redir(t_token *token);
int	is_redir(t_token *token);

void	add_redir(t_redir **tail, t_redir **head, t_token *toks) {
	t_redir *new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	if (!(*head)) {
		*head = new;
		*tail = *head;
	} else {
		(*tail)->next = new;
		*tail = (*tail)->next;
	}
	new->name = NULL;
	new->name = strdup(toks->next->lexeme);
	///better freeeing pls
	new->type = toks->type;
	new->next = NULL;
}

int	get_redirections(t_node *node, t_token *toks) {
	t_redir	*in_tail;
	t_redir	*out_tail;

	node->as.cmd.in_redir = NULL;
	node->as.cmd.out_redir = NULL;
	in_tail = NULL;
	out_tail = NULL;
	while (toks && (is_redir(toks) || toks->type == WORD || toks->type == FILE_NAME)) {
		if (is_in_redir(toks)) {
			add_redir(&in_tail, &(node->as.cmd.in_redir), toks);
			toks = toks->next->next;
		} else if (is_out_redir(toks)) {
			add_redir(&out_tail, &(node->as.cmd.out_redir), toks);
			toks = toks->next->next;
		} else {
			toks = toks->next;
		}
	}
	return (0);
}
