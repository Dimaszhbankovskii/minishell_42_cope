#include "../includes/minishell.h"


static void	create_pipes(t_pipexb *pipex)
{
	(void)pipex->num_cmds;
	int	i;

	pipex->num_pipes = 2 * (pipex->num_cmds - 1);
	pipex->pipes = (int *)malloc(sizeof(int) * pipex->num_pipes);
	if (!pipex->pipes)
	{
		//close_files(pipex);
		exit(1);
	}
	i = 0;
	while (i < pipex->num_cmds - 1)
	{
		if (pipe(pipex->pipes + i * 2) < 0)
		{

			//close_files(pipex);
			exit(1);
		}
		i++;
	}
}


static char	**parsing_paths(t_pipexb *pipex)
{
	char	*tmp1;
	char	*tmp2;
	char	**paths;

	(void)pipex->num_cmds;
	tmp1 = *find_str(g_data.envp, "PATH=");
	if (!tmp1)
		exit(1);
	tmp2 = ft_strdup(tmp1 + ft_strlen("PATH="));
	if (!tmp2)
		exit(1);
	paths = ft_split(tmp2, ':');
	if (!paths)
	{
		free(tmp2);
		exit(1);
	}
	free (tmp2);
	return (paths);
}

static char	*search_paths(t_pipexb *pipex, char *cmd)
{
	int		i;
	char	*tmp;
	char	*filename;
	char *path;


	//absolute path
	if (!access(cmd, X_OK)) {
		//printf("%s\n",cmd);
		return (ft_strdup(cmd));
	}


	//relative path
	path= getcwd(NULL, 0);
	if (!path)
		exit(1); // malloc error
	filename = ft_strjoin(path, cmd);
	if (!access(filename, X_OK)) {
		//printf("%s\n",filename);
		return (filename);
	}


	//envp path
	i = 0;
	while (pipex->paths[i])
	{
		tmp = ft_strjoin(pipex->paths[i], "/");
		if (!tmp)
			exit(1);
		filename = ft_strjoin(tmp, cmd);
		if (!filename)
		{
			free (tmp);
			exit(1);
		}
		free(tmp);
		if (!access(filename, X_OK)) {
			//printf("%s\n",filename);
			return (filename);
		}
		i++;
		free(filename);
	}
	return (NULL);
}

void	create_paths(t_pipexb *pipex)
{
	int	i;
	t_cmd *cmd;
	cmd = g_data.cmd;

	pipex->paths = parsing_paths(pipex);
	pipex->path_cmd = (char **)malloc(sizeof(char *) * (pipex->num_cmds + 1));
	if (!pipex->path_cmd)
		exit(1);
	i = 0;
	while (i < pipex->num_cmds)
	{
		pipex->path_cmd[i] = search_paths(pipex, cmd->args[0]);
		if (!pipex->path_cmd[i]) {
			printf("INVALID COMMAND!!!!!\n");
			exit(1);
		}

		cmd = cmd->next;
		i++;
	}
	pipex->path_cmd[i] = NULL;
}

int count_cmds(t_cmd *cmd)
{
	int counter;

	counter = 0;
	while(cmd)
	{
		counter++;
		cmd = cmd->next;
	}
	return counter;
}

static void	parsing_commands(t_pipexb *pipex)
{
	int	i;
	t_cmd *cmd;

	pipex->num_cmds = count_cmds(pipex->cmd);
	pipex->cmds = (char ***)malloc(sizeof(char **) * (pipex->num_cmds + 1));
	if (!pipex->cmds)
		exit(1);
	i = 0;
	cmd = pipex->cmd;
	while (i < pipex->num_cmds)
	{
		pipex->cmds[i] =  cmd->args;
		if (!pipex->cmds[i])
			exit(1);
		cmd = cmd->next;
		i++;
	}
	pipex->cmds[i] = NULL;
}

void	parsing_data(t_pipexb *pipex)
{
	parsing_commands(pipex);
	create_paths(pipex);

	//open_files(pipex);
	create_pipes(pipex);
}
