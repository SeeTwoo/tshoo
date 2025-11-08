#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "env.h"
#include "nodes.h"
#include "token.h"
#include "redirections.h"

void	safer_close(int *fd) {
	if (*fd < 3)
		return ;
	close(*fd);
	*fd = -1;
}

void	close_every_fd(t_node *ast) {
	if (!ast)
		return ;
	safer_close(&ast->in);
	safer_close(&ast->out);
	close_every_fd(ast->right);
	close_every_fd(ast->left);
}

void	free_double_array(char **array) {
	for (int i = 0; array[i]; i++)
		free(array[i]);
	free(array);
}

void	free_redir_node(t_redir *node) {
	if (!node)
		return ;
	if (node->name)
		free(node->name);
	free(node);
}

void	free_list_redir(t_redir *head) {
	t_redir	*temp;

	while (head) {
		temp = head;
		head = head->next;
		free_redir_node(temp);
	}
}

void	free_node(t_node *node) {
	if (!node)
		return ;
	if (node->arg)
		free_double_array(node->arg);
	if (node->in_redir)
		free_list_redir(node->in_redir);
	if (node->out_redir)
		free_list_redir(node->out_redir);
	free(node);
}

void	free_ast(t_node *ast) {
	if (!ast)
		return ;
	free_ast(ast->right);
	free_ast(ast->left);
	free_node(ast);
}

void	free_token_array(t_token **tok_array) {
	for (int i = 0; tok_array[i]; i++)
		free(tok_array[i]);
	free(tok_array);
}

void	free_token_list(t_token *head) {
	t_token	*temp;

	while (head) {
		temp = head;
		head = head->next;
		free(temp);
	}
}
