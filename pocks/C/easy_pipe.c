#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char	**init_ls(void) {
	char	**args;

	args = malloc(sizeof(char *) * 2);
	args[0] = strdup("/bin/ls");
	args[1] = NULL;
	return (args);
}

char	**init_wc(void) {
	char	**args;

	args = malloc(sizeof(char *) * 3);
	args[0] = strdup("/bin/wc");
	args[1] = strdup("-l");
	args[2] = NULL;
	return (args);
}

int	main(void) {
	char	**args_ls;
	char	**args_wc;
	int		one_pipe[2];
	int		pids[2];
	
	args_ls = init_ls();
	args_wc = init_wc();
	pipe(one_pipe);
	pids[0] = fork();
	if (pids[0] == 0) {
		dup2(one_pipe[1], 1);
		close(one_pipe[1]);
		close(one_pipe[0]);
		execve(args_ls[0], args_ls, NULL);
	}
	pids[1] = fork();
	if (pids[1] == 0) {
		dup2(one_pipe[0], 0);
		close(one_pipe[1]);
		close(one_pipe[0]);
		execve(args_wc[0], args_wc, NULL);
	}
	close(one_pipe[1]);
	close(one_pipe[0]);
	waitpid(pids[0], 0, 0);
	waitpid(pids[1], 0, 0);
	return (0);
}
