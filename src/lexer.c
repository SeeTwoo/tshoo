#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "token_and_node_types.h"

#ifndef DELIMS_SET
# define DELIMS "<>&|;()"
# define MONO_DELIM ";()"
# define MULTI_DELIM "<>&|"
#endif

void	free_token_list(t_token *head);

static void	skip_whitespaces(char **line) {
	while (isspace(**line))
		(*line)++;
}

static int	get_multi_type(char current, int len) {
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
	else if (current == '&' && len == 2)
		return (AND);
	return (WRONG);
}

static int	get_mono_type(char current) {
	if (current == '(')
		return (OPEN_PAR);
	else if (current == ')')
		return (CLOS_PAR);
	else if (current == ';')
		return (SEMI_COL);
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

static size_t	word_len(char **line) {
	size_t	i;
	
	i = 0;
	while (*(*line + i) && !isspace(*(*line + i)) && !strchr(DELIMS, *(*line + i))) {
		if ((*line)[i] == '\'' || (*line)[i] == '\"')
			i += quote_to_quote_len(&(*line)[i]);
		else
			i++;
	}
	(*line) += i;
	return (i);
}

static t_token	*mono_delim_token(char **line) {
	t_token	*new = malloc(sizeof(t_token));

	if (!new)
		return (NULL);
	new->start = *line;
	new->len = 1;
	new->next = NULL;
	new->type = get_mono_type(**line);
	(*line)++;
	return (new);
}

static t_token *multi_delim_token(char **line) {
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
	new->type = get_multi_type(current, len);
	(*line) += len;
	return (new);
}

static t_token	*word_token(char **line) {
	t_token *new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->start = *line;
	new->len = word_len(line);
	new->next = NULL;
	new->type = WORD;
	return (new);
}

static t_token	*token(char **line) {
	if (strchr(MONO_DELIM, **line))
		return (mono_delim_token(line));
	else if (strchr(MULTI_DELIM, **line))
		return (multi_delim_token(line));
	else
		return (word_token(line));
}

t_token	*lexer(char *line) {
	t_token	*head;
	t_token	*tail;

	head = NULL;
	tail = NULL;
	skip_whitespaces(&line);
	if (!(*line))
		return (NULL);
	head = token(&line);
	if (!head)
		return (NULL);
	tail = head;
	while (*line) {
		skip_whitespaces(&line);
		tail->next = token(&line);
		if (!tail->next)
			return (free_token_list(head), NULL);
		tail = tail->next;
	}
	return (head);
}
