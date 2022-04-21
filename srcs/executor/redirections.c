#include "../../includes/minishell.h"

void	redirect_input(t_cmd *cmd)
{
	t_dict	*tmp;
	int		fd;

	tmp = cmd->infd;
	fd = 0;
	while (tmp)
	{
		if (fd)
			close(fd);
		if (tmp->key == RDR_IN)
			fd = open(tmp->value, O_RDONLY | 0644);
		else if (tmp->key == RDR_SRC)
			fd = open(cmd->tmpname, O_RDONLY);
		if (fd < 0)
			error_mess(tmp->value, errno);
		tmp = tmp->next;
	}
	if (fd && dup2(fd, STDIN_FILENO) < 0)
		error_mess(tmp->value, errno);
	if (fd)
		close(fd);
	if (cmd->tmpname && unlink(cmd->tmpname) < 0)
		error_mess(tmp->value, errno);
}

void	redirect_output(t_cmd *cmd)
{
	t_dict	*tmp;
	int		fd;

	tmp = cmd->outfd;
	fd = 0;
	while (tmp)
	{
		if (fd)
			close(fd);
		if (tmp->key == RDR_OUT)
			fd = open(tmp->value, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		else if (tmp->key == RDR_APD)
			fd = open(tmp->value, O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (fd < 0)
			error_mess(tmp->value, errno);
		tmp = tmp->next;
	}
	if (fd && dup2(fd, STDOUT_FILENO) < 0)
		error_mess(tmp->value, errno);
	if (fd)
		close(fd);
}
