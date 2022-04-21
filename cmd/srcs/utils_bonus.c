#include "../includes/minishell.h"

char	**find_str(char **array, char *str)
{
	int	i;

	i = 0;
	while (array && array[i])
	{
		if (!ft_strncmp(array[i], str, ft_strlen(str)))
			return (&array[i]);
		i++;
	}
	return (NULL);
}

void	close_pipes(t_pipexb *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->num_pipes)
	{
		if (close(pipex->pipes[i]) < 0)
		{
			//close_files(pipex);
			exit(1);
		}
		i++;
	}
}

void	close_files(t_pipexb *pipex)
{
//	close(pipex->infile);
//	close(pipex->outfile);
	(void)pipex->num_cmds;
}

