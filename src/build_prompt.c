#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"

#ifndef DEFAULT_PROMPT
# define DEFAULT_PROMPT "\x1b[35m tshoo> \x1b[0m"
#endif

static char	*get_wd(t_key_value *env) {
	char	*last_slash;
	char	*wd;

	wd = get_kv_value(env, "PWD");
	if (!wd)
		return (NULL);
	last_slash = wd;
	while (*wd) {
		if (*wd == '/')
			last_slash = wd;
		wd++;
	}
	if (*last_slash == '/')
		last_slash++;
	return (last_slash);
}

static char	*get_bit(char *format, t_env *env) {
	if (strncmp(format, "wd", 2) == 0)
		return (get_wd(env->env_list));
	return (NULL);
}

static void fill_prompt(char *prompt, char *format, t_env *env) {
	int		total_len;
	int		bit_len;
	char	*bit;

	total_len = 0;
	while (*format && total_len <= 255) {
		if (*format != '%') {
			bit_len = strcspn(format, "%");
			memcpy(&prompt[total_len], format, bit_len);
			total_len += bit_len;
			format += bit_len;
 		} else {
			format++;
			bit = get_bit(format, env);
			if (!bit)
				continue ;
			bit_len = strlen(bit);
			memcpy(&prompt[total_len], bit, bit_len);
			total_len += bit_len;
			format += 2;
		}
	}
	prompt[total_len] = '\0';
}

void	escaping(char *ps1) {
	char	*end;

	while (*ps1) {
		if (*ps1 == '\'' || *ps1 == '\"')
			memmove(ps1, ps1 + 1, strlen(ps1 + 1) + 1);
		if (strncmp(ps1, "\\x", 2) == 0) {
			*ps1 = (char)strtol(ps1 + 2, &end, 16);
			memmove(ps1 + 1, end, strlen(end) + 1);
		}
		ps1++;
	}
}

void	build_prompt(char *prompt, char *format, t_env *env) {
	if (!format) {
		fill_prompt(prompt, DEFAULT_PROMPT, env);
		return ;
	}
	char	*ps1_dup = strdup(format);
	if (!ps1_dup) {
		fill_prompt(prompt, format, env);
	} else {
		escaping(ps1_dup);
		fill_prompt(prompt, ps1_dup, env);
	}
	free(ps1_dup);
}
