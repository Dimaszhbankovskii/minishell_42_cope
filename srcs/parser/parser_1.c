#include "../../includes/minishell.h"

static void	init_cmd(t_token *token)
{
	t_token	*tmp;
	t_cmd	*cmd;

	tmp = token;
	if (tmp->type != PIPE)
	{
		add_cmd(&g_data.cmds, new_cmd());
		cmd = last_cmd(g_data.cmds);
		while (tmp && tmp->type != PIPE)
		{
			if (tmp->type == WORD || tmp->type == QUOTES || tmp->type == DQUOTES)
				cmd->count++;
			tmp = tmp->next;
		}
		cmd->args = malloc_two_array_char(cmd->count);
		if (!cmd->args)
			exit (1); // manager error
	}
}

static void	cmd_get_args(char *arg)
{
	t_cmd	*cmd;

	cmd = last_cmd(g_data.cmds);
	add_back_elem_two_array_char(cmd->args, arg, cmd->count);
}

static t_token	*find_token_path_redirect(t_token *token)
{
	t_token *tmp;

	tmp = token->next;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == SEPARATOR)
			tmp = tmp->next;
		else
			return (tmp);
	}
	return (tmp);
}

static t_token	*cmd_get_redirect(t_token *token)
{
	t_token	*tmp;

	tmp = find_token_path_redirect(token);
	if (token->type == RDR_IN)
		add_dict(&last_cmd(g_data.cmds)->infd, new_dict(RDR_IN, tmp));
	else if (token->type == RDR_OUT)
		add_dict(&last_cmd(g_data.cmds)->outfd, new_dict(RDR_OUT, tmp));
	else if (token->type == RDR_SRC)
		add_dict(&last_cmd(g_data.cmds)->infd, new_dict(RDR_SRC, tmp));
	else if (token->type == RDR_APD)
		add_dict(&last_cmd(g_data.cmds)->outfd, new_dict(RDR_APD, tmp));
	return (tmp);
}

void	parser(void)
{
	t_token	*token;

	token = g_data.tokens;
	while (token)
	{
		init_cmd(token);
		while (token && token->type != PIPE)
		{
			if (token && (token->type == WORD || token->type == QUOTES || \
			token->type == DQUOTES))
				cmd_get_args(token->content);
			if (token && (token->type == RDR_IN || token->type == RDR_OUT \
			|| token->type == RDR_SRC || token->type == RDR_APD))
				token = cmd_get_redirect(token);
			if (token && token->type != PIPE)
				token = token->next;
		}
		if (token)
			token = token->next;
	}
	init_id_cmd();
}
