#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "nodes.h"

int	is_in_redir(t_token *token);
int	is_out_redir(t_token *token);
int	is_redir(t_token *token);

void	add_redir(t_redir **tail, t_redir **head, t_token **toks) {
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
	new->name = strdup((*toks)->next->lexeme);
	///better freeeing pls
	new->kind = (*toks)->kind;
	new->next = NULL;
	*toks = (*toks)->next->next;
}

int	get_redirections(t_node *node, t_token **toks) {
	t_redir	*in_tail = NULL;
	t_redir	*out_tail = NULL;;

	node->as.cmd.in_redir = NULL;
	node->as.cmd.out_redir = NULL;
	while (*toks && (is_redir(*toks) || (*toks)->kind == WORD || (*toks)->kind == FILE_NAME)) {
		if (is_in_redir(*toks)) {
			add_redir(&in_tail, &(node->as.cmd.in_redir), toks);
		} else if (is_out_redir(*toks)) {
			add_redir(&out_tail, &(node->as.cmd.out_redir), toks);
		} else {
			*toks = (*toks)->next;
		}
	}
	return (0);
}
