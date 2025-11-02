#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "key_value.h"
#include "messages.h"

char	*get_kv_n_value(t_key_value *env, char *key, size_t len);

size_t	ft_charcspn(char const *s, char const c) {
	size_t	i = 0;

	while (s[i] && s[i] != c)
		i++;
	if (s[i] == c)
		i++;
	return (i);
}

static size_t	var_name_len(char const *s) {
	size_t	i = 0;
	
	while (s[i] && (isalnum(s[i]) || s[i] == '_'))
		i++;
	return (i);
}
///THERE MUST BE A PRETTIER WAY THAN THIS
static char	*replace_key_with_value(char *s, char *value, char *dollar, size_t key_len) {
	char	*empty = "";
	if (!value)
		value = empty;
	char	*dest = malloc(sizeof(char) * (strlen(s) + 1 - key_len + strlen(value)));
	char	*end_s = &s[dollar + 1 - s + key_len];

	if (!dest) 
		return (dprintf(2, "%s%s: %s\n", WARN_HD, ERR_MALLOC, NO_EXPAND), s);
	memcpy(dest, s, dollar - s);
	memcpy(&dest[dollar - s], value, strlen(value));
	memcpy(&dest[dollar - s + strlen(value)], end_s, strlen(end_s) + 1);
	free(s);
	return (dest);
}

static char	*replace(char *s, char *dollar, t_key_value *env) {
	char	*value;
	size_t	key_len;

	if (!isalpha(dollar[1]) && dollar[1] != '_')
		return (s);
	key_len = var_name_len(&dollar[1]);
	value = get_kv_n_value(env, &dollar[1], key_len);
	return (replace_key_with_value(s, value, dollar, key_len));
}

static char	*expand_string(char *s, t_key_value *env) {
	int		i = 0;

	if (!env)
		return (s);
	while (s[i]) {
		if (s[i] == '\'') {
			i++;
			i += ft_charcspn(&s[i], '\'');
		} else if (s[i] == '$') {
			s = replace(s, &s[i], env);
			i++;
		} else {
			i++;
		}
	}
	return (s);
}

int	expand_command(char **cmd, t_key_value *env) {
	for (int i = 0; cmd[i]; i++)
		cmd[i] = expand_string(cmd[i], env);
	return (0);
}
