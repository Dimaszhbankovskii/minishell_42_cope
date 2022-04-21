#include "../includes/minishell.h"

void	clear_data_loop(void)
{
	if (g_data.input)
		free (g_data.input);
	if (g_data.tokens)
		free_tokens();
	if (g_data.cmds)
		free_cmd();
}

void	clear_g_data(void)
{
	if (g_data.envp)
		free_two_array_char(g_data.envp);
	if (g_data.input)
		free (g_data.input);
	if (g_data.tokens)
		free_tokens();
	if (g_data.cmds)
		free_cmd();
}
