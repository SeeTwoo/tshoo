#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int	main(void) {
	char	c;
	ssize_t	bytes_read;

	bytes_read = read(0, &c, 1);
	if (bytes_read == 0)
		printf("errno = %d and reads : %s\n", errno, strerror(errno));
	return (0);
}
