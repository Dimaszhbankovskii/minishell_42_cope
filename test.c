#include "sys/types.h"
#include "fcntl.h"
#include "unistd.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}


int	main(void)
{
	char	buf[1000];
	int		ret;
	int		fd = 0;

	fd = open("file1", O_RDONLY, 0777);
	ret = read(fd, buf, 1000);
	buf[ret] = '\0';
	write(1, buf, ft_strlen(buf));
	fd = open("file2", O_RDONLY, 0777);
	ret = read(fd, buf, 1000);
	buf[ret] = '\0';
	write(1, buf, ft_strlen(buf));
	return (0);
}