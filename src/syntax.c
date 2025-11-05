#include <stdio.h>
#include <unistd.h>

#include "messages.h"
#include "token.h"
#include "nodes.h"
#include "token_and_node_types.h"

int	is_pipe(t_token *token);
int	is_redir(t_token *token);
int	is_wrong_token(t_token *token);
int	is_word(t_token *token);

int	is_syntaxically_correct(t_token **toks) {
	int	i = 0;
	int	sublvl = 0;

	while (toks[i]) {
		if (sublvl < 0)
			return (dprintf(2, "%s%s\n", MSTK_HD, UNCLOSED_PAR), 0);

		if (is_pipe(toks[i]) && (i == 0 || !toks[i + 1] || is_pipe(toks[i + 1])))
			return (dprintf(2, "%s%s\n", MSTK_HD, UNCLOSED_PIPE), 0);

		if (is_redir(toks[i]) && (!toks[i + 1] || !is_word(toks[i + 1])))
			return (dprintf(2, "%s[%.*s] %s\n", MSTK_HD, (int)toks[i]->len, toks[i]->start, REDIR_FILE_NAME), 0);

		if (is_wrong_token(toks[i]))
			return (dprintf(2, "%s[%.*s] %s\n", MSTK_HD, (int)toks[i]->len, toks[i]->start, UNKNOWN_TOK), 0);

		if (toks[i]->type == OPEN_PAR)
			sublvl++;

		if (toks[i]->type == CLOS_PAR)
			sublvl--;

		toks[i]->sublvl = sublvl;

		i++;
	}
	return (1);
}
