#include "../../includes/minishell.h"

static void	define_input_output(t_pipex *pipex, t_cmd *cmd)
{
	if (!cmd->infd && pipex->i > 0)
	{
		dup2(pipex->pipes[1 - pipex->used_pipes][0], STDIN_FILENO);
		close(pipex->pipes[1 - pipex->used_pipes][0]);
	}
	else
		redirect_input(cmd);
	if (!cmd->outfd && pipex->i < pipex->num - 1)
	{
		dup2(pipex->pipes[pipex->used_pipes][1], STDOUT_FILENO);
		close(pipex->pipes[pipex->used_pipes][0]);
		close(pipex->pipes[pipex->used_pipes][1]);
	}
	else
		redirect_output(cmd);
}

static void	execute_builtin(t_cmd *cmd, int type_builtin)
{
	if (type_builtin == BUILTIN_ECHO)
		execute_echo(cmd);
	else if (type_builtin == BUILTIN_PWD)
		execute_pwd();
	else if (type_builtin == BUILTIN_ENV)
		execute_env();
	else if (type_builtin == BUILTIN_UNSET)
		g_data.envp=execute_unset(g_data.envp, cmd->args);
}

static int	define_builtin(t_cmd *cmd)
{
	if (!ft_strncmp(cmd->args[0], "echo", ft_strlen(cmd->args[0])))
		return (BUILTIN_ECHO);
	else if (!ft_strncmp(cmd->args[0], "pwd", ft_strlen(cmd->args[0])))
		return (BUILTIN_PWD);
	else if (!ft_strncmp(cmd->args[0], "env", ft_strlen(cmd->args[0])))
		return (BUILTIN_ENV);
	else if (!ft_strncmp(cmd->args[0], "unset", ft_strlen(cmd->args[0])))
		return (BUILTIN_UNSET);
	return (0);
}

void	child_process(t_pipex *pipex, t_cmd *cmd)
{
	int	type_builtin;

	define_input_output(pipex, cmd); // перенаправление вводов и выводов
	// перевести в нижний регистр название команды (если требуется)
	type_builtin = define_builtin(cmd);
	if (type_builtin)
		execute_builtin(cmd, type_builtin);
	// else if (check_tmp_vatiable(cmd)) // ARG=text, ARG1=text1
	// 	save_tmp_variable(cmd);
	else
		execute_binary(cmd);
	save_envp();
	printf("check child\n");
	exit (0);
}
