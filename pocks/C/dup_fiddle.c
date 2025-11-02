#include <fcntl.h>
#include <unistd.h>

int	main(void) {
	int	fd;

	fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC);
	if (fd == -1)
		return (1);
	dup2(fd, STDOUT_FILENO);
	write(STDOUT_FILENO, "hello", 5);
	return (0);
}
