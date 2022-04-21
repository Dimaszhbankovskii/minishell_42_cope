#include "libft.h"

int	free_buffer(int read_bytes, char *buffer)
{
	free(buffer);
	buffer = NULL;
	if (read_bytes < 0)
		return (-1);
	else if (read_bytes == 0)
		return (0);
	else
		return (1);
}
