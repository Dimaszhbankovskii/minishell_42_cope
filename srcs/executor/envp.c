#include "../../includes/minishell.h"

void	save_envp(void)
{
	int	i;

	i = 0;
	while (g_data.envp && g_data.envp[i])
	{
		write(g_data.pipes[0][1], g_data.envp[i], ft_strlen(g_data.envp[i]));
		write(g_data.pipes[0][1], "\n", 1);
		i++;
	}
	close(g_data.pipes[0][1]);
	write(g_data.envp[1][1], &i, sizeof(int));
	close(g_data.pipes[1][1]);	
	write(g_data.envp[2][1], &g_data.status, sizeof(int));
	close(g_data.pipes[2][1]);
}

void	get_envp(void)
{
	int		i;
	int		j;

	read(g_data.pipes[1][0], &i, sizeof(int));
	read(g_data.pipes[2][0], &g_data.status, sizeof(int));
	free_two_array_char(g_data.envp);
	g_data.envp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!g_data.envp)
		exit(1); // manage error
	j = 0;
	while (j < i)
	{
		get_next_line(g_data.pipes[0][0], &g_data.envp[j]);
		g_data.envp[j][ft_strlen(g_data.envp[j]) - 1] = '\0';
		j++;
	}
	g_data.envp[j] = NULL;
}
