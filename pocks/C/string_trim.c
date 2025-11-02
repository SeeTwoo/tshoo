/* ************************************************************************** */
/*                                                                            */
/*                                                         :::     ::::::::   */
/*   string_trim.c                                       :+:     :+:    :+:   */
/*                                                     +:+ +:+        +:+     */
/*   By: seetwoo <marvin@42students.fr>              +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*   Created:                                           #+#    #+#            */
/*   Uptated:                                          ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

/*
 *
 *		start|mid|end
 *		startmid |end
 *		startmidend
 *
 *
 */

int	trim_quote_pair(char *s, int i) {
	char	*close_quote = strchr(s + i + 1, s[i]);

	memmove(s + i, s + i + 1, close_quote - (s + i));
	close_quote--;
	memmove(close_quote, close_quote + 2, strlen(close_quote + 2) + 1);
	return (close_quote - (s + i));
}

void	string_trim(char *s) {
	int	i = 0;

	while (s[i]) {
		if (s[i] == '\'' || s[i] == '\"')
			i += trim_quote_pair(s, i);
		else
			i++;
	}
}

char	*quote_free_dup(char *s) {
	if (!s)
		return (NULL);
	string_trim(s);
	return (strdup(s));
}

#define RED "\x1b[31m"
#define RESET "\x1b[0m"

void	test_string(char *s) {
	char	*trimmed_s = quote_free_dup(s);
	if (!trimmed_s)
		return ;
	printf("%s[%s%s%s]%s\n", RED, RESET, s, RED, RESET);
}

int	main(void) {
	char	empty[] = "";
	char	empty_quotes[] = "\"\"";
	char	basic[] = "\"hello\"";
	char	basic_with_space[] = "\"hello world\"";
	char	split[] = "ec\"ho\"";
	char	multiple_nightmare_mix[] = "\'e\'cho some\'th\'ing ";
	char	basic_simple[] = "'hey'";
	char	*nothing = NULL;

	test_string(empty);
	test_string(empty_quotes);
	test_string(basic);
	test_string(basic_with_space);
	test_string(split);
	test_string(multiple_nightmare_mix);
	test_string(basic_simple);
	test_string(nothing);
	return (0);
}
