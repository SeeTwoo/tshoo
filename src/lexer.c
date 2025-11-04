#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "token_and_node_types.h"

void	free_token_list(t_token *head);

static void	skip_whitespaces(char **line) {
	while (isspace(**line))
		(*line)++;
}

static int	get_type(char current, int len) {
	if (current == '<' && len == 1)
		return (IN);
	else if (current == '<' && len == 2)
		return (HD);
	else if (current == '>' && len == 1)
		return (TRUNC);
	else if (current == '>' && len == 2)
		return (APPEND);
	else if (current == '|' && len == 1)
		return (PIPE);
	return (WRONG);
}

t_token *separator_token(char **line) {
	t_token	*new = malloc(sizeof(t_token));
	char	current = **line;
	int		len = 0;

	if (!new)
		return (NULL);
	while ((*line)[len] == current)
		len++;
	new->start = *line;
	new->len = len;
	new->next = NULL;
	new->type = get_type(current, len);
	(*line) += len;
	return (new);
}

static int	isseparator(char c) {
	return (
		c == '<' ||
		c == '>' ||
		c == '|'
	);
}

static int	quote_to_quote_len(char *line) {
	int		i = 0;
	char	quote = line[0];

	i++;
	while (line[i] && line[i] != quote)
		i++;
	if (line[i] == quote)
		i++;
	return (i);
}

static size_t	tok_len(char **line) {
	size_t	i;
	
	i = 0;
	while (1) {
		if (!(*line)[i] || isspace((*line)[i]) || isseparator((*line)[i]))
			break ;
		else if ((*line)[i] == '\'' || (*line)[i] == '\"')
			i += quote_to_quote_len(&(*line)[i]);
		else
			i++;
	}
	(*line) += i;
	return (i);
}

t_token	*word_token(char **line) {
	t_token *new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->start = *line;
	new->len = tok_len(line);
	new->next = NULL;
	new->type = WORD;
	return (new);
}

t_token	*lexer(char *line) {
	t_token	*head;
	t_token	*tail;

	head = NULL;
	tail = NULL;
	skip_whitespaces(&line);
	if (!(*line))
		return (NULL);
	if (*line == '<' || *line == '>' || *line == '|')
		head = separator_token(&line);
	else
		head = word_token(&line);
	if (!head)
		return (NULL);
	tail = head;
	while (*line) {
		skip_whitespaces(&line);
		if (*line == '<' || *line == '>' || *line == '|')
			tail->next = separator_token(&line);
		else
			tail->next = word_token(&line);
		if (!tail->next)
			return (free_token_list(head), NULL);
		tail = tail->next;
	}
	return (head);
}
