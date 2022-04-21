#include "../includes/minishell.h"

char    **new_envp(char **envp, char *key, char *value)
{
	char    **new_envp;
	int     i;
	int	env;
	char *new_env;

	i = 0;
	env = 0;
	while (envp && envp[i])
		i++;
	if (key)
	{
		env = 1;
		new_env = ft_strjoin(key, "=");
	}
	new_envp = (char **)malloc(sizeof(char *) * (i + env + 1));
	if (!new_envp)
		exit (1); // manage error
	i = 0;
	while (envp && envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
			exit (1); // manage error
		i++;
	}
	if(key)
	{
		new_envp[i] = ft_strjoin(new_env, value);
		free(new_env);
	}
	new_envp[i + env] = NULL;
	return (new_envp);
}

void execute_echo(t_cmd *cmd)
{
	if(cmd->args[1] == NULL)
		write(STDOUT_FILENO, "\n", 1);
	else if(!ft_strcmp(cmd->args[1],"-n"))
	{
		if (cmd->args[2])
			ft_putstr_fd(cmd->args[2], STDOUT_FILENO);
	}
	else
	{
		ft_putstr_fd(cmd->args[1], STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", 1);
	}
}

void execute_pwd()
{
	char *path;

	path = getcwd(NULL, 0);
	if (!path)
		exit(1); // malloc error
	ft_putstr_fd(path,1); //fd = ???
	write(1,"\n",1);
}

static void execute_cd(t_cmd *cmd)
{
	char	*path;
	char	*relative_path;
	char **pwd;
	char **oldpwd;
	char *buf;

	//printf("\n%s\n",cmd->args[0]);
	//exit(1);
	if(!cmd->args[1])
	{
		pwd = find_str(g_data.envp, "HOME=");
		if (pwd)
		{
			cmd->args[1] = *(pwd)+ ft_strlen("HOME=");
			cmd->args[2] = NULL;
		}
		else {
			ft_putstr_fd("minishell: cd: HOME not set\n", STDOUT_FILENO);
			exit(1);
		}
	}
	else if(cmd->args[1][0] == '/')
	{
		ft_putstr_fd("cd :", 1);
		ft_putstr_fd(strerror(ENOENT), 1);
		ft_putstr_fd(": ", 1);
		ft_putstr_fd(cmd->args[1], 1);
		ft_putchar_fd('\n', 1);
		exit(1);
	}


	path = getcwd(NULL, 0);
	if (!path)
		exit(1);
	buf = ft_strjoin(path, "/");
	if(!buf)
		exit(1);
	relative_path = ft_strjoin(buf, cmd->args[1]);
	if(!relative_path)
		exit(1);

	if(!chdir(relative_path)  || !chdir(cmd->args[1]))
	{
		pwd = find_str(g_data.envp, "PWD=");
		if (*pwd)
		{
			free(*pwd);
			char *new_path=getcwd(NULL, 0);
			*pwd = ft_strjoin("PWD=", new_path);

			if (!*pwd)
				exit(1);
		}
		oldpwd = find_str(g_data.envp, "OLDPWD=");
		if (*oldpwd)
		{
			free(*oldpwd);
			*oldpwd = ft_strjoin("OLDPWD=",path);
			if(!*oldpwd)
				exit(1);
		}
	}
	else
	{
		ft_putstr_fd("cd :", 1);
		ft_putstr_fd(strerror(errno), 1);
		ft_putstr_fd(": ", 1);
		ft_putstr_fd(cmd->args[1], 1);
		ft_putchar_fd('\n', 1);
		exit(1);
	}
	free(path);
	free(relative_path);
	free(buf);
}


void execute_export(t_cmd *cmd)
{
	char **envp;
	envp = new_envp(g_data.envp, cmd->args[1], cmd->args[2]);
	int i = 0;
	while (g_data.envp[i])
		free(g_data.envp[i++]);
	free(g_data.envp);
	g_data.envp = envp;
}

void execute_unset(t_cmd *cmd)
{
	int     i;
	int	j;
	int	k;
	int  n;
	char    **new_envp;

	if(!cmd->args[1])
	{
		//ft_putstr_fd("unset : not enough arguments", 1);
		return;
	}
	j = 0;
	k = 1;
	while(cmd->args[k])
	{
		char *buf= ft_strjoin(cmd->args[k], "=");
		free(cmd->args[k]);
		cmd->args[k] = buf;
		while(g_data.envp[j])
		{
			if (!ft_strncmp(cmd->args[k], g_data.envp[j], ft_strlen(cmd->args[k])))
			{
				i = 0;
				while (g_data.envp[i])
					i++;
				new_envp = malloc(sizeof(char *) * i );
				if (!new_envp)
					exit(1);

				i = 0;
				n = 0;

				while (g_data.envp[i])
				{
					if (i != j)
					{
						new_envp[n] = ft_strdup(g_data.envp[i]);
						if (!new_envp[n])
							exit(1); // manage error
						n++;
					}
					i++;
				}
				new_envp[n] = NULL;
				i = 0;
				while (g_data.envp[i])
					free(g_data.envp[i++]);
				free(g_data.envp);
				g_data.envp = new_envp;

				break;
			}
			j++;
		}
		j = 0;
		k++;
	}

//	i = 0;
//	while(g_data.envp[i])
//	printf("%s\n",g_data.envp[i++]);

}

void execute_env()
{
	int	i;

	i = 0;
	while(g_data.envp[i])
	{
		ft_putstr_fd(g_data.envp[i++], 1);
		ft_putchar_fd('\n',1);
	}
}

int str_is_num(char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	while(*str)
	{
		if (*str < '0' || *str > '9')
			return(0);
		str++;
	}
	return (1);
}

void execute_exit(t_cmd *cmd)
{
	if (!cmd->args[1])
		exit(1);
	if (str_is_num(cmd->args[1]))
	{
		if(cmd->args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", STDOUT_FILENO);
			return;
		}
		else
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			exit(ft_atoi(cmd->args[1]));
		}
	}
	else
	{
		ft_putstr_fd("minishell: exit: ", STDOUT_FILENO);
		ft_putstr_fd(cmd->args[1], STDOUT_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDOUT_FILENO);
		exit(1);
	}
}

int	execute_builtin(t_cmd *cmd)
{
	int	i;
	int	result;

	i = 0;
	result = 1;
	if (!ft_strcmp(cmd->args[0], "exit"))
		execute_exit(cmd);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		execute_unset(cmd);
	else if (!ft_strcmp(cmd->args[0], "export"))
		execute_export(cmd);
	else
	{
		while (cmd->args[0][i])
			ft_tolower(cmd->args[0][i++]);
		if (!ft_strcmp(cmd->args[0], "echo"))
			execute_echo(cmd);
		if (!ft_strcmp(cmd->args[0], "cd"))
			execute_cd(cmd);
		else if (!ft_strcmp(cmd->args[0], "pwd"))
			execute_pwd();
		else if (!ft_strcmp(cmd->args[0], "env"))
			execute_env();
		else
			result = 0;
	}
	return (result);
}

void	comb_dup(int fd1, int fd2, t_pipexb *pipex)
{

	(void)pipex->num_cmds;
	if (dup2(fd1, STDIN_FILENO) < 0)
	{
		//close_files(pipex);
		exit(1);
	}
	if (dup2(fd2, STDOUT_FILENO) < 0)
	{
		//close_files(pipex);
		exit(1);
	}
}


t_pipexb	*init_pipex(t_cmd *cmd)
{
	t_pipexb	*pipex;
	pipex = (t_pipexb *)malloc(sizeof(t_pipexb));
	if (!pipex)
		exit(1);
	pipex->cmds = NULL;
	pipex->paths = NULL;
	pipex->path_cmd = NULL;
	pipex->pipes = NULL;
	pipex->cmd = cmd;
	pipex->num_cmds = 0;
	while(cmd)
	{
		pipex->num_cmds++;
		cmd = cmd->next;
	}
	return (pipex);
}

void	child_process(t_pipexb *pipex)
{
	pid_t pid;
	pid = fork();

	if (pid < 0)
		exit(1);//handling error
	if (pid == 0)
	{
		if (!pipex->index)
		{
			comb_dup(0, pipex->pipes[1], pipex);
		}
		else if (pipex->index == pipex->num_cmds - 1)
			comb_dup(pipex->pipes[pipex->index * 2 - 2], 1, pipex);
		else
			comb_dup(pipex->pipes[pipex->index * 2 - 2], pipex->pipes[pipex->index * 2 + 1], pipex);

		close_pipes(pipex);
		//close_files(pipex);
		execve(pipex->path_cmd[pipex->index], pipex->cmds[pipex->index], g_data.envp);
		exit(1);// handling error ,free
	}
}

void	executor(t_cmd *cmd)
{
	t_pipexb	*pipex;
	pipex = init_pipex(cmd);
	parsing_data(pipex);
	pipex->index = 0;

	while (pipex->index < pipex->num_cmds)
	{
		child_process(pipex);
		pipex->index++;

	}
	//while(i < pipex->num_cmds )
	//	wait();
	close_pipes(pipex);


	close_files(pipex);
	//free pipex;
}

void simulate_cmds(t_cmd *cmd1, t_cmd *cmd2, t_cmd *cmd3)
{
	cmd1->args = ft_split("unsed HOME", ' ');
	cmd1->next = cmd2;

	cmd2->args = ft_split("unset ZASE", ' ');
	cmd2->next = cmd3;

	cmd3->args = ft_split("yes", ' ');
	cmd3->next = NULL;

	g_data.cmd = cmd1;
}

int main(int argc, char **argv, char **envp)
{

	(void)argc;
	(void)argv;


	g_data.envp = new_envp(envp, NULL, NULL);

	t_dict file1;
	t_dict file2;
	t_dict file3;

	file1.key = "<";
	file1.value = "file1";
	file1.next = &file2;

	file2.key = "<";
	file2.value = "file2";
	file2.next = NULL;

	file3.key = "<";
	file3.value = "file3";
	file3.next = NULL;


	t_cmd cmd1;
	cmd1.infd = &file1;
	t_cmd cmd2;
	cmd2.infd = &file3;
	t_cmd cmd3;
	cmd3.infd = NULL;


	simulate_cmds(&cmd1,&cmd2,&cmd3);

	//printf("FIRST!\n");
	//execute_unset(&cmd2);
	//execute_env();
	//execute_cd(&cmd1);



//	execute_env();



//SEGA!!!
	execute_unset(&cmd1);
	execute_unset(&cmd2);


	//execute_env();
	//execve("/bin/ls", ft_split("/bin/ls", ' '), envp);




	exit(1);
	executor(&cmd1);
}
