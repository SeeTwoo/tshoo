#include <stdio.h>

int	main(int ac, char **av) {
	(void)ac;
	for (int i = 0; av[i]; i++)
		printf("%s\n", av[i]);
	return (1);
}
