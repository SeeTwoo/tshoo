#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "token_and_node_types.h"

#ifndef DELIMS_SET
# define DELIMS "<>&|;()"
# define MONO_DELIM ";"
# define MULTI_DELIM "<>&|"
#endif

void	free_token_list(t_token *head);

static void	skip_parenthesis_and_whitespaces(char **line, int *sublvl) {
	while (**line == '(' || **line == ')' || isspace(**line)) {
		if (**line == '(')
			(*sublvl)++;
		else if (**line == ')')
			(*sublvl)--;
		(*line)++;
	}
}

static int	get_precedence(int type) {
	if (type == SEMI_COL)
		return (0);
	else if (type == AND || type == OR)
		return (1);
	else if (type == PIPE)
		return (2);
	else
		return (3);
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

static t_token *delim_token(char **line, int sublvl) {
	t_token	*new = malloc(sizeof(t_token));
	char	current = **line;
	int		len = 0;

	if (!new)
		return (NULL);
	if (current == ';')
		len = 1;
	else
		while ((*line)[len] == current)
			len++;
	new->start = *line;
	new->len = len;
	new->next = NULL;
	new->type = get_type(current, len);
	new->prec = get_precedence(new->type);
	new->sublvl = sublvl;
	(*line) += len;
	return (new);
}

static t_token	*word_token(char **line, int sublvl) {
	t_token *new = malloc(sizeof(t_token));

	if (!new)
		return (NULL);
	new->start = *line;
	new->len = word_len(line);
	new->next = NULL;
	new->type = WORD;
	new->prec = get_precedence(new->type);
	new->sublvl = sublvl;
	return (new);
}

static t_token	*token(char **line, int sublvl) {
	if (strchr(DELIMS, **line))
		return (delim_token(line, sublvl));
	else
		return (word_token(line, sublvl));
}

t_token	*lexer(char *line) {
	t_token	*head = NULL;;
	t_token	*tail = NULL;;
	int		sublvl = 0;

	skip_parenthesis_and_whitespaces(&line, &sublvl);
	if (!(*line))
		return (NULL);
	head = token(&line, sublvl);
	if (!head)
		return (NULL);
	tail = head;
	while (*line) {
		skip_parenthesis_and_whitespaces(&line, &sublvl);
		if (!(*line))
			break ;
		tail->next = token(&line, sublvl);
		if (!tail->next)
			return (free_token_list(head), NULL);
		tail = tail->next;
	}
	return (head);
}
