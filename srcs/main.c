#include "../includes/minishell.h"

char	*ft_readline(void)
{
	char	*str;

	str = readline(">>");
	if (str)
		add_history(str);
	return (str);
}

char	**new_envp(char **envp)
{
	char	**new_envp;
	int		i;

	i = 0;
	while (envp && envp[i])
		i++;
	new_envp = (char **)malloc(sizeof(char *) * (i + 1));
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
	new_envp[i] = NULL;
	return (new_envp);
}

void	init_g_data(void)
{
	if (pipe(g_data.pipes[0]) < 0)
		exit (1);
	if (pipe(g_data.pipes[1]) < 0)
		exit (1);
	if (pipe(g_data.pipes[2]) < 0)
		exit (1);
	g_data.input = NULL;
	g_data.tokens = NULL;
	g_data.cmds = NULL;
}

static void	kernel_program(void)
{	
	lexer();
	parser();
	executor(g_data.cmds);	
	// printf("--------------- check ----------------\n");
	// check_print(); // check
	clear_data_loop();
}

int	main(int argc, char **argv, char **envp)
{
	if (argc != 1)
		exit (1); // error num args
	(void)argv;
	g_data.envp = new_envp(envp);
	g_data.status = 0;
	while (1)
	{
		init_g_data();
		g_data.input = ft_readline();
		if (!g_data.input)
			exit (0);
		if (!*g_data.input || !ft_strcmp(g_data.input, "\n"))
		{
			free (g_data.input);
			continue;
		}
		if (!ft_strcmp(g_data.input, "exit"))
			break ;
		kernel_program();
	}
	clear_g_data();
	return (0);
}
