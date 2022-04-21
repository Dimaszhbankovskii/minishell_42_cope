#include "../includes/minishell.h"

void	check_print(void)
{
	t_token	*token;
	t_cmd	*cmd;
	t_dict	*tmp;
	int		i;

	token = g_data.tokens;
	printf("--------------------------------------------------------\n\n");
	printf("input:\n%s\n", g_data.input);
	printf("\n--------------------------------------------------------\n\n");
	if (!token)
		printf("g_data.token = NULL\n");
	while (token)
	{
		printf("g_data.token: type = %d \t {%s} {%d}\n", token->type, token->content, token->content[0]);
		token = token->next;
	}

	printf("\n--------------------------------------------------------\n\n");
	cmd = g_data.cmds;
	if (!cmd)
		printf("g_data.cmd = NULL\n");
	while (cmd)
	{
		printf("cmd %s:\n", cmd->id);
		printf("args: ");
		i = 0;
		while (cmd->args && cmd->args[i])
		{
			printf("[%s], ", cmd->args[i]);
			i++;
		}
		printf("\n");
		if (cmd->infd)
		{
			tmp = cmd->infd;
			printf("infd: ");
			while (tmp)
			{
				printf("{'%d'; ", tmp->key);
				if (tmp->value)
					printf("'%s'} ", tmp->value);
				else
					printf("'NULL'} ");
				tmp = tmp->next;
			}
		}
		else
			printf("infd = NULL");
		printf("\n");
		if (cmd->outfd)
		{
			tmp = cmd->outfd;
			printf("outfd: ");
			while (tmp)
			{
				printf("{'%d'; ",tmp->key);
				if (tmp->value)
					printf("'%s'} ", tmp->value);
				else
					printf("'NULL'} ");
				tmp = tmp->next;
			}
		}
		else
			printf("outfd = NULL");
		printf("\n\n");
		cmd = cmd->next;
	}
	printf("--------------------------------------------------------\n");
	// printf("g_data.input=%p\n", g_data.input);
	// printf("g_data.tokens=%p\n", g_data.tokens);
	// printf("g_data.cmds=%p\n", g_data.cmds);
	// printf("g_data.cmds=%p\n", g_data.cmds->infd);
	// printf("g_data.cmds=%p\n", g_data.cmds->outfd);
	// printf("g_data.envp=%p\n", g_data.envp);
}
