#include "token.h"

int	is_redir(t_token *token);

void	set_token_types(t_token *toks) {
	for (; toks; toks = toks->next) {
		if (is_redir(toks))
			toks->next->type = FILE_NAME;
	}
}
