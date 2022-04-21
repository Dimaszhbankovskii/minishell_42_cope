#include "../../includes/minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		exit (1); // manage error
	cmd->id = NULL;
	cmd->args = NULL;
	cmd->count = 0;
	cmd->infd = NULL;
	cmd->outfd = NULL;
	cmd->tmpname = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_cmd	*last_cmd(t_cmd *cmds)
{
	t_cmd	*tmp;

	tmp = cmds;
	if (!tmp)
		return (NULL);
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	add_cmd(t_cmd **cmds, t_cmd *new)
{
	if (new)
	{
		if (!*cmds)
			*cmds = new;
		else
			last_cmd(*cmds)->next = new;
	}
}

void	free_cmd(void)
{
	t_cmd	*tmp;
	t_cmd	*del;

	del = g_data.cmds;
	if (del)
	{
		while (del)
		{
			tmp = del->next;
			if (del->id)
				free (del->id);
			free_two_array_char(del->args);
			del->infd=free_dict(del->infd);
			del->outfd=free_dict(del->outfd);
			if (del->tmpname)
				free(del->tmpname);
			free (del);
			del = tmp;
		}
		g_data.cmds = NULL;
	}
}

int	count_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	tmp = cmds;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
