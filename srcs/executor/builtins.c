#include "../../includes/minishell.h"

void	execute_echo(t_cmd *cmd)
{
	int	i;

	i = 1;
	g_data.status = 0;
	if(!cmd->args[1])
	{
		write(STDOUT_FILENO, "\n", 1);
		return ;
	}
	while (cmd->args[i] && !ft_strncmp(cmd->args[i], "-n", \
	ft_strlen(cmd->args[i])))
		i++;
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (ft_strncmp(cmd->args[1], "-n", ft_strlen(cmd->args[1])))
		ft_putchar_fd('\n', STDOUT_FILENO);
}

void	execute_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		exit(1); // malloc error
	ft_putstr_fd(path, STDOUT_FILENO);
	write(STDOUT_FILENO,"\n",1);
	free (path);
	g_data.status = 0;
}

void execute_env(void)
{
	int	i;

	i = 0;
	while(g_data.envp && g_data.envp[i])
	{
		ft_putstr_fd(g_data.envp[i++], STDOUT_FILENO);
		ft_putchar_fd('\n',STDOUT_FILENO);
	}
	g_data.status = 0;
}
