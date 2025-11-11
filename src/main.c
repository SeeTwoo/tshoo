#include <stdio.h>
#include <signal.h>

#include "env.h"

void	exec_config_file(t_env *env);
int		init_env(t_env *env, char **arg_env);
void	interactive_loop(t_env *env);
void	set_options(t_env *env, int ac, char **av);

typedef struct sigaction	t_sigaction;

int	main(int ac, char **av, char **arg_env) {
	t_env		env;
	t_sigaction	sa;
	
	(void)ac;
	(void)av;
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	if (init_env(&env, arg_env) != 0)
		return (1);
//	set_options(&env, ac, av);
//	exec_config_file(&env);
	interactive_loop(&env);
	free_kv_list(env.env_list);
	free_kv_list(env.aliases);
	return (0);
}
