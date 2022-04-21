#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void	child_process(int argc, char **argv, char **envp)
{
	char	buf[1000];
	int		fd;
	int		ret;
	char	*args[] = {"/bin/cat", NULL};

	fd = open(argv[1], O_RDONLY, 0777);
	if (fd < 0)
	{
		perror(argv[1]);
		printf("errno = %d\n", errno);
		exit (errno);
	}
	else
	{
		ret = read(fd, buf, 1000);
		buf[ret] = '\0';
		write(0, buf, strlen(buf));
	}
	close (fd);
	printf("------------------------------\n");
	fd = open(argv[1], O_RDONLY, 0777);
	if (fd < 0)
	{
		perror(argv[1]);
		printf("errno = %d\n", errno);
		exit (errno);
	}
	else
	{
		dup2(fd, STDIN_FILENO);
		close (fd);
		unlink(argv[1]);
		execve("/bin/cat", args, envp);
		printf("execve erorr\n");
	}
	exit (0);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	child;
	int		status;
	int		a;

	if (argc != 2)
	{
		write(2, "error num args\n", 15);
		exit (1);
	}
	child = fork();
	if (child < 0)
	{
		perror("child process");
		exit (errno);
	}
	if (!child)
		child_process(argc, argv, envp);
	if (child)
		waitpid(-1, &status, 0);
	printf("status child process = %d\n", status);
	return (0);
}
