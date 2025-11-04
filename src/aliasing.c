#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "env.h"

typedef struct s_segment	t_seg;

struct s_segment {
	char	*seg;
	size_t	len;
	t_seg	*next;
};

#ifndef DELIM
# define DELIM "<>|&"
#endif

static size_t	skip_whitespaces(char const *s) {
	size_t	i = 0;

	while (s[i] && isspace(s[i]))
		i++;
	return (i);
}

static size_t	key_len(char const *s) {
	size_t	i = 0;
	
	while (s[i] && !isspace(s[i]) && !strchr(DELIM, s[i]))
		i++;
	return (i);
}

static size_t	skip_to_next_block(char const *s) {
	size_t	i = 0;

	while (s[i] && !strchr(DELIM, s[i]))
		i++;
	while (s[i] && strchr(DELIM, s[i]))
		i++;
	return (i);
}

static t_seg	*new_segment_node(char *s, size_t len) {
	t_seg	*node = malloc(sizeof(t_seg));

	if (!node)
		return (NULL);
	node->seg = s;
	node->len = len;
	node->next = NULL;
	return (node);
}

static void	free_segments(t_seg *list) {
	t_seg	*temp;

	while (list) {
		temp = list;
		list = list->next;
		free(temp);
	}
}

static t_seg	*get_segments(char *raw_line, t_key_value *aliases_list) {
	t_seg	*head;
	t_seg	*tail;
	char	*temp = raw_line;
	char	*alias;
	size_t	little_l = 0;

	head = new_segment_node(temp, 0);
	if (!head)
		return (NULL);
	tail = head;
	while (*temp) {
		little_l = skip_whitespaces(temp);
		temp += little_l;
		tail->len += little_l;
		little_l= key_len(temp);
		alias = get_kv_n_value(aliases_list, temp, little_l);
		if (!alias) {
			little_l = skip_to_next_block(temp);
			temp += little_l;
			tail->len += little_l;
		} else {
			*temp = '\0';
			tail->next = new_segment_node(alias, strlen(alias));
			if (!(tail->next))
				return (free_segments(head), NULL);
			tail = tail->next;
			temp += little_l;
			tail->next = new_segment_node(temp, 0);
			if (!(tail->next))
				return (free_segments(head), NULL);
			tail = tail->next;
		}
	}
	return (head);
}

static char	*build_line(t_seg *segments) {
	char	*dest;
	char	*walker;
	size_t	dest_len = 0;
	t_seg	*temp = segments;

	while (temp) {
		dest_len += temp->len;
		temp = temp->next;
	}
	dest = malloc(sizeof(char) * (dest_len + 1));
	if (!dest)
		return (NULL);
	temp = segments;
	walker = dest;
	while (temp) {
		memcpy(walker, temp->seg, temp->len);
		walker += temp->len;
		temp = temp->next;
	}
	*walker = '\0';
	return (dest);
}

char	*aliasing(char *raw_line, t_key_value *aliases_list) {
	t_seg	*segments;
	char	*aliased_line;

	segments = get_segments(raw_line, aliases_list);
	if (!segments)
		return (NULL);
	aliased_line = build_line(segments);
	free_segments(segments);
	return (aliased_line);
}
