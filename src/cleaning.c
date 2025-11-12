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
	if (ast->kind == CMD) {
		safer_close(&ast->as.cmd.in);
		safer_close(&ast->as.cmd.out);
	} else {
		close_every_fd(ast->as.binary.right);
		close_every_fd(ast->as.binary.left);
	}
}

void	free_double_array(char **array) {
	if (!array)
		return ;
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
	if (node->kind == CMD) {
		free_double_array(node->as.cmd.arg);
		free_list_redir(node->as.cmd.in_redir);
		free_list_redir(node->as.cmd.out_redir);
	}
	free(node);
}

void	free_ast(t_node *ast) {
	if (!ast)
		return ;
	if (ast->kind == CMD || ast->kind == BUILTIN) {
		free_node(ast);
	} else {
		free_ast(ast->as.binary.right);
		free_ast(ast->as.binary.left);
		free_node(ast);
	}
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
		free(temp->lexeme);
		free(temp);
	}
}
