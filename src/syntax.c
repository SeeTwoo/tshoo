#include <stdio.h>
#include <unistd.h>

#include "messages.h"
#include "token.h"
#include "nodes.h"
#include "token_and_node_types.h"

int	is_and(t_token *token);
int	is_or(t_token *token);
int	is_pipe(t_token *token);
int	is_redir(t_token *token);
int	is_wrong_token(t_token *token);
int	is_word(t_token *token);

int	is_syntaxically_correct(t_token *toks) {
	if (is_pipe(toks))
		return (dprintf(2, "%s%s\n", MSTK_HD, PIPE_BINARY), 0);
	if (is_and(toks))
		return (dprintf(2, "%s%s\n", MSTK_HD, AND_BINARY), 0);
	if (is_or(toks))
		return (dprintf(2, "%s%s\n", MSTK_HD, OR_BINARY), 0);
	for (; toks; toks = toks->next) {
		if (toks->sublvl < 0)
			return (dprintf(2, "%s%s\n", MSTK_HD, UNCLOSED_PAR), 0);
		if (is_pipe(toks) && (toks->next == NULL || !is_word(toks->next)))
			return (dprintf(2, "%s%s\n", MSTK_HD, PIPE_BINARY), 0);
		if (is_redir(toks) && (toks->next == NULL || !is_word(toks->next)))
			return (dprintf(2, "%s[%s] %s\n", MSTK_HD, toks->lexeme, REDIR_FILE_NAME), 0);
		if (is_wrong_token(toks))
			return (dprintf(2, "%s[%s] %s\n", MSTK_HD, toks->lexeme, UNKNOWN_TOK), 0);
	}
	return (1);
}
