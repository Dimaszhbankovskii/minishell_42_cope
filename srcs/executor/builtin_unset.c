#include "../../includes/minishell.h"

static void	unset_error_mess(char *name)
{
	ft_putstr_fd("unset: ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
}

static int	check_valid_name_var(char *name) // валидны буквы, цифры и _ (цифры кроме первого символа)
{
	int	i;

	if (name && ft_isdigit(name[0]))
		return (0);
	i = 0;
	while (name && name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	check_valid_unset(char **args)
{
	int	i;
	int	flag_error;

	flag_error = 0;
	i = 1;
	while (args && args[i++])
	{
		if (!check_valid_name_var(args[i]))
		{
			unset_error_mess(args[i]);
			flag_error = 1;
		}
	}
	return (flag_error);
}

static char	**unset_update_envp(char **envp, char *arg)
{
	char	**new_envp;
	int		i;
	int		j;

	new_envp = (char **)malloc(sizeof(char *) + size_two_array_char(envp));
	if (!new_envp)
		return (NULL);
	i = 0;
	j = 0;
	while (envp && envp[i])
	{
		if (!ft_strncmp(arg, envp[i], ft_strlen(arg)))
			i++;
		if (!envp[i])
			break ;
		new_envp[j] = ft_strdup(envp[i]);
		i++;
		j++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

char	**execute_unset(char **envp, char **args)
{
	int		i;
	int		flag_error;

	if (!args[1])
		flag_error = 0;
	else
	{
		flag_error = check_valid_unset(args);
		i = 1;
		while (args && args[i++])
		{
			if (check_valid_name_var(args[i]) && find_str(envp, args[i]))
			{
				envp = unset_update_envp(envp, args[i]);
				if (!envp)
					return (NULL); // error manager
			}
		}
	}
	g_data.status = 0 + flag_error;
	return (envp);
}
