#include "../../includes/minishell.h"

static char	*heredoc(char *stop)
{
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;
	char	*input;

	input = NULL;
	tmp1 = readline("heredoc >");
	while (ft_strncmp(tmp1, stop, ft_strlen(stop)))
	{
		tmp2 = ft_strjoin(tmp1, "\n");
		if (input)
		{
			tmp3 = ft_strjoin(input, tmp2);
			free (input);
			input = ft_strdup(tmp3);
			free (tmp3);
		}
		else
			input = ft_strdup(tmp2);
		free (tmp2);
		free (tmp1);
		tmp1 = readline("heredoc >");
	}
	free (tmp1);
	return (input);
}

static void	create_tmp_file(char *stop, t_cmd *cmd)
{
	char	*input;
	int		fd;

	input = heredoc(stop);
	cmd->tmpname = ft_strjoin(".tmp_heredoc", cmd->id);
	fd = open(cmd->tmpname, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
		exit (1); // error
	write(fd, input, ft_strlen(input));
	close(fd);
	free (input);
}

void	handling_heredoc(t_cmd *cmds)
{
	t_cmd	*tmp_cmd;
	t_dict	*tmp_dict;

	tmp_cmd = cmds;
	while (tmp_cmd)
	{
		tmp_dict = tmp_cmd->infd;
		while (tmp_dict)
		{
			if (tmp_dict->key == RDR_SRC)
				create_tmp_file(tmp_dict->value, tmp_cmd);
			tmp_dict = tmp_dict->next;
		}
		tmp_cmd = tmp_cmd->next;
	}
}
