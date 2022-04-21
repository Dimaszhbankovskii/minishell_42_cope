#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int		g_pipes[2][2];

void	child_process_1(char **argv, char **envp)
{
	char	*cmd[] = {"/bin/cat", NULL};
	int		infd;

		infd = open(argv[1], O_RDONLY, 0777);
		if (infd < 0)
		{
			perror("error open argv1");
			exit (1);
		}
		if (infd && dup2(infd, STDIN_FILENO) < 0)
		{
			perror("dup2:");
			exit (1);
		}
		close(infd);
		if (dup2(g_pipes[0][1], STDOUT_FILENO) < 0)
		{
			perror("dup2:");
			exit (1);
		}
		close(g_pipes[0][0]);
		close(g_pipes[0][1]);
		execve(cmd[0], cmd, envp);
		perror("execve error");
		exit (1);
}

void	child_process_2(char **envp)
{
	char	*cmd[] = {"/bin/cat", NULL};

	if (dup2(g_pipes[0][0], STDIN_FILENO) < 0)
	{
		perror("child2 dup2:");
		exit (1);
	}
	close(g_pipes[0][0]);
	if (dup2(g_pipes[1][1], STDOUT_FILENO) < 0)
	{
		perror("child2 dup2:");
		exit (1);
	}
	close(g_pipes[1][0]);
	close(g_pipes[1][1]);
	execve(cmd[0], cmd, envp);
	perror("child2 execve error");
	exit (1);
}

void	child_process_3(char **envp)
{
	char	*cmd[] = {"/bin/cat", NULL};
	int		outfd;

	if (dup2(g_pipes[1][0], STDIN_FILENO) < 0)
	{
		perror("dup2:");
		exit (1);
	}
	close(g_pipes[1][0]);
	// write(0, "check\n", 6);
	outfd = open("out", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (dup2(outfd, STDOUT_FILENO) < 0)
	{
		perror("dup2:");
		exit (1);
	}
	close(outfd);
	// close(g_pipes[0][0]);
	// close(g_pipes[0][1]);
	execve(cmd[0], cmd, envp);
	perror("execve error");
	exit (1);
}

int	main(int argc, char **argv, char **envp)
{
	int		infd, outfd;
	pid_t	child1, child2, child3;
	int		status1, status2, status3;
	char	*buf;
	int		ret;

	(void)argc;
	g_pipes[0][0] = -1;
	g_pipes[0][1] = -1;
	g_pipes[1][0] = -1;
	g_pipes[1][0] = -1;
	
	pipe(g_pipes[0]);
	pipe(g_pipes[1]);
//-----------------------------------------------------------
	child1 = fork();
	if (!child1) // первый потомок (читаем из файла)
		child_process_1(argv, envp);
	if (child1 < 0)
	{
		printf("error child1\n");
		exit(1);
	}
	close(g_pipes[1][0]);
	close(g_pipes[0][1]);
	if (child1)
		waitpid(0, &status1, 0);
	printf("status1 = %d\n", status1);
//-----	
	// ret = read(g_pipes[0][0], buf, 1000);
	// buf[ret] = '\0';
	// write(1, "result after child1: ", 21);
	// write(1, buf, strlen(buf));
//-----------------------------------------------------------
	// pipe(pipes[0]);
	pipe(g_pipes[1]);
	child2 = fork();
	if (!child2) // второй потомок (читаем из pipe1 и пишем в pipe2)
		child_process_2(envp);
	if (child2 < 0)
	{
		printf("error child2\n");
		exit(1);
	}
	close(g_pipes[0][0]);
	close(g_pipes[1][1]);
	if (child2)
		waitpid(0, &status2, 0);
	printf("status2 = %d\n", status2);
//-----	
	// ret = read(g_pipes[1][0], buf, 1000);
	// buf[ret] = '\0';
	// if (!buf || !*buf)
	// 	write(1, "null\n", 5);
	// write(1, "result after child2: ", 21);
	// write(1, buf, strlen(buf));
// //-----------------------------------------------------------
	// pipe(g_pipes[0]);
	// pipe(pipes[1]);
	child3 = fork();
	if (!child3) // третий потомок (читаем из pipe2 и пишем в файл)
		child_process_3(envp);
	if (child3 < 0)
	{
		printf("error child1\n");
		exit(1);
	}
	close(g_pipes[1][0]);
	close(g_pipes[0][1]);
	if (child3)
		waitpid(0, &status3, 0);
	printf("status3 = %d\n", status3);
//-----------------------------------------------------------
	return (0);
}
