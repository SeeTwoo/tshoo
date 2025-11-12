#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

#ifndef DELIMS_SET
# define DELIMS "<>&|;()"
# define MONO_DELIM "();"
# define MULTI_DELIM "<>&|"
#endif

void	free_token_list(t_token *head);
int		is_redir(t_token *token);

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
	else if (current == '|' && len == 2)
		return (OR);
	else if (current == '&' && len == 2)
		return (AND);
	else if (current == ';' && len == 1)
		return (LST);
	else if (current == '(' && len == 1)
		return (OPEN_PAR);
	else if (current == ')' && len == 1)
		return (CLOS_PAR);
	return (WRONG);
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

static size_t	word_len(char *line) {
	size_t	i;
	
	i = 0;
	while (line[i] && !isspace(line[i]) && !strchr(DELIMS, line[i])) {
		if (line[i] == '\'' || line[i] == '\"')
			i += quote_to_quote_len(line + i);
		else
			i++;
	}
	return (i);
}

static size_t	delim_len (char *line) {
	size_t	i = 0;

	if (strchr(MONO_DELIM, *line))
		return (1);
	while (line[i] == *line)
		i++;
	return (i);
}

static t_token *delim_token(char **line, t_token *prev) {
	t_token	*new = malloc(sizeof(t_token));
	int		len = delim_len(*line);

	if (!new)
		return (NULL);
	new->lexeme = strndup(*line, len);
	if (!new->lexeme)
		return (free(new), NULL);
	new->kind = get_type(**line, len);
	new->next = NULL;
	new->prev = prev;
	(*line) += len;
	return (new);
}

static t_token	*word_token(char **line, t_token *prev) {
	t_token *new = malloc(sizeof(t_token));
	size_t	len = word_len(*line);

	if (!new)
		return (NULL);
	new->lexeme = strndup(*line, len);
	if (!new->lexeme)
		return (free(new), NULL);
	new->kind = WORD;
	new->next = NULL;
	new->prev = prev;
	(*line) += len;
	return (new);
}

static t_token	*token(char **line, t_token *prev) {
	if (strchr(DELIMS, **line))
		return (delim_token(line, prev));
	else
		return (word_token(line, prev));
}

t_token	*lexer(char *line) {
	t_token	*head = NULL;;
	t_token	*tail = NULL;;

	skip_whitespaces(&line);
	if (!(*line))
		return (NULL);
	head = token(&line, NULL);
	if (!head)
		return (NULL);
	tail = head;
	while (*line) {
		skip_whitespaces(&line);
		if (!(*line))
			break ;
		tail->next = token(&line, tail);
		if (!tail->next)
			return (free_token_list(head), NULL);
		if (is_redir(tail))
			tail->next->kind = FILE_NAME;
		tail = tail->next;
	}
	return (head);
}
