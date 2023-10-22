
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int		get_file_length(char *path)
{
	int fd = open(path, O_RDONLY);
	if (fd < 1)
		return -1;
	char buffer;
	int out = 0;
	while (read(fd, &buffer, 1))
		out++;
	close(fd);
	return out;
}

char	*get_file(char *path)
{
	int len = get_file_length(path);
	int fd = open(path, O_RDONLY);
	if (fd < 1)
		return 0;
	char *out = malloc(len);
	if (out)
		read(fd, out, len);
	close(fd);
	return out;
}