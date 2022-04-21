#include "../includes/minishell.h"

void	error_mess(char *mess, int code)
{
	perror(mess);
	exit (code);
}

int	warning(char *mess, int code)
{
	write(STDERR_FILENO, mess, ft_strlen(mess));
	return (code);
}
