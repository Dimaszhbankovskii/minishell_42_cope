#include "../../includes/minishell.h"

static char	*search_paths_in_envp(char **paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*filename;

	i = 0;
	while (paths && paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		filename = ft_strjoin(tmp, cmd);
		if (!filename)
		{
			free (tmp);
			return (NULL);
		}
		free (tmp);
		if (!access(filename, X_OK))
			return (filename);
		free(filename);
		i++;
	}
	return (NULL);
}

static char	*search_paths(char **paths, char *cmd)
{
	char	*filename;
	char	*path;

	if (!access(cmd, X_OK))
		return (ft_strdup(cmd));
	path= getcwd(NULL, 0);
	if (!path)
		return (NULL);
	filename = ft_strjoin(path, cmd);
	if (!filename)
	{
		free (path);
		return (NULL);
	}
	if (!access(filename, X_OK))
	{
		free (path);
		return (filename);
	}
	free (path);
	free (filename);
	return (search_paths_in_envp(paths, cmd));
}

static char	**parsing_paths(char **envp)
{
	char	*tmp1;
	char	*tmp2;
	char	**paths;

	tmp1 = find_str(envp, "PATH=");
	if (!tmp1)
		return (NULL);
	tmp2 = ft_strdup(tmp1 + ft_strlen("PATH="));
	if (!tmp2)
		return (NULL);
	paths = ft_split(tmp2, ':');
	if (!paths)
	{
		free(tmp2);
		return (NULL);
	}
	free (tmp2);
	return (paths);
}

static void	child_process_binary(t_cmd *cmd, char **envp)
{
	char	**paths_envp;
	char	*path_cmd;
	
	paths_envp = parsing_paths(envp);
	if (!paths_envp)
		exit(warning("Error: parsing ENVP PATH\n", EXIT_FAILURE));
	if (!cmd->args[0]) // check ( |   ls -l) (ls -l |    ) два случая надо ограничить
		exit(warning("Error: binary file not exist\n", EXIT_FAILURE)); // ???
	path_cmd = search_paths(paths_envp, cmd->args[0]);
	if (!path_cmd)
	{
		free_two_array_char(paths_envp);
		exit(warning("Error: binary file not exist\n", 127));
	}
	execve(path_cmd, cmd->args, g_data.envp);
	free_two_array_char(paths_envp);
	free (path_cmd);
	error_mess("execve", errno);
}

int	execute_binary(t_cmd *cmd)
{
	pid_t	child;

	child = fork();
	if (child < 0)
		return (1);
	if (!child)
		child_process_binary(cmd, g_data.envp);
	else
		waitpid(child, &g_data.status, 0);
	// сделать макрос
	return (0);
}
