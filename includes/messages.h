#ifndef MESSAGES_H
# define MESSAGES_H

//msg headers
# define MSTK_HD "\x1b[38;2;255;255;0m - tshoo - \x1b[0m"
# define WARN_HD "\x1b[38;2;255;127;0m - tshoo: warning - \x1b[0m"
# define FATAL_HD "\x1b[38;2;255;0;0m - tshoo: fatal - \x1b[0m"

//general syntax
# define UNCLOSED_PIPE "unclosed pipe"
# define REDIR_FILE_NAME "needs a file name to redirect"
# define UNKNOWN_TOK "unknown token"
# define CMD_FND "command not found"

//builtin / shell usage
# define NEED_EQUAL "need a '=' to create an alias : <pattern>=<replace>"
# define NEED_PATTERN "need a pattern to search : <pattern>=<replace>"
# define UNAL_USAGE "usage : unalias <pattern> ... OR unalias -a"
# define NEED_NAME "need a variable name to create a variable"
# define TOO_MANY "too many arguments"
# define UNKNOWN_OPTION "unknown option"

//shell fails
# define NOT_ALIASED "an alias was not applied"
# define NO_EXPAND "a variable was not expanded"
# define NO_ENV "environment may be missing or incomplete"
# define NO_RC "tshoorc will not be run"
# define NO_HOME "HOME variable not set"
# define NO_OLDPWD "OLDPWD variable not set"

//syscall fails
# define ERR_MALLOC "malloc failed"
# define ERR_FORK "fork failed"


#endif
